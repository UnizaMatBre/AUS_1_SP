#ifndef ARRAYLIST_H
#define ARRAYLIST_H

#include <memory>
#include <stdexcept>


namespace Containers {
	template <typename ItemType, typename Allocator = std::allocator<ItemType>>
	class ArrayList {
		Allocator allocator_;

		ItemType* items_ = nullptr;
		size_t capacity_ = 0;
		size_t size_ = 0;


	public:
		/**
		* Creates an empty ArrayList with default allocator
		*/
		explicit ArrayList() {}

		/**
		* Creates empty ArrayList using passed allocator
		*
		* \param allocator : allocator that will be used by collection
		*/
		explicit ArrayList(const Allocator& allocator) : allocator_(allocator) {}

		/**
		* Creates ArrayList with starting capacity and fills it with provided default value. Custom allocator can be provided.
		*
		* \param size : desired size of collection
		* \param defaultValue : value to which every position will be initialized
		* \param allocator : allocator that will be used by collection
		*/
		ArrayList(size_t size, ItemType defaultValue, const Allocator& allocator = Allocator()) : allocator_(allocator) {
			// allocation
			this->capacity_ = size;
			this->size_ = size;
			this->items_ = std::allocator_traits<Allocator>::allocate(this->allocator_, this->capacity_);

			// construction
			for (size_t index = 0; index < this->capacity_; ++index) {
				std::allocator_traits<Allocator>::construct(this->allocator_, &this->items_[index], defaultValue);
			}
		}

		/**
		* Destroys ArrayList
		*/
		~ArrayList() {
			// no items? Do nothing
			if(this->size_ == 0) {
				return;
			}

			// destroy stored objects - this process goes in reverse
			for (size_t index = this->size_ - 1; index > 0; --index) {
				std::allocator_traits<Allocator>::destroy(this->allocator_, &this->items_[index]);
			}

			// deallocate array itself
			std::allocator_traits<Allocator>::deallocate(this->allocator_, this->items_, this->capacity_);
			this->items_ = nullptr;
			this->capacity_ = 0;
			this->size_ = 0;
		}

		/**
		* Returns reference to item at specified index. Doesn't perform bound checking.
		*
		* \param index : index of requested item.
		*/
		ItemType& operator[](size_t index) {
			return this->items_[index];
		}

		/**
		* Returns constant reference to item at specified index. Doesn't perform bound checking.
		*
		* \param index : index of requested item.
		*/
		const ItemType& operator[](size_t index) const {
			return this->items_[index];
		}

		/**
		 * Returns size/number of items in this ArrayList
		 */
		size_t size() const {
			return this->size_;
		};

	};
};


#endif //ARRAYLIST_H
