#include <stdio.h>

int main(int argc, char **argv)
{
  // printf("Hello World!\n");
  // printf("a icin deger girin\n");

  int a;
  FILE *input_file = fopen("/home/usr1/riscv-isa-sim/"
  "a_tets_faruk/spike_link/log/dummy.txt", "r");
  fscanf(input_file,"%d", &a);

  int b;
  scanf("%d", &b);

  a *= b;
  fclose(input_file);

  FILE *fp;
  fp = fopen("/home/usr1/riscv-isa-sim/"
  "a_tets_faruk/spike_link/log/dummy.txt", "w");
  fprintf(fp, "%d", a);
  fclose(fp);
}
