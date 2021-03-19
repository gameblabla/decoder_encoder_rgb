
CC			= gcc
CFLAGS		= -O0

OBJS_DEC		= decoder/decoder.o
PRGNAME_DEC     = decoder.elf

OBJS_ENC		= encoder/encoder.o
PRGNAME_ENC     = encoder.elf

LDFLAGS     = -lc -lgcc -lm -lSDL -lSDL_image -Wl,--as-needed -Wl,--gc-sections -flto -s

all: $(PRGNAME_DEC) $(PRGNAME_ENC)

# Rules to make executable
$(PRGNAME_DEC): $(OBJS_DEC)  
	$(CC) $(CFLAGS) -o $(PRGNAME_DEC) $^ $(LDFLAGS)

$(PRGNAME_ENC): $(OBJS_ENC)  
	$(CC) $(CFLAGS) -o $(PRGNAME_ENC) $^ $(LDFLAGS)

clean:
	rm -f decoder/*.o encoder/*.o
