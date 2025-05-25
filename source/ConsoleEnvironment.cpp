#include <iostream>

#include "ConsoleEnvironment.h"

void ConsoleEnvironment::show_main_menu() {
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
		std::cout << std::endl;

		std::cout << "----------------------" << std::endl;

		switch (choice) {
			case 0: {
				std::cout << "Ukončenie programu" << std::endl;
				return;
			}


			default: {
				std::cout << "Neznáma volba : " << choice << std::endl;
			};
		};

		std::cout << "----------------------" << std::endl;
	};
};
