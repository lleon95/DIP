all: imageLoader
 
# $@ matches the target; $< matches the first dependent
imageLoader: imageLoader.o
	g++ -o $@ $< -lboost_program_options

imageLoader.o: imageLoader.cpp
	g++ -std=c++11 -c $< -lboost_program_options 
     
clean:
	rm imageLoader imageLoader.o 