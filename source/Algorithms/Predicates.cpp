#include "Predicates.h"


bool Algorithms::ContainsSubstringInName::operator()(DataHandling::LandUnitData& landUnitData) const {
	return landUnitData.get_name().find(this->substring_) != std::string::npos;
}


Algorithms::HasMaxResidents Algorithms::HasMaxResidents::InYear(const size_t year, const int limit_) {
	return HasMaxResidents(2020 - year, limit_);
}

bool Algorithms::HasMaxResidents::operator()(const DataHandling::LandUnitData& landUnitData) const {
	return landUnitData.get_total_population_at(this->index_) <= this->limit_;
}




Algorithms::HasMinResidents Algorithms::HasMinResidents::InYear(const size_t year, const int limit_) {
	return HasMinResidents(2020 - year, limit_);
}

bool Algorithms::HasMinResidents::operator()(const DataHandling::LandUnitData& landUnitData) const {
	return landUnitData.get_total_population_at(this->index_) >= this->limit_;
}



bool Algorithms::UnitLevelIs::operator()(const DataHandling::LandUnitData& landUnitData) const {
	return landUnitData.get_unit_level() == this->requested_unit_level_;
}
