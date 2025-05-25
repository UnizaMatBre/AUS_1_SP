#ifndef LINKEDTABLE_H
#define LINKEDTABLE_H
#include <functional>
#include <stdexcept>

namespace Containers {
	/**
	 * Represents hash table with efficient access to its members.
	 * Internally utilizes separate chaining (i.e keys that fall into same bucket create linked list)
	 *
	 * \tparam KeyType : type of key object that identifies pair
	 * \tparam ValueType : type of value stored in pair
	 * \tparam AllocatorType : type of allocator used by table internally
	 */
	template <
		typename KeyType,
		typename ValueType,
		typename AllocatorType = std::allocator<std::pair<const KeyType, ValueType>>
	>
	class LinkedTable {
		using ItemType = std::pair<const KeyType, ValueType>;

		struct Node {
			Node* next = nullptr;
			ItemType item;

			Node(const ItemType& item) : item(item) {};
			KeyType& key() const { return item.first; }
			ValueType& value() const { return item.second; }
		};

		using NodeAllocatorType = typename std::allocator_traits<AllocatorType>::template rebind_alloc<Node>;
		using NodeListAllocatorType = typename std::allocator_traits<AllocatorType>::template rebind_alloc<Node*>;

		// allocators
		NodeAllocatorType nodeAllocator_;
		NodeListAllocatorType nodeListAllocator_;

		// tools for key indexing and comparing
		std::hash<KeyType> keyHash_;
		std::equal_to<KeyType> keyEqual_;

		// array of buckets and information about it
		Node** buckets_ = nullptr;
		size_t capacity_ = 0;
		size_t itemCount_ = 0;


	public:
		LinkedTable(): nodeAllocator_(), nodeListAllocator_(this->nodeAllocator_) {};

		LinkedTable(const AllocatorType& allocator) : nodeAllocator_(allocator), nodeListAllocator_(this->nodeAllocator_) {};

		LinkedTable(const LinkedTable<KeyType, ValueType, AllocatorType>& other) {
			throw std::runtime_error("Not implemented.");
		};

		~LinkedTable() {
			throw std::runtime_error("Not implemented.");
		};

		ValueType& insert(const KeyType& key, const ValueType& value) {
			throw std::runtime_error("Not implemented.");
		};

		ValueType& at(const KeyType& key) {
			throw std::runtime_error("Not implemented.");
		};

		ValueType& at(const KeyType& key) const {
			throw std::runtime_error("Not implemented.");
		};




	};

}



#endif //LINKEDTABLE_H
