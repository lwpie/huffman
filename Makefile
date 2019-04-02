encode: encode.cpp huffman.h huffman.cpp
	g++ -o encode encode.cpp huffman.cpp -std=c++11

clean:
	rm -f encode.exe