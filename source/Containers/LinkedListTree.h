#ifndef LINKEDLISTTREE_H
#define LINKEDLISTTREE_H

#include <memory>

#include "LinkedList.h"


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


		class Iterator {
			Node* position_;
			LinkedListTree& myTree_;

			// NOTE: having std::list here is probably not allowed. Good think i made custom one
			// "but it doesn't take allocator from Tree" ssshhh
			Containers::LinkedList<Node*> queue_;
		public:
			using value_type		= ItemType;
			using pointer			= ItemType*;
			using reference			= ItemType&;
			using difference_type	= std::ptrdiff_t;

			Iterator(Node* position, LinkedListTree& myTree) : position_(position), myTree_(myTree) {};

			reference operator*() {
				return this->position_->value;
			};

			pointer operator->() {
				return &this->position_->value;
			};

			/**
			* Moves to next element using breadth-first search
			*/
			Iterator& operator++() {
				// position is empty?
				if (this->position_ == nullptr) {
					return *this;
				};

				// has current node any children?
				if (this->position_->children != nullptr) {
					// true: put that children for later search
					this->queue_.push_back(this->position_->children);
				};

				// has current node siblings?
				if (this->position_->sibling != nullptr) {
					// true: move to sibling
					this->position_ = this->position_->sibling;
				};
				else {
					// false: check if queue is empty
					if (this->queue_.empty()) {
						// true: we are finished
						this->position_ = nullptr;
					}
					else {
						// false: move to top of queue
						this->position_ = this->queue_.front();
						this->queue_.pop_front();
					};
				};
				return *this;
			};

			Iterator operator++(int) {
				Iterator tmp = *this;
				++*this;
				return tmp;
			};


			bool operator==(const Iterator& other) const {
				return this->position_ == other.position_;
			};
			bool operator!=(const Iterator& other) const {
				return this->position_ != other.position_;
			};
		};

	};

}


#endif //LINKEDLISTTREE_H
