CXX = g++
INCLUDES = -Iinclude
CXXFLAGS = -lstdc++ -std=c++0x -O

SOURCES = $(shell find src -type f -name "*.cpp")
DEBUG_OBJECTS = $(patsubst src/%,objects/%,$(SOURCES:.cpp=_debug.o))

INCDIR = include/

## main targets:
.PHONY: vm
vm: objects/main_vm.o objects/GenericMachine.o objects/VirtualMachine.o objects/CentralProcessingUnit.o objects/ControlUnit.o objects/Ram.o objects/ArithmeticalLogicalUnit.o objects/CommunicationChannel.o objects/CommunicationUnit.o objects/Tests.o
	$(CXX) -o vm $^

.PHONY: asm
asm: objects/main_asm.o objects/Assembler.o
	$(CXX) -o asm $^

vm_debug: $(DEBUG_OBJECTS)
	$(CXX) $^ -o vm_debug

## secondary targets:
objects/%.o: src/%.cpp
	mkdir -p objects
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -o $@ $<

objects/%_debug.o: src/%.cpp
	mkdir -p objects
	$(CXX) $(CXXFLAGS) -D DEBUG $(INCLUDES) -c -o $@ $<

.PHONY: clean
clean:
	rm -f objects/*.o 
