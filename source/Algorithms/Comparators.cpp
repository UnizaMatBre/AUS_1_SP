#include "Comparators.h"

#include <stdexcept>

int Algorithms::CompareAlphabetical(const DataHandling::LandUnitData &left, const DataHandling::LandUnitData &right) {
	return left.get_name().compare(right.get_name());
};


bool Algorithms::ComparePopulation::operator()(const DataHandling::LandUnitData& left, const DataHandling::LandUnitData& right) const {
	switch (this->category_) {
		case ComparePopulation::Category::Male: {
			return left.get_male_population_at(this->index_) - right.get_male_population_at(this->index_);
		};

		case ComparePopulation::Category::Female: {
			return left.get_female_population_at(this->index_) - right.get_female_population_at(this->index_);
		};
		case ComparePopulation::Category::Both: {
			return left.get_total_population_at(this->index_) - right.get_total_population_at(this->index_);
		}
		default: {
			throw std::invalid_argument("Unexpected category.");
		};
	}
}
