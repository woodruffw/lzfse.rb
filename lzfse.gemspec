# frozen_string_literal: true

require_relative "lib/lzfse/version"

Gem::Specification.new do |s|
  s.name = "lzfse"
  s.version = LZFSE::VERSION
  s.summary = "Ruby bindings for Apple's LZFSE (+ LZVN)"
  s.description = <<~DESCRIPTION
    lzfse is a Ruby gem containing bindings for Apple's LZFSE and LZVN
    compression algorithms. It binds Apple's reference implementation
    and exposes compression and decompression APIs for both LZFSE and
    LZVN.
  DESCRIPTION
  s.authors = ["William Woodruff"]
  s.email = "william@yossarian.net"
  s.files = Dir["**/LICENSE", "README.md", ".yardopts", "lib/**/*.rb", "ext/**/*.{c,h}"]
  s.extensions << "ext/lzfse/extconf.rb"
  s.homepage = "https://github.com/woodruffw/lzfse.rb"
  s.license = "MIT"
  s.required_ruby_version = ">= 3.0"
  s.metadata["rubygems_mfa_required"] = "true"
end
