require "mkmf"

append_cflags %w[-Os -Wall -Wno-unknown-pragmas -Wno-unused-variable -DNDEBUG -D_POSIX_C_SOURCE -std=c99 -fvisibility=hidden]

create_makefile "lzfse"
