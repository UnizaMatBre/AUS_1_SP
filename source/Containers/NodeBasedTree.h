#ifndef NODEBASEDTREE_H
#define NODEBASEDTREE_H

#include <memory>

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



	};

}

#endif //NODEBASEDTREE_H
