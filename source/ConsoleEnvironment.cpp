#include <iostream>

#include "DataHandling/LandUnitData.h"
#include "Containers/NodeBasedTree.h"


#include "ConsoleEnvironment.h"

#include "Algorithms/Comparators.h"
#include "Algorithms/Predicates.h"

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

			case 2: {
				this->show_tree_menu();
				break;
			}

			default: {
				std::cout << "Neznáma volba : " << choice << std::endl;
			};
		};

		std::cout << "----------------------" << std::endl;
	};
};


void ConsoleEnvironment::show_tree_menu() {
	auto tree_iterator = this->holder_.get_tree_iterator();

	int choice = -1;

	while (true) {
		std::cout << "== STROM ==" << std::endl;
		std::cout << "Iter: " << (*tree_iterator)->get_name() << std::endl;

		std::cout << std::endl;

		std::cout << "[1] presun hore" << std::endl;
		std::cout << "[2] presun dole podľa mena" << std::endl;
		std::cout << "[3] presun dole podľa id" << std::endl;
		std::cout << "[5] resetuj iterator" << std::endl;
		std::cout << "[0] koniec " << std::endl;
		std::cout << std::endl;

		std::cout << ":: ";
		std::cin >> choice;
		std::cout << std::endl;
		std::cout << "----------------------" << std::endl;

		switch (choice) {
			case 0: {
				std::cout << "Ukončenie pod-menu" << std::endl;
				return;
			};

			case 1: {
				auto ok = tree_iterator.move_to_parent();

				if (ok) { std::cout << "Iterátor sa úspešne preniesol na rodiča" << std::endl; }
				else { std::cout << "Rodič neexistuje" << std::endl; }

				break;
			};

			case 2: {
				std::string target_name;
				std::cout << "Zadaj podreťazec ::";
				std::cin >> target_name;
				std::cout << std::endl;

				auto ok = tree_iterator.move_to_children([&target_name](auto& land_unit) {
					return land_unit.get_item()->get_name() == target_name;
				});

				if (ok) { std::cout << "Iterátor sa úspešne preniesol na dieťa" << std::endl; }
				else { std::cout << "Dieťa z daným reťazcom v mene neexistuje." << std::endl; }
			};


			case 3: {
				std::string target_id;
				std::cout << "Zadaj id ::";
				std::cin >> target_id;
				std::cout << std::endl;

				auto ok = tree_iterator.move_to_children([&target_id](auto child_land) {
					return child_land.get_item()->get_identifier() == target_id;
				});

				if (ok) { std::cout << "Iterátor sa úspešne preniesol na dieťa" << std::endl; }
				else { std::cout << "Dieťa z id neexistuje" << std::endl; }
			}

			case 5: {
				tree_iterator = this->holder_.get_tree_iterator();
				break;
			};

			default: {
				std::cout << "Neznáma volba : " << choice << std::endl;
			};
		}

		std::cout << "----------------------" << std::endl;

	}
}

