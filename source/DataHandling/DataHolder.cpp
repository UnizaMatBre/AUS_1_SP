#include <iostream>
#include <fstream>

#include "CsvLineReader.h"
#include "DataHolder.h"


/**
* Job of this is to just add population into all parent nodes
*/
void add_population_(Containers::TreeNode<DataHandling::LandUnitData*>* node, int category, size_t year_index, int population) {
	if (node == nullptr) {
		return;
	}

	add_population_(node->get_parent(), category, year_index, population);

	switch (category) {
		case 0: {
			node->get_item()->male_population_at(year_index) +=  population;
			break;
		};
		case 1: {
			node->get_item()->female_population_at(year_index) +=  population;
			break;
		};
		default: {
			throw std::runtime_error("Category does not exist");
		}
	}

}

DataHandling::DataHolder::DataHolder() {

	/*
	Step 1: load data starting from higher units to lower
	Step 2: for each unit, create node in tree
	Step 3: store this node in temporary table which maps shortened id => node
	Step 4: after everything is loaded, load populations
	Step 5: for each population change, also add population into upper units
	*/

	// STEP 1 (ONE)
	// initialize temporary table
	Containers::LinkedTable<std::string, LandNodeType*> id_to_node_mapper;

	// add austria into this temporary table
	id_to_node_mapper.insert("AT", &this->root_node_);

	// load upper areas
	{
		// open stream
		auto stream = std::ifstream(R"(../../data/uzemie.csv)");
		std::string line;

		if (not stream.is_open()) {
			throw std::runtime_error("Could not open file");
		};

		// cycle over every line in file
		while (std::getline(stream, line)) {
			std::string name, full_id, restricted_id;

			// read data
			DataHandling::CsvLineReader(line)
				.handleField([&name](const std::string& field ) {
					name = field;
				})
				->handleField([&full_id, &restricted_id](const std::string& field ) {
					full_id = field;
					restricted_id = field.substr(1, field.size() - 2);
				});

			// create parent's restricted id - this is achievable by the fact that parent id is always shorter by one digit from child
			std::string restricted_parent_id = restricted_id.substr(0, restricted_id.size() - 1);

			// get parent node
			auto parent_node_ptr = id_to_node_mapper.at(restricted_parent_id);

			// create new land unit
			auto new_land_unit_ptr = &this->land_units_list_.push_back(
				{name, full_id, parent_node_ptr->get_item()->get_unit_level() + 1}
			);

			// create new tree node
			auto new_land_node_ptr =  parent_node_ptr->push_back_children(new_land_unit_ptr);

			// insert new land unit into correct table
			switch (new_land_unit_ptr->get_unit_level()) {
				case 1:
					this->geographic_areas_table_.insert(name, new_land_unit_ptr);
					break;
				case 2:
					this->republics_table_.insert(name, new_land_unit_ptr);
					break;
				case 3:
					this->regions_table_.insert(name, new_land_unit_ptr);
					break;
				default:
					throw std::runtime_error("This is not supposed to happend");
			}

			// insert land node into mapper
			id_to_node_mapper.insert(restricted_id, new_land_node_ptr);
		}
		stream.close();
	};

	// load town categorization
	{
		// open stream
		auto stream = std::ifstream(R"(../../data/obce.csv)");
		std::string line;
		if (not stream.is_open()) {
			throw std::runtime_error("Could not open file");
		};

		// cycle over every line in file
		while (std::getline(stream, line)) {
			std::string name, full_id, restricted_id, restricted_parent_id;

			// read data
			DataHandling::CsvLineReader(line)
				.handleField([&name](const std::string& field ) {
					name = field;
				})
				->handleField([&full_id, &restricted_id](const std::string& field ) {
					full_id = field;
					restricted_id = field.substr(1, field.size() - 2);
				})
				->handleField([&restricted_parent_id](const std::string& field ) {
					restricted_parent_id = field.substr(0, field.size() - 1);
				});

			// get parent node
			auto parent_node_ptr = id_to_node_mapper.at(restricted_parent_id);

			// create new land unit
			auto new_land_unit_ptr = &this->land_units_list_.push_back(
				{name, full_id, parent_node_ptr->get_item()->get_unit_level() + 1}
			);

			// create new tree node
			auto new_land_node_ptr =  parent_node_ptr->push_back_children(new_land_unit_ptr);

			// insert unit into table
			auto list = Containers::LinkedList<LandUnitData*>();
			list.push_back(new_land_unit_ptr);

			if (this->towns_table_.try_insert(name, list) == nullptr) {
				this->towns_table_.at(name).push_back(new_land_unit_ptr);
			}

			// insert land node into mapper
			id_to_node_mapper.insert(restricted_id, new_land_node_ptr);
		};
	};


	// load town population data
	{
		std::ifstream streams[] = {
			std::ifstream(R"(../../data/2020.csv)"),
			std::ifstream(R"(../../data/2021.csv)"),
			std::ifstream(R"(../../data/2022.csv)"),
			std::ifstream(R"(../../data/2023.csv)"),
			std::ifstream(R"(../../data/2024.csv)"),
		};

		for (auto& sideStream : streams) {
			if (!sideStream.is_open()) {
				throw std::runtime_error("Error opening file stream.");
			};
		};

		std::string lines[5];
		while (getline(streams[0], lines[0])) {
			for (size_t index = 1; index < 5; index++) {
				getline(streams[index], lines[index]);
			}


			// find node we will be filling with data
			LandNodeType* current_land_node = nullptr;
			bool is_special_case = false;

			// we will find this by reading part of first line
			DataHandling::CsvLineReader(lines[0])
				.skipField()
				->handleField([&current_land_node, &id_to_node_mapper, &is_special_case](const std::string& field ) {
					auto restricted_town_id = field.substr(1, field.size() - 2);

					try {
						current_land_node = id_to_node_mapper.at(restricted_town_id);
					}
					catch (std::out_of_range& e) {
						// only one item will trigger this - "Nicht klassifizierbar"
						// i don't know what job it has, but it is not used anwyhere, soo...
						is_special_case = true;
					}
			});

			// is special case? Skip it
			if (is_special_case) {
				is_special_case = false;
				continue;;
			}

			// iterate over all lines
			for (size_t index = 0; index < 5; ++index) {
				DataHandling::CsvLineReader(lines[index])
					.skipField()->skipField() // skip first two fields
					->handleField([&index, &current_land_node](const std::string& field ) {
						auto population = std::stoi(field);

						add_population_(current_land_node, 0, index, population );
					})
					->skipField()
					->handleField([&index, &current_land_node](const std::string& field ) {
						auto population = std::stoi(field);

						add_population_(current_land_node, 1, index, population );
					});
			};

		};

		for (auto& stream : streams) {
			stream.close();
		};
	};

}
