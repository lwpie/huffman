#pragma once
#include <algorithm>
#include <array>
#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>

struct HuffmanNode
{
	char value;
	size_t weight;
	HuffmanNode *left;
	HuffmanNode *right;
	HuffmanNode(char c, size_t v);
	HuffmanNode(HuffmanNode *l, HuffmanNode *r);
};

class HuffmanTable
{
  private:
	void dfs(HuffmanNode *node, std::vector<int> &path);
	std::array<std::vector<int>, 256> code;
	size_t count[256] = {0};
	HuffmanNode *tree = NULL;
	friend std::ostream &operator<<(std::ostream &out, const HuffmanTable &x);

  public:
	void insert(char c, size_t v);
	void construct();
	void destruct(HuffmanNode *node);
	void walk();
	size_t encode(char *buffer, size_t size, std::ostream &out);
	void decode(char *buffer, size_t size, size_t total, std::ostream &out);
	~HuffmanTable();
};