#include "huffman.h"
#include <cstring>

HuffmanNode::HuffmanNode(char c, size_t v)
	: value(c), weight(v), left(NULL), right(NULL){};

HuffmanNode::HuffmanNode(HuffmanNode *l, HuffmanNode *r) : left(l), right(r)
{
	weight = l->weight + r->weight;
}

void HuffmanTable::insert(char c) { count[(uint8_t)c]++; }

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
	int t = 0;
	for (int i = 0; i < x.code.size(); ++i)
		if (x.code[i].size() != 0)
			t++;
	out << t << std::endl;

	for (int i = 0; i < x.code.size(); ++i)
		if (x.code[i].size() != 0)
		{
			size_t p = 0;
			for (int j = 0; j < x.code[i].size(); j++)
				p += (x.code[i][j] * (1 << j));
			out << std::hex << p << "\t" << x.code[i].size() << "\t" << (char)i
				<< std::endl;
		}

	return out;
}

size_t HuffmanTable::encode(char *buffer, size_t size, std::ostream &out)
{

	out << *this;
	size_t p = 0;

	for (int i = 0; i < code.size(); i++)
		if (code[i].size() != 0)
			p += (code[i].size() * count[i]);
	out << p << std::endl;

	size_t q = std::ceil(p / 8.0);
	char *buf = new char[q];

	memset(buf, 0, q);

	{
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
	}

	out.write(buf, q);

	delete[] buf;
	return q;
}

HuffmanTable::~HuffmanTable() { destruct(tree); }