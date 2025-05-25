#include <iostream>

#include "DataHandling/DataHolder.h"

#include "ConsoleEnvironment.h"

int main() {
	// initialize holder
	auto holder = DataHandling::DataHolder();

	auto environment = ConsoleEnvironment(holder);

	environment.show_main_menu();
	return 0;
}