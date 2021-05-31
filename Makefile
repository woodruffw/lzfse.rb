EXT_SRCS := ext/lzfse/ext.c ext/lzfse/ext.h

.PHONY: all
all: vendor/bundle
	bundle exec rake compile

.PHONY: lint
lint: lint-ext

.PHONY: lint-ext
lint-ext: $(EXT_SRCS)
	clang-format -i -style=file $(EXT_SRCS)

vendor/bundle:
	bundle
