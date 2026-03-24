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




CC      = gcc
CFLAGS  = -Wall -Wextra -std=c11 -O2
LIBS    = -lncursesw -lm
TARGET  = atm_tui
SRCS    = src/main.c src/ui.c src/screens_customer.c src/auth.c src/system.c
OBJS    = $(SRCS:.c=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

%.o: %.c atm.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET)


# make -f Makefile