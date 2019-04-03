#include "huffman.h"

HuffmanNode::HuffmanNode(char c, long v)
	: value(c), weight(v), left(NULL), right(NULL){};

HuffmanNode::HuffmanNode(HuffmanNode *l, HuffmanNode *r) : left(l), right(r)
{
	weight = (*l).weight + (*r).weight;
}

void HuffmanTable::insert(char c)
{
	long p = (long)c + 128;
	dict[p]++;
}

void HuffmanTable::construct()
{
	std::vector<HuffmanNode *> btree;
	for (long i = 0; i < 256; i++)
		if (dict[i] != 0)
			btree.push_back(new HuffmanNode((char)(i - 128), dict[i]));
	while (btree.size() > 1)
	{
		std::sort(btree.begin(), btree.end(), [](HuffmanNode *x, HuffmanNode *y) {
			return (*x).weight < (*y).weight;
		});
		// nearly equals to
		/*
		long i = 0;
		while ((i < btree.size()) &&
			   (!(((*btree[i]).weight <= (*merge).weight) &&
				  ((*merge).weight <= (*btree[i + 1]).weight))))
			i++;
		btree.insert(btree.begin() + i, merge);
		*/
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

std::ostream &operator<<(std::ostream &out, const HuffmanTable &x)
{
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

long HuffmanTable::encode(char *buffer, long size, std::ostream &out)
{
	out << *this;

	long p = 0;
	for (auto v : code)
		p += (v.second.size() * dict[(long)v.first + 128]);
	out << p << std::endl;

	long q = std::ceil(p / 8.0);
	char *buf = new char[q];
	for (long i = 0; i < q; i++)
		buf[i] = 0;
	long ptr = 0;
	for (long i = 0; i < size; i++)
		for (auto j : code[buffer[i]])
		{
			buf[ptr / 8] += j * (1 << (ptr % 8));
			ptr++;
		}

	out.write(buf, q);

	delete[] buf;
	return q;
}

HuffmanTable::~HuffmanTable() { destruct(); }