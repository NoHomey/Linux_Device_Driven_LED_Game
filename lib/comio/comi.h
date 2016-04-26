#ifndef _COMI_H
#define _COMI_H

#include <stdint.h>

int comi_open(void);
void comi_flush(const int fd);
int comi_do(const int fd, uint8_t* byte);

#endif
