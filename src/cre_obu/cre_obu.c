
#include <cre_obu.h>
#include "version.h"

int main()
{
    /* threads pid */
    pthread_t pid[2];
    int retval = 0;

    printf("hello world\n");

    /* create v2x_thread */
    retval = pthread_create(&pid[0], NULL, v2x_thread, NULL);

    if (retval != 0) {
        printf("v2x_thread create fail\n");
        goto app_exit;
    }

    while (1) {
        sleep (1);
    }

app_exit:
    return 0;
}
