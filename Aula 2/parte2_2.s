      .equ  resetCoreTimer,12
      .equ  readCoreTimer,11
      .equ  putChar,3
      .equ  printInt,6
      .data
      .text
      .globl main
main:
      li    $t0,0             # cnt1 = 0
      li    $t1,0             # cnt5 = 0
      li    $t2,0             # cnt10 = 0
while:
      la    $a0,'\r'
      li    $v0,putChar
      syscall
      move  $a0,$t0
      li    $a1,5
      sll   $a1,$a1,16
      ori   $a1,$a1,10
      li    $v0,printInt
      syscall

      la    $a0,'\t'
      li    $v0,putChar
      syscall
      move  $a0,$t1
      li    $a1,5
      sll   $a1,$a1,16
      ori   $a1,$a1,10
      li    $v0,printInt
      syscall

      la    $a0,'\t'
      li    $v0,putChar
      syscall
      move  $a0,$t2
      li    $a1,5
      sll   $a1,$a1,16
      ori   $a1,$a1,10
      li    $v0,printInt
      syscall

      li    $v0,resetCoreTimer
      syscall
delay:
      li    $v0,readCoreTimer
      syscall
      blt   $v0,2000000,delay
      rem   $t3,$t2,10
      beq   $t3,0,cnt1
      j     cnt5
cnt1:
      addi  $t0,$t0,1
      rem   $t3,$t2,2
      beq   $t3,0,cnt5
      j     cnt10
cnt5:
      addi  $t1,$t1,1
cnt10:
      addi  $t2,$t2,1
      j     while

      jr    $ra
