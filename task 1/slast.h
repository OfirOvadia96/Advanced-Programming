#include <stdio.h>
#include <stdlib.h>
#include <utmp.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>

bool show_info(struct utmp *);
void show_time(long time_val);