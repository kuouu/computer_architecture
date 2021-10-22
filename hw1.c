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