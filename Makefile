
RAYOBJS = main.o projection.o list.o model.o \
	      object.o plane.o material.o veclib.o \
  		  image.o raytrace.o sphere.o pplane.o \
		  light.o fplane.o matrixlib.o tplane.o \
		  spotlight.o psphere.o lens.o

#INCLUDE = ray.h rayhdrs.h

# CFLAGS = -DAA_SAMPLES=12 
ray: $(RAYOBJS)
		gcc -Wall -o ray -g $(RAYOBJS) -lm

clean: 
		rm *.o
$(RAYOBJS): $(INCLUDE)
#DEBUG = -DDBG_PIX -DDBG_HIT #-DDBG_FIND #-DDBG_WORLD -DDBG_AMB  
#DIFF = -DDBG_DIFFUSE
.c.o: $< 
	   -gcc -c -Wall $(CFLAGS) $(DEBUG) $(DIFF) -g $< 2> $(@:.o=.err) 
	   cat $*.err

