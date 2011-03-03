# substitua os PATHs pelo local correto de suas bibliotecas

all: grid janela janela_gtk janela_qt mtk retangulo_framebuffer
janela: janela.c
	gcc janela.c -o janela -L/usr/X11R6/lib -lX11
janela_gtk: janela_gtk.c
	gcc janela_gtk.c -o janela_gtk `pkg-config --cflags --libs gtk+-2.0`
janela_qt: janela_qt.cc
	g++ janela_qt.cc -o janela_qt -L /usr/lib/qt/lib -I/usr/lib/qt/include -lqt
mtk: mtk.c
	gcc mtk.c -o mtk -L/usr/X11R6/lib -lX11
retangulo_framebuffer: retangulo_framebuffer.c
	gcc retangulo_framebuffer.c -o retangulo_framebuffer -O2
grid: grid.c
	gcc grid.c -o grid `sdl-config --cflags --libs`
clean:
	rm -rf retangulo_framebuffer mtk janela janela_gtk janela_qt grid

