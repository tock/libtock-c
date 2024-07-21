VERSION_HASH := c4f9cd9f8717661c46be16bfbcb0017d785db3c1


CFLAGS = -g
CFLAGS += -I../../u8g2/u8g2-$(VERSION_HASH)/csrc/.
CFLAGS += -I../../
CFLAGS += -I../../u8g2
CFLAGS += -Iapp

SRC = $(shell ls ../../u8g2/u8g2-$(VERSION_HASH)/csrc/*.c)
SRC += $(shell ls ../../u8g2/u8g2-$(VERSION_HASH)/sys/bitmap/common/u8x8_d_bitmap.c)
SRC += $(shell ls ./app/*.c)
SRC += $(shell ls ./*.c)

SRCS := $(filter-out ../../u8g2/u8g2-$(VERSION_HASH)/csrc/u8x8_display.c,$(SRC))


OBJ = $(SRCS:.c=.o)

u8g2app: $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ) -o $@

RATE ?= 10

gif:
	mogrify -format png out/*.tga
	gifski -o app.gif -r $(RATE) out/display_*.png

clean:
	-rm -f u8g2app out/*.tga out/*.png app/*
