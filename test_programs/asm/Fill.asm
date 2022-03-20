// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Fill.asm

// Runs an infinite loop that listens to the keyboard input.
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel;
// the screen should remain fully black as long as the key is pressed.
// When no key is pressed, the program clears the screen, i.e. writes
// "white" in every pixel;
// the screen should remain fully clear as long as no key is pressed.

// Put your code here.

  @color
  M=0
  @lastKey
  M=0
  @FILL
  0; JMP

(MAIN_LOOP)
  // if lastKey == *KBD goto MAIN_LOOP
  @lastKey
  D=M
  @KBD
  D=D-M
  @MAIN_LOOP
  D; JEQ

  // lastKey = *KBD
  @KBD
  D=M
  @lastKey
  M=D

  // color = 0
  @color
  M=0
  // if *KBD == 0 goto FILL
  @KBD
  D=M
  @FILL
  D; JEQ
  // color = -1
  @color
  M=-1

(FILL)
  // i = 8192
  @8192
  D=A
  @i
  M=D

  // addr = SCREEN
  @SCREEN
  D=A
  @addr
  M=D

(FILL_LOOP)
  // if i<=0 goto MAIN_LOOP
  @i
  D=M;
  @MAIN_LOOP
  D; JLE

  // *addr = color
  @color
  D=M
  @addr
  A=M
  M=D

  // i--; addr++
  @i
  M=M-1
  @addr
  M=M+1

  // goto FILL_LOOP
  @FILL_LOOP
  0; JMP