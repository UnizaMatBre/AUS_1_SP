#ifndef SORTING_H
#define SORTING_H
#include <iterator>

namespace Algorithms {
	/**
	 *
	 * \tparam SortedIterType type of iterators defining range on which sorting is performed
	 * \tparam ComparatorType type of comparator used to determine ordering
	 *
	 * \param begin start of to-be-sorted range
	 * \param end end of to-be-sorted range
	 * \param comparator callable with 2 parameters which determined ordering (returns integer)
	 */
	template<typename SortedIterType, typename ComparatorType>
	void selectionSort(SortedIterType start, SortedIterType end, ComparatorType comparator) {
		if(end - start < 0) {
			throw std::invalid_argument("Start and end don't create a valid range");
		}

		SortedIterType head = start;
		while (head != end) {
			SortedIterType minimum = head;

			for (auto current = (head + 1); current != end; ++current) {
				int difference = comparator(*minimum, *current);

				if (difference > 0) {
					minimum = current;
				}
			}

			// swap two values
			auto tmp = *head;
			*head = *minimum;
			*minimum = tmp;

			// advance head forward
			++head;
		};


	};
};


#endif //SORTING_H
