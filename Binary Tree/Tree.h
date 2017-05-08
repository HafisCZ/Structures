#pragma once

#include <memory>
#include <vector>
#include <cstdint>

enum class TreeCompression {
	PRE_ORDER_BIT_PAIRING
};

template <class TYPE = int> class Tree {
	private:
		// Internal node
		struct Node {
			std::unique_ptr<Node> m_left = nullptr, m_right = nullptr;
			TYPE m_value;

			Node() {}
			Node(const TYPE& value) : m_value(value) {}
		};

		std::unique_ptr<Node> m_head = nullptr;

		// targeted add()
		void add(std::unique_ptr<Node>& sub, const TYPE& val) {
			if (sub == nullptr) sub = std::make_unique<Node>(val);
			else add((val < sub->m_value ? sub->m_left : sub->m_right), val);
		}

		// subtree to bit pairs
		void exportSubtreeToBitPairs(std::unique_ptr<Node>& sub, std::vector<std::uint8_t>& bytes) {
			if (sub == nullptr) return;

			bytes.emplace_back((sub->m_left != nullptr ? 0b10 : 0b00) + (sub->m_right != nullptr ? 0b01 : 0b00));
			exportSubtreeToBitPairs(sub->m_left, bytes);
			exportSubtreeToBitPairs(sub->m_right, bytes);
		}

		// bit pairs to subtree
		void importSubtreeFromBitPairs(const std::vector<uint8_t>& bytes, std::size_t &iterator, std::unique_ptr<Node>& target) {
			target = std::make_unique<Node>();

			const std::uint8_t byte = bytes[iterator];

			if (byte >> 1 & 0b1) importSubtreeFromBitPairs(bytes, ++iterator, target->m_left);
			if (byte >> 0 & 0b1) importSubtreeFromBitPairs(bytes, ++iterator, target->m_right);
		}

		// export tree by preorder bit pairing compression
		std::vector<std::uint8_t> PRE_ORDER_BIT_PAIRING_COMPRESSION() {
			std::vector<uint8_t> bytes, compacted_bytes;
			exportSubtreeToBitPairs(m_head, bytes);

			if (bytes.size() % 4 != 0) bytes.insert(bytes.end(), 4 - bytes.size() % 4, 0);

			for (std::size_t i = 0; i < bytes.size() / 4; i++) {
				std::uint8_t byte = 0;
				for (std::size_t shift = 0; shift < 4; shift++) byte += (bytes[i * 4 + shift] << (2 * (3 - shift)));

				compacted_bytes.emplace_back(byte);
			}
			return compacted_bytes;
		}

		// import tree from preorder bit pairing compression
		void PRE_ORDER_BIT_PAIRING_DECOMPRESSION(const std::vector<std::uint8_t>& bytes) {
			std::size_t iterator = 0;

			std::vector<std::uint8_t> expanded_bytes;
			for (const std::uint8_t &byte : bytes) {
				for (std::size_t shift = 0; shift <= 6; shift += 2) {
					expanded_bytes.emplace_back((byte >> (6 - shift)) & 0b11);
				}
			}

			importSubtreeFromBitPairs(expanded_bytes, iterator, m_head);
		}
	public:
		void add(const TYPE& val) {
			add(m_head, val);
		}

		std::vector<std::uint8_t> compress(TreeCompression compression_type = TreeCompression::PRE_ORDER_BIT_PAIRING) {
			switch (compression_type) {
				default:
				case TreeCompression::PRE_ORDER_BIT_PAIRING:
					return PRE_ORDER_BIT_PAIRING_COMPRESSION();
			}
		}

		void decompress(TreeCompression compression_type, const std::vector<std::uint8_t>& bytes) {
			switch (compression_type) {
				case TreeCompression::PRE_ORDER_BIT_PAIRING:
					return PRE_ORDER_BIT_PAIRING_DECOMPRESSION(bytes);
			}
		}
};