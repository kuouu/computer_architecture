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
