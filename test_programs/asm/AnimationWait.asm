  @16384
  M=-1
  @16416
  M=-1
  @16448
  M=-1
  @16480
  M=-1
  @16512
  M=-1
  @16544
  M=-1
  @16576
  M=-1
  @16608
  M=-1
  @16640
  M=-1
  @16672
  M=-1
  @16704
  M=-1
  @16736
  M=-1
  @16768
  M=-1
  @16800
  M=-1
  @16832
  M=-1
  @16864
  M=-1

  @SCREEN
  D=A
  @row
  M=D

(DESCEND)
  @15   // wait time in milliseconds
  D=A
  @24577   // address of timer register
  M=D
  @DESCEND_WAIT
(DESCEND_WAIT)
  @24577
  D=M
  @DESCEND_WAIT
  D;JGT
(DESCEND_GO)
  @row
  AD=M
  M=0     // clear top row of square
  @512
  A=D+A
  M=-1    // set next bottom row of square
  @32
  D=A
  @row
  MD=D+M  // row = row + 32
  @24064  // row y = 216-32
  D=D-A;
  @ASCEND
  D;JEQ
  @DESCEND
  0;JMP

(ASCEND)
  @15   // wait time in milliseconds
  D=A
  @24577   // address of timer register
  M=D
  @ASCEND_WAIT
(ASCEND_WAIT)
  @24577
  D=M
  @ASCEND_WAIT
  D;JGT
(ASCEND_GO)
  @32
  D=A
  @row
  AMD=M-D    // row = row - 32
  M=-1      // set the next top row of square
  @512
  A=D+A
  M=0       // clear bottom row of square
  @row
  D=M
  @SCREEN
  D=D-A
  @DESCEND
  D;JEQ
  @ASCEND
  0;JMP