encode: encode.cpp huffman.h huffman.cpp
	g++ -o encode -std=c++11 -O2 encode.cpp huffman.cpp

clean:
	rm -f encode.exe