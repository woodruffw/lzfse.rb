#include "ext.h"

VALUE mLZFSE = Qnil;
VALUE cEncodeError = Qnil;
VALUE cDecodeError = Qnil;

static VALUE mLZFSE_lzfse_encode_intern(VALUE self, VALUE input) {
  Check_Type(input, T_STRING);

  size_t input_len = RSTRING_LEN(input);
  uint8_t *input_buf = (uint8_t *)RSTRING_PTR(input);

  if (input_len <= 0) {
    rb_raise(cEncodeError, "input buffer must not be empty");
  }

  size_t output_len = input_len + 4096;
  uint8_t *output_buf = malloc(output_len);

  output_len = lzfse_encode_buffer(output_buf, output_len, input_buf, input_len, NULL);

  if (output_len <= 0) {
    rb_raise(cEncodeError, "encode failed (output buffer possibly too small)");
  }

  VALUE output_str = rb_str_new((char *)output_buf, output_len);
  free(output_buf);

  return output_str;
}

static VALUE mLZFSE_lzfse_decode_intern(VALUE self, VALUE input) {
  Check_Type(input, T_STRING);

  size_t input_len = RSTRING_LEN(input);
  uint8_t *input_buf = (uint8_t *)RSTRING_PTR(input);

  if (input_len <= 0) {
    rb_raise(cDecodeError, "input buffer must not be empty");
  }

  // Educated guess: start with a decompression buffer twice as large and work
  // from there.
  size_t output_len = input_len * 2;
  uint8_t *output_buf = malloc(output_len);
  size_t bytes_written = 0;

  do {
    bytes_written = lzfse_decode_buffer(output_buf, output_len, input_buf, input_len, NULL);

    if (bytes_written < output_len) {
      // Successful decode.
      break;
    } else {
      // Try again with a bigger buffer.
      output_len *= 2;
      output_buf = realloc(output_buf, output_len);
    }
  } while (true);

  VALUE output_str = rb_str_new((char *)output_buf, bytes_written);
  free(output_buf);

  return output_str;
}

static VALUE mLZFSE_lzvn_encode_intern(VALUE self, VALUE input) {
  Check_Type(input, T_STRING);

  size_t input_len = RSTRING_LEN(input);
  uint8_t *input_buf = (uint8_t *)RSTRING_PTR(input);

  if (input_len <= 0) {
    rb_raise(cEncodeError, "input buffer must not be empty");
  }

  if (input_len < LZVN_ENCODE_MIN_SRC_SIZE) {
    rb_raise(cEncodeError, "input is too small for LZVN (%zu < %zu)", input_len,
             LZVN_ENCODE_MIN_SRC_SIZE);
  }

  size_t output_len = input_len + 4096;
  uint8_t *output_buf = malloc(output_len);

  // NOTE(ww): lzvn_encode_buffer takes the same scratch buffer as the LZFSE
  // APIs. See lzfse_encode_buffer_with_scratch in lzfse_encode.c.
  void *scratch_buffer = malloc(lzfse_encode_scratch_size() + 1);

  output_len = lzvn_encode_buffer(output_buf, output_len, input_buf, input_len, scratch_buffer);
  if (output_len <= 0) {
    rb_raise(cEncodeError, "encode failed (output buffer possibly too small)");
  }

  free(scratch_buffer);
  VALUE output_str = rb_str_new((char *)output_buf, output_len);
  free(output_buf);

  return output_str;
}

static VALUE mLZFSE_lzvn_decode_intern(VALUE self, VALUE input) {
  Check_Type(input, T_STRING);

  size_t input_len = RSTRING_LEN(input);
  uint8_t *input_buf = (uint8_t *)RSTRING_PTR(input);

  if (input_len <= 0) {
    rb_raise(cDecodeError, "input buffer must not be empty");
  }

  // Educated guess: start with a decompression buffer twice as large and work
  // from there.
  size_t output_len = input_len * 2;
  uint8_t *output_buf = malloc(output_len);

  // NOTE(ww): Very similar to LZFSE decoding above; the main difference is that
  // our decoder state is a stack object instead.
  lzvn_decoder_state state;
  do {
    memset(&state, 0x00, sizeof(lzvn_decoder_state));
    state.src = input_buf;
    state.src_end = input_buf + input_len;
    state.dst = output_buf;
    state.dst_begin = output_buf;
    state.dst_end = output_buf + output_len;

    lzvn_decode(&state);

    // NOTE(ww): Unlike `lzfse_decode`, `lzvn_decode` doesn't provide a status
    // API. We need to check various decoder states to determine our next step.
    // Below is mostly ripped from the internals of `lzfse_decode`, case
    // `LZFSE_COMPRESSEDLZVN_BLOCK_MAGIC`.
    size_t src_used = state.src - input_buf;
    size_t dst_used = state.dst - output_buf;

    // Sanity check: we shouldn't have overrun either buffer.
    if (src_used > input_len || dst_used > output_len) {
      rb_raise(cDecodeError, "decode failed: internal buffer mismatch");
    }

    // If we're in a valid state and we've consumed the entire input, we're
    // done. Observe that we can't check the validity of dst_used here, since a
    // raw LZVN stream doesn't include its raw output size.
    if (src_used == input_len && state.end_of_stream) {
      break;
    }

    // If we're in an invalid state, die.
    if (src_used == input_len || state.end_of_stream) {
      rb_raise(cDecodeError, "internal error while decoding (lzvn, %zu, %zu)", src_used, dst_used);
    }

    // Otherwise, our output buffer was too small. Retry.
    output_len *= 2;
    output_buf = realloc(output_buf, output_len);
  } while (true);

  VALUE output_str = rb_str_new((char *)output_buf, state.dst - output_buf);
  free(output_buf);

  return output_str;
}

__attribute__((visibility("default"))) void Init_lzfse() {
  mLZFSE = rb_define_module("LZFSE");
  cEncodeError = rb_const_get(mLZFSE, rb_intern("EncodeError"));
  cDecodeError = rb_const_get(mLZFSE, rb_intern("DecodeError"));

  rb_define_singleton_method(mLZFSE, "lzfse_encode_intern", mLZFSE_lzfse_encode_intern, 1);
  rb_define_singleton_method(mLZFSE, "lzfse_decode_intern", mLZFSE_lzfse_decode_intern, 1);
  rb_define_singleton_method(mLZFSE, "lzvn_encode_intern", mLZFSE_lzvn_encode_intern, 1);
  rb_define_singleton_method(mLZFSE, "lzvn_decode_intern", mLZFSE_lzvn_decode_intern, 1);
}
