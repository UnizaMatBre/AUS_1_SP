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
}

#endif //PREDICATES_H
