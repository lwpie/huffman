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
	gflags::SetUsageMessage("Usage: ./decode --infile path-to-input-file --outfile path-to-output-file");
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

	// load huffman table
	HuffmanTable hash;
	int n;
	in >> std::hex >> n;
	in.get();
	for (int i = 0; i < n; i++)
	{
		char c;
		size_t v;
		in.get(c);
		in >> v;
		in.get();
		hash.insert(c, v);
	}
	hash.construct();

#ifdef DEBUG
	now = clock();
	std::cout << (now - start + 0.0) / CLOCKS_PER_SEC << "\tHuffman Loaded" << std::endl;
#endif

	// decode
	size_t total, size;
	in >> total >> size;
	in.get();
	size_t byte = std::ceil(size / 8.0);
	std::filebuf *ptr;
	ptr = in.rdbuf();
	char *buffer = new char[byte];
	for (size_t i = 0; (i << 10) < byte; i++)
		ptr->sgetn(buffer + (i << 10), std::min(size_t(1 << 10), byte - (i << 10)));
	in.close();

#ifdef DEBUG
	double sz = std::ceil(size / 8.0);
	std::string r = "BKMGTE";
	size_t e = 0;
	while (sz >= 1024)
	{
		sz /= 1024;
		e++;
	}
	std::cout << "File Size: " << std::setiosflags(std::ios::fixed)
			  << std::setprecision(2) << std::ceil(size / 8.0) / pow(1024, e) << r[e] << std::endl;
	now = clock();
	std::cout << (now - start + 0.0) / CLOCKS_PER_SEC << "\tFile Read"
			  << std::endl;
#endif

	hash.decode(buffer, size, total, out);
	out.close();

#ifdef DEBUG
	now = clock();
	std::cout << (now - start + 0.0) / CLOCKS_PER_SEC << "\tFile Decoded" << std::endl;
#endif

#ifdef DEBUG
	now = clock();
	std::cout << (now - start + 0.0) / CLOCKS_PER_SEC << "\tStopped" << std::endl;
	sz = std::ceil(size / 8.0) / ((now - start + 0.0) / CLOCKS_PER_SEC);
	e = 0;
	while (sz >= 1024)
	{
		sz /= 1024;
		e++;
	}
	std::cout << "Overall Speed: "
			  << total / ((now - start + 0.0) / CLOCKS_PER_SEC) / pow(1024, e)
			  << r[e] << "/s" << std::endl;
#endif

	return 0;
}