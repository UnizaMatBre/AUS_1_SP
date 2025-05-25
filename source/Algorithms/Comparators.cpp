#include "Comparators.h"

#include <stdexcept>

int Algorithms::CompareAlphabetical::operator()(const DataHandling::LandUnitData &left, const DataHandling::LandUnitData &right) const {
	return left.get_name().compare(right.get_name());
};


int Algorithms::ComparePopulation::operator()(const DataHandling::LandUnitData& left, const DataHandling::LandUnitData& right) const {
	int left_int = -1;;
	int right_int = -1;

	switch (this->category_) {
		case ComparePopulation::Category::Male: {
			left_int = left.male_population_at(this->index_);
			right_int = right.male_population_at(this->index_);
			break;
		};

		case ComparePopulation::Category::Female: {
			left_int = left.female_population_at(this->index_);
			right_int = right.female_population_at(this->index_);
			break;
		};
		case ComparePopulation::Category::Both: {
			left_int = left.get_total_population_at(this->index_);
			right_int = right.get_total_population_at(this->index_);
			break;
		}
		default: {
			throw std::invalid_argument("Unexpected category.");
		};
	}

	return left_int - right_int;
}
