CFLAGS = -Wall -Werror -Wextra -Wpedantic
SAN = -fsanitize=address,undefined

all: main.c cttp.c req.c
	cc main.c cttp.c req.c -o cttp $(CFLAGS) $(SAN)

run: all
	./cttp

build_test: test.c cttp.c req.c
	cc test.c cttp.c req.c -o test $(CFLAGS) $(SAN)

test: build_test
	./test
