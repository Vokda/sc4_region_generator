#include "city_plotter.hpp"
#include "lodepng.h"
#include "config.hpp"
//#include <random> //for future randomization of plots

using namespace std;

city_plotter::city_plotter(const conf& cfg):
    _cfg(cfg),
	_out(_cfg.km_width * 3 + _cfg.km_height * 3)
{
}

void city_plotter::generate()
{
    int w = _cfg.km_width;
    int h = _cfg.km_height;

	for(size_t i = 0; i < _out.size(); i+=3)
	{
		size_t x = i/3 % _cfg.width;
		size_t y = i/3 / _cfg.height;

		//testing create small plots
		_out[i] = 255;
		_out[i+1] = _out[i+2] = 0; //rgb
		//_map[i+3] = 255; //alpha
		//cout << "Iteration " << i << " completed" << endl;
	}
}

void city_plotter::save(const string& name) const
{
	//encodeBMP(_out, name, _cfg.km_width, _cfg.km_height);
}

