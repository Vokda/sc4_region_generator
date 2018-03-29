#ifndef CONFIG_READER_HPP
#define CONFIG_READER_HPP
#include "config.hpp"
#include <string>

class config_reader
{
	public:
		//config_reader(const string& name);
		conf read_config(const std::string& name);
};

#endif
