#ifndef TOWNDATA_H
#define TOWNDATA_H

#include <string>

namespace DataHandling {
	class TownData {
		const static size_t POPULATIONS_COUNT = 5;

		std::string name_;
		int idNumber_;

		int womenPopulations_[TownData::POPULATIONS_COUNT];
		int menPopulations_[TownData::POPULATIONS_COUNT];

	public:
		// TODO: I don't know if this is the best approach
		TownData() = delete;

		explicit TownData(std::string* lines);
	};
}

#endif //TOWNDATA_H
