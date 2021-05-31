lzfse.rb
========

Ruby bindings for Apple's reference implementation of [LZFSE](https://github.com/lzfse/lzfse).

Bindings are also provided for the internal LZVN APIs.

## Usage

You'll need the gem:

```bash
gem install lzfse
```

Then, use the four public APIs:

```ruby
require "lzfse"

# LZFSE
compressed = LZFSE.lzfse_compress "please compress this string"
LZFSE.lzfse_decompress compressed

# LZVN
compressed = LZFSE.lzvn_compress "please also compress this string!"
LZFSE.lzvn_compress compressed
```

Each API takes a single string and returns a string.

## License

*lzfse.rb* is licensed under the MIT license.

Apple's reference implementation of LZFSE is licensed under the 3-clause BSD license.
