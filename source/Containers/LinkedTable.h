#ifndef LINKEDTABLE_H
#define LINKEDTABLE_H
#include <functional>
#include <stdexcept>
#include <bits/locale_facets_nonio.h>

namespace Containers {
	/**
	 * Represents hash table with efficient access to its members.
	 * Internally utilizes separate chaining (i.e keys that fall into same bucket create linked list)
	 *
	 * \tparam KeyType : type of key object that identifies pair
	 * \tparam ValueType : type of value stored in pair
	 * \tparam AllocatorType : type of allocator used by table internally
	 */
	template <
		typename KeyType,
		typename ValueType,
		typename AllocatorType = std::allocator<std::pair<const KeyType, ValueType>>
	>
	class LinkedTable {
	public:
		LinkedTable() {
			throw std::runtime_error("Not implemented.");
		};

		LinkedTable(const AllocatorType& allocator) {
			throw std::runtime_error("Not implemented.");
		};

		LinkedTable(const LinkedTable<KeyType, ValueType, AllocatorType>& other) {
			throw std::runtime_error("Not implemented.");
		};

		~LinkedTable() {
			throw std::runtime_error("Not implemented.");
		};

		ValueType& insert(const KeyType& key, const ValueType& value) {
			throw std::runtime_error("Not implemented.");
		};

		ValueType& at(const KeyType& key) {
			throw std::runtime_error("Not implemented.");
		};

		ValueType& at(const KeyType& key) const {
			throw std::runtime_error("Not implemented.");
		};




	};

}



#endif //LINKEDTABLE_H
