csrc =  $(wildcard app/app.c)\
	$(wildcard gui/gui.c)\
	$(wildcard input/input.c)\
	$(wildcard main.c)\
obj = $(csrc:.c=.o)

CFLAGS = -lpthread `pkg-config --libs gtk+-3.0 --cflags gtk+-3.0`

YTPlayer: $(obj)
	$(GCC) -o $@ $^ $(CFLAGS)
