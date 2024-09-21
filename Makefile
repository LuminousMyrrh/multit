CC = gcc
CFLAGS = -Wall -Wextra

TARGET = multit

BUILD_DIR = build

SRCS = $(wildcard src/*.c)

OBJS = $(patsubst src/%.c,$(BUILD_DIR)/%.o,$(SRCS))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(BUILD_DIR)/*.o $(TARGET)
