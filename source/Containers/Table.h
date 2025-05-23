#ifndef MAP_H
#define MAP_H

#include <memory>
#include <functional>
#include <iostream>


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
			Status status = Status::Empty;

			// this is done to prevent automatic construction of item
			char itemBytes[sizeof(ItemType)];

			ItemType* itemPtr() {
				return reinterpret_cast<ItemType*>(&this->itemBytes);
			}

			const KeyType& key() { return this->itemPtr()->first; }
			ValueType& value() { return this->itemPtr()->second; }
		};

		ItemAllocatorType itemAllocator_;
		BucketAllocatorType bucketAllocator_;

		std::hash<KeyType> keyHash_;
		std::equal_to<KeyType> keyEqual_;

		Bucket* buckets_ = nullptr;
		size_t capacity_ = 0;
		size_t occupied_ = 0;


		/**
		 * Checks if map is full enough and if yes, expands internal bucket array
		 */
		void resolveFullness_() {
			if (this->occupied_ < static_cast<size_t>(this->capacity_ * 0.8)) {
				return;
			}

			size_t oldCapacity = this->capacity_;
			Bucket* oldBuckets = this->buckets_;

			this->capacity_ = (oldCapacity == 0) ? 8 : oldCapacity * 2;
			this->occupied_ = 0;
			this->buckets_ = std::allocator_traits<BucketAllocatorType>::allocate(this->bucketAllocator_, this->capacity_);
			for (size_t index = 0; index < this->capacity_; ++index) {
				std::allocator_traits<BucketAllocatorType>::construct(this->bucketAllocator_, &this->buckets_[index]);
			}

			for (size_t index = 0; index < oldCapacity; ++index) {
				Bucket* oldBucket = &oldBuckets[index];

				if (oldBucket->status == Bucket::Status::Valuable) {
					size_t itemIndex = this->keyHash_(oldBucket->key()) % this->capacity_;

					while (this->buckets_[itemIndex].status != Bucket::Status::Empty) {
						itemIndex = (itemIndex + 1) % this->capacity_;
					}

					std::allocator_traits<BucketAllocatorType>::construct(
						this->bucketAllocator_, &this->buckets_[index]
					);

					std::allocator_traits<ItemAllocatorType>::construct(
						this->itemAllocator_, this->buckets_[index].itemPtr()
					);

					this->buckets_[index].status = Bucket::Status::Valuable;
					++this->occupied_;
				}
			}
		}

	public:
		/**
		 * Creates empty table with default constructed allocators and tools
		 */
		Table() : itemAllocator_(), bucketAllocator_(this->itemAllocator_) {}


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
			if (this->buckets_ == nullptr) {
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



		bool insert(const KeyType& key, ValueType& value) {
			this->resolveFullness_();

			size_t index = this->keyHash_(key) % this->capacity_;

			while (this->buckets_[index].status != Bucket::Status::Empty) {
				if (this->keyEqual_(key, this->buckets_[index].key())) {
					return false;
				}

				index = (index + 1) % this->capacity_;
			}

			std::allocator_traits<ItemAllocatorType>::construct(
				this->itemAllocator_,
				this->buckets_[index].itemPtr(),
				std::make_pair(key, value)
			);

			this->buckets_[index].status = Bucket::Status::Valuable;
			++this->occupied_;

			return true;
		}



		void dumpInternals() {
			for (size_t index = 0; index < this->capacity_; ++index) {
				if (this->buckets_[index].status == Bucket::Status::Valuable) {
					std::cout << this->buckets_[index].key()  << " -> " << this->buckets_[index].value() << std::end;
				}
				else {
					std::cout << "empty" << std::endl;
				}

			}

		}
	};
}

#endif //MAP_H
