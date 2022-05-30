@50     // Set A-register to 50
D=A     // Set D-register to the value in A
@100    // Set A-register to 100
M=D     // Set Mem[A] to the value in D

        // Mem[100] should be 50

A=M     // Set A-register to Mem[A]

        // A-register should be 50

D=A+1   // Set D-register to A+1

        // D-register should be 51

@0      // Set A-register to 0
M=D     // Set Mem[0] to the value in D

        // Mem[0] should be 51