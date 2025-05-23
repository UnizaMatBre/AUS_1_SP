#include <fstream>
#include <string>
#include <vector>
#include <fstream>
#include <optional>

#include "DataHandling/TownData.h"
#include "DataHandling/DataHolder.h"


int main() {
		auto towns = std::vector<DataHandling::TownData>();
		const auto towns_iter = std::back_insert_iterator(towns);

		loadTowns(towns_iter);

		std::optional<DataHandling::TownData> town;
	return 0;
}