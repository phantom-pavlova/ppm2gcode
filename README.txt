/* 
ppm2gcode
*/ 



to compile use 'make'

to use, get an image, then convert to ppm (convert xxx.jpg xxx.ppm)
then:
ee@ee>ppm2gcode xxx.ppm

it will ask you for 
-count - how many samples
-plot size - final image width in mm (height is according to aspect ratio of original image)
-render type - 0 for softer tones, 1 for cartoons/strong images
-filename - zzz will write to zzz.ngc



check your output with a gcode sender (ugsplatform, bCNC etc..) before plotting!
try (plotsize/5)^2 for count, increase/decrease by order of magnitude until you get a good render.
my plotter is about 2m x 4m, a good image at that size can take days to print. 






TODO
.......................
circles, they are 10 % quicker, but put down 10% less ink.

still not entirely happy with the variety of square sizes,
 maybe do a log somewhere so white is missed, light grey is 3mm and black is max size.
 we could calculate the ink per cm2 given a line width ?!

why do we loose a few points in optimisation?

optmise a bit more for more points? 100 points per tile?
 - if unsure count distance in G0 and G1, if GO is more than 50% you can remove hours from the plot with better tiling or subtile optimisation
 - this is only true for square render, spiral has no G0

configurable feed rate and cut depth (from rc.ppm2gcode)
 - also if you have pulse time, pulse per mm, max speed and acceleration, you can estimate plot time (must stick these values into bCNC)

other rendering styles - (spiral amplitude/pulse width modulation), cloud points -> triangles.  google LinesLab/www.artnsoul.io

gui - so you can see what you will get (need to estimate plot time)

svg output (gcode2svg exists)

time: the chain 
	{
	working plotter (no missing bits (depth map) or hangs), 
	identifiable file (8h for reprint!!), 
	least drawing for good image, 
	least G0, 
	fastest acceptable rate/acc
	}
 - these all affect the print time. The big plotter will prevent other work if it takes a day to print 24 patts (still better than current hand plots), 
	but if it can do this in 2 hrs at 7am, we have gained days! 



SHELVED / OTHER SOLUTION FOUND
.......................
title (filename)
progress bar (draw rectangle with progress inside, and mark a bar every nth move(say bar=20, 10000 moves, mark every 500 moves)
 - not nec. as the sender tels you, and you can see the tiles 




DONE
.......................
load ppm

output gcode with squares proportional to greyscale

basic path optimisation (actually this can save you a week on a 6 Hr plot)

levels better (leena had been a problem)

put size in comments, so easier to check before print.

more optimisation, according to square count
-more to do I suspect. still 80/20 rule!

do not plot the same shapes twice, just destroy the image data after a read, and check it is valid before a read (> 0 and < 3x255)
 - you may still get very close points, but I have seen plotter redraw the exact same squares! - pointless! 









