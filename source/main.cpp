#include <fstream>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <optional>

#include "Algorithms/Querying.h"
#include "Algorithms/Sorting.h"
#include "DataHandling/TownData.h"
#include "DataHandling/DataHolder.h"


int main() {
	/*
		auto towns = std::vector<DataHandling::TownData>();
		const auto towns_iter = std::back_insert_iterator(towns);

		loadTowns(towns_iter);

		std::optional<DataHandling::TownData> town;
	*/


	std::vector<int> vals = {5,6,7,1,3,2,4,9,8};

	Algorithms::selectionSort(
		vals.begin(),
		vals.end(),
		[](int left, int right) { return right - left; }
	);

	for (auto i : vals) {
		std::cout << i << std::endl;
	}

	return 0;
}