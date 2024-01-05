
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
 * @brief       SPDU를 생성하여 반환한다.
 * @param[in]   psid : 송신 PSID
 * @param[in]   dataunit :  payload
 * @param[in]   dataunit_len : payload 길이
 *
 * @retval      apiResult
 *
 * @author      albert
 * @date        2024-01-03
 */
apiResult v2x_send_spdu(lte_dot3PsidRange psid, unsigned char *dataunit, int dataunit_len)
{
    apiResult result = ar_success;
    unsigned char dest_mac[6] = { 0xff };

    result = lteUnsignedWsmTransmit(psid, lte_power_tx_default, lte_userPriority_max, lte_dataRate_default,
            dataunit, dataunit_len, dest_mac);

    if (result == ar_success) {
        printf("send success\n");
        return 0;
    } else {
        printf("send fail(%d)\n", result);
        return -1;
    }
} /* v2x_send_spdu */

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
    int retval = 0;
    lte_dot3PsidRange psid = 0;
    printf("v2x_tx_thread start\n");

    while (1) {
        sleep(1);
    }

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
    unsigned char test[16];
    pthread_t pid[2];
    int result = 0;
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
        v2x_send_spdu(0x20, test, 16);
        sleep(1);
    }

    /* v2x deinit */
    lteDeinit();

    printf("v2x_thread stop\n");

    pthread_exit(NULL);
} /* v2x_thread */

/* end of v2x_inteface.c */
