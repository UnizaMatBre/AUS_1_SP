#ifndef TOWNDATA_H
#define TOWNDATA_H

#include <string>

class TownData {
	const static size_t POPULATIONS_COUNT = 5;

	std::string name_;
	int idNumber_;

	int womenPopulations[TownData::POPULATIONS_COUNT];
	int menPopulations[TownData::POPULATIONS_COUNT];

public:
	TownData() = default;
};

#endif //TOWNDATA_H
