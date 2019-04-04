#include "huffman.h"
#include <iomanip>
#ifdef DEBUG
#include <ctime>
#endif

int main(int argv, char *argc[])
{
#ifdef DEBUG
	clock_t start = clock();
	std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(2);
#endif
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
	clock_t now = clock();
	std::cout << (now - start + 0.0) / CLOCKS_PER_SEC << "\tStarted" << std::endl;
#endif

	// read entire file to memory
	std::filebuf *ptr;
	ptr = in.rdbuf();
	auto size = ptr->pubseekoff(0, std::ios::end, std::ios::in);
	ptr->pubseekpos(0, std::ios::in);
	char *buffer = new char[size];
	ptr->sgetn(buffer, size);
	in.close();

#ifdef DEBUG
	double sz = size;
	std::string r = "BKMGTE";
	long e = 0;
	while (sz >= 1024)
	{
		sz /= 1024;
		e++;
	}
	std::cout << "File Size: " << std::setiosflags(std::ios::fixed)
			  << std::setprecision(2) << size / pow(1024, e) << r[e] << std::endl;
	now = clock();
	std::cout << (now - start + 0.0) / CLOCKS_PER_SEC << "\tFile Read"
			  << std::endl;
#endif
	// encode
	HuffmanTable hash;
	for (size_t i = 0; i < size; i++)
	{
		hash.insert(buffer[i]);
	}
#ifdef DEBUG
	now = clock();
	std::cout << (now - start + 0.0) / CLOCKS_PER_SEC << "\tChar Counted"
			  << std::endl;
#endif
	// write into file
	hash.construct();
#ifdef DEBUG
	now = clock();
	std::cout << (now - start + 0.0) / CLOCKS_PER_SEC << "\tTree Constructed"
			  << std::endl;
#endif
	hash.walk();
#ifdef DEBUG
	now = clock();
	std::cout << (now - start + 0.0) / CLOCKS_PER_SEC << "\tTree Walked"
			  << std::endl;
#endif
	auto q = hash.encode(buffer, size, out);
#ifdef DEBUG
	now = clock();
	std::cout << (now - start + 0.0) / CLOCKS_PER_SEC << "\tFile Encoded"
			  << std::endl;
#endif
	delete[] buffer;
#ifdef DEBUG
	now = clock();
	std::cout << (now - start + 0.0) / CLOCKS_PER_SEC << "\tStopped" << std::endl;
	sz = size / ((now - start + 0.0) / CLOCKS_PER_SEC);
	e = 0;
	while (sz >= 1024)
	{
		sz /= 1024;
		e++;
	}
	std::cout << "Overall Speed: "
			  << size / ((now - start + 0.0) / CLOCKS_PER_SEC) / pow(1024, e)
			  << r[e] << "/s" << std::endl;
	std::cout << "Compression Rate: " << ((q + 0.0) / size) * 100 << "%"
			  << std::endl;
#endif
	return 0;
}