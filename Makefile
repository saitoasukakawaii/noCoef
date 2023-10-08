#*************************************************************************#
#                                                                         #
#  Program: Makefile                                                      #
#  Version: 2.0                                                           #
#  By: Mette Olufsen                                                      #
#  Date: 14. Jan. 1997                                                    #
#                                                                         # 
#  A makefile that ensures that all modules are linked together in the    #
#  right order.                                                           #
#*************************************************************************#

# $Id: Makefile,v 1.8 2010-10-20 14:39:59 heine Exp $




# c++ compiler
CXX = g++
# fortran compiler
FC = gfortran
# loader
LD = g++

CXXFLAGS=-O3 -g -Wall -std=c++11 -D_REENTRANT

FFLAGS=-O2 -g -Wall -I/usr/include
FLIBS=-lgfortran -lfftw3

LIBS=$(FLIBS) -lm

LDFLAGS=-O3

MAIN=sor07_new

SRCS := $(wildcard *.cpp *.C *.f90)
OBJS := $(patsubst %.cpp,%.o,$(SRCS))
OBJS := $(patsubst %.C,%.o,$(SRCS))
OBJS := $(patsubst %.f90,%.o,$(SRCS))


.PHONY: all
all: $(MAIN)

$(MAIN): $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LIBS)
	
%.o: %.f90
	$(FC) $(FFLAGS) -c $<

%.o: %.C
	$(CXX) $(CXXFLAGS) -c $<

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

.PHONY: clean
clean:
	-rm -f *.o *.mod 

.PHONY: veryclean
veryclean: clean
	-rm $(MAIN) a.out *~ ./result/q* ./result/p* ./result/U* ./result/A* ./result/C* ./result/NO* ./result/CA* ./result/cGMP* ./result/WSS* Zhat*
	
.PHONY: run
run: ${MAIN}
	./${MAIN} ${ARGS}
