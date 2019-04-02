#include "huffman.h"

int main()
{
	// read entire file to memory
	std::ifstream in;
	in.open("input", std::ios::binary);
	std::filebuf *ptr;
	ptr = in.rdbuf();
	long size = ptr->pubseekoff(0, std::ios::end, std::ios::in);
	ptr->pubseekpos(0, std::ios::in);
	char *buffer = new char[size];
	ptr->sgetn(buffer, size);
	in.close();
	// encode
	HuffmanTable hash;
	for (long i = 0; i < size; i++)
		hash.insert(buffer[i]);
	// write into file
	std::ofstream out;
	out.open("output", std::ios::binary);
	hash.encode(buffer, size, out);
	free(buffer);
	return 0;
}