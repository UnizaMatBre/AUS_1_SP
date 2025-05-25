#ifndef CONSOELENVIRONMENT_H
#define CONSOELENVIRONMENT_H

#include "DataHandling/DataHolder.h"


class ConsoleEnvironment {
	DataHandling::DataHolder& holder_;

public:
	explicit ConsoleEnvironment(DataHandling::DataHolder& data_holder) : holder_(data_holder) {}

	void show_main_menu();
};

#endif //CONSOELENVIRONMENT_H
