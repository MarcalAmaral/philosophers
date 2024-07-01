#include <string.h>
#include <errno.h>
#include <stdio.h>

int main(void)
{
    printf("error message ENOMEM %s\n", strerror(ENOMEM));
    printf("error message EINVAL %s\n", strerror(EINVAL));
    printf("error message EBUSY %s\n", strerror(EBUSY));
    printf("error message EPERM %s\n", strerror(EPERM));
    printf("error message EAGAIN %s\n", strerror(EAGAIN));
    printf("error message ESRCH %s\n", strerror(ESRCH));
    printf("error message EDEADLK %s\n", strerror(EDEADLK));
    return (0);
}
