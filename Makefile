ifndef $(MSPGCCDIR)
	MSPGCCDIR = /opt/msp430-gcc
endif

#paths
INCLUDES_DIRECTORY = $(MSPGCCDIR)/include 
DEVICE = msp430g2553

#compiler options
CC=$(MSPGCCDIR)/bin/msp430-elf-gcc

CFLAGS = -I . -I $(INCLUDES_DIRECTORY) -mmcu=$(DEVICE)
LFLAGS = -L . -L $(INCLUDES_DIRECTORY)

#mspdebug driver
DRIVER:= tilib

#compiling
all: main.elf

%.elf : %.c
	$(CC) $(CFLAGS) $(LFLAGS) $< -o $@

#upload to board
install.%: %.elf
	mspdebug $(DRIVER) "prog $<" --allow-fw-update

clean:
	rm -f *.o *.elf