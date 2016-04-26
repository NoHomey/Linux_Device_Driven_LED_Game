#ifndef _COMO_H
#define _COMO_H

#include <stdint.h>

int como_open(void);
void como_flush(const int fd);
int como_do(const int fd, uint8_t* byte);

#endif
