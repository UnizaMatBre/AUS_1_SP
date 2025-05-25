#ifndef SORTING_H
#define SORTING_H

#include <iterator>

#include "Itertools.h"

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
	void selection_sort(SortedIterType start, SortedIterType end, ComparatorType comparator) {
		if(end - start < 0) {
			throw std::invalid_argument("Start and end don't create a valid range");
		}

		SortedIterType head = start;
		while (head != end) {
			SortedIterType minimum = head;

			for (auto current = (head + 1); current != end; ++current) {
				if (comparator(*current, *minimum) < 0) {
					minimum = current;
				}
			}

			// swap two values
			Algorithms::swap_iterators(head, minimum);

			// advance head forward
			++head;
		};


	};


	template<typename SortedIterType, typename ComparatorType>
	void quick_sort(SortedIterType start, SortedIterType end, ComparatorType comparator) {
		if(end <= start) {
			return;
		};

		// pick pivot - we will always pick first iter as pivot
		SortedIterType pivot = start;

		// pick iter next to pivot as "middle"
		// this will be the value that will be swaped around
		SortedIterType middle = (start + 1);

		// this is iter that is always behind middle
		// why? So that the iterator is not forced to implement subtraction operator
		SortedIterType prev_middle = start;

		// iterate from middle to end (we don't want to iterate from pivot)
		for (auto iter = middle; iter < end; ++iter) {
			// is current iterator less than pivot?
			if (comparator(*iter, *pivot) < 0)	{

				// swap current value and 'middle'
				Algorithms::swap_iterators(iter, middle);

				// store middle in prev_mide
				prev_middle = middle;

				// move middle to next element
				++middle;
			}
		}
		// swap pivot with value PREVIOUS to the middle
		// this is done because middle contains value LARGER than pivot and swaping with that would break it
		// but value BEFORE middle is smaller than pivot - that is because smaller value is swaped into place of previous middle
		Algorithms::swap_iterators(pivot, prev_middle);


		/*
		for (auto printIter = start; printIter < end; ++printIter) {
			std::cout << *printIter << " ";
		}
		std::cout << " :: "  << *middle << std::endl;
		*/

		// call quicksort on left and right part
		Algorithms::quick_sort(start, prev_middle, comparator);
		Algorithms::quick_sort(middle, end, comparator);
	}
};


#endif //SORTING_H
