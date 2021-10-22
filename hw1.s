.data
input:  .word 0x00000005

.text
main:
  lw a0, input
  
  jal ra, findComplement
  jal ra, printResult
  
  li a7, 10 # end program
  ecall
  
findComplement:
  li t0, 0xffffffff # mask
  loop:
    and t1, t0, a0 # mask & input
    beq t1, x0, exit # t1 == 0, goto exit
    slli t0, t0, 1 # mask <<= 1
    j loop
  exit:
  not t0, t0 # mask = ~mask
  xor a0, a0, t0 # input = input ^ mask
  jr ra
  
printResult:
  li a7, 1 # print integer
  ecall
  jr ra