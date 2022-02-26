"""
from PIL import Image

im = Image.open('AVE2.png') # Can be many different formats.
pix = im.load()
print (im.size)  # Get the width and hight of the image for iterating over
for y in range(64):
	for x in range(48):
		p = pix[x,y]
		if p[0] == 0:
			print ('0,', end =" ")  # Get the RGBA Value of the a pixel of an image
		else:
			print ('1,', end =" ")  # Get the RGBA Value of the a pixel of an image
	print ()
"""

from os import walk

filenames = next(walk("src"), (None, None, []))[2]  # [] if no file

# pop files that end with .h in filenames
outs = ""
for x in filenames:
	if ".c" in x:
		n = x.split(".")[0]
		outs += n + ".o "
		#print(n+".o: "+n+".c")
		#print("\t$(cc) $(flags) -o $@ -c $^")

print()
# Clean for CMD not rm -f
print(outs)
print("\trm -f *.o")
