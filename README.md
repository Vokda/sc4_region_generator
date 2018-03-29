This program will generate random regions with city plots for SimCity 4.
Heightmaps are created using simplex noise in two dimensions.
**The code is in an early stage, not all features are implemented yet.**

## Compilation
To compile run
```
mkdir build && cd build
cmake ../CMakeLists.txt
make
```
The resulting binary will search for a configuration file in its directory, meaning you will need to move,
copy or link the config file `reg_gen.cfg` to the build directory.  Parameters can be set in
`reg_gen.cfg`. Width and height must to be integers.

## Generating a Region
To obtain the heightmap simply run `./sc4_region_gen`. 
Feel free to play around with the parameters in `reg_gen.cfg`.
The heightmap will be called `out.png`.
No `config.bmp` (city plots) is generated at the moment
and you will have to draw one yourself.

## City Plots Instructions
Since no `config.bmp` is generated you will have to create one yourself. Don't worry, it's easy.
The bmp can be created in your favourite painting program.
Create a bmp of size equal to width*height in your config in pixels.
E.g.: 
if your config says
```
width 12
height 12
```
then your `config.bmp` should be 12*12 pixels.
Colour it accordingly:
- Small city plot: red, 1*1 pixel
- Medium city plot: green, 2*2 pixel
- Large city plot: red, 4*4 pixel

The plots can be placed anywhere as long as you fill the entire image.

## Importing it to SimCity
1. Find the directory where the regions are located. It can be `~/Documents/SimCity4/Regions/`. On Windows it
is most likely under 'My Documents'.
2. Create a directory in the `Region/` directory called whatever you want to call your region.
3. Put the heightmap `out.png` and the city plots `config.bmp` in the directory you just created.
4. Open SimCity. Load your region. DON'T PANIC! Its supposed to be flat. 
5. Press `ctrl+shift+alt+r`. Select `out.png`.
6. Wait....
7. If you have generated a large map, wait even more.
8. Done! Have fun! :)

## Credits
The .png files are handled with Lode Vandevenne's [lodepng](https://github.com/lvandeve/lodepng).
Stefan Gustavson's paper [Simplex noise demystified](http://staffwww.itn.liu.se/~stegu/simplexnoise/simplexnoise.pdf)  helped a great deal in understanding the algorithm. It also has a code example in Java of the simplex noise algorithm for up to 4 dimensions which was useful for comparing results. This project's simplex noise algorithm is more or less a C++ port of the example code given in the paper. 
