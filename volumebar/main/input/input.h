#ifndef input_H
#define input_H

#include <stdio.h>
#include <fcntl.h>
#include <linux/input.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>

bool input_read(void);

#endif
