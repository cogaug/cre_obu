/**
 * @file    common.h
 * @author  albert(albert@it-telecom.co.kr)
 * @date    2025-12-22
 * @brief   common header
 * @version 1.1
 */

#ifndef __COMMON_H__
#define __COMMON_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <net/if.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

/* uds files */
#define UDS_FILE_ACU_TX         "/tmp/acu_tx.uds"
#define UDS_FILE_ACU_RX         "/tmp/acu_rx.uds"

/* PSIDs */
#define PSID_CRE_RECOGNINTION   92050 /* (0x16792) */
#define PSID_CRE_SITUATION      92051 /* (0x16793) */
#define PSID_CRE_NEGOTIATION    92052 /* (0x16794) */

#ifdef __cplusplus
}
#endif

#endif /* __COMMON_H__ */
