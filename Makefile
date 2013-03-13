PROJECT := pulse430
SOURCES := main.c
MCU := msp430g2553

CC := msp430-gcc
CFLAGS := -mmcu=$(MCU)
LDFLAGS :=
LDLIBS :=

OBJECTS := $(SOURCES:%.c=%.o)

all: $(PROJECT)

$(PROJECT): $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJECTS) $(LDLIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o $(PROJECT)

clear:
	clear

.PHONY: clean clear
