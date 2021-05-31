# frozen_string_literal: true

require "mkmf"

RbConfig::MAKEFILE_CONFIG["CC"] = ENV["CC"] if ENV["CC"]

append_cflags %w[-Os -Wno-unknown-pragmas -Wno-unused-variable -DNDEBUG -D_POSIX_C_SOURCE
                 -std=c99 -fvisibility=hidden -g]

create_makefile "lzfse"
