CXX = g++
INCLUDES = -Iinclude
CXXFLAGS = -lstdc++ -std=c++0x -O

## FILES = VirtualMachine CentralProcessingUnit ControlUnit ArithmeticalLogicalUnit
## SRCDIR = src/
## SRC = $(SRCDIR)/$(FILES).cpp
## SRC = $(SRCDIR)/VirtualMachine.cpp $(SRCDIR)/CentralProcessingUnit.cpp $(SRCDIR)/ControlUnit.cpp $(SRCDIR)/ArithmeticalLogicalUnit.cpp

INCDIR = include/
## OBJ = $(SRC:.cpp=.o)

## EXECUTABLE = vm

## main targets:

.PHONY: vm
vm: main_vm.o  VirtualMachine.o CentralProcessingUnit.o ControlUnit.o Ram.o ArithmeticalLogicalUnit.o CommunicationChannel.o CommunicationUnit.o
	$(CXX) -o vm $^

.PHONY: asm
asm: main_asm.o Assembler.o
	$(CXX) -o asm $^
	
## secondary VM targets:
VirtualMachine.o: src/VirtualMachine.cpp include/VirtualMachine.hpp
	$(CXX) $(CXXFLAGS) -c $(INCLUDES) src/VirtualMachine.cpp -o $@
	
CentralProcessingUnit.o: src/CentralProcessingUnit.cpp include/CentralProcessingUnit.hpp
	$(CXX) $(CXXFLAGS) -c $(INCLUDES) src/CentralProcessingUnit.cpp -o $@
	
ControlUnit.o: src/ControlUnit.cpp include/ControlUnit.hpp include/Instructions_VM.hpp
	$(CXX) $(CXXFLAGS) -c $(INCLUDES) src/ControlUnit.cpp -o $@
	
Ram.o: src/Ram.cpp include/Ram.hpp
	$(CXX) $(CXXFLAGS) -c $(INCLUDES) src/Ram.cpp -o $@
	
ArithmeticalLogicalUnit.o: src/ArithmeticalLogicalUnit.cpp include/ArithmeticalLogicalUnit.hpp
	$(CXX) $(CXXFLAGS) -c $(INCLUDES) src/ArithmeticalLogicalUnit.cpp -o $@
	
CommunicationUnit.o: src/CommunicationUnit.cpp include/CommunicationUnit.hpp
	$(CXX) $(CXXFLAGS) -c $(INCLUDES) src/CommunicationUnit.cpp -o $@
	
CommunicationChannel.o: src/CommunicationChannel.cpp include/CommunicationChannel.hpp
	$(CXX) $(CXXFLAGS) -c $(INCLUDES) src/CommunicationChannel.cpp -o $@

main_vm.o: main_vm.cpp
	$(CXX) $(CXXFLAGS) -c $(INCLUDES) main_vm.cpp -o $@

## secondary ASM targets:	
Assembler.o: src/Assembler.cpp include/Assembler.hpp include/Instructions_VM.hpp include/Instructions_ASM.hpp
	$(CXX) $(CXXFLAGS) -c $(INCLUDES) src/Assembler.cpp -o $@
	
main_asm.o: main_asm.cpp
	$(CXX) $(CXXFLAGS) -c $(INCLUDES) main_asm.cpp -o $@

.PHONY: clean
clean:
	rm -f *.o 
