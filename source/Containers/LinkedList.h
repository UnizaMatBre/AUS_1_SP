#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <memory>
#include <stdexcept>


namespace Containers {

	template <typename ItemType, typename AllocatorType = std::allocator<ItemType> >
	class LinkedList {
		struct Node {
			Node* next = nullptr;
			ItemType value;

			Node(const ItemType& value) : value(value) {};
		};

		using MyType = LinkedList<ItemType, AllocatorType>;
		using ItemAllocatorType = AllocatorType;
		using NodeAllocatorType = typename std::allocator_traits<AllocatorType>::template rebind_alloc<LinkedList::Node>;


		ItemAllocatorType itemAllocator_;
		NodeAllocatorType nodeAllocator_;

		Node* front_ = nullptr;
		Node* back_ = nullptr;

		// this is done in outer class so we don't need to pass allocator to node
		void finishNode_(Node* node) {
			if (node != nullptr) {
				this->finishNode_(node->next);

				std::allocator_traits<NodeAllocatorType>::destroy(this->nodeAllocator_, node);
				std::allocator_traits<NodeAllocatorType>::deallocate(this->nodeAllocator_, node, 1);
			}
		}

	public:

		/**
		* Creates empty collection with default constructed allocator
		*/
		LinkedList() : itemAllocator_(), nodeAllocator_(this->itemAllocator_) {};

		/**
		* Creates empty collection with provided allocator
		*
		* \param allocator : allocator that will be used by collection
		*/
		LinkedList(const AllocatorType& allocator) : itemAllocator_(allocator), nodeAllocator_(this->itemAllocator_) {};


		LinkedList(const MyType& other) : itemAllocator_(other.itemAllocator_), nodeAllocator_(other.nodeAllocator_) {
			if (other.front_ == nullptr) {
				return;
			}

			auto other_first_node = other.front_;

			auto this_first_node = std::allocator_traits<NodeAllocatorType>::allocate(this->nodeAllocator_, 1);
			std::allocator_traits<NodeAllocatorType>::construct(this->nodeAllocator_, this_first_node, other_first_node->value);

			auto active_node = this_first_node;

			while (other_first_node->next != nullptr) {
				other_first_node = other_first_node->next;

				active_node->next = std::allocator_traits<NodeAllocatorType>::allocate(this->nodeAllocator_, 1);
				std::allocator_traits<NodeAllocatorType>::construct(this->nodeAllocator_, active_node, other_first_node->value);

				active_node = active_node->next;
			};

			this->front_ = this_first_node;
			this->back_ = active_node;
		};
		/**
		 * Destroys linked list
		 */
		~LinkedList() {
			this->clear();
		}

		/**
		 * Insert item to the back of list.
		 *
		 * \param value : item to be inserted
		 * \return value : item that is now stored inside of list
		 */
		ItemType& push_back(const ItemType& value) {
			// create new node
			Node* newNode = std::allocator_traits<NodeAllocatorType>::allocate(this->nodeAllocator_, 1);
			std::allocator_traits<NodeAllocatorType>::construct(this->nodeAllocator_, newNode, value);

			if (this->front_ == nullptr) {
				this->front_ = newNode;
				this->back_ = newNode;
			}
			else {
				this->back_->next = newNode;
				this->back_ = newNode;
			}

			return newNode->value;
		}

		/**
		 *	Removes item from the fron of the list. Throws error if list is empty
		 *
		 *	\throw std::out_of_range : list is empty
		 */
		void pull_front() {
			if (this->front_ == nullptr) {
				throw std::out_of_range("Cannot pull from empty list.");
			}

			Node* tobeDeleted = this->front_;

			if (this->front_ == this->back_) {
				this->front_ = nullptr;
				this->back_ = nullptr;
			}
			else {
				this->front_ = this->front_->next;
			}

			std::allocator_traits<NodeAllocatorType>::destroy(this->nodeAllocator_, tobeDeleted);
			std::allocator_traits<NodeAllocatorType>::deallocate(this->nodeAllocator_, tobeDeleted, 1);
		}


		void clear() {
			this->finishNode_(this->front_);

			this->front_ = nullptr;
			this->back_ = nullptr;
		}


		/**
		 * Checks if list is empty.
		 *
		 * @return true if there are no items, false otherwise
		 */
		bool empty() {
			return this->front_ == nullptr;
		}



		/**
		* Returns constant reference to item at specified index. Doesn't perform bound checking.
		*
		* \param index : index of requested item.
		* \return item at specified index
		*/
		ItemType& operator[](const size_t index) {
			Node* current = this->front_;

			for (int counter = 0; counter < index; ++counter) {
				current = current->next;
			}
			return current->value;
		}


		/**
		* Returns constant reference to item at specified index. Performs bound checking.
		*
		* \param index : index of requested item.
		* \return item at specified index
		* \throw std::out_of_range index is larger than number of nodes
		*/
		ItemType& at(const size_t index) {
			Node* current = this->front_;

			for (int counter = 0; counter < index; ++counter) {
				if (current == nullptr) {
					throw std::out_of_range("Index out of range.");
				}
				current = current->next;
			}
			return current->value;
		}

		class Iterator {
			Node* position_;

		public:
			using iterator_category = std::bidirectional_iterator_tag;

			using value_type = ItemType;
			using pointer = ItemType*;
			using reference = ItemType&;
			using difference_type = std::ptrdiff_t;


			Iterator(Node* node) : position_(node) {};

			reference operator*() const {
				return *this->position_->value;
			}

			pointer operator->() {
				return this->position_->value;
			}

			Iterator& operator++() {
				this->position_ = this->position_->next;

				return *this;
			}

			Iterator operator++(int) {
				auto old = *this;

				++(*this);

				return old;
			};

			bool operator==(const Iterator& other) {
				return this->position_ == other.position_;
			}

			bool operator!=(const Iterator& other) {
				return this->position_ != other.position_;
			}
		};


		Iterator begin() {
			return Iterator(this->front_);
		};
		Iterator end() {
			return Iterator(nullptr);
		};



	};
};


#endif //LINKEDLIST_H
