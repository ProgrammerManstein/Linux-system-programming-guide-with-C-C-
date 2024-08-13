#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <stdlib.h>

void memory_leak() {
    int *leak = (int *)malloc(sizeof(int) * 10);
    // 忘记释放内存，导致内存泄漏
    printf("memory leak!!\n");
}

void use_uninitialized_memory() {
  int a, b, c;
  c = a + b;
  printf("Use uninitialized memory!!\n");
  printf("c = %d \n", c);
}

void subscript_out() {
    char *array = malloc(13);
    array[13] = 13;
    free(array);
}

void overflow() {
  int array[1000000];
  overflow();
}

void suspend(){
  int *p=new int;
  delete p;
  std::cout<<*p;
}

int main(int argc, char** argv) {
  int flag = atoi(argv[1]);
  switch (flag)
  {
    case 1:
      {
        memory_leak();
        break;
      }
    case 2:
      {
        use_uninitialized_memory();
        break;
      }
    case 3:
      {
        subscript_out();
        break;
      }
    case 4:
      {
        overflow();
        break;
      }
    case 5:
      {
	suspend();
	break;
      }
  }
  return 0;
}
