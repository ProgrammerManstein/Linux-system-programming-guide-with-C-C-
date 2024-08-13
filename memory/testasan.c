#include <string.h>
#include <stdlib.h>
int main(int argc, char** argv) {
     char *pszVal = malloc(16);
     strcpy(pszVal, "eeeeeeeeeeeee");
     //pszVal[16]='5';
	free(pszVal);
	return 0;
}
