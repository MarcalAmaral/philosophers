#include <stdio.h>
#include <sys/time.h>

int main(void)
{
    struct timeval tv;
    double   time;

    if (!gettimeofday(&tv, NULL))
        time = (tv.tv_sec * 1e6) + tv.tv_usec;
    printf("current time %f\n", time);
    return (0);
}