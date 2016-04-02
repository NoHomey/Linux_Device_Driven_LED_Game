#include <tlc5947.h>
#include <stdio.h>

int main(void) {

    printf("%d\n", tlc5947_write(1024, 8));

    return 0;
}
