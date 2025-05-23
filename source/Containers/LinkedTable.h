#ifndef LINKEDTABLE_H
#define LINKEDTABLE_H
#include <functional>

namespace Containers {

	template <
		typename KeyType,
		typename ValueType,
		typename AllocatorType = std::allocator<std::pair<const KeyType, ValueType>>
	>
	class LinkedTable {
		struct Node;

		// TODO: Replace this with taking item type from allocator
		using ItemType = std::pair<const KeyType, ValueType>;

		using ItemAllocatorType = AllocatorType;
		using NodeAllocatorType = typename std::allocator_traits<AllocatorType>::template rebind_alloc<Node>;
		using NodeListAllocatorType = typename std::allocator_traits<AllocatorType>::template rebind_alloc<Node*>;

		struct Node {
			Node* next = nullptr;
			char itemBytes[sizeof(ItemType)];

			ItemType* itemPtr() { return static_cast<ItemType*>(&itemBytes); };
			const KeyType& key() { return this->itemPtr()->first; };
			ItemType& item() { return this->itemPtr()->second; };
		};

		ItemAllocatorType itemAllocator_;
		NodeAllocatorType nodeAllocator_;
		NodeListAllocatorType nodeListAllocator_;

		std::hash<KeyType> keyHash_;
		std::equal_to<KeyType> keyEqual_;

		Node** buckets_ = nullptr;
		size_t capacity_ = 0;
		size_t itemCount_ = 0;

	public:
		LinkedTable() : itemAllocator_(), nodeAllocator_(), nodeListAllocator_() {};

		explicit LinkedTable(const AllocatorType& allocator) :
			itemAllocator_(allocator),
			nodeAllocator_(this->itemAllocator_),
			nodeListAllocator_(this->itemAllocator_)
		{};
	};

}



#endif //LINKEDTABLE_H
