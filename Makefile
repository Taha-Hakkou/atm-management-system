###########################
# VERSION 1
###########################

# objects = src/main.o src/system.o src/auth.o

# atm : $(objects)
# 	cc -o atm $(objects)

# main.o : src/header.h
# kbd.o : src/header.h
# command.o : src/header.h
# display.o : src/header.h
# insert.o : src/header.h
# search.o : src/header.h
# files.o : src/header.h
# utils.o : src/header.h

# clean :
# 	rm -f $(objects)


###########################
# VERSION 2
###########################

# CC      = gcc
# CFLAGS  = -Wall -Wextra -std=c11 -O2 $(shell pkg-config --cflags ncursesw)
# LIBS    = $(shell pkg-config --libs ncursesw) -lm # -lncursesw
# TARGET  = atm_tui
# SRCS    = src/main.c src/ui.c src/screens_customer.c src/auth.c src/system.c
# OBJS    = $(SRCS:.c=.o)

# .PHONY: all clean

# all: $(TARGET)

# $(TARGET): $(OBJS)
# 	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

# %.o: %.c atm.h
# 	$(CC) $(CFLAGS) -c -o $@ $<

# clean:
# 	rm -f $(OBJS) $(TARGET)


###########################
# VERSION 3
###########################

CC      = gcc
CFLAGS  = -Wall -Wextra -std=c11 -O2 \
          -I./include \
          $(shell pkg-config --cflags ncursesw)
LIBS    = $(shell pkg-config --libs ncursesw) -lm

TARGET  = bin/atm_tui
SRCS    = $(wildcard src/*.c)
OBJS    = $(patsubst src/%.c, obj/%.o, $(SRCS))

.PHONY: all clean

all: $(TARGET) #dirs $(TARGET)

# dirs:
# 	mkdir -p bin obj data

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

obj/%.o: src/%.c include/atm.h # $
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f obj/*.o bin/atm_tui