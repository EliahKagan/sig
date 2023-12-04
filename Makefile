CFLAGS := -std=c11 -pedantic-errors -Wall -Wextra $(CFLAGS)
TARGET = sig

all: $(TARGET)

clean:
	$(RM) $(TARGET)
