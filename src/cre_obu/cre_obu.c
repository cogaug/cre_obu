
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

/**
 * @brief       TCP로 수신한 데이터 처리
 * @param[in]   sock : TCP 클리아언트 소켓
 * @retval      음수 : 데이터 없음, 0 : 연결끊김, 양수 : 수신데이터의 크기
 * @author      albert
 * @date        2024-01-09
 */
int recv_client_packet(int sock)
{
    unsigned char buffer[MAX_BUFFER_SIZE];
    o2a_header_t *pheader = (o2a_header_t *) buffer;
    int retval = 0;
    int length = 0;
    int left = 0;
    int retry = 0;

    /* 헤더 크기 읽음 */
    retval = recv(sock, buffer, O2A_HEADER_SIZE, 0);

    if (retval < 0) { /* no data */
        return -1;
    } else if (retval == 0) { /* disconnect */
        return 0;
    } else if (retval == O2A_HEADER_SIZE) {
        if (pheader->header != O2A_HEADER) {
            printf("header not match %08X\n", pheader->header);
            return -1;
        }

        length = htonl(pheader->length);

        if (length >= 1400) {
            printf("invalid length %d\n", length);
            return -1;
        }

        left = length;

        /* 데이터(messageframe) 부분 읽어오기 */
        while (left > 0) {
            retval = recv(sock, &buffer[O2A_HEADER_SIZE + (length - left)], left, 0);

            if (retval > 0) {
                left -= retval;
            } else if (retval == 0) { /* 연결 끊어짐 */
                return 0;
            } else { /* 못읽는 경우 */
                if (retry++ >= 10000) {
                    // printf("over\n");
                    return -1;
                }
            }
        }

        if (left == 0) {
            acu_send_to_v2x(buffer, O2A_HEADER_SIZE + length);
        } else {
            return -1;
        }

        return (O2A_HEADER_SIZE + length);
    }

    return 0;
}/* recv_client_packet */
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
        /* ACU로부터 송신요청 수신시 처 */
        retval = recv_client_packet(g_client_sock);

        if (retval == 0) { /* 연결 끊김 */
            client_run = 0;
            break;
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
