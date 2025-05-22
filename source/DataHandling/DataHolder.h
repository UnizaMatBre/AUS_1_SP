#ifndef DATAHOLDER_H
#define DATAHOLDER_H

#include <fstream>
#include "TownData.h"


namespace DataHandling {
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
}

#endif //DATAHOLDER_H
