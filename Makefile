CC = gcc

CFLAGS = -Wall -Werror -Wextra -pedantic

TARGET = tdp

SRCS = src/main.c src/temp_functions.c

OBJS = $(SRCS:.c=.o)

INCS = -Iinclude

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)
	rm -f $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) $(INCS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
