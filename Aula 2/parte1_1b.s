      .equ  readCoreTimer,11
      .equ  resetCoreTimer,12
      .equ  putChar,3
      .equ  printInt,6
      .data
      .text
      .globl main
main:
      li    $t0,0                   # counter = 0
while:                              # while (1) {
      la    $a0,'\r'                #     putChar('\r')
      li    $v0,putChar             #
      syscall                       #
      move  $a0,$t0                 #     printInt(counter,
      li    $a1,4                   #
      sll   $a1,$a1,16              #
      ori   $a1,$a1,10              #     10 | 4 << 16)
      li    $v0,printInt            #
      syscall                       #
      li    $v0,resetCoreTimer      #     resetCoreTimer()
      syscall                       #
timerloop:                          #     while(
      li    $v0,readCoreTimer       #     readCoreTimer()
      syscall                       #
      blt   $v0,200000,timerloop    #     < 200_000)
      addi  $t0,$t0,1               #     counter++
      j     while                   # }
      jr    $ra
