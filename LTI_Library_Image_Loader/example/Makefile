
LTIBASE:=/home/lleon95/ltilib-2
LTICMD:=$(LTIBASE)/linux/lti-local-config
LIBS:=$(shell $(LTICMD) --libs) $(EXTRALIBPATH) $(EXTRALIBS)

ifeq "$(CXX_MAJOR)" "2"
  VPATHADDON=:g++
  CPUARCH = -march=i686 -ftemplate-depth-35
  CPUARCHD = -march=i686 -ftemplate-depth-35
else
  ifeq "$(CXX_MAJOR)" "3"
  VPATHADDON=
  CPUARCH = -march=pentium4
  CPUARCHD = -march=pentium4
  else
  VPATHADDON=
  CPUARCH = -march=native
  CPUARCHD = 
  endif
endif

VPATH:=$(VPATHADDON)

CXXINCLUDE:= $(patsubst %,-I%,$(subst :, ,$(VPATH)))

LTICXXFLAGS:=$(shell $(LTICMD) --cxxflags)
CXXFLAGSREL:=-c -O3 $(CPUARCH) -Wall -ansi $(LTICXXFLAGS) $(CXXINCLUDE)


all: imageShow
 
# $@ matches the target; $< matches the first dependent
imageShow: imageShow.o
	g++ -o $@ $< $(LIBS)

imageShow.o: imageShow.cpp
	g++ -std=c++11 $(CXXFLAGSREL) $<
     
clean:
	rm imageShow imageShow.o 