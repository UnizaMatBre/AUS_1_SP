#include <iostream>
#include <fstream>

#include "CsvLineReader.h"
#include "DataHolder.h"


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
		auto stream = std::ifstream(R"(../../data/geograficke_oblasti.csv)");
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
}
