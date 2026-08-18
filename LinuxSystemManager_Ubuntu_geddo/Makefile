CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -pthread

TARGET = linux_system_manager
SOURCES = main.cpp SystemInfo.cpp ProcessManager.cpp FileManager.cpp NetworkManager.cpp ThreadDemo.cpp
OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET) signal_test

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

signal_test: signal_test.cpp
	$(CXX) $(CXXFLAGS) -o signal_test signal_test.cpp

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET) signal_test

.PHONY: all clean
