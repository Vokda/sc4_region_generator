#include "heightmap.hpp"
#include "config_reader.hpp"
#include "city_plotter.hpp"

int main(int argc, char** argv)
{
	config_reader cr;
	conf cfg = cr.read_config("reg_gen.cfg");
	//generate height map with simplex noise
	heightmap hg(cfg);
	hg.generate();

	//generate city plots on map.
	city_plotter cp(cfg);
	cp.generate();

	cp.save("config.bmp");
	hg.save("out.png");
	return 0;
}
