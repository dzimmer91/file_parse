
CXX	:= c++
CXXFLAGS := -g -Iinclude

OBJECTS	:= file_parse.o libfile_parse.a #main.o

all: deps/file_parse.o bin/num_in_view

#my_program: $(OBJECTS)
#	$(CXX) $(OBJECTS) -o my_program

deps/file_parse.o: src/file_parse.cpp include/file_parse.h
	$(CXX) $(INCLUDES) $(CXXFLAGS) -c src/file_parse.cpp -o deps/file_parse.o
	ar rcs lib/libfile_parse.a deps/file_parse.o

bin/num_in_view: mains/main_num_in_view.cpp include/file_parse.h lib/libfile_parse.a
		$(CXX) $(INCLUDES) $(CXXFLAGS) mains/main_num_in_view.cpp -o bin/num_in_view lib/libfile_parse.a

#main.o: main.cpp
#	$(CXX) $(INCLUDES) $(CXXFLAGS) -c main.cpp -o main.o

clean:
	rm deps/*.o
	rm lib/*.a
	rm bin/*
