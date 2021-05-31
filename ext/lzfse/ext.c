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

  output_len =
      lzfse_encode_buffer(output_buf, output_len, input_buf, input_len, NULL);

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

  // NOTE(ww): The public `lzfse_decode_buffer` API is a little bit braindead,
  // so we use the "internal" `lzfse_decode` API instead. This requires
  // us to set the decoder state up for ourselves.
  void *scratch_buffer = malloc(lzfse_decode_scratch_size() + 1);
  lzfse_decoder_state *s = (lzfse_decoder_state *)scratch_buffer;

  // NOTE(ww): It would be nice if these was an allocate-on-demand API, but
  // there isn't. So we do things the bad way.
  do {
    // We have to update each of these fields with every try, to avoid
    // mixing-and-matching decompression states between attempts.
    memset(s, 0x00, sizeof(*s));
    s->src = input_buf;
    s->src_begin = input_buf;
    s->src_end = input_buf + input_len;
    s->dst = output_buf;
    s->dst_begin = output_buf;
    s->dst_end = output_buf + output_len;

    int status = lzfse_decode(s);
    if (status == LZFSE_STATUS_OK) {
      break;
    } else if (status == LZFSE_STATUS_DST_FULL) {
      // Try again. There is almost certainly a better way of doing this
      // that involves preserving the decoder state and reusing the partially
      // decoded buffer, but this suffices for now.
      output_len *= 2;
      output_buf = realloc(output_buf, output_len);
    } else {
      rb_raise(cDecodeError, "internal error while decoding (%d)", status);
    }
  } while (true);

  free(scratch_buffer);

  VALUE output_str = rb_str_new((char *)output_buf, s->dst - output_buf);
  free(output_buf);

  return output_str;
}

__attribute__((visibility("default"))) void Init_lzfse() {
  mLZFSE = rb_define_module("LZFSE");
  cEncodeError = rb_const_get(mLZFSE, rb_intern("EncodeError"));
  cDecodeError = rb_const_get(mLZFSE, rb_intern("DecodeError"));

  rb_define_singleton_method(mLZFSE, "lzfse_encode_intern",
                             mLZFSE_lzfse_encode_intern, 1);
  rb_define_singleton_method(mLZFSE, "lzfse_decode_intern",
                             mLZFSE_lzfse_decode_intern, 1);
}
