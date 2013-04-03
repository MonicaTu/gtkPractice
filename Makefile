PKG_CONFIG=`pkg-config --cflags --libs gtk+-2.0`

gtkImage:
	gcc -o gtkImage gtkImage.c $(PKG_CONFIG)

clean:
	rm gtkImage
