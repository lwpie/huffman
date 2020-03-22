all: encode decode

encode: encode.cpp huffman.h huffman.cpp
	g++ -o encode -m64 --std=c++11 -O3 -DDEBUG encode.cpp huffman.cpp -lgflags

decode: decode.cpp huffman.h huffman.cpp
	g++ -o decode -m64 --std=c++11 -O3 -DDEBUG decode.cpp huffman.cpp -lgflags

clean:
	rm -f encode decode