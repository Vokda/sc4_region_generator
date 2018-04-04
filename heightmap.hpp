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

		//map to values to a png later. noise written to this vector first.
		std::vector<unsigned char> _map; 

		//shors are used since it's the only thing that is the only 
		std::vector<short> _hash={151,160,137,91,90,15,
  131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
  190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
  88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
  77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
  102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
  135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
  5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
  223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
  129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
  251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
  49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
  138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180};

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
			void normalize()
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
		 * gradients for 3 dimensions
		 * overkill since this program only generates noise in 2d
		 */
		std::vector<grad> _grads= 
		{ 
			grad{1,1,0}, 
			grad{-1,1,0}, 
			grad{1,-1,0}, 
			grad{-1,-1,0},
			grad{1,0,1}, 
			grad{-1,0,1}, 
			grad{1,0,-1}, 
			grad{-1,0,-1},
			grad{0,1,1}, 
			grad{0,-1,1}, 
			grad{0,1,-1}, 
			grad{0,-1,-1}
		};
		/*{ //maybe better for 2d case
			grad{1, 1},
			grad{-1, -1},
			grad{0, 0},
			grad{1, 0},
			grad{0, 1},
			grad{-1, 0},
			grad{-1, 1},
			grad{0, -1},
			grad{1, -1},
			};
			*/
		
		
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
