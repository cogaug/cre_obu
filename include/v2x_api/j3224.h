/**
 * @file j3224.h
 * @author albert
 * @brief J3224 Interface Header
 * @version 0.1
 * @date 2025-07-28
 *
 * @copyright Copyright it-telecom (c) 2025
 *
 */

#ifndef _J3224_H_
#define _J3224_H_

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push, 1)       // 1바이트 크기로 정렬

#define HDR_FLAG            0x53415445
#define RECOGNITION_MSG     0x16792
#define SITUATION_MSG       0x16793
#define NEGOTIATION_MSG     0x16794

typedef struct {
    unsigned int hdr_flag;
    unsigned int msgID;
    unsigned int msgLen;
} v2x_intf_hdr_type;

typedef struct {
    unsigned short year;    // DE_DYear (J2735), years (0..4095)
    unsigned char month;    // DE_DMonth (J2735), months (0..12)
    unsigned char day;      // DE_DDay (J2735), days (0..31)
    unsigned char hour;     // DE_DHour (J2735), hours (0..31)
    unsigned char minute;   // DE_DMinute (J2735), minutes (0..60)
    unsigned short msec;    // DE_DSecond (J2735), milliseconds (0..65535)
    short offset;           // DE_DOffset (J2735), minutes from UTC time (-840..840)
} j3224_ddatetime_t;        // DF_DDateTime (J2735)

typedef struct {
    long latitude;          // DE_Latitude (J2735) (-900000000..900000001) -90.0 ~ 90.0 in 1/10 micro degree
    long longitude;         // DE_Longitude (J2735) INTEGER (-1799999999..1800000001) -180.0 ~ 180.0 in 1/10 micro degree
} j3224_position3d_t;       // DF_Position3D (J2735)

typedef struct {
    unsigned char semiMajor; // DE_SemiMajorAxisAccuracy (J2735), 255 - unavailable
    unsigned char semiMinor; // DE_SemiMinorAxisAccuracy (J2735), 255 - unavailable
    unsigned short orientation; // DE_SemiMajorAxisOrientation (J2735), 65535 - unavailable
} j3224_position_accuracy_t; // DF_PositionalAccuracy (J2735)

typedef struct {
    short offsetX;          // DE_ObjectDistance(J3224) -32767 ~ 32767 in 0.1m
    short offsetY;          // DE_ObjectDistance(J3224) -32767 ~ 32767 in 0.1m
} j3224_position_offset_xyz_t; // DF_PositionOffsetXYZ (J3224)

typedef struct {
  unsigned char pos;
  unsigned char elevation;
} j3224_position_confidence_set_t;


typedef struct {
    unsigned char objType;      // DE_ObjectType (J3224) unknown(0),vehicle(1), vru(2), animal(3)
    unsigned char objTypeCfd;   // DE_ClassificationConfidence (J3224) (0..100), 0 - unknown, 101 - unavailable
    unsigned short objectID;    // DE_ObjectID (J3224) temporary ID assinged by source ()
    short measurementTime;      // DE_MeasurementTimeOffset (J3224) (-1500..1500) in 1ms
    unsigned char timeConfidence; // DE_TimeConfidence (J3224) PositionOffsetXYZ pos; // DF_PositionOffsetXYZ (J3224)
    j3224_position_confidence_set_t posConfidence; // DE_PositionConfidence
    unsigned short speed;       // DE_Speed (J2735) (0..8191) in 0.02m/s
    unsigned char speedConfidence; // DE_SpeedConfidence (J2735) (0..7) 0 - unavailable
    unsigned short heading;     // DE_Heading (J2735) (0..28800) in 0.0125 degree
    unsigned char headingConf;  // DE_HeadingConfidence (J2735) (0..7) 0 - unavailable
} j2735_detected_object_common_data_t; // DF_DetectedObjectCommonData (J3224)

#define NUM_OF_OBJECTS  255

typedef struct {
    unsigned char msgCnt;       // DE_MessageCount(J3224) 0 ~ 255, incremented by 1 for each message
    unsigned char sourceID[4];  // DE_SourceID(J3224) 4 bytes, unique ID of the source
    unsigned char equipmentType;        // DE_EquipmentType(J3224) unknown (0), rsu (1), obu (2), vru (3)
    j3224_ddatetime_t sDSMTimeStamp;    // DF_DDateTime(J2735) SDSM transmission time
    j3224_position3d_t refPos;          // DF_Position3D(J2735) sender reference position
    j3224_position_accuracy_t refPosXYConf; // DF_PositionalAccuracy (J2735)
    unsigned char numDetectedObjects;   // number of detected objects, objects 필드의 개수
    j2735_detected_object_common_data_t objects[NUM_OF_OBJECTS]; // sequnece of DF_DetectedObjectCommonData (J3224)
} recognition_data_type;

typedef struct {
    v2x_intf_hdr_type hdr;
    recognition_data_type data;
} v2x_recognition_msg_type;

typedef union {
    v2x_intf_hdr_type hdr;
    v2x_recognition_msg_type recog;
} v2x_intf_msg_type;

#pragma pack(pop)       // 정렬 설정을 이전 상태(기본값)로 되돌림


extern int j3224_encode(recognition_data_type *pdata, unsigned char *pbuffer, int buffer_size, unsigned char trace);

#ifdef __cplusplus
}
#endif

#endif /* _J3224_H_ */