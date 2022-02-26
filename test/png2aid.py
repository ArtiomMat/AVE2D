"""
	How this script works:
		It looks for all PNG files in the current directory.
		It then converts each PNG file to an AID file.
		If a PNG file's name ends with a number it will become a frame in the final AID file.
		example: if we have "X_0.png" and "X_1.png" it will become "X.aid" with two frames
		the first will have X_0.png's pixel data and the second will have X_1.png's pixel data.
		If a PNG file's name does not end with a number it becomes a single frame AID file.
		The script also creates a PAL file, containing all entries used
		for the AID files, the game can then simply load all the AID files
		and the PAL file, and it will work like magic.

	Now keep in mind:
		You must have PIL installed
"""
from lib2to3.pgen2.token import LESS
import os, sys
import PIL.Image

# Turns pixels(array of RGB values) into a list of palette indices
# by finding the index of the color in the palette list
# Then it returns the list of indices
def rgb_to_indices(pixels, palette):
	indices = []
	for pixel in pixels:
		indices.append(palette.index(pixel))
	return indices

def main():
	# Index 0 is reserved for the transparent color, which is magenta
	palentries = [(255, 0, 255)]
	# Get all PNG files in the current directory
	pngfiles = [f for f in os.listdir('.') if f.endswith('.png')]
	# First create the PAL file and setup palentries
	fpal = open("PAL", "wb")
	for pngfile in pngfiles:
		# Open the PNG file
		png = PIL.Image.open(pngfile)
		# Get all the colors used in the PNG file
		colors = png.getcolors()
		# Set colors to the actual colors without their count
		colors = [color[1] for color in colors]
		# Remove the alpha channel
		colors = [color[:3] for color in colors]
		# Add the colors to the palentries if they are not already in there
		for color in colors:
			if color not in palentries:
				palentries.append(color)
	for color in palentries:
		fpal.write(bytes(color))
	# Fill the rest of the file(the file is 256 bytes long) with 0x00
	fpal.write(bytes([0] * (256 - len(palentries)*3)))
	fpal.close()
	# Now create the AID files
	pngfiles.sort()
	i = 0
	while i < len(pngfiles):
		name = pngfiles[i].split('.')[0]
		png = PIL.Image.open(pngfiles[i])
		
		totalframes = 0
		width, height = png.size # We assume all frames are the same size, unsafe but whatever

		if name.split('_')[-1].isdigit():
			faid = open(name.split('_')[0] + ".aid", "wb")
			# Put all files that have the name before _ into a list
			all = []
			for j in range(len(pngfiles)):
				if pngfiles[j].split('.')[0].split('_')[0] == name.split('_')[0]:
					all.append(pngfiles[j])
			i += len(all) - 1 # Skip because we already did this one
			# Sort the list by the number, I found this on StackOverflow
			all.sort(key=lambda f: int(''. join(filter(str. isdigit, f))))
			totalframes = len(all)

			indices = []
			# We no will open all the files and get their pixel data into the pixeldata list
			for j in range(len(all)):
				png = PIL.Image.open(all[j])
				# Get the pixel data
				pixels = png.getdata()
				# Turn pixels into regular list of RGB values, NOT RGBA
				pixels = [pixel[:3] for pixel in pixels]
				# Convert the pixels to indices
				indices += rgb_to_indices(pixels, palentries)
			
			faid.write(bytes([0x69, ord('A'), ord('I'), ord('D')]))
			faid.write((width).to_bytes(4, "little"))
			faid.write((height).to_bytes(4, "little"))
			faid.write((totalframes).to_bytes(4, "little"))
			for index in indices:
				faid.write(index.to_bytes(1, "little"))
			faid.close()
		else:
			faid = open(name + ".aid", "wb")
			totalframes = 1
			# Getting the pixels
			pixels = png.getdata()
			# Turn pixels into regular list of RGB values, NOT RGBA
			pixels = [pixel[:3] for pixel in pixels]
			# Convert the pixels to indices
			indices = rgb_to_indices(pixels, palentries)
			# Write the header
			faid.write(bytes([0x69, ord('A'), ord('I'), ord('D')]))
			# Width(uint32_t), height(uint32_t), total frames(uint32_t)
			faid.write((width).to_bytes(4, "little"))
			faid.write((height).to_bytes(4, "little"))
			faid.write((totalframes).to_bytes(4, "little"))
			# Write the indices
			for index in indices:
				faid.write(index.to_bytes(1, "little"))
			# Close the AID file
			faid.close()
		i+=1

		

if __name__ == '__main__':
	main()
