CC = gcc
FLAGS = -Wall -Wextra -std=c17 $(shell pkg-config --libs --cflags gpgme nss yaml-0.1 libcjson) -g

TARGET = password-tool

SRC = main.c passwords.c logins.c crypt.c
OBJ = $(SRC:.c=.o)

$(TARGET): $(OBJ)
	$(CC) $(FLAGS) -o $@ $^

%.o: %.c
	$(CC) $(FLAGS) -c $<

clean:
	rm -f $(OBJ) $(TARGET)

