PROJECT = bmp
CXX = g++
CXXFLAGS = -Wall -Wextra -pedantic
SRC = BMP.cpp
OBJ = $(SRC:.cpp=.o)

all: $(PROJECT)

$(PROJECT): $(OBJ)
	$(CXX) -o $@ $^

%.o: %.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)

clean:
	rm -f $(OBJ) $(PROJECT)

.PHONY: all clean
