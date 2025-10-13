/*
 * p5.h - Single header C library providing p5.js-like functionality using sokol
 * Version: 0.0.1
 *
 */

#ifndef P5_H
#define P5_H

#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// TODO macro for unimplemented functions
#define TODO(msg) do { \
    printf("[WARNING] TODO: %s (function: %s, file: %s, line: %d)\n", \
           msg, __func__, __FILE__, __LINE__); \
} while(0)

#endif

