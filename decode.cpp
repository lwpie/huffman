#include <array>
#include <bitset>
#include <cmath>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <unordered_map>
#include <set>
#include <queue>
#include <vector>

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

	// decode
	in >> std::hex;
	std::unordered_map<int, std::pair<char, int>> code;
	std::queue<int> stream;
	int n;
	in >> n;
	for (int i = 0; i < n; i++)
	{
		char c;
		int v, offset;
		in >> v >> offset;
		in.get();
		in.get(c);
		code.insert(std::make_pair(v, std::make_pair(c, offset)));
	}
#ifdef DEBUG
	now = clock();
	std::cout << (now - start + 0.0) / CLOCKS_PER_SEC << "\tHuffman Table Loaded" << std::endl;
#endif
	size_t p, cnt = 0;
	in >> p;
	in.get();
	size_t t = std::ceil(p / 8.0);
	size_t s = 0;
	int o = 0;
	for (size_t q = 0; q < t; q++)
	{
		char c;
		in.get(c);
		std::bitset<8> byte((uint8_t)c);
		for (int i = 0; i < std::min((size_t)8, (p - cnt)); i++)
			stream.push(byte.test(i));
		cnt += 8;
		while (stream.size() != 0)
		{
			auto f = stream.front();
			stream.pop();
			s += f * (1 << o++);
			auto ptr = code.find(s);
			if ((ptr != code.end()) && (o == ptr->second.second))
			{
				out << ptr->second.first;
				s = 0;
				o = 0;
			}
		}
	}
	out << "\0";
#ifdef DEBUG
	now = clock();
	std::cout << (now - start + 0.0) / CLOCKS_PER_SEC << "\tFile Decoded" << std::endl;
#endif
	in.close();
	out.close();
	return 0;
}