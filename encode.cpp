#include "huffman.h"
#ifdef DEBUG
#include <ctime>
#endif

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

	// start timing
#ifdef DEBUG
	time_t start_time = time(0);
	std::cout << 0 << " Started" << std::endl;
#endif

	// read entire file to memory
	std::filebuf *ptr;
	ptr = in.rdbuf();
	long size = ptr->pubseekoff(0, std::ios::end, std::ios::in);
	ptr->pubseekpos(0, std::ios::in);
	char *buffer = new char[size];
	ptr->sgetn(buffer, size);
	in.close();
#ifdef DEBUG
	time_t now_time = time(0);
	std::cout << now_time - start_time << " File Read" << std::endl;
#endif
	// encode
	HuffmanTable hash;
	for (long i = 0; i < size; i++)
		hash.insert(buffer[i]);
#ifdef DEBUG
	now_time = time(0);
	std::cout << now_time - start_time << " Char Counted" << std::endl;
#endif
	// write into file
	hash.construct();
#ifdef DEBUG
	now_time = time(0);
	std::cout << now_time - start_time << " Tree Constructed" << std::endl;
#endif
	hash.walk();
#ifdef DEBUG
	now_time = time(0);
	std::cout << now_time - start_time << " Tree Walked" << std::endl;
#endif
	hash.encode(buffer, size, out);
#ifdef DEBUG
	now_time = time(0);
	std::cout << now_time - start_time << " File Encoded" << std::endl;
#endif
	delete[] buffer;
	return 0;
}