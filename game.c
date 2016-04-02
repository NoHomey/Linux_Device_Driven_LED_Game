#include <tlc5947.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

int main(void) {
    printf("%d\n", tlc5947_write(0, 8));

    return 0;
}
