
/**
 * @file    v2x_interface.c
 * @author  albert(albert@it-telecom.co.kr)
 * @date    2024-01-08
 * @brief   V2X 인터페이스
 * @version 1.0
 */

#include "v2x_interface.h"
#include "lte_api.h"

/**
 * @brief LED GPIO 초기화
 *
 * @return int  1 : success, -1 : fail
 *
 * @author albert
 * @date 2024-08-06
 */
int led_init(void)
{
    FILE *fp = NULL;

    /* device open */
    fp = fopen("/sys/class/gpio/export", "w");

    /* result check */
    if (fp == NULL) {
        printf("Failed open /sys/class/gpio/export\n");
        return -1;
    }

    /* pin */
    fprintf(fp, "%d\n", 49);

    fclose(fp);

    /* device open */
    fp = fopen("/sys/class/gpio/gpio49/direction", "w");

    /* result check */
    if (fp == NULL) {
        printf("Failed open /sys/class/gpio/gpio49/direction\n");
        return -1;
    }

    /* direction */
    fprintf(fp, "out\n");

    fclose(fp);

    return 1;
}

/**
 * @brief LED GPIO 제어 종료
 *
 * @return int  1 : success, -1 : fail
 *
 * @author albert
 * @date 2024-08-06
 */
int led_deinit(void)
{
    FILE *fp = NULL;

    /* device open */
    fp = fopen("/sys/class/gpio/unexport", "w");

    /* result check */
    if (fp == NULL) {
        printf("Failed open /sys/class/gpio/unexport\n");
        return -1;
    }

    /* pin */
    fprintf(fp, "%d\n", 49);

    fclose(fp);

    return 1;
}

/**
 * @brief LED ON
 *
 * @author albert
 * @date 2024-08-06
 */
void led_on(void)
{
    FILE *fp = NULL;

    /* device open */
    fp = fopen("/sys/class/gpio/gpio49/value", "w");

    /* result check */
    if (fp == NULL) {
        printf("Failed open /sys/class/gpio49/value\n");
        return;
    }

    /* pin out */
    fprintf(fp, "%d\n", 0);

    fclose(fp);
}

/**
 * @brief LED OFF
 *
 * @author albert
 * @date 2024-08-06
 */
void led_off(void)
{
    FILE *fp = NULL;

    /* device open */
    fp = fopen("/sys/class/gpio/gpio49/value", "w");

    /* result check */
    if (fp == NULL) {
        printf("Failed open /sys/class/gpio49/value\n");
        return;
    }

    /* pin out */
    fprintf(fp, "%d\n", 1);

    fclose(fp);
}

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
    o2a_header_t *pheader = (o2a_header_t *) rx_buffer;
    unsigned char dest_mac[6] = { 0xff };
    lte_dot3PsidRange psid = 0;
    int length = 0;
    apiResult result = ar_success;
    int retval = 0;

    /* uds server open */
    if (access(UDS_FILE_ACU_TX, F_OK) == 0) {
        unlink(UDS_FILE_ACU_TX);
    }

    uds_sock = socket(AF_UNIX, SOCK_DGRAM, 0);

    /* uds socket create error */
    if (uds_sock < 0) {
        printf("uds server socket fail\n");
        sleep(1);
        goto tx_thread_exit;
    }

    memset(&msg_addr_un, 0x00, sizeof(struct sockaddr_un));
    msg_addr_un.sun_family = AF_UNIX;
    strcpy(msg_addr_un.sun_path, UDS_FILE_ACU_TX);

    if (bind(uds_sock, (struct sockaddr *) &msg_addr_un, sizeof(struct sockaddr_un)) < 0) {
        printf("uds server bind fail\n");
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
            /* get header info */
            psid = htonl(pheader->psid);
            length = htonl(pheader->length);

            if ((psid == PSID_CRE_RECOGNINTION) || (psid == PSID_CRE_SITUATION) ||
                (psid == PSID_CRE_SITUATION)) {

                result = lteUnsignedWsmTransmit(psid, lte_power_tx_default, lte_userPriority_max, lte_dataRate_default,
                        &rx_buffer[O2A_HEADER_SIZE], length, dest_mac);

                if (result == ar_success) {
                    printf("send success\n");
                } else {
                    printf("send fail(%d)\n", result);
                }
            } else {
                printf("psid error (%d:0x%x)\n", psid, psid);
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
    o2a_header_t *pheader = (o2a_header_t *) buffer;

    printf("v2x_rx_thread start\n");

    while (1) {
        /* 비서명 WSM 수신 - blocking 동작 */
        retval = lteUnsignedWsmReceive(&psid, &buffer[O2A_HEADER_SIZE],
                sizeof(buffer) - O2A_HEADER_SIZE);


        if ((psid == PSID_CRE_RECOGNINTION) || (psid == PSID_CRE_SITUATION) ||
            (psid == PSID_CRE_SITUATION)) {

            if (retval > 0) {
                printf("rx : %d packet(psid %d)\n", retval, psid);
                pheader->header = O2A_HEADER;
                pheader->psid = htonl(psid);
                pheader->length = htonl(retval);
                v2x_send_to_acu(buffer, retval);
            }
        }

        /* 10msec sleep */
        usleep(10000);
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
    bool led_index = false;

    printf("v2x_thread start\n");

    /* led init */
    led_init();

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
        if (lteCheckStatus() >= ar_success) {
            (led_index == true) ? led_on() : led_off();
            led_index = !led_index;
        } else {
            led_off();
        }

        sleep(1);
    }

    /* v2x deinit */
    lteDeinit();

    /* led deinit */
    led_deinit();

    printf("v2x_thread stop\n");

    pthread_exit(NULL);
} /* v2x_thread */

/* end of v2x_inteface.c */
