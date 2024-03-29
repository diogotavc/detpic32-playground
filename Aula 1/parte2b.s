      .equ  inkey,1
      .equ  putChar,3
      .equ  printInt,6
      .data
      .text
      .globl main
main:
      move  $s0,$0
loop:
      li    $v0,inkey
      syscall
      beq   $v0,$0,else
      move  $t0,$v0
if:
      move  $a0,$v0
      li    $v0,putChar
      syscall
      j     endif
else:
      li    $a0,'.'
      li    $v0,putChar
      syscall
endif:
      addi  $s0,$s0,1
      bne   $t0,'\n',loop
endloop:
      li    $v0,printInt
      move  $a0,$s0
      li    $a1,10
      syscall

      jr $ra
