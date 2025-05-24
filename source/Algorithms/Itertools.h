#ifndef ITERTOOLS_H
#define ITERTOOLS_H

namespace Algorithms {
	/**
	 * Swaps values accessed in passed iterators
	 *
	 * \param iter1 first iterator whose value will be swaped
	 * \param iter2 second iterator whose value will be swaped
	 */
	template<typename IterType1, typename IterType2>
	void swap_iterators(IterType1 iter1, IterType2 iter2) {
		auto tmp = *iter1;
		*iter1 = *iter2;
		*iter2 = tmp;
	}

}


#endif //ITERTOOLS_H
