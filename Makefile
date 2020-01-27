
CXX	:= c++
CXXFLAGS := -g -Iinclude

OBJECTS	:= file_parse.o libfile_parse.a #main.o

all: parse.o libfile_parse.a

#my_program: $(OBJECTS)
#	$(CXX) $(OBJECTS) -o my_program

parse.o: src/file_parse.cpp include/file_parse.h
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c src/file_parse.cpp -o src/file_parse.o
libfile_parse.a:
	ar rcs lib/libfile_parse.a src/file_parse.o
	rm src/*.o

#main.o: main.cpp
#	$(CXX) $(INCLUDES) $(CXXFLAGS) -c main.cpp -o main.o
clean:
	rm src/*.o
	rm lib/*.a
