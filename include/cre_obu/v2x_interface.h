
/**
 * @file    v2x_interface.h
 * @author  albert(albert@it-telecom.co.kr)
 * @date    2024-01-02
 * @brief   V2X 인터페이스
 * @version 1.0
 */

#ifndef __V2X_INTERFACE_H__
#define __V2X_INTERFACE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"
#include "lte_api.h"

/* extern functions */
extern void *v2x_thread(void *param);

#ifdef __cplusplus
}
#endif

#endif /* __V2X_INTERFACE_H__ */
