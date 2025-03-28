#ifndef CSVLINEREADER_H
#define CSVLINEREADER_H

#include <sstream>
#include <fstream>
#include <string>

namespace DataHandling {
	class CsvLineReader {
		std::stringstream lineStream;

	public:
		explicit CsvLineReader(const std::string& line) : lineStream(line) { }

		CsvLineReader* skipField() {
			std::string field;
			getline(this->lineStream, field, ';');

			return this;
		}

		template<typename OperationType>
		CsvLineReader* handleField(OperationType operation) {
			std::string field;
			getline(this->lineStream, field, ';');

			operation(field);

			return this;
		}
	};
}

#endif //CSVLINEREADER_H
