#ifndef LINKEDLISTTREE_H
#define LINKEDLISTTREE_H

#include <memory>


namespace Containers {
	template <typename ItemType, typename AllocatorType = std::allocator<ItemType>>
	class LinkedListTree {

		struct Node;

		using ItemAllocatorType = AllocatorType;
		using NodeAllocatorType = typename std::allocator_traits<AllocatorType>::template rebind_alloc<Node>;

		ItemAllocatorType itemAllocator_;
		NodeAllocatorType nodeAllocator_;
		Node* root_ = nullptr;

		/**
		* Represents node in tree, with links to parent, next sibling and first children
		* Nodes on same layer and with same parent form linked list, with first node in this list being referenced by parent's children link.
		*/
		struct Node {
			Node* parent = nullptr;
			Node* sibling = nullptr;
			Node* children = nullptr;

			ItemType value;

			explicit Node(const ItemType& value) : value(value) {}
		};

		//* Finalization of node tree is in outer class because otherwise we would need to pass allocator to Node
		void finalizeNode_(Node* node) {
			if (node == nullptr) {
				return;
			}

			this->finalizeNode_(node->children);
			node->children = nullptr;
			this->finalizeNode_(node->sibling);
			node->sibling = nullptr;

			std::allocator_traits<ItemAllocatorType>::destroy(itemAllocator_, node->value);
			std::allocator_traits<NodeAllocatorType>::destroy(this->nodeAllocator_, node);
			std::allocator_traits<NodeAllocatorType>::deallocate(this->nodeAllocator_, node);

		}


	public:
		/**
		* Creates empty tree with default allocator
		*/
		LinkedListTree() : itemAllocator_(), nodeAllocator_(this->itemAllocator_) {};

		/**
		* Creates empty tree with provided allocator
		*
		* \param allocator : allocator that will be used by tree
		*/
		LinkedListTree(const AllocatorType& allocator) : itemAllocator_(allocator), nodeAllocator_(this->itemAllocator_) {};

		/**
		* Creates tree and constructs root node using provided item
		*
		* \param item : value stored in root node
		* \param allocator : allocator that will be used by collection
		*/
		LinkedListTree(const ItemType& item, const AllocatorType& allocator = AllocatorType()) : LinkedListTree(allocator) {
			this->root_ = std::allocator_traits<NodeAllocatorType>::allocate(this->nodeAllocator_, 1);
			std::allocator_traits<NodeAllocatorType>::construct(this->nodeAllocator_, this->root_, item);
		};

		/**
		* Destroys tree
		*/
		~LinkedListTree() {
			this.finalizeNode_(this->root_);
			this->root_ = nullptr;
		}

	};

}


#endif //LINKEDLISTTREE_H
