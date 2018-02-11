
# Get all the libraries needed (LTI)
LTIBASE:=/home/lleon95/ltilib-2
LTICMD:=$(LTIBASE)/linux/lti-local-config
LIBS:=$(shell $(LTICMD) --libs) $(EXTRALIBPATH) $(EXTRALIBS)

# Defining architecture
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

# Defining VPATH
VPATH:=$(VPATHADDON)

# Setting the compilation flags
CXXINCLUDE:= $(patsubst %,-I%,$(subst :, ,$(VPATH)))
LTICXXFLAGS:=$(shell $(LTICMD) --cxxflags)
CXXFLAGSREL:=-c -O3 $(CPUARCH) -Wall -ansi $(LTICXXFLAGS) $(CXXINCLUDE)


all: imageLoader
 
# Creating the executable
imageLoader: imageLoader.o
	g++ -o $@ $< $(LIBS)
# Creating the object
imageLoader.o: imageLoader.cpp
	g++ -std=c++11 $(CXXFLAGSREL) $<
# Remove all the settings to recompile
clean:
	rm imageLoader imageLoader.o 