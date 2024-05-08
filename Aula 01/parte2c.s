      .equ  printStr,8
      .equ  readInt10,5
      .equ  printInt10,7
      .equ  printInt,6
      .data
msg:        .asciiz     "\nIntroduza um inteiro (sinal e módulo): "
signed10:   .asciiz     "\nValor em base 10 (signed): "
base2:      .asciiz     "\nValor em base 2: "
base16:     .asciiz     "\nValor em base 16: "
unsig10:    .asciiz     "\nValor em base 10 (unsigned): "
format10:   .asciiz     "\nValor em base 10 (unsigned), formatado: "
      .text
      .globl main
main:
      li    $v0,printStr
      la    $a0,msg
      syscall
      li    $v0,readInt10
      syscall
      move  $s0,$v0

      li    $v0,printStr
      la    $a0,signed10
      syscall
      li    $v0,printInt10
      move  $a0,$s0
      syscall

      li    $v0,printStr
      la    $a0,base2
      syscall
      li    $v0,printInt
      move  $a0,$s0
      li    $a1,2
      syscall

      li    $v0,printStr
      la    $a0,base16
      syscall
      li    $v0,printInt
      move  $a0,$s0
      li    $a1,16
      syscall

      li    $v0,printStr
      la    $a0,unsig10
      syscall
      li    $v0,printInt
      move  $a0,$s0
      li    $a1,10
      syscall

      li    $v0,printStr
      la    $a0,format10
      syscall

      li    $v0,printInt
      move  $a0,$s0
      li    $t0,5
      sll   $t0,$t0,16
      ori   $t0,$t0,10
      move  $a1,$t0
      syscall

      j main

