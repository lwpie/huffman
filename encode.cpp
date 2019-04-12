#include "gflags/gflags.h"
#include "huffman.h"

DEFINE_string(infile, "input", "Input file name");
DEFINE_string(outfile, "output", "Output file name");

#ifdef DEBUG
#include <ctime>
#endif

int main(int argc, char **argv)
{
#ifdef DEBUG
	clock_t start = clock();
	std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(2);
#endif

	// get command line parameters
	gflags::SetVersionString("0.1.0");
	gflags::SetUsageMessage("Usage: ./encode --infile path-to-input-file --outfile path-to-output-file");
	gflags::ParseCommandLineFlags(&argc, &argv, true);

	std::ifstream in;
	in.open(FLAGS_infile, std::ios::binary);
	std::ofstream out;
	out.open(FLAGS_outfile, std::ios::binary);
	if (!in || !out)
	{
		std::cerr << "File Open Failure" << std::endl;
		std::cerr << gflags::ProgramUsage() << std::endl;
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
	size_t e = 0;
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
		hash.insert(buffer[i], 1);
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