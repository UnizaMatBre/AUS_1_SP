#ifndef DATAHOLDER_H
#define DATAHOLDER_H

#include <string>

#include "../Containers/NodeBasedTree.h"
#include "../Containers/LinkedTable.h"

#include "LandUnitData.h"
#include "../Containers/NodeBasedTree.h"


namespace DataHandling {
	/*
	template<class OutputIterType>
	void loadTowns(OutputIterType targetCurent) {
		std::ifstream streams[] = {
			std::ifstream(R"(../../data/2020.csv)"),
			std::ifstream(R"(../../data/2021.csv)"),
			std::ifstream(R"(../../data/2022.csv)"),
			std::ifstream(R"(../../data/2023.csv)"),
			std::ifstream(R"(../../data/2024.csv)"),
		};

		for (auto& sideStream : streams) {
			if (!sideStream.is_open()) {
				throw std::runtime_error("Error opening file stream.");
			};
		};

		std::string lines[5];
		while (getline(streams[0], lines[0])) {
			for (size_t index = 1; index < 5; index++) {
				getline(streams[index], lines[index]);
			}

			targetCurent = DataHandling::TownData(lines);
			++targetCurent;
		};
	}
	*/

	class DataHolder {
		// table for each level of land unit
		Containers::LinkedTable<std::string, LandUnitData> geographic_areas_table_;
		Containers::LinkedTable<std::string, LandUnitData> republics_table_;
		Containers::LinkedTable<std::string, LandUnitData> regions_table_;
		Containers::LinkedTable<std::string, LandUnitData> towns_table_;

		// highest territorial unit - great austrian repulic itself.
		DataHandling::LandUnitData austria_unit_ = {"Rakúsko", "<AT>", 0};

		// node type
		using LandNodeType = Containers::TreeNode<LandUnitData*>;

		// root of hierarchy
		LandNodeType root_node_ = LandNodeType(&austria_unit_);
	public:
		DataHolder();

	};
}

#endif //DATAHOLDER_H
