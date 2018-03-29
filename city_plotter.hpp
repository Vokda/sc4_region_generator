#ifndef CITY_PLOTTER_HPP
#define CITY_PLOTTER_HPP
#include <string>
#include <vector>
struct conf;

class city_plotter
{
	public:
		city_plotter(const conf& cfg);

		/*
		 * simple generation,
		 * will try to fit as many large cities as possible.
		 */
		void generate();

		void save(const std::string& name) const;
	private:
		const conf& _cfg;
		std::vector<unsigned char> _out;
		int SMALL = 1;
		int MEDIUM = 2;
		int LARGE = 4;
};
#endif
