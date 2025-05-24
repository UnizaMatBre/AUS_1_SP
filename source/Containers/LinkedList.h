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

				std::allocator_traits<ItemAllocatorType>::destroy(this->itemAllocator_, node->value);
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

		/**
		 * Destroys linked list
		 */
		~LinkedList() {
			if (this->front_ != nullptr) {
				this->finishNode_(this->front_);
				this->front_ = nullptr;
			}

			this->back_ = nullptr;
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
					throw std::out_of_range();
				}
				current = current->next;
			}
			return current->value;
		}


	};

};


#endif //LINKEDLIST_H
