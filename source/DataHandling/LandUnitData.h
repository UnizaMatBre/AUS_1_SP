#ifndef LANDUNITDATA_H
#define LANDUNITDATA_H

#include <string>

namespace DataHandling {
	const size_t LAND_UNIT_POPULATION_COUNT = 5;

	class LandUnitData {
		std::string name_;
		std::string identifier_;
		int territory = -1;

		int male_population_count_[LAND_UNIT_POPULATION_COUNT] = {};
		int female_population_count_[LAND_UNIT_POPULATION_COUNT] = {};

	public:
		LandUnitData(const std::string &name, const std::string& identifier, const int territory) {
			this->name_ = name;
			this->identifier_ = identifier;
			this->territory = territory;
		};
	};
}


#endif //LANDUNITDATA_H
