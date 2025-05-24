#include "Predicates.h"


bool Algorithms::ContainsSubstringInName::operator()(DataHandling::LandUnitData& landUnitData) const {
	return landUnitData.get_name().find(this->substring_) != std::string::npos;
}

