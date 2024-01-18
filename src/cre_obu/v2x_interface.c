
/**
 * @file    v2x_interface.c
 * @author  albert(albert@it-telecom.co.kr)
 * @date    2023-01-02
 * @brief   V2X 인터페이스
 * @version 1.0
 */

#include "v2x_interface.h"
#include "lte_api.h"

/**
 * @brief       send data to acu inteface using uds
 * @param[in]   buffer : 전달할 데이터
 * @param[in]   len : 전달할 데이터의 크기
 * @retval      none
 * @author      albert
 * @date        2024-01-09
 */
void v2x_send_to_acu(unsigned char *buffer, int len)
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
    strcpy(addr_un.sun_path, UDS_FILE_ACU_RX);

    sendto(sock, buffer, len, 0, (struct sockaddr *)&addr_un, sizeof(struct sockaddr_un));

    close(sock);
} /* v2x_send_to_acu */

/**
 * @brief       v2x 송신 쓰레드
 *
 * @param[in]   param : 파라미터 포인터(미사용)
 * @retval      void * : 미사용
 *
 * @author      albert
 * @date        2024-01-05
 */
void * v2x_tx_thread(void *param)
{
    int uds_sock = -1;
    struct sockaddr_un msg_addr_un;
    struct sockaddr_un msg_recv_un;
    int namelen = sizeof(struct sockaddr_un);
    int fl = 0;
    unsigned char rx_buffer[2302];
    unsigned char dest_mac[6] = { 0xff };
    lte_dot3PsidRange psid = 0;
    apiResult result = ar_success;
    int retval = 0;

    /* uds server open */
    uds_sock = socket(AF_UNIX, SOCK_DGRAM, 0);

    /* uds socket create error */
    if (uds_sock < 0) {
        sleep(1);
        goto tx_thread_exit;
    }

    memset(&msg_addr_un, 0x00, sizeof(struct sockaddr_un));
    msg_addr_un.sun_family = AF_UNIX;
    strcpy(msg_addr_un.sun_path, UDS_FILE_ACU_TX);

    if (uds_sock < 0) {
        sleep(1);
        goto tx_thread_exit;
    }

    if (bind(uds_sock, (struct sockaddr *) &msg_addr_un, sizeof(struct sockaddr_un)) < 0) {
        sleep(1);
        goto uds_server_exit;
    }

    /* set uds socket nonblocking */
    if ((fl = fcntl(uds_sock, F_GETFL, NULL)) == -1) {
        sleep(1);
        goto uds_server_exit;
    }

    fl |= O_NONBLOCK;

    if (fcntl(uds_sock, F_SETFL, fl) == -1) {
        sleep(1);
        goto uds_server_exit;
    }
    printf("v2x_tx_thread start\n");

    while (1) {
        /* recv from acu interface */
        retval = recvfrom(uds_sock, rx_buffer, sizeof(rx_buffer), 0, (struct sockaddr *)&msg_recv_un, (socklen_t*)&namelen);

        /* There is something to send to v2x */
        if (retval > 0) {
            result = lteUnsignedWsmTransmit(psid, lte_power_tx_default, lte_userPriority_max, lte_dataRate_default,
                    rx_buffer, retval, dest_mac);

            if (result == ar_success) {
                printf("send success\n");
            } else {
                printf("send fail(%d)\n", result);
            }
        }

        /* 1msec sleep */
        usleep(1000);
    }

uds_server_exit:
    if (uds_sock > 0) {
        close(uds_sock);
    }

tx_thread_exit:
    printf("v2x_tx_thread stop\n");

    pthread_exit(NULL);
} /* v2x_tx_thread */

/**
 * @brief       v2x 수신 쓰레드
 *
 * @param[in]   param : 파라미터 포인터(미사용)
 * @retval      void * : 미사용
 *
 * @author      albert
 * @date        2024-01-05
 */
void * v2x_rx_thread(void *param)
{
    int retval = 0;
    lte_dot3PsidRange psid = 0;
    unsigned char buffer[2302];

    printf("v2x_rx_thread start\n");

    while (1) {
        /* 비서명 WSM 수신 - blocking 동작 */
        retval = lteUnsignedWsmReceive(&psid, buffer, sizeof(buffer));

        if (retval > 0) {
            printf("rx : %d packet(psid %d)\n", retval, psid);
            v2x_send_to_acu(buffer, retval);
        }

        sleep(1);
    }

    printf("v2x_rx_thread stop\n");

    pthread_exit(NULL);
} /* v2x_rx_thread */

/**
 * @brief       v2x 인터페이스 쓰레드
 *
 * @param[in]   param : 파라미터 포인터(미사용)
 * @retval      void * : 미사용
 *
 * @author      albert
 * @date        2024-01-05
 */
void* v2x_thread(void *param)
{
    pthread_t pid[2];
    apiResult ar_result = 0;

    printf("v2x_thread start\n");

    /* v2x init */
    ar_result = lteInit(178, debug_all);

    if (ar_result != ar_success) {
        printf("lteInit fail %d\n", ar_result);
    }

    /* wsm service add - all psid */
    ar_result = lteWsmServiceRegisterAdd(lte_dot3Psid_all);

    if (ar_result != ar_success) {
        printf("lteWsmServiceRegisterAdd fail %d\n", ar_result);
    }

    /* create tx thread */
    pthread_create(&pid[0], NULL, v2x_tx_thread, NULL);

    /* create rx thread */
    pthread_create(&pid[1], NULL, v2x_rx_thread, NULL);

    while (1) {
        sleep(1);
    }

    /* v2x deinit */
    lteDeinit();

    printf("v2x_thread stop\n");

    pthread_exit(NULL);
} /* v2x_thread */

/* end of v2x_inteface.c */
