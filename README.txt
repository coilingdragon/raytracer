This project was written during my computer graphics class, please note that one requirement of this project was to build it in an object oriented fashion. This means that the code is not organized in a normal C coding conventions. The txt files hold the source files for the images, the location of various objects and such. The ray tracer will build the images as ppm files. ppm files were chosen because of ease of the formatting of the file. 

extra's implemented:
spotlights
transparency
refraction
concave lens

transparency explination:
the flag '9000' after the information on the world size and coodinates is the 
indicator that transparency is turned on

then after that, after the ambient, diffuse and specular of each material,
add a decimal that is between 1 and 0. 0 indicates non-transparent and 1 
indicates full transparency the decimal equates to the percentage the object
is transparent by


refraction explination:
the flag '9001' indicates that refraction is turned on, and transparency must
be turned on for refraction to work.

the index of refraction is put on the line below the transparency percentage

lens:
a concave lens made from a hollow half sphere

