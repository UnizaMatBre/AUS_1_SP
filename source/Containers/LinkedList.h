#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#include <memory>

#include <memory>

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
			if (node->next != nullptr) {
				this->finishNode_(node->next);

				std::allocator_traits<ItemAllocatorType>::destroy(this->itemAllocator_, node->value);
				std::allocator_traits<NodeAllocatorType>::destroy(this->nodeAllocator_, node);
				std::allocator_traits<NodeAllocatorType>::deallocate(this->nodeAllocator_, node, 1);
			}
		}

	public:

	};

};


#endif //LINKEDLIST_H
