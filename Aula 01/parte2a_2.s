      .equ        getChar,2
      .equ        putChar,3
      .equ        printInt,6
      .data
      .text
      .global main
main:
      move  $s0,$0
loop:
      li    $v0,getChar
      syscall
      # move  $a0,$v0
      move  $s1,$v0     # save v0 for exiting at '\n'
      addi  $a0,$v0,1
      li    $v0,putChar
      syscall
      addi  $s0,$s0,1
      bne   $s1,'\n',loop
endloop:
      li    $v0,printInt
      move  $a0,$s0
      li    $a1,10
      syscall
      
      jr    $ra
