# frozen_string_literal: true

require_relative "lib/lzfse/version"

Gem::Specification.new do |s|
  s.name = "lzfse"
  s.version = LZFSE::VERSION
  s.summary = "Ruby bindings for Apple's LZFSE (+ LZVN)"
  s.description = "Ruby bindings for Apple's LZFSE (+ LZVN)"
  s.authors = ["William Woodruff"]
  s.email = "william@yossarian.net"
  s.files = Dir["**/LICENSE", "README.md", ".yardopts", "lib/**/*.rb", "ext/**/*.{c,h}"]
  s.extensions << "ext/lzfse/extconf.rb"
  s.homepage = "https://github.com/woodruffw/lzfse.rb"
  s.license = "MIT"
  s.required_ruby_version = ">= 2.7"

  s.add_development_dependency "minitest"
  s.add_development_dependency "rake-compiler"
  s.add_development_dependency "rubocop"
  s.add_development_dependency "yard"
end
