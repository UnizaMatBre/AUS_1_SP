#include <iostream>

#include "Algorithms/Querying.h"
#include "Algorithms/Sorting.h"
#include "Algorithms/Predicates.h"
#include "Algorithms/Comparators.h"

#include "DataHandling/LandUnitData.h"
#include "Containers/NodeBasedTree.h"


#include "ConsoleEnvironment.h"



void print_land_unit(DataHandling::LandUnitData* land_unit) {
	std::cout << land_unit->get_name() << " [ " << land_unit->get_identifier()  << " ] | ";
	for (size_t index = 0; index < DataHandling::LAND_UNIT_POPULATION_COUNT; ++index) {
		std::cout << " ( " <<  land_unit->male_population_at(index) << " : " << land_unit->female_population_at(index) << " ) ";
 	};
	std::cout << std::endl;
}

int request_choice_input(std::initializer_list<int> valid_choices) {
	int choice = -1;

	while (true) {
		std::cout << std::endl;
		std::cout << ":: ";
		std::cin >> choice;

		for (auto& one_valid_choice : valid_choices) {
			if (choice == one_valid_choice) {
				return one_valid_choice;
			}
		}

		std::cout << "Neznáma volba : " << choice << std::endl;
	};
};

void ConsoleEnvironment::show_main_menu() {
	int choice = -1;

	while (true) {
		std::cout << "== MENU ==" << std::endl;
		std::cout << "[2] nástroje stromu uzemných jednotiek" << std::endl;
		std::cout << "[3] tabulky uzemných jednotiek" << std::endl;
		std::cout << "[0] koniec" << std::endl;

		choice = request_choice_input({0,2,3});
		switch (choice) {
			case 0: {
				std::cout << "Ukončenie programu" << std::endl;
				return;
			}

			case 2: {
				this->show_tree_menu();
				break;
			}
			case 3: {
				this->show_tables_menu();
				break;
			};

			default: {
				std::cout << "Neznáma volba : " << choice << std::endl;
			};
		};

		std::cout << "----------------------" << std::endl;
	};
};



using TreeIterator = Containers::TreeNode<DataHandling::LandUnitData*>::Iterator;

void show_selection_submenu(TreeIterator& begin, TreeIterator& end) {
	Containers::LinkedList<DataHandling::LandUnitData*> output_list;
	int choice = -1;

	std::cout << "== SELEKCIA ==" << std::endl;
	std::cout << "Vyberte predikat:" << std::endl;
	std::cout << "[0] alwaysTrue - akceptuje všetky prvky." << std::endl;
	std::cout << "[1] containsStr - nazov obsahuje zadaný retazec." << std::endl;
	std::cout << "[2] hasMaxResidents - v zadanom roku ma menej občanov ako limit" << std::endl;
	std::cout << "[3] hasMinResidents - v zadanom roku ma menej občanov ako limit"  << std::endl;

	while (true) {
		std::cout << std::endl;
		std::cout << ":: ";
		std::cin.clear();
		std::cin >> choice;


	}
};

void ConsoleEnvironment::show_tree_menu() {
	auto tree_iterator = this->holder_.get_tree_iterator();
	auto tree_iterator_end = this->holder_.root_node_.end();
	int choice = -1;

	while (true) {
		std::cout << "== STROM ==" << std::endl;
		std::cout << "Iter: " << (*tree_iterator)->get_name() << std::endl;

		std::cout << std::endl;

		std::cout << "[1] presun hore" << std::endl;
		std::cout << "[2] presun dole podľa mena" << std::endl;
		std::cout << "[3] presun dole podľa id" << std::endl;
		std::cout << "[4] resetuj iterator" << std::endl;
		std::cout << "[5] selektuj z iteratorov" << std::endl;
		std::cout << "[0] koniec " << std::endl;

		choice = request_choice_input({0,1,2,3,4,5});

		switch (choice) {
			case 0: {
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

			case 4: {
				tree_iterator = this->holder_.get_tree_iterator();
				break;
			};

			case 5: {
				show_selection_submenu(tree_iterator, tree_iterator_end);
				break;
			} ;

			default: {
				std::cout << "Neznáma volba : " << choice << std::endl;
			};
		}

		std::cout << "----------------------" << std::endl;

	}
}


void ConsoleEnvironment::show_tables_menu() {
	while (true) {
		std::cout << "== TABULKY ==" << std::endl;

		int table_number = 0;


		std::cout << "Vyber level administrativnej jednotky (1-4)" << std::endl;
		std::cin.clear();
		table_number = request_choice_input({1,2,3,4});


		std::string table_unit_name;

		std::cout << std::endl;
		std::cout << "Vyber meno vyhladávanej administratívnej jednotky" << std::endl;
		std::cout << ":: ";
		std::cin.ignore();
		std::getline(std::cin, table_unit_name);
		std::cout << std::endl;

		std::cout << "Vysledok:" << std::endl << "  ";
		try {
			switch (table_number) {
				case 1: {
					const auto result = this->holder_.geographic_areas_table_.at(table_unit_name);
					print_land_unit(result);
					break;
				};
				case 2: {
					const auto result = this->holder_.republics_table_.at(table_unit_name);
					print_land_unit(result);
					break;
				};
				case 3: {
					const auto result = this->holder_.regions_table_.at(table_unit_name);
					print_land_unit(result);
					break;

				};
				case 4: {
					auto result = this->holder_.towns_table_.at(table_unit_name);
					for (auto& one_town : result) {
						print_land_unit(one_town);
						std::cout << std::endl;
					}
					break;
				};
				default: {
					throw std::runtime_error("This isn't allowed");
				};
			}
		}
		catch (std::out_of_range& e) {
			std::cout << "Jednotka na danej úrovni z daným názvom neexistuje" << std::endl;
		}

		std::cout << std::endl;
		std::cout << "Koniec? [0 ak ano] [1 ak nie]" << std::endl;
		table_number = request_choice_input({0,1});

		if (table_number == 0) {
			return;
		}
	};

}





