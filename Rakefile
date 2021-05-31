# frozen_string_literal: true

$LOAD_PATH.unshift File.expand_path("lib")

require "rake/extensiontask"
require "rake/testtask"

Rake::ExtensionTask.new "lzfse" do |e|
  e.lib_dir = "lib/ext"
end

Rake::TestTask.new { |t| t.libs << "test" }

desc "Run tests"
task default: :test
