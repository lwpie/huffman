#pragma once
#include <algorithm>
#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>
#include <array>

struct HuffmanNode {
	char value;
	long weight;
	HuffmanNode *left;
	HuffmanNode *right;
	HuffmanNode(char c, long v);
	HuffmanNode(HuffmanNode *l, HuffmanNode *r);
};

class HuffmanTable {

private:
	void dfs(HuffmanNode *node, std::vector<int> &path);
	std::array<std::vector<int>, 256> code;
	size_t count[256] = {0};
	HuffmanNode *tree = NULL;
	friend std::ostream &operator<<(std::ostream &out, const HuffmanTable &x);

public:
	void insert(char c);
	void construct();
	void destruct(HuffmanNode *node);
	void walk();
	size_t encode(char *buffer, size_t size, std::ostream &out);
	~HuffmanTable();

};