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

# CXX=/opt/local/bin/g++-mp-4.6
CXX = g++
CXXFLAGS=-O3 -g -Wall -std=c++11 -D_REENTRANT

# ifeq ($(shell arch),ppc)
# FC=/usr/local/bin/g95
# FFLAGS=-O2 -g -Wall -fno-second-underscore
# FLIBS=-L$(shell $(FC) --print-file-name=) -lf95
# else
# FC=/opt/local/bin/gfortran-mp-4.6
# FFLAGS=-O2 -g -Wall
# FLIBS=-lgfortran
# endif

FC=gfortran
FFLAGS=-O2 -g -Wall
FLIBS=-lgfortran

LIBS=$(FLIBS) -lm

LDFLAGS=-O3

MAIN=sor07_new

OBJS1=tools.o ${MAIN}.o arteries.o
OBJS2=impedance_sub.o impedance_init_sub.o root_imp.o f90_tools.o special_functions.o



all: $(MAIN)

$(MAIN): $(OBJS1) $(OBJS2) 
	$(CXX) -o $(MAIN) $(LDFLAGS) $(OBJS1) $(OBJS2) $(LIBS)
	
# sor06.o: sor06.C sor06.h
# 	$(CXX) -c $(CXXFLAGS) sor06.C
# sor07.o: sor07.cpp sor07.h
# 	$(CXX) -c $(CXXFLAGS) sor07.cpp
${MAIN}.o: ${MAIN}.cpp ${MAIN}.h
	$(CXX) -c $(CXXFLAGS) ${MAIN}.cpp
	
arteries.o: arteries.C arteries.h tools.h ${MAIN}.h
	$(CXX) -c $(CXXFLAGS) arteries.C
	
tools.o: tools.C tools.h
	$(CXX) -c $(CXXFLAGS) tools.C
		
root_imp.o: root_imp.f90 f90_tools.o
	$(FC) -c $(FFLAGS) root_imp.f90
	
f90_tools.o: f90_tools.f90
	$(FC) -c $(FFLAGS) f90_tools.f90

special_functions.o: special_functions.f90
	$(FC) -c $(FFLAGS) special_functions.f90
	
impedance_sub.o: impedance_sub.f90 f90_tools.o root_imp.o
	$(FC) -c $(FFLAGS) impedance_sub.f90
	
impedance_init_sub.o: impedance_init_sub.f90 f90_tools.o root_imp.o
	$(FC) -c $(FFLAGS) impedance_init_sub.f90
	
clean:
	-rm -f *.o *.mod ./result/q* ./result/p* ./result/U* ./result/A* ./result/C*
	
veryclean: clean
	-rm $(MAIN) a.out *~
	
run: ${MAIN}
	./${MAIN} ${ARGS}
