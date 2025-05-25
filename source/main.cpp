#include <iostream>

#include "DataHandling/DataHolder.h"


int main() {
	// initialize holder
	auto holder = DataHandling::DataHolder();

	int choice = -1;

	while (true) {
		std::cout << "== MENU ==" << std::endl;
		std::cout << "[1] nástroje zoznamu uzemných jednotiek" << std::endl;
		std::cout << "[2] nástroje stromu uzemných jednotiek" << std::endl;
		std::cout << "[3] tabulky uzemných jednotiek" << std::endl;
		std::cout << "[0] koniec" << std::endl;
		std::cout << std::endl;
		std::cout << ":: ";
		std::cin >> choice;

		if (choice == 0) {
			break;
		}

		if (choice == 3) {

		}
	}
	return 0;
}