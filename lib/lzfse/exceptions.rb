module LZFSE
  class LZFSEError < RuntimeError
  end

  class EncodeError < LZFSEError
  end

  class DecodeError < LZFSEError
  end
end
