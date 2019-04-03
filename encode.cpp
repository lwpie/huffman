#include "huffman.h"

int main(int argv, char *argc[])
{
	// get command line parameters
	if ((argv != 4) ||
		((strcmp(argc[1], "-o") != 0) && (strcmp(argc[3], "-o") != 0)))
	{
		std::cerr << "Invalid Input" << std::endl;
		return 1;
	}
	std::string infile, outfile;
	if (strcmp(argc[1], "-o") == 0)
	{
		outfile = argc[2];
		infile = argc[3];
	}
	else
	{
		outfile = argc[3];
		infile = argc[2];
	}
	std::ifstream in;
	in.open(infile, std::ios::binary);
	std::ofstream out;
	out.open(outfile, std::ios::binary);
	if (!in || !out)
	{
		std::cerr << "File Open Failure" << std::endl;
		return 1;
	}
	in.sync_with_stdio(false);
	out.sync_with_stdio(false);

	// read entire file to memory
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
	hash.encode(buffer, size, out);
	delete[] buffer;
	return 0;
}