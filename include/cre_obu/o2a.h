/**
 * @file    o2a.h
 * @author  ablert(albert@it-telecom.co.kr)
 * @date    2024-01-09
 * @brief   CITS to DPU 프로토콜 데이터 형식
 * @version 1.0
 */

#ifndef __O2A_H__
#define __O2A_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"

#define O2A_HEADER                  0x53415445
#define MAX_BUFFER_SIZE             2048
#define SERVER_PORT                 9201

#pragma pack(push, 1)                       /* 1바이트 크기로 정렬 */

/* O2A 프토토콜 헤더 */

typedef struct {
    unsigned int header;
    unsigned int msg_id;
    unsigned int length;
} o2a_header_t;

#define O2A_HEADER_SIZE sizeof(o2a_header_t)

#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif /* __O2A_H__ */

