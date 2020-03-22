#pragma once
#include <iostream>
#include <string>
#include "Map.h"
#include "priority_queue.h"
#include "Queue.h"

class HuffmanTree
{
private:
	class Node
	{
	public:
		char symbol;
		int amount;
		Node* right;
		Node* left;

		Node(char symbol, int amount, Node* left, Node* right)
		{
			this->symbol = symbol;
			this->amount = amount;
			this->right = right;
			this->left = left;
		}

		~Node()
		{
			this->right = nullptr;
			this->left = nullptr;
		}
	};

	Node* root;
	priority_queue<Node*> nodes_list; //stores huffman's tree nodes while constructing it

	void clear(Node* node)
	{
		if (node != nullptr)
		{
			clear(node->left);
			clear(node->right);
			delete node;
		}
	}

	//creates code and decode tables from the tree
	void putTo(Map<char, std::string>& code_table, Map<std::string, char>& decode_table, Node* direction, std::string& code_str)
	{
		if (direction != nullptr)
		{
			//if node is leaf
			if (direction->left == nullptr && direction->right == nullptr)
			{
				code_table.insert(direction->symbol, code_str);
				decode_table.insert(code_str, direction->symbol);
			}

			//goes left - adds 1 to the code
			code_str.push_back('0');
			putTo(code_table, decode_table, direction->left, code_str);

			//goes right - adds 0 to the code
			code_str.push_back('1');
			putTo(code_table, decode_table, direction->right, code_str);
			
		}

		if(!code_str.empty())
			code_str.pop_back();
	}
	
public:

	Map<char, std::string> code_table;
	Map<std::string, char> decode_table;

	HuffmanTree()
	{
		root = nullptr;
	}

	~HuffmanTree()
	{
		clear(root);
		root = nullptr;
	}

	//creates Huffman tree basing on Map of symbol:frequency pairs
	void createFrom(Map<char, int>& arr)
	{
		if (arr.getSize() == 0)
			throw std::logic_error("Map to create huffman code from was empty");

		List<char> symbols;
		arr.get_keys(symbols);

		//filling priority queue with nodes
		while(!symbols.isEmpty())
		{			
			Node* node = new Node(symbols.at(0), arr.find(symbols.at(0)), nullptr, nullptr);
			symbols.pop_front();
			nodes_list.insert(node, node->amount);
		}

		//connecting nodes
		while(nodes_list.getSize() > 1)
		{
			Node* left = nodes_list.extract_min();
			Node* right = nodes_list.extract_min();

			Node* parent = new Node(0, left->amount + right->amount, left, right);

			nodes_list.insert(parent, parent->amount);
		}

		root = nodes_list.extract_min();

		std::string code_str;
		putTo(code_table, decode_table, root, code_str); //create code and decode tables
		
	}
};

//get Huffman code for the string
inline std::string EncodeToHuffmanCode(const std::string& encodable, HuffmanTree& encoding)
{
	Map<char, int> frequencies;

	//counts symbols repeats amount
	for (auto symbol : encodable)
	{
		if (frequencies.contains(symbol))
			frequencies.update(symbol, frequencies.find(symbol) + 1);
		else
			frequencies.insert(symbol, 1);
	}

	encoding.createFrom(frequencies); //creates Huffman tree

	//encoding string with created code table
	std::string encoded;
	for (auto symbol : encodable)
	{
		encoded += encoding.code_table.find(symbol);
		encoded += " ";
	}
	encoded.pop_back();
	
	return encoded;
}

//decodes the code string using Huffman's tree for this string
inline std::string DecodeHuffmanCode(const std::string& encoded_str, HuffmanTree& h_tree)
{
	if (h_tree.decode_table.getSize() == 0)
		throw std::logic_error("Decode table is empty");

	//filling queue with each code separated by space
	Queue<std::string> symbols_codes;
	std::string code;
	for (auto symbol : encoded_str)
	{
		if (symbol == '1' || symbol == '0')
			code += symbol;
		else if (symbol == ' ')
		{
			symbols_codes.enqueue(code);
			code.clear();
		}
		else
			throw std::invalid_argument("Wrong code string format!");	
	}

	symbols_codes.enqueue(code);
	code.clear();

	//decodes code string with decode table created from Huffman's tree
	std::string decode;
	while(symbols_codes.getSize() > 0)
	{
		code = symbols_codes.dequeue();

		if (!h_tree.decode_table.contains(code))
			throw std::invalid_argument("Attached huffman tree is wrong!");

		decode += h_tree.decode_table.find(code);
	}

	return decode;
}

//deletes all spaces in the string
inline std::string trim(std::string str)
{
	std::string trimmed;
	for (auto element : str)
	{
		if (element != ' ')
			trimmed += element;
	}

	return trimmed;
}

//calculates Compression Coefficient 
inline float CompressionCoefficient(std::string& original, std::string& coded)
{
	if (trim(coded).empty())
		throw std::logic_error("Coded string can not be empty");

	return (float)(original.length() * 8) / trim(coded).length();
}

inline void PrintFrequencyAndCodeTable(HuffmanTree& h_tree, const std::string& encodable)
{
	Map<char, int> frequencies;
	std::string symbols;

	//calculates symbols repeats amount
	for (auto symbol : encodable)
	{
		if (frequencies.contains(symbol))
			frequencies.update(symbol, frequencies.find(symbol) + 1);
		else
		{
			symbols += symbol;
			frequencies.insert(symbol, 1);
		}
	}

	for (auto element : symbols)
	{
		cout << "symbol: " << element << "\tcode: " << h_tree.code_table.find(element) << "\tfrequency: " << frequencies.find(element) << endl;
	}
}