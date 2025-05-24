#ifndef LINKEDTABLE_H
#define LINKEDTABLE_H
#include <functional>
#include <stdexcept>
#include <bits/locale_facets_nonio.h>

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
		struct Node;

		// TODO: Replace this with taking item type from allocator
		using ItemType = std::pair<const KeyType, ValueType>;

		using ItemAllocatorType = AllocatorType;
		using NodeAllocatorType = typename std::allocator_traits<AllocatorType>::template rebind_alloc<Node>;
		using NodeListAllocatorType = typename std::allocator_traits<AllocatorType>::template rebind_alloc<Node*>;

		/**
		 * Represents one node in bucket's linked list
		 *
		 * TODO: Maybe this entire thing could be replaced by linked list class? Idk
		 */
		struct Node {
			Node* next = nullptr;
			char itemBytes[sizeof(ItemType)];

			ItemType* itemPtr() { return reinterpret_cast<ItemType*>(&itemBytes); };
			const KeyType& key() { return this->itemPtr()->first; };
			ValueType& value() { return this->itemPtr()->second; };
		};

		// allocators
		ItemAllocatorType itemAllocator_;
		NodeAllocatorType nodeAllocator_;
		NodeListAllocatorType nodeListAllocator_;

		// tools for key indexing and comparing
		std::hash<KeyType> keyHash_;
		std::equal_to<KeyType> keyEqual_;

		// array of buckets and informations about it
		Node** buckets_ = nullptr;
		size_t capacity_ = 0;
		size_t itemCount_ = 0;


		void finalizeNode_(Node* node) {
			std::allocator_traits<ItemAllocatorType>::destroy(this->itemAllocator_, node->itemPtr());
			std::allocator_traits<NodeAllocatorType>::destroy(this->nodeAllocator_, node);
			std::allocator_traits<NodeAllocatorType>::deallocate(this->nodeAllocator_, node, 1);
		}


		/**
		 * Takes key/value pair and tries to insert it into table.
		 * Important note that this doesn't check for fullness.
		 *
		 * \param key : unique identifier of pair
		 * \param value : value stored in pair
		 * \return newNode : if key/value were successfully inserted
		 * \return nullptr : if same key already exists in table
		 */
		Node* insert_(const KeyType& key, const ValueType& value) {
			auto result = this->findNode_(key);

			// node does already? Return false
			if (result.first) {
				return nullptr;
			}

			// node doesn't exist? Create it and return true
			Node* newNode = std::allocator_traits<NodeAllocatorType>::allocate(this->nodeAllocator_, 1);
			std::allocator_traits<NodeAllocatorType>::construct(this->nodeAllocator_, newNode);
			std::allocator_traits<ItemAllocatorType>::construct(this->itemAllocator_, newNode->itemPtr(), std::make_pair(key, value));

			auto previousFirst = *result.second;
			*(result.second) = newNode;

			++this->itemCount_;

			return newNode;
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
					const KeyType& key = currentNode->key();
					ValueType& value = currentNode->value();
					this->insert_(key, value);

					// destroy current node
					this->finalizeNode_(currentNode);
				};
			};

			// deallocate the old bucket array
			std::allocator_traits<NodeListAllocatorType>::deallocate(this->nodeListAllocator_, oldBuckets, oldCapacity);
		};


		/**
		 * Searches for node with same key as passed one.
		 *
		 * @param key key we are looking for
		 * @return <true, Node**> when node is found - Node** points to said node
		 * @return <false, Node**> when node is not found - Node** points to place where new node can be inserted
		 */
		std::pair<bool, Node**> findNode_(const KeyType& key) {
			size_t index = this->keyHash_(key) % this->capacity_;

			Node* activeNode = this->buckets_[index];

			// if bucket node is not null, we need to check nodes there
			if (activeNode != nullptr) {
				while (activeNode != nullptr) {
					if (this->keyEqual_(key, activeNode->key())) {
						return std::make_pair(true, &activeNode);
					}
					activeNode = activeNode->next;
				}
			}

			// either bucket is null or no node had key we wanted
			return std::make_pair(false, &this->buckets_[index]);
		}

	public:
		LinkedTable() : itemAllocator_(), nodeAllocator_(), nodeListAllocator_() {};

		explicit LinkedTable(const AllocatorType& allocator) :
			itemAllocator_(allocator),
			nodeAllocator_(this->itemAllocator_),
			nodeListAllocator_(this->itemAllocator_)
		{};


		/**
		* Copy constructor
		*/
		LinkedTable(const LinkedTable& other) : itemAllocator_(other.itemAllocator_), nodeAllocator_(other.nodeAllocator_), nodeListAllocator_(other.nodeListAllocator_) {
			this->capacity_ = other.capacity_;
			this->itemCount_ = 0;
			this->buckets_ = std::allocator_traits<NodeListAllocatorType>::allocate(this->nodeListAllocator_, this->capacity_);

			for (size_t index = 0; index < other.capacity_; ++index) {
				Node* selectedNode = other.buckets_[index];
				Node* activeNode = selectedNode;

				while (activeNode != nullptr) {
					// remember active node and move to next one
					auto currentNode = activeNode;
					activeNode = activeNode->next;

					// insert current node
					const KeyType& key = currentNode->key();
					ValueType& value = currentNode->value();
					this->insert_(key, value);
				};
			};
		};


		/**
		 * Destroys table
		 */
		~LinkedTable() {
			for (size_t index = 0; index < this->capacity_; ++index) {
				Node* activeNode = this->buckets_[index];

				while (activeNode != nullptr) {
					// remember active node and move to next one
					auto currentNode = activeNode;
					activeNode = activeNode->next;

					// destroy current node
					this->finalizeNode_(currentNode);
				};
			};

			std::allocator_traits<NodeListAllocatorType>::deallocate(this->nodeListAllocator_, this->buckets_, this->capacity_);
		}

		/**
		 * Takes key/value pair and tries to insert it into table.
		 *
		 * \param key : unique identifier of pair
		 * \param value : value stored in pair
		 * \return value : inserted into tbe table
		 * \throw std::out_of_range : insertion failed because key already exists
		 */
		ValueType& insert(const KeyType& key, const ValueType& value) {
			this->resolveFullness_();
			auto result = this->insert_(key, value);

			if (result == nullptr) {
				// TODO: Is this good error?
				throw std::out_of_range("Key already exists.");
			}

			return result->value();
		};




		ValueType& operator[](const KeyType& key) {
			this->resolveFullness_();
			auto result = this->findNode_(key);

			// node exists? Return its value
			if (result.first) {
				return (*result.second)->value();
			}

			// node doesn't exist? Create it and return its value
			Node* newNode = std::allocator_traits<NodeAllocatorType>::allocate(this->nodeAllocator_, 1);
			std::allocator_traits<NodeAllocatorType>::construct(this->nodeAllocator_, newNode);

			// THIS REQUIRES THAT VALUE TYPE HAS NON-PARAM CONSTRUCTOR
			// IF IT DOESN'T, THEN IT IS YOUR FAULT YOU USE THIS UNSAFE METHOD
			std::allocator_traits<ItemAllocatorType>::construct(this->itemAllocator_, newNode->itemPtr(), std::make_pair(key, {}));

			auto previousFirst = *result.second;
			*(result.second) = newNode;

			return newNode->value();
		}



		ValueType& at(const KeyType& key) {
			this->resolveFullness_();

			auto result = this->findNode_(key);
			if (result.first == false) {
				throw std::out_of_range("Key not found");
			}

			return (*result.second)->value();
		}

		const ValueType& at(const KeyType& key) const {
			this->resolveFullness_();

			auto result = this->findNode_(key);
			if (result.first == false) {
				throw std::out_of_range("Key not found");
			}

			return (*result.second)->value();
		}
	};

}



#endif //LINKEDTABLE_H
