#pragma once
#include <algorithm>
#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include <stack>
#include <string>
#include <vector>

struct HuffmanNode
{
	char value;
	long weight;
	HuffmanNode *left;
	HuffmanNode *right;
	HuffmanNode(std::pair<char, long> p);
	HuffmanNode(HuffmanNode *l, HuffmanNode *r);
};

class HuffmanTable
{
  private:
	void dfs(HuffmanNode *node, std::vector<bool> &path);

  public:
	std::map<char, long> dict;
	std::map<char, std::vector<bool>> code;
	HuffmanNode *tree = NULL;
	void insert(char c);
	void construct();
	void destruct();
	void walk();
	friend std::ostream &operator<<(std::ostream &out, HuffmanTable &x);
	void encode(char *buffer, long size, std::ostream &out);
	~HuffmanTable();
};