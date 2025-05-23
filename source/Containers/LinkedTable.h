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


		/**
		 * Takes key/value pair and tries to insert it into table.
		 * Important note that this doesn't check for fullness.
		 *
		 * \param key : unique identifier of pair
		 * \param value : value stored in pair
		 * \return true : if key/value were successfully inserted
		 * \return false : if same key already exists in table
		 */
		bool insert_(const KeyType& key, ValueType& value) {
			// calculate index and select node
			size_t index = this->keyHash_(key) % this->capacity_;
			Node* selectedNode = this->buckets_[index];

			// if there is already node, we need to check for duplicities
			if (selectedNode != nullptr) {
				auto activeNode = selectedNode;

				// cycle until we reach end of the node chain
				while (activeNode != nullptr) {
					// key is already in the table, return false
					if (this->keyEqual_(key, activeNode->key())) {
						return false;
					}

					// move to next node
					activeNode = activeNode->next;
				}
			}


			// create new node at the start of the chain in bucket
			this->buckets_[index] = std::allocator_traits<NodeAllocatorType>::template rebind_alloc<Node>(this->nodeAllocator_, 1);
			std::allocator_traits<NodeAllocatorType>::construct(this->nodeAllocator_, this->buckets_[index]);
			std::allocator_traits<ItemAllocatorType>::construct(this->itemAllocator_, this->buckets_[index]->itemPtr(), std::make_pair(key, value));

			// put previously first node into second position
			this->buckets_[index]->next = selectedNode;

			++this->itemCount_;

			return true;
		}



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
