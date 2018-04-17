#include "city_plotter.hpp"
#include <cassert>
#include "lodepng.h"
#include "config.hpp"
#include <iostream>
//#include <random> //for future randomization of plots

using namespace std;

city_plotter::city_plotter(const conf& cfg):
    _cfg(cfg),
	_out(_cfg.km_width * _cfg.km_height * 3, 0),
	_uncolored(cfg.km_height, vector<char>(cfg.km_width, '_'))
{
}

void city_plotter::generate()
{
	if(_cfg.km_width % LARGE == 0 && _cfg.km_height % LARGE == 0)
	{
		cout << "filling with large plots" << endl;
		large_fill();
	}
	else //fill with largest plot first, following the rest
	{
		for(int plot_size = LARGE; plot_size > 0; plot_size /= 2)
		{
			cout << "plot size: " << plot_size << endl;
			for(int h = 0; h < _cfg.km_width; h += plot_size)
			{
				for(int w = 0; w < _cfg.km_width; w += plot_size)
				{
					if(!fill(plot_size, w, h))
					{
						cerr << "Could not fit " << plot_size << " @ " << w << ", " << h << endl;
						continue;
					}

					/*for(size_t i = 0; i < _out.size(); i+=3)
					  {
					  _out[i] = 255;
					  _out[i+1] = _out[i+2] = 0; //rgb
					  }*/
				}
			}
		}
	}

	//debug
	for(vector<char>& v: _uncolored)
	{
		for(char i: v)
			cout << i;
		cout << endl;
	}
}

void city_plotter::save(const string& name)
{
	encodeBMP(_bmp, &_out[0], _cfg.km_width, _cfg.km_height);
	lodepng::save_file(_bmp, "config.bmp");
	cout << "Saved city plots to config.bmp" << endl;
}

//Lode Vandevenne's PNG -> BMP converter
//Input image must be RGB buffer (3 bytes per pixel), but you can easily make it
//support RGBA input and output by changing the inputChannels and/or outputChannels
//in the function to 4.
void city_plotter::encodeBMP(std::vector<unsigned char>& bmp, const unsigned char* image, int w, int h)
{
  //3 bytes per pixel used for both input and output.
  int inputChannels = COLOR_CHANNELS;
  int outputChannels = 3;
  
  //bytes 0-13
  bmp.push_back('B'); bmp.push_back('M'); //0: bfType
  bmp.push_back(0); bmp.push_back(0); bmp.push_back(0); bmp.push_back(0); //2: bfSize; size not yet known for now, filled in later.
  bmp.push_back(0); bmp.push_back(0); //6: bfReserved1
  bmp.push_back(0); bmp.push_back(0); //8: bfReserved2
  bmp.push_back(54 % 256); bmp.push_back(54 / 256); bmp.push_back(0); bmp.push_back(0); //10: bfOffBits (54 header bytes)

  //bytes 14-53
  bmp.push_back(40); bmp.push_back(0); bmp.push_back(0); bmp.push_back(0);  //14: biSize
  bmp.push_back(w % 256); bmp.push_back(w / 256); bmp.push_back(0); bmp.push_back(0); //18: biWidth
  bmp.push_back(h % 256); bmp.push_back(h / 256); bmp.push_back(0); bmp.push_back(0); //22: biHeight
  bmp.push_back(1); bmp.push_back(0); //26: biPlanes
  bmp.push_back(outputChannels * 8); bmp.push_back(0); //28: biBitCount
  bmp.push_back(0); bmp.push_back(0); bmp.push_back(0); bmp.push_back(0);  //30: biCompression
  bmp.push_back(0); bmp.push_back(0); bmp.push_back(0); bmp.push_back(0);  //34: biSizeImage
  bmp.push_back(0); bmp.push_back(0); bmp.push_back(0); bmp.push_back(0);  //38: biXPelsPerMeter
  bmp.push_back(0); bmp.push_back(0); bmp.push_back(0); bmp.push_back(0);  //42: biYPelsPerMeter
  bmp.push_back(0); bmp.push_back(0); bmp.push_back(0); bmp.push_back(0);  //46: biClrUsed
  bmp.push_back(0); bmp.push_back(0); bmp.push_back(0); bmp.push_back(0);  //50: biClrImportant
  
  /*
  Convert the input RGBRGBRGB pixel buffer to the BMP pixel buffer format. There are 3 differences with the input buffer:
  -BMP stores the rows inversed, from bottom to top
  -BMP stores the color channels in BGR instead of RGB order
  -BMP requires each row to have a multiple of 4 bytes, so sometimes padding bytes are added between rows
  */

  int imagerowbytes = outputChannels * w;
  imagerowbytes = imagerowbytes % 4 == 0 ? imagerowbytes : imagerowbytes + (4 - imagerowbytes % 4); //must be multiple of 4
  
  for(int y = h - 1; y >= 0; y--) //the rows are stored inversed in bmp
  {
    int c = 0;
    for(int x = 0; x < imagerowbytes; x++)
    {
      if(x < w * outputChannels)
      {
        int inc = c;
        //Convert RGB(A) into BGR(A)
        if(c == 0) inc = 2;
        else if(c == 2) inc = 0;
        bmp.push_back(image[inputChannels * (w * y + x / outputChannels) + inc]);
      }
      else bmp.push_back(0);
      c++;
      if(c >= outputChannels) c = 0;
    }
  }

  // Fill in the size
  bmp[2] = bmp.size() % 256;
  bmp[3] = (bmp.size() / 256) % 256;
  bmp[4] = (bmp.size() / 65536) % 256;
  bmp[5] = bmp.size() / 16777216;
}

