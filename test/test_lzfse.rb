# frozen_string_literal: true

require "minitest/autorun"
require "lzfse"

# Unit tests for LZFSE.
class LZFSETest < Minitest::Test
  def test_lzfse_end_to_end
    original = "hello, world!"
    compressed = LZFSE.lzfse_compress original
    assert_equal original, LZFSE.lzfse_decompress(compressed)

    original = "hello" * (4096 * 4096)
    compressed = LZFSE.lzfse_compress original
    assert_equal original, LZFSE.lzfse_decompress(compressed)
  end

  def test_lzvn_end_to_end
    original = "hello, world!"
    compressed = LZFSE.lzvn_compress original
    assert_equal original, LZFSE.lzvn_decompress(compressed)

    original = "hello" * (4096 * 4096)
    compressed = LZFSE.lzvn_compress original
    assert_equal original, LZFSE.lzvn_decompress(compressed)
  end

  def test_no_empty_inputs
    assert_raises LZFSE::EncodeError do
      LZFSE.lzfse_compress ""
    end

    assert_raises LZFSE::EncodeError do
      LZFSE.lzvn_compress ""
    end

    assert_raises LZFSE::DecodeError do
      LZFSE.lzfse_decompress ""
    end

    assert_raises LZFSE::DecodeError do
      LZFSE.lzvn_decompress ""
    end
  end

  def test_input_too_short_for_lzvn
    assert_raises LZFSE::EncodeError do
      LZFSE.lzvn_compress "small"
    end
  end
end
