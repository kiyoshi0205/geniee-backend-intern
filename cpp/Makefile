.PHONY: inner-build inner-run

inner-build:
	g++-12 src/main.cpp -std=gnu++20 -o auction-worker \
	-Ilib/rapidjson/include \
	-Ilib/fmtlib/include \
	-DRAPIDJSON_HAS_STDSTRING \
	-DFMT_HEADER_ONLY \
	-lpthread -levent -levent_pthreads -lmysqlcppconn -lprometheus-cpp-pull -lprometheus-cpp-core -lz \
	-Wall -Wextra -O3

inner-run:
	./auction-worker

format:
	find src -name '*.cpp' -o -name '*.h' | xargs clang-format -i --style=file
