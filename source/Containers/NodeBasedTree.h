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
