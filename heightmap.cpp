#include "heightmap.hpp"
#include <iostream>
#include <cmath>
#include <cassert>
#include "lodepng.h"
#include <algorithm>
#include <numeric>
#include "config.hpp"

using namespace std;

heightmap::heightmap(const conf& cfg):
	_cfg(cfg),
	_map(_cfg.width * _cfg.height * 4, 255), 
	_hash(256),
	F( 0.5 * (std::sqrt(3.0)-1.0) ),
	G( (3.0 - std::sqrt(3.0)) /6.0),
	rand(_cfg.seed)
{

	std::iota(_hash.begin(), _hash.end(), 0);
	std::random_shuffle(_hash.begin(), _hash.end(), [this](int i){return rand() % i;});

	for(size_t i = 0; i < 512; ++i)
	{
		_p[i] = _hash[i & 255];
		_pm[i] = short(_p[i] % _grads.size());
		//cout << _pm[i] << ' ';
	}
	cout << "Heightmap settings:" << endl;
	cout << "Width " << _cfg.width << " (" << (_cfg.width - 1) / 64 << " km)" << endl;
	cout << "Height " <<_cfg.height << " (" << (_cfg.height - 1) / 64 << " km)" << endl;
	cout << "Frequency " << _cfg.frequency << endl;
	cout << "Octaves " << _cfg.octaves << endl;
	cout << "Persistance " << _cfg.persistance << endl;
}

void heightmap::generate()
{
	for(size_t i = 0; i < _map.size(); i+=4)
	{
		size_t x = i/4 % _cfg.width;
		size_t y = i/4 / _cfg.height;

		_map[i] = _map[i+1] = _map[i+2] = sum_octaves(x, y); //rgb
		_map[i+3] = 255; //alpha
		//cout << "Iteration " << i << " completed" << endl;
	}
}

float heightmap::sum_octaves(unsigned x, unsigned y)
{
	float out = 0;
	float amp = 1;
	float max_amp = 0;
	float freq = _cfg.frequency;
	for(int i = 1; i <= _cfg.octaves; ++i)
	{
		//sum noise with different parameters
		out += simplex_noise(x * freq, y * freq) * amp;
		max_amp += amp;
		amp *= _cfg.persistance;
		freq *= 2;
	}
	//average the different height maps created.
	out /= max_amp; 
	return out;
}

float heightmap::simplex_noise(float x, float y)
{
	float n0 = 0, n1 = 0, n2 = 0; //noise contributions from each of the three corners

	//transform(x, y);

	//skew the input space to determine which simplex cell we are in.
	float s = (x + y) * F;
	int px = std::floor(x + s);
	int py = std::floor(y + s);

	//unskew the cell origin back to (x,y) space
	float t = (px + py) * G;
	float x0 = x - (px - t);
	float y0 = y - (py - t);

	/*
	 * For the 2D case, the simplex shape is an equilateral triangle
	 * determine which simplex we are in.
	 */

	int i, j; //Offsets for second (middle) corner of simplex in (px, py) coordinates
	if(x0 > y0) //lower triangle XY order
	{
		i = 1;
		j = 0;
	}
	else //upper triangle YX order
	{
		i = 0;
		j = 1;
	}

	//offsets for the middle corner 
	float x1 = x0 - i + G;
	float y1 = y0 - j + G;
    float x2 = x0 - 1.0 + 2.0 * G; 
    float y2 = y0 - 1.0 + 2.0 * G;

	cell_coords c{x0, y0, x1, y1, x2, y2};


	//calculate the hashed gradient of the three simplex corners
	int ig = px & 255;
	int jg = py & 255;
	//cout << "ig " << ig << " jg " << jg << endl;

	/*cout << "g0 " << g0 << endl;
	cout << "g1 " << g1 << endl;
	cout << "g2 " << g2 << endl;*/

	int g0 = _pm[ig+_p[jg]];
    int g1 = _pm[ig+i+_p[jg+j]];
	int g2 = _pm[ig+1+_p[jg+1]];
	
	//calculate contribution from the three corners
	n0 = contribution(g0, c.x0, c.y0);
	n1 = contribution(g1, c.x1, c.y1);
	n2 = contribution(g2, c.x2, c.y2);

	//return (70*(n0 + n1 + n2));
	return ((70*(n0 + n1 + n2))+1)*127.5;
}

float heightmap::contribution(int g, float x, float y)
{
	//cout << "Contrib in: g " << g << ", x " << x << ", y " << y << endl;
	float out = 0;
	float t = MN - std::pow(x, 2) - std::pow(y, 2);
	//cout << MN << " - " << x << "^2 - " << y << "^2 = " << t << endl;
	if(t < 0)
	{
		out = 0.0;
	}
	else
	{
		t *= t;
		out = std::pow(t, 2) * dot(_grads[g], x, y); 
	}
	return out;
}

float heightmap::dot(const grad& g, float x, float y)
{
	return g.x * x + g.y * y;
}

void heightmap::save(const std::string& name)
{
	unsigned error = lodepng::encode(name, _map, _cfg.width, _cfg.height);
	if(error)
		throw std::runtime_error(lodepng_error_text(error));
	cout << "Saved height map to " << name << endl;
}

void heightmap::print_coord(const cell_coords& c) const
{
	cout << "corner 0 " << c.x0 << " " << c.y0 << endl;
	cout << "corner 1 " << c.x1 << " " << c.y1 << endl;
	cout << "corner 2 " << c.x2 << " " << c.y2 << endl;
}
