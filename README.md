To compile and run the test program Prim:

1) Make asm
  compiles main_asm.cpp, creates a program called asm
2) Make vm
  compiles main_vm.cpp, creates a program called vm

3) run asm
  compiles the programm in "Prim.txt" from assembly-code to machine-code. Creates a new file "mc_Prim.txt".
4) run vm
  runs the Virtual machine, which is initialized by the program from "mc_Prim.txt"

5) enjoy :)

6) see the file ram.txt which is displays the status of the ram of the vm at termination.

See main_asm.cpp and main_vm.cpp for some general settings and to change input- and output- filenames.
See include/Instructions_VM.hpp for an overview of all instructions provided by the virtual machine.

Documentation will be available here in the future. (...maybe)
