Compilation example :

cd libgraphiz_hack/
mkdir build && cd build
CFLAGS="-DENABLE_PANGO_MARKUP" ../graphviz-2.17.20080206.0540/configure --prefix="$PWD/../install_Linux64" && make -j16 && make install-strip

----
	Bombela.
