#include "huffman.h"
#include <cstring>
#include <cmath>
#include <bitset>

HuffmanNode::HuffmanNode(char c, size_t v)
	: value(c), weight(v), left(NULL), right(NULL){};

HuffmanNode::HuffmanNode(HuffmanNode *l, HuffmanNode *r) : left(l), right(r)
{
	weight = l->weight + r->weight;
}

void HuffmanTable::insert(char c, size_t v) { count[(uint8_t)c] += v; }

void HuffmanTable::construct()
{
	std::vector<HuffmanNode *> btree;

	for (int i = 0; i < 256; i++)
	{
		if (count[i] != 0)
		{
			btree.push_back(new HuffmanNode((char)(i), count[i]));
		}
	}

	while (btree.size() > 1)
	{
		std::sort(btree.begin(), btree.end(), [](HuffmanNode *x, HuffmanNode *y) {
			return x->weight < y->weight;
		});
		HuffmanNode *merge = new HuffmanNode(btree[0], btree[1]);
		btree.erase(btree.begin(), btree.begin() + 2);
		btree.push_back(merge);
	}

	tree = btree[0];
}

void HuffmanTable::dfs(HuffmanNode *node, std::vector<int> &path)
{

	if ((node->left == nullptr) && (node->right == nullptr))
	{
		code[(uint8_t)node->value] = path;
		return;
	}

	if (node->left != nullptr)
	{
		path.push_back(0);
		dfs(node->left, path);
		path.pop_back();
	}
	if (node->right != nullptr)
	{
		path.push_back(1);
		dfs(node->right, path);
		path.pop_back();
	}
}

void HuffmanTable::walk()
{
	auto root = tree;
	std::vector<int> path;
	dfs(root, path);
}

void HuffmanTable::destruct(HuffmanNode *node)
{
	if (node == nullptr)
		return;
	destruct(node->left);
	destruct(node->right);
	delete (node);
}

std::ostream &operator<<(std::ostream &out, const HuffmanTable &x)
{
	out << std::hex;
	int t = 0;
	for (int i = 0; i < 256; i++)
		if (x.count[i] != 0)
			t++;
	out << t << std::endl;

	for (int i = 0; i < 256; i++)
		if (x.count[i] != 0)
			out << (char)(uint8_t)i << '\t' << x.count[i] << std::endl;

	return out;
}

size_t HuffmanTable::encode(char *buffer, size_t size, std::ostream &out)
{

	out << *this;
	size_t p = 0;

	for (int i = 0; i < code.size(); i++)
		if (code[i].size() != 0)
			p += (code[i].size() * count[i]);
	out << size << '\t' << p << std::endl;

	size_t q = std::ceil(p / 8.0);
	char *buf = new char[q];

	memset(buf, 0, q);

	size_t ptr = 0;
	int power = 1;
	for (int i = 0; i < size; i++)
	{
		for (auto j : code[(uint8_t)buffer[i]])
		{
			buf[ptr++ >> 3] += j * power;
			power == 1 << 7 ? power = 1 : power <<= 1;
		}
	}

	out.write(buf, q);

	delete[] buf;
	return q;
}

void HuffmanTable::decode(char *buffer, size_t size, size_t total, std::ostream &out)
{
	char *buf = new char[total];
	size_t ptr = 0;
	size_t t = std::ceil(size / 8.0);
	size_t cnt = 0;
	HuffmanNode *node = tree;
	for (size_t i = 0; i < t; i++)
	{
		std::bitset<8> byte((uint8_t)buffer[i]);
		for (int j = 0; j < std::min((size_t)8, size - cnt); j++)
		{
			bool p = byte.test(j);
			if (p == 0)
				node = node->left;
			else
				node = node->right;
			if ((node->left == NULL) && (node->right == NULL))
			{
				buf[ptr++] = node->value;
				node = tree;
			}
		}
		cnt += 8;
	}
	out.write(buf, total);
	return;
}

HuffmanTable::~HuffmanTable() { destruct(tree); }