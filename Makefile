main: encode.cpp decode.cpp huffman.h huffman.cpp
	g++ -o encode --std=c++11 -O3 -DDEBUG encode.cpp huffman.cpp -lgflags & g++ -o decode --std=c++11 -O3 -DDEBUG decode.cpp huffman.cpp -lgflags

encode: encode.cpp huffman.h huffman.cpp
	g++ -o encode --std=c++11 -O3 -DDEBUG encode.cpp huffman.cpp -lgflags

decode: decode.cpp huffman.h huffman.cpp
	g++ -o decode --std=c++11 -O3 -DDEBUG decode.cpp huffman.cpp -lgflags

clean:
	rm -f encode decode