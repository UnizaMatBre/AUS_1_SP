#ifndef MAP_H
#define MAP_H

#include <memory>
#include <functional>


namespace Containers {
	template<
		typename KeyType,
		typename ValueType,
		typename AllocatorType = std::allocator<std::pair<const KeyType, ValueType>>
	>
	class Table {
		struct Bucket;

		using ItemType = std::pair<const KeyType, ValueType>;

		using ItemAllocatorType = AllocatorType;
		using BucketAllocatorType = typename std::allocator_traits<AllocatorType>::template rebind_alloc<Bucket>;


		/**
		* Represents bucket in open addressing. Can be either empty (no value, can insert), valuable (has value) or deleted (no value, insertion prohibited)
		*/
		struct Bucket {
			enum class Status { Empty, Valuable, Deleted };
			Status status;

			// this is done to prevent automatic construction of item
			char itemBytes[sizeof(ItemType)];

			ItemType* itemPtr() {
				return reinterpret_cast<ItemType>(&itemBytes);
			}
		};

		ItemAllocatorType itemAllocator_;
		BucketAllocatorType bucketAllocator_;

		std::hash<KeyType> keyHash_;
		std::equal_to<KeyType> keyEqual_;

		Bucket* buckets_ = nullptr;
		size_t capacity_ = 0;
		size_t occupied_ = 0;

	public:
		/**
		 * Creates empty table with default constructed allocators and tools
		 */
		Table() : itemAllocator_(), bucketAllocator_(this.itemAllocator_) {}


		/**
		* Creates empty table with provided allocator
		*
		* \param allocator : allocator that will be used by table
		*/
		Table(const AllocatorType& allocator) : itemAllocator_(allocator), bucketAllocator_(this->itemAllocator_) {};

		/**
		* Destroys table
		*/
		~Table() {
			if (this.buckets_ == nullptr) {
				return;
			}

			for (size_t index = 0; index < this->capacity_; ++index) {
				Bucket* bucket = &this->buckets_[index];

				if (bucket->status == Bucket::Status::Valuable) {
					std::allocator_traits<ItemAllocatorType>::destroy(this->itemAllocator_, bucket->itemPtr());
				}
				std::allocator_traits<BucketAllocatorType>::destroy(this->bucketAllocator_, bucket);
			}

			std::allocator_traits<BucketAllocatorType>::deallocate(this->bucketAllocator_, this->buckets_, this->capacity_);
		}
	};
}

#endif //MAP_H
