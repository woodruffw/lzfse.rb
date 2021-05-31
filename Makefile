EXT_SRCS := ext/lzfse/ext.c ext/lzfse/ext.h

.PHONY: all
all: compile

.PHONY: compile
compile: vendor/bundle
	bundle exec rake compile

.PHONY: lint
lint: lint-ext lint-rb

.PHONY: lint-ext
lint-ext: $(EXT_SRCS)
	clang-format -i -style=file $(EXT_SRCS)
	git diff --exit-code

.PHONY: lint-rb
lint-rb: vendor/bundle
	bundle exec rubocop

vendor/bundle:
	bundle
