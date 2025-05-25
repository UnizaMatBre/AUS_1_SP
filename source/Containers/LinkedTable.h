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
			const KeyType& key() const { return item.first; }
			ValueType& value() { return item.second; }
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


		/**
		* Checks if table is too full and expands it if needed
		* NOTE: We actually don't need to destroy old nodes - we can reuse them with new buckets
		*/
		void resolve_fullness_() {
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

			// initialize new buckets into null (just in case)
			for (size_t index = 0; index < this->capacity_; ++index) {
				this->buckets_[index] = nullptr;
			}

			// insert any values from old buckets and destroys them afterward
			for (size_t index = 0; index < oldCapacity; ++index) {
				Node* current_node = oldBuckets[index];

				while (current_node != nullptr) {
					auto next_node = current_node->next;

					// find nice new place for node in new buckets
					auto new_location_index = this->find_node_(current_node->key()).first;

					current_node->next = this->buckets_[new_location_index];
					this->buckets_[new_location_index] = current_node;

					++this->itemCount_;

					current_node = next_node;
				};

				oldBuckets[index] = nullptr;
			};

			// deallocate the old bucket array
			std::allocator_traits<NodeListAllocatorType>::deallocate(this->nodeListAllocator_, oldBuckets, oldCapacity);
		};

		/**
		 * Searches for node with same key as passed one. DOESN'T CHECK IF BUCKETS ARE EMPTY
		 *
		 * @param key key we are looking for
		 * @return <0, Node*> when node is found - Node* point at it
		 * @return <index, nullptr> when node is not found - index specified best bucket to insert new node
		 */
		std::pair<int, Node*> find_node_(const KeyType& key) {
			size_t index = this->keyHash_(key) % this->capacity_;

			Node* activeNode = this->buckets_[index];

			// if bucket node is not null, we need to check nodes there
			if (activeNode != nullptr) {
				while (activeNode != nullptr) {
					if (this->keyEqual_(key, activeNode->key())) {
						return std::make_pair(0, activeNode);
					}
					activeNode = activeNode->next;
				}
			}

			// either bucket is null or no node had key we wanted
			return std::make_pair(index, nullptr );
		}

		void finalize_node_(Node* node) {
			std::allocator_traits<NodeAllocatorType>::destroy(this->nodeAllocator_, node);
			std::allocator_traits<NodeAllocatorType>::deallocate(this->nodeAllocator_, node, 1);
		};

		void finalize_node_chain_(Node* node) {
			if (node == nullptr) { return; }
			this->finalize_node_chain_(node->next);

			std::allocator_traits<NodeAllocatorType>::destroy(this->nodeAllocator_, node);
			std::allocator_traits<NodeAllocatorType>::deallocate(this->nodeAllocator_, node, 1);
		}

	public:
		LinkedTable(): nodeAllocator_(), nodeListAllocator_(this->nodeAllocator_) {};

		LinkedTable(const AllocatorType& allocator) : nodeAllocator_(allocator), nodeListAllocator_(this->nodeAllocator_) {};

		LinkedTable(const LinkedTable<KeyType, ValueType, AllocatorType>& other) {
			throw std::runtime_error("Not implemented.");
		};

		~LinkedTable() {
			if (this->buckets_ == nullptr) {
				return;
			}

			for (size_t index = 0; index < this->capacity_; ++index) {
				if (this->buckets_[index] != nullptr) {
					this->finalize_node_chain_(this->buckets_[index]);
				}
			}
			std::allocator_traits<NodeListAllocatorType>::deallocate(this->nodeListAllocator_, this->buckets_, this->capacity_);
			this->capacity_ = 0;
			this->itemCount_ = 0;
		};

		ValueType& insert(const KeyType& key, const ValueType& value) {
			this->resolve_fullness_();

			auto find_result = this->find_node_(key);
			if (find_result.second != nullptr) {
				throw std::out_of_range("Key '" + key + "' already exists.");
			}

			Node* new_node = std::allocator_traits<NodeAllocatorType>::allocate(this->nodeAllocator_, 1);
			std::allocator_traits<NodeAllocatorType>::construct(this->nodeAllocator_, new_node, std::make_pair(key, value));

			new_node->next = this->buckets_[find_result.first];
			this->buckets_[find_result.first] = new_node;

			++this->itemCount_;

			return new_node->value();
		};

		ValueType& at(const KeyType& key) {
			auto find_result = this->find_node_(key);
			if (find_result.second == nullptr) {
				throw std::out_of_range("Key '" + key + "' doesn't exists.");
			}

			return find_result.second->value();
		};

		ValueType& at(const KeyType& key) const {
			auto find_result = this->find_node_(key);
			if (find_result.second == nullptr) {
				throw std::out_of_range("Key '" + key + "' doesn't exists.");
			}

			return find_result.second->value();
		};




	};

}



#endif //LINKEDTABLE_H
