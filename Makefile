.PHONY: all
all: vendor/bundle
	bundle exec rake compile

vendor/bundle:
	bundle
