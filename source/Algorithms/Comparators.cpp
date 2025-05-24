#include "Comparators.h"

int Algorithms::CompareAlphabetical(const DataHandling::LandUnitData &left, const DataHandling::LandUnitData &right) {
	return left.get_name().compare(right.get_name());
};
