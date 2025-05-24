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

		const std::string& get_name() const {
			return this->name_;
		};

		const std::string& get_identifier() const {
			return this->identifier_;
		};

		int get_territory_level() const {
			return this->territory;
		}

		int get_male_population_at(const size_t index) const {
			return this->male_population_count_[index];
		}

		int get_female_population_at(const size_t index) const {
			return this->female_population_count_[index];
		}

		int get_total_population_at(const size_t index) const {
			return this->male_population_count_[index] + this->female_population_count_[index];
		}

	};
}


#endif //LANDUNITDATA_H
