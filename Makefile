# http://www.partow.net/programming/makefile/index.html

CXX      := g++
CXXFLAGS := -Wall -std=c++17
LDFLAGS  := -lSDL2
BUILD    := ./build
OBJ_DIR  := $(BUILD)/objects
TARGET   := canvas
INCLUDE  := -I./ -Ilib
SRC      := $(wildcard lib/imgui/*.cpp) \
			$(wildcard ./*.cpp)

OBJECTS  := $(SRC:%.cpp=$(OBJ_DIR)/%.o)

ifeq ($(OS), Windows_NT)
	LDFLAGS += -lopengl32 -lglew32
else
	LDFLAGS += -lGL -lGLEW
endif

all: build $(BUILD)/$(TARGET)

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@ $(LDFLAGS)

$(BUILD)/$(TARGET): $(OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -o $(BUILD)/$(TARGET) $^ $(LDFLAGS)

.PHONY: all build clean debug release run

build:
	@mkdir -p $(BUILD)
	@mkdir -p $(OBJ_DIR)

debug: CXXFLAGS += -DDEBUG -g
debug: all

profile: CXXFLAGS += -pg
profile: debug

release: CXXFLAGS += -O2
release: all

clean:
	-@rm -rvf $(BUILD)/*

run:
	$(BUILD)/$(TARGET)