void city_plotter::large_fill()
{
	for(size_t i = 0; i < _out.size(); i+=COLOR_CHANNELS)
	{
		//testing create small plots
		_out[i] = _out[i+1] = 0; //rg
		_out[i+2] = 255; //b
	}
}

bool city_plotter::colorized(const unsigned char* pixel) const
{
	return pixel[0] > 0 || pixel[1] > 0 || pixel[2] > 0;
}

/*
 * width & height is the position 
 */
bool city_plotter::fill(int plot_size, int x, int y)
{
	size_t p = x * y;
	size_t byte_p = p * COLOR_CHANNELS;
	int sqr_plot = plot_size * plot_size;
	cout << "w " << x << ", h " << y << endl;
	//check to make sure no other plot is at the position
	if(colorized(&_out[x * y * 3]))
	{
		cout << "Pixel is already part of another plot!" << endl;
		return false;
	}
	//check if can fit
	if(x + plot_size > _cfg.km_width || y + plot_size > _cfg.km_height)
	{
		cout << "Plot does not fit!" << endl;
		return false;
	}
	int size = 0;
	switch(plot_size)
	{
		case(1): //small
			_out[byte_p] = 255; //red
			cout << "small plot" << endl;
			_uncolored[y][x] = 'S';
			break;
		case(2): //medium
			for(size_t i = 0; i < sqr_plot * COLOR_CHANNELS; i+=3)
			{
				size_t row = (i / _cfg.km_width) + y;
				cout << "medium plot" << endl;
				_out[byte_p + 1 + i + row * _cfg.km_width] = 255; //green
				size++;
				_uncolored[y][x + i/3] = 'M';
			}
			assert(size == sqr_plot);
			break;
		case(4): //large
			for(size_t i = 0; i < sqr_plot * COLOR_CHANNELS; i+=3)
			{
				size_t row = (i / _cfg.km_width) + y;
				cout << "large plot" << endl;
				//_out[byte_p+2+i] = 255; //blue
				_out[byte_p + 2 + i + row * _cfg.km_width] = 255; //blue
				size++;
				_uncolored[y][x + i/3] = 'L';
			}
			assert(size == sqr_plot);
			break;
		default:
			throw("PLOT SIZE NOT RECOGNIZED");
			break;
	}
	return true;
}
