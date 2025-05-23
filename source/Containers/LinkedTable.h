#ifndef LINKEDTABLE_H
#define LINKEDTABLE_H
#include <functional>
#include <stdexcept>

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

		/**
		 * Checks if table is too full and expands it if needed
		 */
		void resolveFullness_() {
			// we will resize ONLY if item count is at least 80% of capacity
			if (this->itemCount_ < static_cast<size_t>(this->capacity_ * 0.8)) {
				return;
			}

			// store old internals
			Node** oldBuckets = this->buckets_;
			size_t oldCapacity = this->capacity_;

			// create new internals
			this->itemCount_ = 0;
			this->capacity_ = (oldCapacity == 0) ? 8 : (oldCapacity * 2);
			this->buckets_ = std::allocator_traits<NodeListAllocatorType>::allocate(this->nodeListAllocator_, this->capacity_);
			for (size_t index = 0; index < this->capacity_; ++index) {
				this->buckets_[index] = nullptr;
			}

			// insert any values from old buckets and destroys them afterward
			for (size_t index = 0; index < oldCapacity; ++index) {
				Node* selectedNode = oldBuckets[index];
				Node* activeNode = selectedNode;

				while (activeNode != nullptr) {
					// remember active node and move to next one
					auto currentNode = activeNode;
					activeNode = activeNode->next;

					// insert current node
					this->insert_(currentNode->key(), currentNode->value());

					// destroy current node
					std::allocator_traits<ItemAllocatorType>::destroy(this->itemAllocator_, currentNode->itemPtr());
					std::allocator_traits<NodeAllocatorType>::destroy(this->nodeAllocator_, currentNode);
					std::allocator_traits<NodeAllocatorType>::deallocate(this->nodeAllocator_, currentNode, 1);
				};
			};

			// deallocate the old bucket array
			std::allocator_traits<NodeListAllocatorType>::deallocate(this->nodeListAllocator_, oldBuckets, oldCapacity);
		};


	public:
		LinkedTable() : itemAllocator_(), nodeAllocator_(), nodeListAllocator_() {};

		explicit LinkedTable(const AllocatorType& allocator) :
			itemAllocator_(allocator),
			nodeAllocator_(this->itemAllocator_),
			nodeListAllocator_(this->itemAllocator_)
		{};

		/**
		 * Takes key/value pair and tries to insert it into table.
		 *
		 * \param key : unique identifier of pair
		 * \param value : value stored in pair
		 * \return true : if key/value were successfully inserted
		 * \return false : if same key already exists in table
		 */
		bool insert(const KeyType& key, ValueType& value) {
			this->resolveFullness_();
			return this->insert_(key, value);
		};




		ValueType& operator[](const KeyType& key) {
			throw std::runtime_error("Not implemented yet");
		}

		const ValueType& operator[](const KeyType& key) const {
			throw std::runtime_error("Not implemented yet");
		}

		ValueType& at(const KeyType& key) {
			throw std::runtime_error("Not implemented yet");
		}

		const ValueType& at(const KeyType& key) const {
			throw std::runtime_error("Not implemented yet");
		}
	};

}



#endif //LINKEDTABLE_H
