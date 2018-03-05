
CXX	:= c++
CXXFLAGS := -g
    
OBJECTS	:= file_parse.o #main.o

#my_program: $(OBJECTS)
#	$(CXX) $(OBJECTS) -o my_program

parse.o: file_parse.cpp file_parse.h
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c file_parse.cpp -o file_parse.o
#main.o: main.cpp
#	$(CXX) $(INCLUDES) $(CXXFLAGS) -c main.cpp -o main.o
clean:
	rm file_parse.o

