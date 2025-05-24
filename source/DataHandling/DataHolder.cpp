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


	// load geographic areas
	{
		auto stream = std::ifstream(R"(../../data/geograficke_oblasti.csv)");
		std::string line;

		while (getline(stream, line)) {

			std::string name, full_id, restricted_id;

			// read data
			auto csv_reader = DataHandling::CsvLineReader(line);
			csv_reader
				.handleField([&name](const std::string& field ) {
					name = field;
				})
			    ->handleField([&full_id, &restricted_id](const std::string& field ) {
					full_id = field;
			    	restricted_id = field.substr(1, field.size() - 2);
				});

			auto land_unit_ptr = &this->land_units_list_.push_back({name, full_id, 1});

			// insert into geographic area table
			this->geographic_areas_table_.insert(name, land_unit_ptr);

			// create new node
			auto new_node_ptr = this->root_node_.push_back_children(land_unit_ptr);

			// insert this new node into mapping table
			id_to_node_mapper.insert(restricted_id, new_node_ptr);
		};

	}

	// load republics
	{
		auto stream = std::ifstream(R"(../../data/republiky.csv)");
		std::string line;

		while (getline(stream, line)) {

			std::string name, full_id, restricted_id;

			// read data
			auto csv_reader = DataHandling::CsvLineReader(line);
			csv_reader
				.handleField([&name](const std::string& field ) {
					name = field;
				})
				->handleField([&full_id, &restricted_id](const std::string& field ) {
					full_id = field;
					restricted_id = field.substr(1, field.size() - 2);
				});

			auto land_unit_ptr = &this->land_units_list_.push_back({name, full_id, 1});

			// insert into geographic area table
			this->republics_table_.insert(name, land_unit_ptr);

			// get parent node
			std::string parent_restricted_id = restricted_id.substr(0, restricted_id.size() - 1);
			auto parent_node = id_to_node_mapper.at(parent_restricted_id);

			// create new node
			auto new_node_ptr = parent_node->push_back_children(land_unit_ptr);

			// insert this new node into mapping table
			id_to_node_mapper.insert(restricted_id, new_node_ptr);
		};

	}

}
