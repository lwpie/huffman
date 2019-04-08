main: encode.cpp decode.cpp huffman.h huffman.cpp
	g++ -o encode --std=c++11 -O3 encode.cpp huffman.cpp & g++ -o decode --std=c++11 -O3 -DDEBUG decode.cpp huffman.cpp

encode: encode.cpp huffman.h huffman.cpp
	g++ -o encode -std=c++11 -O3 -DDEBUG encode.cpp huffman.cpp

decode: decode.cpp huffman.h huffman.cpp
	g++ -o decode --std=c++11 -O3 -DDEBUG decode.cpp huffman.cpp

clean:
	rm -f encode.exe