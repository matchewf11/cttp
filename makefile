CFLAGS = -Wall -Werror -Wextra -Wpedantic
SAN = -fsanitize=address,undefined
BIN = cttp
TEST_BIN = test
CC = cc

build_test: test.c cttp.c
	$(CC) test.c cttp.c -o $(TEST_BIN) $(CFLAGS) $(SAN)

all: main.c cttp.c
	$(CC) main.c cttp.c -o $(BIN) $(CFLAGS) $(SAN)

run: all
	./$(BIN)

test: build_test
	./$(TEST_BIN)
