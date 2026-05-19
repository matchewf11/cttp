CFLAGS = -Wall -Werror -Wextra -Wpedantic
SAN = -fsanitize=address,undefined
BIN = cttp

test: test.c
	cc test.c cttp.c -o test $(CFLAGS) $(SAN)
	./test

all: main.c
	cc main.c cttp.c -o $(BIN) $(CFLAGS) $(SAN)

run: all
	./$(BIN)
