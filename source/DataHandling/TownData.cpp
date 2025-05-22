#include "CsvLineReader.h"

#include "TownData.h"


DataHandling::TownData::TownData(std::string* lines) {
	// read first line - from this one, we will get all info about territory
	auto primaryReader = DataHandling::CsvLineReader(lines[0]);
	primaryReader
		.handleField([this](const std::string& field) {
			// read name
			this->name_ = field;
		})
		->handleField([this](const std::string& field) {
			// read numeric identifier
			this->idNumber_ = std::stoi( field.substr(1, field.size() - 2) );
		})
		->handleField([this](const std::string& field) {
			if(field == "-") {
				this->menPopulations_[0] = -1;
				return;
			}

			// read number of men in specific year
 			this->menPopulations_[0] = std::stoi( field );
		})
		->skipField() // skip empty field
		->handleField([this](const std::string& field) {
			if(field == "-") {
				this->womenPopulations_[0] = -1;
				return;
			}

			// read number of women in specific year
			this->womenPopulations_[0] = std::stoi( field );
		});


	// read other 4 lines - from these one, we will be taking just population stats
	for(int index = 1; index < 5; index++) {
		auto sideLineReader = DataHandling::CsvLineReader(lines[index]);

		sideLineReader.skipField()->skipField()
			->handleField([this, &index](const std::string& field) {
				if(field == "-") {
					this->menPopulations_[index] = -1;
					return;
				};

				this->menPopulations_[index] = std::stoi(field);
			})
			->skipField()
			->handleField([this, &index](const std::string& field) {
				if(field == "-") {
					this->womenPopulations_[index] = -1;
					return;
				}
				this->womenPopulations_[index] = std::stoi(field);
			});
		}
}