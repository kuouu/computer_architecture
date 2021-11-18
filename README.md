# Assignment2: RISC-V Toolchain

[![hackmd-github-sync-badge](https://hackmd.io/nbJ7Qs5lTVCQ_GI66nVguQ/badge)](https://hackmd.io/nbJ7Qs5lTVCQ_GI66nVguQ)

###### tags: `Computer Architecture 2021`

contributed by `<erickuo5124>`

## Rewrite [Counting Bits](https://leetcode.com/problems/counting-bits)

I rewrite [this problem](https://hackmd.io/@oscarshiang/arch_hw1) because the topic is written by my friend. If I need help, he can figure out my problem quickly.

```c=
int popcnt(int a)
{
    int cnt = 0;
    while (a != 0) {
        a &= (a - 1);
        cnt++;
    }
    return cnt;
}

void count_bits(int n, int *rsize, int* res)
{
    for (int i = 0; i <= n; i++) {
        res[i] = popcnt(i);
    }

    *rsize = n + 1;
}

void _start()
{
    int rsize;
    int n = 5;
    int res[n+1];
    count_bits(n, &rsize, res);

    print_char('[');
    print_int32(res[0]);
    for (int i=1; i < rsize; i++) {
    	print_char(',');
	print_int32(res[i]);
    }
    print_char(']');
    print_char('\n');
}
```

execution result（with -O3）: 

```shell
[0,1,1,2,1,2]

>>> Execution time: 774884 ns
>>> Instruction count: 607 (IPS=783343)
>>> Jumps: 204 (33.61%) - 72 forwards, 132 backwards
>>> Branching T=197 (85.65%) F=33 (14.35%)
```

because there is no standard library, so I need to write my own print function:

```c=
static volatile char *tx = (volatile char *)0x40002000;

void print_char(char c)
{
    *tx = c;
}

static int pow10[] = {
    1000000000, 100000000, 10000000, 1000000, 100000, 10000, 1000, 100, 10, 1
};

int div_32(int a, int b)
{
    int q = 0;
    for (; b-a >= 0; q++, b-=a);
    for (; q-10 > 0; q-=10); // get rem of 10
    return q;
}

void print_int32(int num)
{
    int flag = 0; // avoid printing leading zeros
    for (int i=0; i<10; ++i){
	char c = '0' + div_32(pow10[i], num);
	for (; num-pow10[i] >= 0; num-=pow10[i]); // remove printed number
	if(c != '0') flag = 1;
	if(flag) print_char(c);
    }
    if(!flag) print_char('0'); // print '0' if num == 0
}
```

## Disassemble the ELF files 

### `-O0`

```asm=
00010220 <popcnt>:
   10220:	fd010113          	addi	sp,sp,-48
   10224:	02812623          	sw	s0,44(sp)
   10228:	03010413          	addi	s0,sp,48
   1022c:	fca42e23          	sw	a0,-36(s0)
   10230:	fe042623          	sw	zero,-20(s0)
   10234:	0240006f          	j	10258 <popcnt+0x38>
   10238:	fdc42783          	lw	a5,-36(s0)
   1023c:	fff78793          	addi	a5,a5,-1
   10240:	fdc42703          	lw	a4,-36(s0)
   10244:	00f777b3          	and	a5,a4,a5
   10248:	fcf42e23          	sw	a5,-36(s0)
   1024c:	fec42783          	lw	a5,-20(s0)
   10250:	00178793          	addi	a5,a5,1
   10254:	fef42623          	sw	a5,-20(s0)
   10258:	fdc42783          	lw	a5,-36(s0)
   1025c:	fc079ee3          	bnez	a5,10238 <popcnt+0x18>
   10260:	fec42783          	lw	a5,-20(s0)
   10264:	00078513          	mv	a0,a5
   10268:	02c12403          	lw	s0,44(sp)
   1026c:	03010113          	addi	sp,sp,48
   10270:	00008067          	ret

00010274 <count_bits>:
   10274:	fd010113          	addi	sp,sp,-48
   10278:	02112623          	sw	ra,44(sp)     # res[0]
   1027c:	02812423          	sw	s0,40(sp)     # res[1]
   10280:	02912223          	sw	s1,36(sp)     # res[2]
   10284:	03010413          	addi	s0,sp,48
   10288:	fca42e23          	sw	a0,-36(s0)    # res[3]
   1028c:	fcb42c23          	sw	a1,-40(s0)    # res[4]
   10290:	fcc42a23          	sw	a2,-44(s0)    # res[5]
   10294:	fe042623          	sw	zero,-20(s0)
   10298:	0300006f          	j	102c8 <count_bits+0x54>
   # for loop
   1029c:	fec42783          	lw	a5,-20(s0)    # i = 0
   102a0:	00279793          	slli	a5,a5,0x2
   102a4:	fd442703          	lw	a4,-44(s0)
   102a8:	00f704b3          	add	s1,a4,a5
   102ac:	fec42503          	lw	a0,-20(s0)
   102b0:	f71ff0ef          	jal	ra,10220 <popcnt>
   102b4:	00050793          	mv	a5,a0
   102b8:	00f4a023          	sw	a5,0(s1)
   102bc:	fec42783          	lw	a5,-20(s0)
   102c0:	00178793          	addi	a5,a5,1   # i++
   102c4:	fef42623          	sw	a5,-20(s0)
   102c8:	fec42703          	lw	a4,-20(s0)
   102cc:	fdc42783          	lw	a5,-36(s0)
   102d0:	fce7d6e3          	bge	a5,a4,1029c <count_bits+0x28>
   
   102d4:	fdc42783          	lw	a5,-36(s0)
   102d8:	00178713          	addi	a4,a5,1
   102dc:	fd842783          	lw	a5,-40(s0)
   102e0:	00e7a023          	sw	a4,0(a5)
   102e4:	00000013          	nop
   102e8:	02c12083          	lw	ra,44(sp)
   102ec:	02812403          	lw	s0,40(sp)
   102f0:	02412483          	lw	s1,36(sp)
   102f4:	03010113          	addi	sp,sp,48
   102f8:	00008067          	ret

000102fc <_start>:
   102fc:	fd010113          	addi	sp,sp,-48
   10300:	02112623          	sw	ra,44(sp)    # res[0]
   10304:	02812423          	sw	s0,40(sp)    # res[1]
   10308:	02912223          	sw	s1,36(sp)    # res[2]
   1030c:	03010413          	addi	s0,sp,48
   10310:	00010313          	mv	t1,sp
   10314:	00030493          	mv	s1,t1
   10318:	00500313          	li	t1,5
   1031c:	fe642423          	sw	t1,-24(s0)
   10320:	fe842303          	lw	t1,-24(s0)
   10324:	00130313          	addi	t1,t1,1
   10328:	fff30e13          	addi	t3,t1,-1
   1032c:	ffc42223          	sw	t3,-28(s0)
   10330:	00030e13          	mv	t3,t1
   10334:	000e0813          	mv	a6,t3
   10338:	00000893          	li	a7,0
   1033c:	01b85e13          	srli	t3,a6,0x1b
   10340:	00589693          	slli	a3,a7,0x5
   10344:	00de66b3          	or	a3,t3,a3
   10348:	00581613          	slli	a2,a6,0x5
   1034c:	00030693          	mv	a3,t1
   10350:	00068513          	mv	a0,a3
   10354:	00000593          	li	a1,0
   10358:	01b55693          	srli	a3,a0,0x1b
   1035c:	00559793          	slli	a5,a1,0x5
   10360:	00f6e7b3          	or	a5,a3,a5
   10364:	00551713          	slli	a4,a0,0x5
   10368:	00030793          	mv	a5,t1
   1036c:	00279793          	slli	a5,a5,0x2
   10370:	00f78793          	addi	a5,a5,15
   10374:	0047d793          	srli	a5,a5,0x4
   10378:	00479793          	slli	a5,a5,0x4
   1037c:	40f10133          	sub	sp,sp,a5
   10380:	00010793          	mv	a5,sp
   10384:	00378793          	addi	a5,a5,3
   10388:	0027d793          	srli	a5,a5,0x2
   1038c:	00279793          	slli	a5,a5,0x2
   10390:	fef42023          	sw	a5,-32(s0)
   10394:	fdc40793          	addi	a5,s0,-36
   10398:	fe042603          	lw	a2,-32(s0)
   1039c:	00078593          	mv	a1,a5
   103a0:	fe842503          	lw	a0,-24(s0)
   103a4:	ed1ff0ef          	jal	ra,10274 <count_bits>
   103a8:	05b00513          	li	a0,91            # '['
   103ac:	cc9ff0ef          	jal	ra,10074 <print_char>
   103b0:	fe042783          	lw	a5,-32(s0)
   103b4:	0007a783          	lw	a5,0(a5)
   103b8:	00078513          	mv	a0,a5
   103bc:	d61ff0ef          	jal	ra,1011c <print_int32>
   # for loop
   103c0:	00100793          	li	a5,1
   103c4:	fef42623          	sw	a5,-20(s0)
   103c8:	0340006f          	j	103fc <_start+0x100>
   103cc:	02c00513          	li	a0,44            # ','
   103d0:	ca5ff0ef          	jal	ra,10074 <print_char>
   103d4:	fe042703          	lw	a4,-32(s0)
   103d8:	fec42783          	lw	a5,-20(s0)
   103dc:	00279793          	slli	a5,a5,0x2
   103e0:	00f707b3          	add	a5,a4,a5
   103e4:	0007a783          	lw	a5,0(a5)
   103e8:	00078513          	mv	a0,a5
   103ec:	d31ff0ef          	jal	ra,1011c <print_int32>
   103f0:	fec42783          	lw	a5,-20(s0)
   103f4:	00178793          	addi	a5,a5,1
   103f8:	fef42623          	sw	a5,-20(s0)
   103fc:	fdc42783          	lw	a5,-36(s0)
   10400:	fec42703          	lw	a4,-20(s0)
   10404:	fcf744e3          	blt	a4,a5,103cc <_start+0xd0>
   
   10408:	05d00513          	li	a0,93            # ']'
   1040c:	c69ff0ef          	jal	ra,10074 <print_char>
   10410:	00a00513          	li	a0,10            # '\n'
   10414:	c61ff0ef          	jal	ra,10074 <print_char>
   10418:	00048113          	mv	sp,s1
   1041c:	00000013          	nop
   10420:	fd040113          	addi	sp,s0,-48
   10424:	02c12083          	lw	ra,44(sp)
   10428:	02812403          	lw	s0,40(sp)
   1042c:	02412483          	lw	s1,36(sp)
   10430:	03010113          	addi	sp,sp,48
   10434:	00008067          	ret
```

### `-Os`

```asm=
00010144 <popcnt>:
   10144:	00050793          	mv	a5,a0
   10148:	00000513          	li	a0,0           # cnt = 0
   # while loop
   1014c:	00079463          	bnez	a5,10154   # a != 0
   
   10150:	00008067          	ret
   
   10154:	fff78713          	addi	a4,a5,-1   # a4 = a - 1
   10158:	00e7f7b3          	and	a5,a5,a4       # a & a4
   1015c:	00150513          	addi	a0,a0,1    # cnt++
   10160:	fedff06f          	j	1014c <popcnt+0x8>

00010164 <count_bits>:
   10164:	fe010113          	addi	sp,sp,-32
   10168:	00812c23          	sw	s0,24(sp)
   1016c:	00912a23          	sw	s1,20(sp)
   10170:	01212823          	sw	s2,16(sp)
   10174:	01312623          	sw	s3,12(sp)
   10178:	00112e23          	sw	ra,28(sp)
   1017c:	00050413          	mv	s0,a0
   10180:	00058913          	mv	s2,a1
   10184:	00060993          	mv	s3,a2
   # for loop
   10188:	00000493          	li	s1,0            # i = 0
   1018c:	02945463          	bge	s0,s1,101b4     # n >= i
   
   10190:	00140413          	addi	s0,s0,1     # rsize = n + 1
   10194:	00892023          	sw	s0,0(s2)
   10198:	01c12083          	lw	ra,28(sp)
   1019c:	01812403          	lw	s0,24(sp)
   101a0:	01412483          	lw	s1,20(sp)
   101a4:	01012903          	lw	s2,16(sp)
   101a8:	00c12983          	lw	s3,12(sp)
   101ac:	02010113          	addi	sp,sp,32
   101b0:	00008067          	ret
   
   101b4:	00048513          	mv	a0,s1
   101b8:	f8dff0ef          	jal	ra,10144 <popcnt>
   101bc:	00249793          	slli	a5,s1,0x2
   101c0:	00f987b3          	add	a5,s3,a5
   101c4:	00a7a023          	sw	a0,0(a5)        # res[i] = cnt
   101c8:	00148493          	addi	s1,s1,1     # i++
   101cc:	fc1ff06f          	j	1018c <count_bits+0x28>

000101d0 <_start>:
   101d0:	fc010113          	addi	sp,sp,-64
   101d4:	00810613          	addi	a2,sp,8
   101d8:	00410593          	addi	a1,sp,4
   101dc:	00500513          	li	a0,5            # a0 = 5
   101e0:	02112e23          	sw	ra,60(sp)       # ra = &res
   101e4:	02812c23          	sw	s0,56(sp)       # s0 = res[1]
   101e8:	02912a23          	sw	s1,52(sp)       # s1 = res[2]
   101ec:	03212823          	sw	s2,48(sp)       # s2 = res[3]
   101f0:	03312623          	sw	s3,44(sp)       # s3 = res[4]
   101f4:	f71ff0ef          	jal	ra,10164 <count_bits>
   101f8:	400027b7          	lui	a5,0x40002
   101fc:	05b00713          	li	a4,91           # '['
   10200:	00e78023          	sb	a4,0(a5)        # print_char('[')
   10204:	00812503          	lw	a0,8(sp)
   10208:	00810413          	addi	s0,sp,8
   1020c:	00100493          	li	s1,1            # i = 1
   10210:	e7dff0ef          	jal	ra,1008c <print_int32>
   10214:	40002937          	lui	s2,0x40002
   10218:	02c00993          	li	s3,44           # ','
   1021c:	00412783          	lw	a5,4(sp)
   10220:	00440413          	addi	s0,s0,4
   10224:	02f4ca63          	blt	s1,a5,10258     # i < rsize
   
   10228:	400027b7          	lui	a5,0x40002
   1022c:	05d00713          	li	a4,93           # ']'
   10230:	00e78023          	sb	a4,0(a5)        # print_char(']')
   10234:	00a00713          	li	a4,10           # '\n'
   10238:	00e78023          	sb	a4,0(a5)        # print_char('\n')
   1023c:	03c12083          	lw	ra,60(sp)
   10240:	03812403          	lw	s0,56(sp)
   10244:	03412483          	lw	s1,52(sp)
   10248:	03012903          	lw	s2,48(sp)
   1024c:	02c12983          	lw	s3,44(sp)
   10250:	04010113          	addi	sp,sp,64
   10254:	00008067          	ret
   
   10258:	01390023          	sb	s3,0(s2)        # print_char(',')
   1025c:	00042503          	lw	a0,0(s0)
   10260:	00148493          	addi	s1,s1,1     # i++
   10264:	e29ff0ef          	jal	ra,1008c <print_int32>
   10268:	fb5ff06f          	j	1021c <_start+0x4c>
```

### Observation

1. The structure of for loops are different. `-Os` one place the loop body in the back of the section, while `-O0` implement for loop very straight forward.
2. `-Os` expand my `print_char` function to shrink the size.
3. `-O0` has a lot of computation to store variable into memory.

## Compare optimization options

### size

```shell
riscv-none-embed-size test1
```

| optimization | text | data | bss | dec | hex |
| - | - | - | - | - | - |
| `-O0` | 964 | 44 | 0 | 1008 | 3f0 |
| `-O1` | 632 | 0 | 0 | 632 | 278 |
| `-O2` | 612 | 0 | 0 | 612 | 264 | 
| `-O3` | 820 | 0 | 0 | 820 | 334 | 
| `-Os` | 576 | 0 | 0 | 576 | 240 |

## time

| optimization | Execution time | Instruction count | Jumps | Branching | 
| - | - | - | - | - |
| `-O0` | - | - | - | - | 
| `-O1` | 1069215 ns | 1567 (IPS=1465561) |302 (19.27%) - 147 forwards, 155 backwards0 | T=148 (42.77%) F=198 (57.23%) | 
| `-O2` | 831973 ns | 644 (IPS=774063) | 123 (19.10%) - 35 forwards, 88 backwards | T=104 (45.61%) F=124 (54.39%) | 
| `-O3` | 540399 ns | 600 (IPS=1110290) | 203 (33.83%) - 72 forwards, 131 backwards | T=196 (85.96%) F=32 (14.04%) | 
| `-Os` | 789238 ns | 1681 (IPS=2129902) | 323 (19.21%) - 163 forwards, 160 backwards | T=151 (37.66%) F=250 (62.34%) | 

my virtural machine only have 1G RAM, the `-O0` one crash because lack of memory.

![](https://i.imgur.com/HTHNi0j.png)

