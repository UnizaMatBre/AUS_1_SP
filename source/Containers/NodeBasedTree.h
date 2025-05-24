#ifndef NODEBASEDTREE_H
#define NODEBASEDTREE_H

#include <memory>

#include "LinkedList.h"

namespace Containers {
	/**
	 * Represents singular tree node.
	 */
	template<typename ItemType, typename AllocatorType = std::allocator<ItemType>>
	class TreeNode {
		using MyType = TreeNode<ItemType, AllocatorType>;

		using NodeAllocatorType = typename std::allocator_traits<AllocatorType>::template rebind_alloc<MyType>;

		NodeAllocatorType nodeAllocator_;

		TreeNode* parent_ = nullptr;
		TreeNode* sibling_ = nullptr;
		TreeNode* children_ = nullptr;

		ItemType item_;

		void finalize_node_(MyType* node) {
			if (node == nullptr) {
				return;
			}

			std::allocator_traits<NodeAllocatorType>::destroy(this->nodeAllocator_, node);
			std::allocator_traits<NodeAllocatorType>::deallocate(this->nodeAllocator_, node, 1);
		}

	public:
		explicit TreeNode(ItemType& item, const AllocatorType& allocator = AllocatorType()) : nodeAllocator_(allocator), item_(item) {}

		~TreeNode() {
			this->finalize_node_(this->children_);
			this->finalize_node_(this->sibling_);
		}


		void push_back_children(ItemType& item) {
			MyType* newNode = std::allocator_traits<NodeAllocatorType>::allocate(this->nodeAllocator_, 1);
			std::allocator_traits<NodeAllocatorType>::construct(this->nodeAllocator_, newNode, item);

			// has no children? Then this is our first one
			if (this->children_ == nullptr) {
				this->children_ = newNode;
				return;
			};

			// has children? Go to the end and append it there
			MyType* currentChildren = this->children_;
			while (currentChildren->sibling_ != nullptr) {
				currentChildren = currentChildren->sibling_;
			}
			currentChildren->sibling_ = newNode;
		}

		TreeNode* get_parent() const {
			return this->parent_;
		}

		TreeNode* get_children() const {
			return this->children_;
		}

		ItemType& get_item() const {
			return this->item_;
		}




		class Iterator {
			MyType* position_;
			Containers::LinkedList<MyType*, AllocatorType> queue_;


		public:
			using iterator_category = std::bidirectional_iterator_tag;

			using value_type = ItemType;
			using pointer = ItemType*;
			using reference = ItemType&;
			using difference_type = std::ptrdiff_t;


			Iterator(MyType* position, const AllocatorType& allocator) : position_(position), queue_(allocator) {}


			reference operator*() {
				return this->position_->item_;
			};

			pointer operator->() {
				return &this->position_->item_;
			};

			Iterator& operator++() {
				if (this->position_ == nullptr) {
					return *this;
				}

				// has current node any children?
				if (this->position_->get_children() != nullptr) {
					// true: put that children for later search
					this->queue_.push_back(this->position_->get_children());
				};

				// has current node siblings after it?
				if (this->position_->sibling_ != nullptr) {
					// true: move to sibling
					this->position_ = this->position_->sibling_;
				}
				else {
					// false: check if queue is empty
					if (this->queue_.empty()) {
						// true: we are finished
						this->position_ = nullptr;
					}
					else {
						// false: move to top of queue
						this->position_ = this->queue_[0];
						this->queue_.pull_front();
					};
				};
				return *this;
			};


		};

	};

}

#endif //NODEBASEDTREE_H
