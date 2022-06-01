//#include <cstdio>
//#include <stdlib.h>
//
//int main(int argc, char** argv)
//{
//    printf("here\n");
//    char * buffer = static_cast<char *>(malloc(1024));
//    sprintf(buffer, "%d", argc);
//    printf("%s",buffer);
//    return 0;
//}
//

#include <stdlib.h>

void *p;

int main() {
    p = malloc(7);
    p = 0; // The memory is leaked here.
    return 0;
}