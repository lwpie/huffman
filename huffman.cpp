#include "huffman.h"

HuffmanNode::HuffmanNode(std::pair<char, long> p)
	: value(p.first), weight(p.second), left(NULL), right(NULL){};

HuffmanNode::HuffmanNode(HuffmanNode *l, HuffmanNode *r) : left(l), right(r)
{
	weight = (*l).weight + (*r).weight;
}

void HuffmanTable::insert(char s)
{
	auto ptr = dict.find(s);
	if (ptr == dict.end())
		dict.insert(std::make_pair(s, 1));
	else
		dict[s]++;
	return;
}

void HuffmanTable::construct()
{
	// std::sort(dict.begin(), dict.end(),
	//		  [](std::pair<char, long> x, std::pair<char, long> y) {
	//			  return x.second < y.second;
	//		  });
	std::vector<HuffmanNode *> btree;
	for (auto item : dict)
		btree.push_back(new HuffmanNode(item));
	while (btree.size() > 1)
	{
		std::sort(btree.begin(), btree.end(), [](HuffmanNode *x, HuffmanNode *y) {
			return (*x).weight < (*y).weight;
		});
		HuffmanNode *first = btree[0];
		HuffmanNode *second = btree[1];
		HuffmanNode *merge = new HuffmanNode(first, second);
		btree.erase(btree.begin(), btree.begin() + 2);
		btree.push_back(merge);
	}
	tree = btree[0];
	return;
}

void HuffmanTable::dfs(HuffmanNode *node, std::vector<bool> &path)
{
	if ((node->left == NULL) && (node->right == NULL))
		code.insert(std::make_pair(node->value, path));
	else
	{
		if (node->left != NULL)
		{
			path.push_back(false);
			dfs(node->left, path);
			path.pop_back();
		}
		if (node->right != NULL)
		{
			path.push_back(true);
			dfs(node->right, path);
			path.pop_back();
		}
	}
	return;
}

void HuffmanTable::walk()
{
	auto root = tree;
	std::vector<bool> path;
	dfs(root, path);
	return;
}

void HuffmanTable::destruct()
{
	std::vector<HuffmanNode *> btree;
	btree.push_back(tree);
	while (btree.size() != 0)
	{
		long size = btree.size();
		for (long i = 0; i < size; i++)
		{
			auto node = btree[i];
			if (node != NULL)
			{
				if (node->left != NULL)
					btree.push_back(node->left);
				if (node->right != NULL)
					btree.push_back(node->right);
				free(node);
			}
		}
		btree.erase(btree.begin(), btree.begin() + size);
	}
	return;
}

std::ostream &operator<<(std::ostream &out, HuffmanTable &x)
{
	x.construct();
	x.walk();
	out << x.code.size() << std::endl;
	for (auto item : x.code)
	{
		long p = 0;
		for (long i = 0; i < item.second.size(); i++)
			p += (item.second[i] * (1 << i));
		out << std::hex << p << "\t" << item.first << std::endl;
	}
	return out;
}

void HuffmanTable::encode(char *buffer, long size, std::ostream &out)
{
	out << *this;

	long p = 0;
	for (auto v : code)
		p += (v.second.size() * dict[v.first]);

	bool content[p];
	long q = 0;
	for (long i = 0; i < size; i++)
	{
		auto &enc = code[buffer[i]];
		for (auto j : enc)
		{
			content[q] = j;
			q++;
		}
	}

	q = std::ceil(p / 8.0);
	char *buf = new char[q];
	for (long i = 0; i < p; i++)
		buf[i / 8] += content[i] * (1 << (i % 8));
	for (long i = 0; i < p / 8.0; i++)
		out << buf[i];
	out << std::endl;

	out << p << std::endl;

	return;
}

HuffmanTable::~HuffmanTable() { destruct(); }