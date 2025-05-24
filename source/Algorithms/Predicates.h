#ifndef PREDICATES_H
#define PREDICATES_H

#include <string>
#include <utility>

#include "../DataHandling/LandUnitData.h"

namespace Algorithms {
	class ContainsSubstringInName {
		std::string substring_;

	public:
		explicit ContainsSubstringInName(const std::string& substring) : substring_(substring) {}
		bool operator()(DataHandling::LandUnitData& landUnitData) const;
	};



	class HasMaxResidents {
		const size_t index_;
		const int limit_;

	public:
		static HasMaxResidents InYear(const size_t year, const int limit_);

		explicit HasMaxResidents(const size_t index, const int limit) : index_(index), limit_(limit) {}
		bool operator()(const DataHandling::LandUnitData& landUnitData) const;
	};



	class HasMinResidents {
		const size_t index_;
		const int limit_;

	public:
		static HasMinResidents InYear(const size_t year, const int limit_);

		explicit HasMinResidents(const size_t index, const int limit) : index_(index), limit_(limit) {}
		bool operator()(const DataHandling::LandUnitData& landUnitData) const;
	};



	class UnitLevelIs {
		const int requested_unit_level_;

	public:
		explicit UnitLevelIs(const int requested_unit_level) : requested_unit_level_(requested_unit_level) {}
		bool operator()(const DataHandling::LandUnitData& landUnitData) const;
	};
}

#endif //PREDICATES_H
