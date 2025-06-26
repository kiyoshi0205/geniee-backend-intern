.PHONY: inner-build inner-run


inner-build: # prometheusのために-buildvcs=falseを入れる
	/usr/lib/go-1.22/bin/go build -buildvcs=false -o auction-worker

inner-run:
	./auction-worker
