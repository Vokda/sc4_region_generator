#ifndef HEIGHTMAP_HPP
#define HEIGHTMAP_HPP
#include <string>
#include <vector>
#include <cmath>
#include <random>

using namespace std;
struct conf;

class heightmap
{
	public:
		heightmap(const conf& cfg);
		//heightmap(size_t x, size_t y, float scale, int octaves, float persistance);

		//sequential 
		void generate();

		void save(const std::string& name);
		
	private:
		//vars
		const conf& _cfg;

		//map to values to a png later. noise is written to this vector first.
		//afterwards it is given as an argument to lodepng for png processing.
		std::vector<unsigned char> _map; 

		//hash values for obtaining the correct gradient
		std::vector<short> _hash;

		//some values needed for calculations
		const float F;
		const float G;

		/*const int OLD_MIN = -1;
		const int NEW_MIN = 0;
		const int OLD_RANGE = 1 - OLD_MIN; //2
		const int NEW_RANGE = 255 - NEW_MIN; //255*/
		short _p[512];
		short _pm[512];

		struct grad
		{
			float x, y, z;
		};

		struct cell_coords
		{
			float x0, y0, x1, y1, x2, y2;
			void normalize() //used for testing, can be removed
			{
				float v0 = sqrt(pow(x0, 2) + pow(y0, 2));
				float v1 = sqrt(pow(x1, 2) + pow(y1, 2));
				float v2 = sqrt(pow(x2, 2) + pow(y2, 2));
				float m = max(v0, max(v1, v2));
				x0 /= m;
				y0 /= m;
				x1 /= m;
				y1 /= m;
				x2 /= m;
				y2 /= m;
			}
		};

		/*
		 * gradients for 2 dimensions
		 */
		std::vector<grad> _grads= 
		{ 
			grad{1,1},
			grad{-1,1},
			grad{1,-1},
			grad{-1,-1},
			grad{1,0},
			grad{-1,0},
			grad{1,0},
			grad{-1,0},
			grad{0,1},
			grad{0,-1},
			grad{0,1},
			grad{0,-1}
		};
		
		
		//sum octaves
		float sum_octaves(unsigned x, unsigned y);

		//contribution of a corner
		float contribution(int g, float x, float y);

		float dot(const grad& a, float b, float c);

		/*
		 * generates a value between 0 and 255
		 */
		float simplex_noise(float x, float y);

		void print_coord(const cell_coords& c) const;

		const float MN = 0.5; //The magic number that all projects need!

		//random generator
		mt19937 rand;
};

#endif 
