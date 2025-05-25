#include "Comparators.h"

#include <stdexcept>

int Algorithms::CompareAlphabetical::operator()(const DataHandling::LandUnitData &left, const DataHandling::LandUnitData &right) const {
	return left.get_name().compare(right.get_name());
};


bool Algorithms::ComparePopulation::operator()(const DataHandling::LandUnitData& left, const DataHandling::LandUnitData& right) const {
	switch (this->category_) {
		case ComparePopulation::Category::Male: {
			return left.male_population_at(this->index_) - right.male_population_at(this->index_);
		};

		case ComparePopulation::Category::Female: {
			return left.female_population_at(this->index_) - right.female_population_at(this->index_);
		};
		case ComparePopulation::Category::Both: {
			return left.get_total_population_at(this->index_) - right.get_total_population_at(this->index_);
		}
		default: {
			throw std::invalid_argument("Unexpected category.");
		};
	}
}
