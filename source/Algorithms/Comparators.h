#ifndef COMPARATORS_H
#define COMPARATORS_H

#include "../DataHandling/LandUnitData.h"

namespace Algorithms {


	/**
	* Represents comparator that compares two units by their name alphabetically
	*/
	class CompareAlphabetical {
	public:
		int operator()(const DataHandling::LandUnitData& left, const DataHandling::LandUnitData& right) const;

		int operator()(const DataHandling::LandUnitData* left, const DataHandling::LandUnitData* right) {
			return this->operator()(*left, *right);
		};
	};

	/**
	 * Represents comparator that compares two units by population in specific year.
	 */
	class ComparePopulation {
	public:
		enum class Category {Male = 0, Female = 1, Both = 2};

	private:
		const size_t index_;
		const Category category_;

	public:
		static ComparePopulation InYear(const size_t year, const Category category) {
			return {2020 - year, category};
		};
		ComparePopulation(const size_t index, const Category category) : index_(index), category_(category) {};
		bool operator()(const DataHandling::LandUnitData& left, const DataHandling::LandUnitData& right) const;

		bool operator()(DataHandling::LandUnitData* left, DataHandling::LandUnitData* right) const {
			return operator()(*left, *right);
		}
	};

};

#endif //COMPARATORS_H
