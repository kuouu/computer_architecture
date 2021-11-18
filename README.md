# Assignment1: RISC-V Assembly and Instruction Pipeline

[![hackmd-github-sync-badge](https://hackmd.io/CQQd2TqzQrCDrAV8f588Fw/badge)](https://hackmd.io/CQQd2TqzQrCDrAV8f588Fw)

###### tags: `Computer Architecture 2021`
contributed by <`erickuo5124`>

## Number Complement

The problem is chosen from [LeetCode476](https://leetcode.com/problems/number-complement/) with difficulty "Easy" as the assignment request. We need to **return the complement** of an given integer. For example:

> Input: 5 (101)
> Output: 2 (010)
>
> Input: 13 (1101)
> Output: 2 (0010)

## Implementation

To get the complement of input number, I compute an unsigned mask first, which represent the **valid bits** of input number. The way to get mask is left shift when it is not zero after AND operation.

$$
input: 00000101 \\
mask: 00000111 \\
\\XOR ------------ \\
output:00000010
$$

> I try to get the result like the expression below.

### C code

```c=
#include<stdio.h>

int findComplement(int num){
  unsigned mask = 0xffffffff;
  while(mask & num) mask <<= 1;
  return num ^ ~mask;
}

int main(){
  int input = 1;
  int result = findComplement(input);
  printf("%d", result);
}
```

I use while loop to get mask. In the last part of function, I need to flip the bits of mask.

### Assembly

```asm=
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
```

In asslembly, we need to specify which register to read or write. In addition, risc-v use register `a7` to tell `ecall` which action should be execute.

## Analysis

- `IF`: Instruction Fetch
- `ID`: Instruction Decode and register fetch
- `IE`: Execute
- `MEM`: Memory access
- `WB`: register Write Back

![](https://i.imgur.com/87bfc2B.png)

## Data Hazard

```asm
0000001c <loop>:
	1c:		00a2f333		and x6 x5 x10
	20:		00030663		beq x6 x0 12 <exit>
	24:		00129293		slli x5 x5 1
	28:		ff5ff06f		jal x0 -12 <loop>
```

Data hazard occur in the section of `<loop>`, and it occur as a RAW(read after write) data hazard. This could be solve by insert `nop` or data forwarding. In ripes, it implements the last one.

## Control Hazard

![](https://i.imgur.com/EDsqTkm.png)

After the branch execution, some instruction must be discard due to jump. Ripes insert `nop` to these instruction.

## Reference

- [riscv-spec](https://riscv.org/wp-content/uploads/2019/06/riscv-spec.pdf)
- [Ripes Introduction](https://github.com/mortbopet/Ripes/wiki/Ripes-Introduction)
- [在Hazard尋求解法是否搞錯了什麼](https://hackmd.io/@dZfCcN4hT8aUuDPv3B8CWQ/BkGKEG61F)
