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

		void save(const std::string& name);
	private:
		
		//Lode Vandevenne's PNG to BMP converter
		void encodeBMP(std::vector<unsigned char>& bmp, const unsigned char* image, int w, int h);

		//fill with large city plots 
		void large_fill();

		//find if is pixel colored
		bool  colorized(const unsigned char* pixel) const;

		//fill plot, returns false if fails to fill in plot
		bool fill(int plot_size, int width, int height);

		const conf& _cfg;
		std::vector<unsigned char> _out;
		std::vector<unsigned char> _bmp;
		std::vector<std::vector<char>> _uncolored;
		//city plots
		const int SMALL = 1;	//red
		const int MEDIUM = 2;	//green
		const int LARGE = 4;	//blue
		const int COLOR_CHANNELS = 3;
};
#endif
