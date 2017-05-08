#include <iostream>

#include <random>

#include "Tree.h"

const unsigned int TREE_NODE_COUNT = 10000;

int main() {
	Tree<int> tree;

	std::random_device rand;
	std::mt19937_64 mt_genererator(rand());
	std::uniform_int_distribution<int> distribution(-1000, 1000);

	for (unsigned int i = 0; i < TREE_NODE_COUNT; i++) tree.add(distribution(mt_genererator));

	std::vector<std::uint8_t> compressed_tree = tree.compress();

	std::cout << "(1) Compressed tree (" << compressed_tree.size() << " bytes): ";
	for (const std::uint8_t& byte : compressed_tree) {
		std::cout << std::hex << static_cast<int>(byte);
	}

	tree.decompress(TreeCompression::PRE_ORDER_BIT_PAIRING, compressed_tree);
	compressed_tree = tree.compress();

	std::cout << std::endl << "(2) Compressed tree (" << std::dec << compressed_tree.size() << " bytes): ";
	for (const std::uint8_t& byte : compressed_tree) {
		std::cout << std::hex << static_cast<int>(byte);
	}

	std::cout << std::dec;

	std::cout << std::endl << std::endl << "____ Carrier nodes ____" << std::endl;
	std::cout << "Data:     \t" << 8 * sizeof(int) << std::endl;
	std::cout << "Pointers: \t" << 2 * 8 * sizeof(std::unique_ptr<int>) << std::endl;
	std::cout << "Total:    \t" << 8 * sizeof(int) + 2 * 8 * sizeof(std::unique_ptr<int>) << std::endl;

	std::cout << std::endl << std::endl << "____ Tree structure ____" << std::endl;
	std::cout << "Pointer: \t" << 8 * sizeof(std::unique_ptr<int>) << std::endl;
	std::cout << "Nodes:   \t" << TREE_NODE_COUNT * (8 * sizeof(int) + 2 * 8 * sizeof(std::unique_ptr<int>)) << std::endl;
	std::cout << "Total:   \t" << 8 * sizeof(std::unique_ptr<int>) + TREE_NODE_COUNT * (8 * sizeof(int) + 2 * 8 * sizeof(std::unique_ptr<int>)) << std::endl;

	std::cout << std::endl << std::endl << "____ Pre-order Bit Pairing compression ____" << std::endl;
	std::cout << "Vector:  \t" << 8 * sizeof(std::vector<std::uint8_t>) << std::endl;
	std::cout << "Node entry:\t" << 2 * sizeof(std::uint8_t) << std::endl;
	std::cout << "Total:   \t" << 8 * sizeof(std::vector<std::uint8_t>) + TREE_NODE_COUNT * (2 * sizeof(std::uint8_t)) << std::endl;
	std::cout << std::endl;
	std::cout << "Raw ratio:\t" << static_cast<float>(TREE_NODE_COUNT * (2 * sizeof(std::uint8_t))) / (TREE_NODE_COUNT * 8 * sizeof(int)) << std::endl;
	std::cout << "Real ratio:    \t" << static_cast<float>(8 * sizeof(std::vector<std::uint8_t>) + TREE_NODE_COUNT * (2 * sizeof(std::uint8_t))) / (TREE_NODE_COUNT * (8 * sizeof(int) + 2 * 8 * sizeof(std::unique_ptr<int>))) << std::endl;
	std::cout << "Full ratio:   \t" << static_cast<float>(8 * sizeof(std::vector<std::uint8_t>) + TREE_NODE_COUNT * (2 * sizeof(std::uint8_t))) / (8 * sizeof(std::unique_ptr<int>) + TREE_NODE_COUNT * (8 * sizeof(int) + 2 * 8 * sizeof(std::unique_ptr<int>))) << std::endl;
	std::cout << std::endl;
	std::cout << "Raw size: \t" << 8 * compressed_tree.size() << std::endl;

	std::cin.get();
	return 0;
}