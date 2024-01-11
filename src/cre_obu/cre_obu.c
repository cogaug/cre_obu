
#include <cre_obu.h>
#include "version.h"

/**
 * @brief       send data to v2x using uds
 * @param[in]   buffer : 전달할 데이터
 * @param[in]   len : 전달할 데이터의 크기
 * @retval      none
 * @author      albert
 * @date        2024-01-09
 */
void acu_send_to_v2x(unsigned char *buffer, int len)
{
    int sock = -1;
    struct sockaddr_un addr_un;

    sock = socket(AF_UNIX, SOCK_DGRAM, 0);

    /* socket create error */
    if (sock < 0) {
        return;
    }

    memset(&addr_un, 0x00, sizeof(struct sockaddr_un));
    addr_un.sun_family = AF_UNIX;
    strcpy(addr_un.sun_path, UDS_FILE_ACU_TX);

    sendto(sock, buffer, len, 0, (struct sockaddr *)&addr_un, sizeof(struct sockaddr_un));

    close(sock);
} /* acu_send_to_v2x */
            acu_send_to_v2x(buffer, O2A_HEADER_SIZE + length);
    int uds_sock = -1;
    struct sockaddr_un msg_addr_un;
    struct sockaddr_un msg_recv_un;
    int namelen = sizeof(struct sockaddr_un);
    int retval = 0;

    /* uds server open */
    uds_sock = socket(AF_UNIX, SOCK_DGRAM, 0);

    /* uds socket create error */
    if (uds_sock < 0) {
        sleep(1);
        goto client_tcp_exit;
    }

    memset(&msg_addr_un, 0x00, sizeof(struct sockaddr_un));
    msg_addr_un.sun_family = AF_UNIX;
    strcpy(msg_addr_un.sun_path, UDS_FILE_ACU_RX);

    if (uds_sock < 0) {
        sleep(1);
        goto client_tcp_exit;
    }

    if (bind(uds_sock, (struct sockaddr *) &msg_addr_un, sizeof(struct sockaddr_un)) < 0) {
        sleep(1);
        goto client_uds_exit;
    }


    /* set uds socket nonblocking */
    if ((fl = fcntl(uds_sock, F_GETFL, NULL)) == -1) {
        sleep(1);
        goto client_uds_exit;
    }

    fl |= O_NONBLOCK;

    if (fcntl(uds_sock, F_SETFL, fl) == -1) {
        sleep(1);
        goto client_uds_exit;
    }
        /* recv from v2x */
        retval = recvfrom(uds_sock, rx_buffer, sizeof(rx_buffer), 0, (struct sockaddr *)&msg_recv_un, (socklen_t*)&namelen);

    /* uds close */
    if (uds_sock > 0) {
        close(uds_sock);
    }

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
