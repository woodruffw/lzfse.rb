require_relative "lzfse/exceptions"
require_relative "ext/lzfse"

module LZFSE
  module_function

  def lzfse_compress(string)
    lzfse_encode_intern string
  end

  def lzfse_decompress(string)
    lzfse_decode_intern string
  end

  def lzvn_compress(string)
    lzvn_encode_intern string
  end

  def lzvn_decompress(string)
    lzvn_decode_intern string
  end
end
