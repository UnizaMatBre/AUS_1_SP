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

#include "Containers/LinkedList.h"


struct DestroyCheck {
	int id;

	DestroyCheck(int id) : id(id) {}

	~DestroyCheck() {
		std::cout << "DestroyCheck " << this->id << " successful" << std::endl;
	}
};

int main() {
	/*
		auto towns = std::vector<DataHandling::TownData>();
		const auto towns_iter = std::back_insert_iterator(towns);

		loadTowns(towns_iter);

		std::optional<DataHandling::TownData> town;
	*/


	auto list = Containers::LinkedList<DestroyCheck>();

	for (int index = 0; index < 10; index++) {
		auto destroy = DestroyCheck(index);
		list.push_back(destroy);
	}

	for (int index = 0; index < 10; index++) {
		auto destroy = DestroyCheck(index + 10);
		list.push_back(destroy);
		list.pull_front();
	}

	for (int index = 0; index < 10; index++) {
		list.pull_front();
	}

	return 0;
}