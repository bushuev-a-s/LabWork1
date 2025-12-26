PROJECT = bmp
CXX = g++
CXXFLAGS =-std=c++17 -Wall -Wextra -pedantic -fpermissive
SRC = main.cpp BMP.cpp BMPheader.cpp
OBJ = $(SRC:.cpp=.o)

all: $(PROJECT)

$(PROJECT): $(OBJ)
	$(CXX) -o $@ $^

%.o: %.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)

clean:
	rm -f gaussian_blurred.bmp  rotated_ccw.bmp rotated_cw.bmp $(OBJ) $(PROJECT)

.PHONY: all clean
