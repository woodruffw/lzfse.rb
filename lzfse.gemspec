# frozen_string_literal: true

require_relative "lib/lzfse/version"

Gem::Specification.new do |s|
  s.name = "inih"
  s.version = LZFSE::VERSION
  s.summary = "Ruby bindings for Apple's lzfse (+ lzvn)"
  s.description = "Ruby bindings for Apple's lzfse (+ lzvn)"
  s.authors = ["William Woodruff"]
  s.email = "william@yossarian.net"
  s.files = Dir["**/LICENSE", "README.md", ".yardopts", "lib/**/*.rb", "ext/**/*.{c,h}"]
  s.extensions << "ext/inih/extconf.rb"
  s.homepage = "https://github.com/woodruffw/lzfse.rb"
  s.license = "MIT"

  s.add_development_dependency "minitest"
  s.add_development_dependency "rake-compiler"
  s.add_development_dependency "redcarpet"
  s.add_development_dependency "yard"
end
