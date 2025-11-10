// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/4/Fill.asm

// Runs an infinite loop that listens to the keyboard input. 
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel. When no key is pressed, 
// the screen should be cleared.

//// Replace this comment with your code.



// @SCREEN is the memory address 16384 to 24575 for the screen map
// @KBD is the memory address 24576 for the keyboard
(MAIN_LOOP)
    // check Keyboard memory value, D = value of key pressed
    @KBD
    D=M

    // @COLOR initilized at default value of 0 for white
    @COLOR
    M=0
    // if no key is pressed, jump to (NOT_PRESSED)
    @NOT_PRESSED
    D;JEQ

    // set color to black if key is pressed
    @COLOR
    M=-1

    (NOT_PRESSED)
    @SCREEN
    D=A
    // @PTR initialized at start of screen memory address 16384
    @PTR
    M=D

    (SCREEN_LOOP)
        // subtract keyboard memory value stored in PTR from keyboard memory value
        @PTR
        D=M
        @KBD
        D=D-A
        // if D is >= 0, jump to (END_SCREEN)
        @END_SCREEN
        D;JGE

        // set the current screen pixel block to black or white
        @COLOR
        D=M
        @PTR
        A=M     // A = current address
        M=D     // set memory at current address to the color

        // PTR increment, so PTR++
        @PTR
        M=M+1

        @SCREEN_LOOP
        0;JMP
    (END_SCREEN)

@MAIN_LOOP
0;JMP




