#include "config_reader.hpp"
#include <fstream>
#include <sstream>
#include <map>
#include <iostream>

using namespace std;

conf config_reader::read_config(const string& file_name)
{
	conf cfg;
	string line, key;
	ifstream ifs(file_name);
	map<string, float> values;

	//read from file
	if(ifs.is_open())
	{
		while(getline(ifs, line))
		{
			//cout << line << endl;
			//comment
			if(line[0] == '#')
			{
				cout << "Skipped comment!" << endl;
				continue;
			}

			stringstream ss(line);
			ss >> key;
			ss >> values[key];
			/*cout << "key " << key << endl;
			cout << "value " << values[key] << endl;*/
		}
	}
	else
	{
		cerr << "Failed to open " << file_name << '!' << endl;
		throw 1;
	}

	ifs.close();

	//width and height needs to be altered in this way to generate 
	cfg.km_width = values["width"];
	cfg.km_height = values["height"];
	cfg.width = values["width"] * 64 + 1;
	cfg.height = values["height"] * 64 + 1;


	cfg.frequency = values["frequency"];
	cfg.octaves = values["octaves"];
	cfg.persistance = values["persistance"];

	return cfg;
}
