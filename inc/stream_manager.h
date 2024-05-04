#ifndef STREAM_MANAGER_H
#define STREAM_MANAGER_H

#include <stdarg.h>

typedef int write_func(const char *, ...);

write_func *write = printf;

void switchToPrintf();
void switchToLabel();

int print_label(const char *, ...);

#endif