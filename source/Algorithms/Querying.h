#ifndef QUERYING_H
#define QUERYING_H

#include <stdexcept>
#include <iterator>

namespace Algorithms {
	/**
	*
	* Gets values from source collection and puts those that satisfy selector into target colection
	*
	* \tparam InputIterType : iterator type of collection from which function gets items
	* \tparam OutputIterType : iterator type of collection into which function puts items
	* \tparam UnaryOperation : callable type used to select items
	*
	* \param sourceStart : iterator pointing to the beginning of the source collection
	* \param sourceEnd : iterator pointing to the end of the source collection
	* \param targetCurrent : iterator pointing to the collection where we want to put selected items.
	* \param selector : callable object which select valid items. It has one parameter
	*/
	template<typename InputIterType, typename OutputIterType, typename UnaryOperation>
	void select(InputIterType sourceStart, InputIterType sourceEnd, OutputIterType targetCurrent, UnaryOperation selector) {
		/*
		if(sourceEnd - sourceStart < 0) {
			throw std::invalid_argument("Source start and end don't create a valid range");
		}
		*/

		while(sourceStart != sourceEnd) {
			typename std::iterator_traits<InputIterType>::value_type item = *sourceStart;

			if(selector(item)) {
				*targetCurrent = item;
				++targetCurrent;
			}

			++sourceStart;
		}
	}

}



#endif //QUERYING_H
