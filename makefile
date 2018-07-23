
YTPlayer: gui_keyboard.o gui_treelist.o gui.o input.o app.o main.o clean
	gcc -Wall -o YTPlayer app/gui/gui_keyboard.c app/gui/gui_treelist.c app/gui/gui.c app/input/input.c app/app.c main.c -lpthread `pkg-config --libs gtk+-3.0 --cflags gtk+-3.0`

gui_keyboard.o: app/gui/gui_keyboard.c
	gcc -c app/gui/gui_keyboard.c `pkg-config --libs gtk+-3.0 --cflags gtk+-3.0`

gui_treelist.o: app/gui/gui_treelist.c
	gcc -c app/gui/gui_treelist.c `pkg-config --libs gtk+-3.0 --cflags gtk+-3.0`

gui.o: app/gui/gui.c
	gcc -c app/gui/gui.c `pkg-config --libs gtk+-3.0 --cflags gtk+-3.0`

input.o: app/input/input.c
	gcc -c app/input/input.c

app.o: app/app.c
	gcc -c app/app.c

main.o: main.c
	gcc -c main.c

clean:
	rm gui_keyboard.o gui_treelist.o gui.o input.o app.o main.o
