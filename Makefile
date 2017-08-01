SOURCES+=framebuffer.c
SOURCES+=texture.c
SOURCES+=quad.c
SOURCES+=shader.c
SOURCES+=utils.c
rtsight: main.c $(SOURCES)
	clang -o $@ $< `pkg-config --cflags --libs SDL2 GLEW` -framework OpenGL $(SOURCES)
