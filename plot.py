import matplotlib.pyplot as plt
import re
import sys
import numpy as np

#find float
float_pattern = re.compile('[-]?(\d(\.)?\d*)+(e(-)?\d+)?')

if len(sys.argv) < 2:
    sys.exit("Enter one file of input")
lines = open(sys.argv[1]).read().splitlines()

print 'gathering data...'

noise =[]
g = []
coords = []
grads = []
px = []
py = []
for line in lines:
    if "perlin output" in line:
        noise += [float(float_pattern.search(line).group())]
    elif "g: " in line:
        g += float(float_pattern.search(line).group())
    elif "corner" in line:
        for n in line[9:].split( ):
            coords += [float(n)]
        #print [float(float_pattern.search(line).group())]
        #coords += [float(float_pattern.search(line).group())]
    elif "px " in line:
        px += [float(float_pattern.search(line).group())]
    elif "py " in line:
        py += [float(float_pattern.search(line).group())]

        
print 'Done!'

i = np.arange(0, len(noise))
#plt.plot(noise)

plt.scatter(noise, i)
plt.title('noise')
plt.show()

#plt.scatter(px, py)
#plt.title('simplex cell coordinate')
#plt.show()

#plt.title('coords')
#plt.scatter(coords[0::2], coords[1::2])
#plt.show()
