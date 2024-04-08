###########################################
# Simple Makefile for HIDAPI test program
#
# Alan Ott
# Signal 11 Software
# 2010-06-01
###########################################

all: hidtest-hidraw

CC       ?= gcc
CFLAGS   ?= -Wall -g -fpic

LDFLAGS  ?= -Wall -g


COBJS     = hid.o test.o
OBJS      = $(COBJS)
LIBS_UDEV = `pkg-config libudev --libs` -lrt
LIBS      = $(LIBS_UDEV)
INCLUDES ?= `pkg-config libusb-1.0 --cflags`


# Console Test Program
hidtest-hidraw: $(COBJS)
	$(CC) $(LDFLAGS) $^ $(LIBS_UDEV) -o $@

# Objects
$(COBJS): %.o: %.c
	$(CC) $(CFLAGS) -c $(INCLUDES) $< -o $@


clean:
	rm -f $(OBJS) hidtest-hidraw $(COBJS)

.PHONY: clean libs
