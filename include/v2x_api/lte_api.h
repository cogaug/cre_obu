/*
 * Copyright (c) 2023 It-telecom, Inc.
 *
 * This software is furnished under a license and may be used and copied
 * only in accordance with the terms of such license and with the
 * inclusion of the above copyright notice. This software or any other
 * copies thereof may not be provided or otherwise made available to any
 * other person. No title to and ownership of the software is hereby
 * transferred.
 *
 * The information in this software is subject to change without notice
 * and should not be construed as a commitment by It-telecom, Inc.
 *
 * PROPRIETARY NOTICE
 *
 * This software is an unpublished work subject to a confidentiality agreement
 * and is protected by copyright and trade secret law.  Unauthorized copying,
 * redistribution or other use of this work is prohibited.
 *
 * The above notice of copyright on this source code product does not indicate
 * any actual or intended publication of such source code.
 *
 *****************************************************************************/
/**
 * @file lte_api.h
 * This header file contains deprecated definitions of lte_api runtime
 * constants, data structures, and other defintions.
 */
#ifndef _LTE_API_H_
#define _LTE_API_H_

#include <system.h>

#define EXTERN     extern

#ifdef __cplusplus
extern "C" {
#endif

///< @brief Debug Message output level - higher level includes lower levels
typedef enum
{
    debug_nothing                                       = (  0), ///< 아무 메시지도 출력하지 않는다
    debug_err                                           = (  1), ///< 에러 메시지 출력
    debug_init                                          = (  2), ///< 초기화 및 종료 절차에 관련된 메시지 출력
    debug_config                                        = (  3), ///< 설정에 관련된 메시지 출력
    debug_mgmt                                          = (  4), ///< 관리에 관련된 메시지 출력
    debug_event                                         = (  5), ///< 이벤트발생에 관련된 메시지 출력
    debug_pktTrx                                        = (  6), ///< 패킷 송수신 및 각종 이벤트/통지에 관련된 메시지 출력
    debug_pktDump                                       = (  7), ///< 패킷 덤프 메시지 출력
    debug_intr                                          = (  8), ///< 인터럽트 메시지 출력
    debug_all                                           = (  9), ///< 모든 메시지 출력
    debug_max
} apiDbgLevel;


///< @brief api result code
typedef enum
{
    /* success */
    ar_success                                          = (  0), ///< 성공

    ar_success_queue_almost_full                        = (  1), ///< 송신큐가 거의 다 찼음.
    ar_success_queue_almost_empty                       = (  2), ///< 송신큐가 거의 다 비었음.

    ar_success_code_end                                 = (  3), ///< 성공값 경계 구분자 - 성공값은 이 코드 이전에 정의되어야 한다.

    /* fails */
    ar_fail_code_start                                  = (  4), ///< 실패값 경계 구분자 - 실패값은 이 코드 이후에 정의되어야 한다.

    ar_fail_no_mem                                      = (  5), ///< 메모리 할당 실패
    ar_fail_system_call                                 = (  6), ///< 시스템 콜 실패 - 정확한 원인은 시스템 콜 반환값이나 errno를 확인해야 한다.
    ar_fail_null_params                                 = (  7), ///< 전달된 파라미터가 Null
    ar_fail_invalid_index                               = (  8), ///< 유효하지 않은 식별번호
    ar_fail_kernel_register                             = (  9), ///< 커널 등록 실패
    ar_fail_invalid_num                                 = ( 10), ///< 유효하지 않은 개수
    ar_fail_invalid_ioctl                               = ( 11), ///< 유효하지 않은 ioctl() 명령
    ar_fail_too_long                                    = ( 12), ///< 너무 긴 데이터
    ar_fail_invalid_multi_phy_mode                      = ( 13), ///< 유효하지 않은 multi-PHY 모드
    ar_fail_queue_full                                  = ( 14), ///< Queue full
    ar_fail_queue_empty                                 = ( 15), ///< Queue empty
    ar_fail_expired                                     = ( 16), ///< 만기됨(유효기간이 지남)
    ar_fail_not_opened                                  = ( 17), ///< 아직 열리지 않았음(초기화되지 않았음).
    ar_fail_invalid_string                              = ( 18), ///< 유효하지 않은 문자열
    ar_fail_insufficient_size                           = ( 19), ///< 데이터 크기 부족
    ar_fail_invalid_value                               = ( 20), ///< 유효하지 않은 값
    ar_fail_invalid_type                                = ( 21), ///< 유효하지 않은 유형
    ar_fail_invalid_id                                  = ( 22), ///< 유효하지 않은 식별자
    ar_fail_not_found                                   = ( 23), ///< 찾을 수 없음
    ar_fail_no_entry                                    = ( 24), ///< 엔트리가 없음
    ar_fail_table_full                                  = ( 25), ///< 테이블이 꽉 찼음
    ar_fail_table_empty                                 = ( 26), ///< 테이블이 비어 있음
    ar_fail_hw_access                                   = ( 27), ///< 하드웨어 접근 실패
    ar_fail_not_support                                 = ( 28), ///< 지원하지 않음
    ar_fail_invalid_channel                             = ( 29), ///< 유효하지 않은 채널
    ar_fail_invalid_countrystring                       = ( 30), ///< 유효하지 않은 Country string
    ar_fail_invalid_datarate                            = ( 31), ///< 유효하지 않은 datarate
    ar_fail_invalid_power                               = ( 32), ///< 유효하지 않은 전송출력(레벨)
    ar_fail_invalid_aci                                 = ( 33), ///< 유효하지 않은 ACI 값
    ar_fail_invalid_timeslot                            = ( 34), ///< 유효하지 않은 시간슬롯
    ar_fail_invalid_edca_params                         = ( 35), ///< 유효하지 않은 EDCA 파라미터
    ar_fail_invalid_immediate_access                    = ( 36), ///< 유효하지 않은 immediateAccess 값
    ar_fail_already_assigned                            = ( 37), ///< 이미 접속 중임
    ar_fail_invalid_service_class                       = ( 38), ///< 유효하지 않은 서비스 클래스
    ar_fail_invalid_priority                            = ( 39), ///< 유효하지 않은 사용자우선순위
    ar_fail_invalid_address                             = ( 40), ///< 유효하지 않은 주소
    ar_fail_not_running                                 = ( 41), ///< 동작 중이 아님
    ar_fail_running                                     = ( 42), ///< 동작 중임
    ar_fail_too_short                                   = ( 43), ///< 너무 짧은 데이터
    ar_fail_invalid_netapi_id                           = ( 44), ///< 유효하지 않은 Net API 식별자
    ar_fail_invalid_netapi_data                         = ( 45), ///< 유효하지 않은 Net API 데이터
    ar_fail_encode                                      = ( 46), ///< 인코딩 실패
    ar_fail_decode                                      = ( 47), ///< 디코딩 실패
    ar_fail_invalid_psid                                = ( 48), ///< 유효하지 않은 PSID
    ar_fail_invalid_action                              = ( 49), ///< 유효하지 않은 Action
    ar_fail_unknown_target                              = ( 50), ///< 목적지를 알 수 없음
    ar_fail_invalid_pkt                                 = ( 51), ///< 유효하지 않은 패킷
    ar_fail_invalid_psc                                 = ( 52), ///< 유효하지 않은 PSC
    ar_fail_resource_not_available                      = ( 53), ///< 자원이 유효하지 않음
    ar_fail_invalid_params                              = ( 54), ///< 유효하지 않은 파라미터
    ar_fail_vendor_specific                             = ( 55), ///< 각 벤더 플랫폼에 관련된 에러

    ar_fail_invalid_key_algorithm                       = ( 56), ///< 유효하지 않은 키 알고리즘
    ar_fail_invalid_cmh_state                           = ( 57), ///< 유효하지 않은 CMH 상태
    ar_fail_duplicated                                  = ( 58), ///< 중복된 정보
    ar_fail_invalid_length                              = ( 59), ///< 유효하지 않은 길이값
    ar_fail_verification                                = ( 60), ///< 검증 실패
    ar_fail_invalid_noti                                = ( 61), ///< 유효하지 않은 notification
    ar_fail_invalid_certchain_len                       = ( 62), ///< 유효하지 않은 인증서체인 길이
    ar_fail_unknown_cert                                = ( 63), ///< 알려지지 않은(=저장되어 있지 않은) 인증서
    ar_fail_inconsistent_psid                           = ( 64), ///< Inconsistent PSID
    ar_fail_invalid_certform                            = ( 65), ///< 유효하지 않은 인증서 형식
    ar_fail_no_data                                     = ( 66), ///< 데이터가 존재하지 않음
    ar_fail_asn1                                        = ( 67), ///< ASN.1 관련 에러
    ar_fail_not_trust                                   = ( 68), ///< 신뢰할 수 없음.
    ar_fail_cannot_be_here                              = ( 69), ///< 발생할 수 없는 에러

    ar_fail_invalid_facapi_id                           = ( 70), ///< 유효하지 않은 Fac API 식별자

    ar_fail_unspecified                                 = ( 71), ///< 정의되지 않은 에러
    ar_fail_codeend                                     = ( 72), ///< 실패값 경계 구분자 - 실패값은 이 코드 이전에 정의되어야 한다.

    ar_max
} apiResult;

typedef enum {
    /* success */
    sr_success                                          = (  0), ///< No error

    /* fails */
    sr_fail_sys                                         = (  1), ///< System call failed
    sr_fail_config_syntax                               = (  2), ///< Syntax error in configuration file
    sr_fail_db_bdb                                      = (  3), ///< Internal Berkeley DB failure
    sr_fail_db_deadlock                                 = (  4), ///< Deadlock on transactional operation; try again?
    sr_fail_db_not_found                                = (  5), ///< Database not found
    sr_fail_nullparam                                   = (  6), ///< NULL parameter
    sr_fail_data                                        = (  7), ///< Badly formed input WSA
    sr_fail_buflen                                      = (  8), ///< Buffer supplied for signing is too short
    sr_fail_bad_sig                                     = (  9), ///< WSA signature didn't verify
    sr_fail_nomem                                       = ( 10), ///< Out of memory
    sr_fail_app_authentication                          = ( 11), ///< Authenticating application
    sr_fail_bad_args                                    = ( 12), ///< Bad arguments to method
    sr_fail_ca_app_id_scope                             = ( 13), ///< Certificate authority application identity scope
    sr_fail_unable_to_decrypt_message                   = ( 14), ///< Ws err unable to decrypt message
    sr_fail_message_replay                              = ( 15), ///< Duplicate message -- possible replay attack
    sr_fail_certificate_expired                         = ( 16), ///< Expired certificate
    sr_fail_certificate_outside_bounds                  = ( 17), ///< Ws err certificate outside bounds
    sr_fail_invalid_certificate_subject_type            = ( 18), ///< Ws err invalid certificate subject type
    sr_fail_no_certificate                              = ( 19), ///< Missing end-entity certificate or no valid signing certificate
    sr_fail_no_ca_certificate                           = ( 20), ///< Missing CA certificate in chain
    sr_fail_invalid_certificate                         = ( 21), ///< Certificate contains a disallowed set of field values
    sr_fail_message_gen_before_cert_validity_period     = ( 22), ///< Message generated before certificate is valid
    sr_fail_message_gen_after_cert_validity_period      = ( 23), ///< Message generated after certificate expiration
    sr_fail_message_exp_before_cert_validity_period     = ( 24), ///< Message expiration is before certificate is valid
    sr_fail_message_exp_after_cert_validity_period      = ( 25), ///< Message expiration is after certificate expiration
    sr_fail_message_outside_bounds                      = ( 26), ///< Ws err message outside bounds
    sr_fail_no_key_available                            = ( 27), ///< Could not find a key to perform the requested operation
    sr_fail_no_matching_key                             = ( 28), ///< No key in WSC to match key in imported cert
    sr_fail_recipient_no_key                            = ( 29), ///< No certificate or key corresponding to recipient identifier
    sr_fail_certificate_revoked                         = ( 30), ///< Ws err certificate revoked
    sr_fail_unknown_ca_certificate                      = ( 31), ///< Unknown certificate authority certificate
    sr_fail_parse_fail                                  = ( 32), ///< Ws err parse fail
    sr_fail_wave_security_context                       = ( 33), ///< Ws err wave security context
    sr_fail_unsupported_crypto_algorithm                = ( 34), ///< Ws err unsupported crypto algorithm
    sr_fail_wrong_key_type                              = ( 35), ///< Key does not support an operation of the specified type
    sr_fail_crypto_internal                             = ( 36), ///< Undocumented internal error in crypto libraries
    sr_fail_bad_data_from_database                      = ( 37), ///< Bad data read from database
    sr_fail_hpsam                                       = ( 38), ///< HPSAM error. To use software crypto, set crypto_provider = software_security_builder in /etc/viicsec.conf. To use the HPSAM emulator (for NTRU internal development purposes only), set the environment variable VIICSEC_HARDWARE_TYPE to FLAG_SIMULATED_HW.
    sr_fail_message_does_not_use_generation_time        = ( 39), ///< Message does not use generation time
    sr_fail_message_does_not_use_generation_location    = ( 40), ///< Message does not use generation location
    sr_fail_message_does_not_use_expiry                 = ( 41), ///< Message does not use expiry
    sr_fail_parse_ignore                                = ( 42), ///< Ws err parse ignore
    sr_fail_no_position_information_available           = ( 43), ///< Positioning information unavailable
    sr_fail_incomplete_state                            = ( 44), ///< Object has not been given all the information needed to carry out this action
    sr_fail_read_config                                 = ( 45), ///< Error reading global configuration file
    sr_fail_config_val                                  = ( 46), ///< Unexpected value in global configuration file
    sr_fail_encode                                      = ( 47), ///< Encoding error
    sr_fail_symmetric_authentication_check_failed       = ( 48), ///< Symmetric authentication check failed
    sr_fail_recipient_revoked                           = ( 49), ///< The recipient's certificate has been revoked
    sr_fail_recipient_expired                           = ( 50), ///< The recipient's certificate has expired
    sr_fail_recipient_duplicate                         = ( 51), ///< Recipient identifier already exists
    sr_fail_no_generation_time                          = ( 52), ///< Expected but did not find generation time field in signed message
    sr_fail_no_generation_location                      = ( 53), ///< Expected but did not find generation location field in signed message
    sr_fail_no_expiry_time                              = ( 54), ///< Expected but did not find expiry time field in signed message
    sr_fail_invalid_permissions_request                 = ( 55), ///< Certificate does not have permissions of the requested type
    sr_fail_no_external_ca_key                          = ( 56), ///< No external CA key available for requested PSID
    sr_fail_lcm_feature_unsupported                     = ( 57), ///< The Local Certificate Manager does not support this feature
    sr_fail_lcmd_unavailable                            = ( 58), ///< Unable to connect to the CAMP LCM daemon
    sr_fail_internal                                    = ( 59), ///< Internal error
    sr_fail_already_open                                = ( 60), ///< The specified Security Context is already open
    sr_fail_invalid_psid                                = ( 61), ///< The PSID in the message does not match the PSID in the context
    sr_fail_invalid_permissions                         = ( 62), ///< The provided permissions are not a valid format
    sr_fail_not_initialized                             = ( 63), ///< The security services has not been initialized
    sr_fail_context_not_initialized                     = ( 64), ///< The security context has not been initialized
    sr_fail_buffer_too_small                            = ( 65), ///< The supplied buffer is too small
    sr_fail_invalid_context                             = ( 66), ///< The object is not associated with a valid Security Context
    sr_fail_invalid_handle                              = ( 67), ///< The provided handle is not valid
    sr_fail_psid_not_in_context                         = ( 68), ///< The provided Security Context does not contain the specified PSID
    sr_fail_not_implemented                             = ( 69), ///< The mesage type is not supported
    sr_fail_invalid_global_config                       = ( 70), ///< An error with the global configuration file occured
    sr_fail_invalid_profile_config                      = ( 71), ///< An error with a profile configuration file occured
    sr_fail_invalid_context_config                      = ( 72), ///< An error with a context configuration file occured
    sr_fail_message_expired                             = ( 73), ///< Message has expired
    sr_fail_message_in_future                           = ( 74), ///< Message is in the future
    sr_fail_data_not_extracted                          = ( 75), ///< Parser has not been given all the information needed to carry out this action
    sr_fail_not_supported_for_this_content_type         = ( 76), ///< Messages of this content type do not support this operation
    sr_fail_external_data_missing                       = ( 77), ///< Message requires external data be provided for this operation
    sr_fail_external_data_not_needed                    = ( 78), ///< Message does not allow external data to be provided for this operation
    sr_fail_message_signed_outside_region               = ( 79), ///< Cert not authorized to sign where it did
    sr_fail_unauthorized_permissions                    = ( 80), ///< Psid/ITS-AID not found in signing certificate
    sr_fail_unauthorized_certificate_type               = ( 81), ///< Correct holder type not found in signing certificate
    sr_fail_value_already_set                           = ( 82), ///< Ws err value already set
    sr_fail_psid_not_in_certificate                     = ( 83), ///< Specified psid is not in the certificate permissions list
    sr_fail_not_supported_for_this_message_security_type = ( 84), ///< Function does not support this message's security type
    sr_fail_key_already_tagged                          = ( 85), ///< Encryption Key has already been tagged
    sr_fail_key_not_tagged                              = ( 86), ///< Encryption Key has not been tagged
    sr_fail_duplicate_encryption_id                     = ( 87), ///< The encryption id is already used
    sr_fail_invalid_recipient                           = ( 88), ///< Recipient is not known
    sr_fail_invalid_signer_id_type                      = ( 89), ///< The SignerIdentifier is not valid
    sr_fail_disallowed_message_security_type            = ( 90), ///< The message security parameters are not allowed
    sr_fail_invalid_private_key_encoding                = ( 91), ///< The private key encoding is not supported
    sr_fail_crypto_queue_full                           = ( 92), ///< All threads in the pool are in use
    sr_fail_certificates_inconsistent                   = ( 93), ///< Data in the certificate chain is inconsistent
    sr_fail_consistency_not_checked                     = ( 94), ///< Consistency checks for the message have not been run
    sr_fail_memory_failure                              = ( 95), ///< Memory cannot be allocated
    sr_fail_message_expiry_before_generation            = ( 96), ///< Message expiry time is before the message generation time
    sr_fail_message_from_too_far_away                   = ( 97), ///< Message was generated too far from the current location
    sr_fail_message_too_old                             = ( 98), ///< Message was signed too far in the past
    sr_fail_message_signature_verification_failed       = ( 99), ///< Message signature fails to verify
    sr_fail_certificate_signature_verification_failed   = (100), ///< Certificate signature fails to verify
    sr_fail_disallowed_in_profile                       = (101), ///< Input parameters do not meet the requirements of the security profile
    sr_fail_extra_header                                = (102), ///< Message violates the security profile associated with the PSID/(ITS-AID) by including a header that is not allowed
    sr_fail_invalid_field                               = (103), ///< Field has an invalid value
    sr_fail_missing_header                              = (104), ///< Message violates the security profile associated with the PSID/(ITS-AID) because it lacks a header field that is required
    sr_fail_unable_to_decrypt_data                      = (105), ///< Failed to decrypt the data with the symmetric key
    sr_fail_unable_to_decrypt_symmetric_key             = (106), ///< Failed to decrypt symmetric key
    sr_fail_wsa_certs_not_in_context                    = (107), ///< WSA certificates are not configured in the security context associated with the secured message
    sr_fail_invalid_wsa                                 = (108), ///< Wsa is invalid
    sr_fail_unauthorized_region                         = (109), ///< Certificate is not allowed to sign a message at the current location
    sr_fail_already_registered                          = (110), ///< The  context/lcm combination is already registered
    sr_fail_id_change_abort                             = (111), ///< Id change aborted
    sr_fail_id_change_blocked                           = (112), ///< Id changes have been blocked
    sr_fail_id_change_not_locked                        = (113), ///< No locks to release
    sr_fail_not_registered                              = (114), ///< The context/lcm combination is not registered
    sr_fail_unknown_lcm                                 = (115), ///< The specified LCM name is not known
    sr_fail_inconsistent_signing_key_type               = (116), ///< The ACF's signing key type is not valid.
    sr_fail_hsm_cla_not_supported                       = (117), ///< HSM: The CLA value is not supported by the applet
    sr_fail_hsm_ins_not_supported                       = (118), ///< HSM: The INS value is not supported by the applet
    sr_fail_hsm_incorrect_p1p2                          = (119), ///< HSM: The value of parameter P1 or P2 is invalid
    sr_fail_hsm_wrong_length                            = (120), ///< HSM: The value of parameter Lc or Le is invalid
    sr_fail_hsm_wrong_data                              = (121), ///< HSM: The data field of the command contains wrong data
    sr_fail_hsm_file_full                               = (122), ///< HSM: No more memory available
    sr_fail_hsm_execution_error_nvram_unchanged         = (123), ///< HSM: Internal execution error and the result is that the NVRAM is unchanged
    sr_fail_hsm_execution_error_nvram_changed           = (124), ///< HSM: Internal execution error and the result is that the NVRAM is changed
    sr_fail_hsm_no_precise_diagnosis                    = (125), ///< HSM: An exception occured of which no precise diagnosis is available.
    sr_fail_hsm_conditions_not_satisfied                = (126), ///< HSM: Conditions not satisfied
    sr_fail_hsm_security_status_not_satisfied           = (127), ///< HSM: Security status not satisfied
    sr_fail_hsm_authentication_method_blocked           = (128), ///< HSM: Authentication method blocked
    sr_fail_hsm_undefined_error                         = (129), ///< HSM: Error code not relevant.
    sr_fail_hsm_api_error                               = (130), ///< HSM: Internal API Wrapper error
    sr_fail_hsm_memory_failure                          = (131), ///< HSM: EEPROM Memory Failure
    sr_fail_invalid_crypto_api_error                    = (132), ///< Invalid aerolink crypto api return code
    sr_fail_parse_fail_extra_data                       = (133), ///< The parse failed due to extra data present
    sr_fail_data_not_signed                             = (134), ///< The processed data was not a Signed Message as expected
    sr_fail_no_profiles                                 = (135), ///< No configured profiles
    sr_fail_duplicate_profiles                          = (136), ///< Multiple profiles found with the same psid.
    sr_fail_no_assurance_level                          = (137), ///< Expected but did not find assurance level.
    sr_fail_pki_generate_request                        = (138), ///< PKI: Failed to generate service request
    sr_fail_pki_service_request                         = (139), ///< PKI: Failed to request service
    sr_fail_pki_service_response                        = (140), ///< PKI: Failed to process response
    sr_fail_pki_invalid_lcm_type                        = (141), ///< PKI: Invalid LCM type
    sr_fail_pki_invalid_url                             = (142), ///< PKI: Invalid URL
    sr_fail_pki_invalid_config                          = (143), ///< PKI: Invalid PKI config detected
    sr_fail_pki_encode_base64                           = (144), ///< PKI: Failed to encode base64
    sr_fail_pki_bad_data                                = (145), ///< PKI: Corrupted server response
    sr_fail_save_file                                   = (146), ///< Failed to save file
    sr_fail_unsupported_custom_service                  = (147), ///< Unsupported Aerolink service.
    sr_fail_unsupported_custom_setting                  = (148), ///< Unsupported Aerolink custom setting.
    sr_fail_invalid_custom_setting_format               = (149), ///< Invalid Aerolink custom setting format.
    sr_fail_service_disabled                            = (150), ///< The specified service is disabled.
    sr_fail_invalid_encryption_key                      = (151), ///< Encryption key is invalid
    sr_fail_encryption_key_not_found                    = (152), ///< Encryption key not found
    sr_fail_cmac_verification_failed                    = (153), ///< CMAC verification failed
    sr_fail_invalid_ca_certificate_store                = (154), ///< Invalid CA certificate store format
    sr_fail_ca_certificate_store_verification_failed    = (155), ///< Failed to verify CA certificate store
    sr_fail_payload_not_spdu                            = (156), ///< The provided SPDU payload is not valid.
    sr_fail_no_profile_for_permisson                    = (157), ///< There is not persmission for requested permission.
    sr_fail_invalid_crl_type                            = (158), ///< The crl type is not supported by this crl processor.
    sr_fail_read_file                                   = (159), ///< Failed to read file
    sr_fail_invalid_spdu_hash                           = (160), ///< The hash algorithm in the signed SPDU does not match the hash algorithm used by the signer
    sr_fail_pki_service_response_partial_data           = (161), ///< PKI: server response contains partial data
    sr_fail_invalid_acf_header                          = (162), ///< The provided data cannot be encoded into an acf header.
    sr_fail_crl_too_overdue                             = (163), ///< A valid CRL for the certificate is not available
    sr_fail_crl_signer_inconsistent                     = (164), ///< The signer of a CRL is inconsistent with the CraCa ID in the CRL contents
    sr_fail_crl_times_inconsistent                      = (165), ///< The issue date of a CRL is not strictly before the nextCrl date.
    sr_fail_no_report_available                         = (166), ///< No misbehavior reports are available.
    sr_fail_security_library_not_initialized            = (167), ///< The security library is not initialized.
    sr_fail_security_library_misbehavior_configuration  = (168), ///< The security library is not configured for misbehavior.
    sr_fail_certificate_linkage_i_not_current           = (169), ///< Linkage values for the time period indicated by the i value in a certificate's linkage revocation data have not been calculated, so a revocation check cannot be made.
    sr_fail_no_etsi_registration                        = (170), ///< Cannot find valid ETSI registration information.
    sr_fail_lcm_dir_does_not_exist                      = (171), ///< The LCM directory provided to the AT Record does not exist.
    sr_fail_atrecord_not_initialized                    = (172), ///< The AT Record has not been initialized.
    sr_fail_create_directory                            = (173), ///< Failed to create the directory.
    sr_fail_delete_file                                 = (174), ///< Failed to delete the file.
    sr_fail_request_hash_not_being_serviced             = (175), ///< The request hash associated with the certificate is not being serviced.
    sr_fail_canonical_key_already_exists                = (176), ///< The canonical key has already been generated and can only be generated once.
    sr_fail_pki_lcm_not_initialized                     = (177), ///< The LCM has not be initialized for SCMS service.
    sr_fail_invalid_enrollment_config                   = (178), ///< The enrollment.conf file has one or more invalid entries.
    sr_fail_invalid_enrollment_data                     = (179), ///< The enrollment certificate and/or its signing key is invalid.
} securityResult;

/* 파워 값(= dBm단위 전송출력/수신파워) 범위 */
typedef enum
{
    lte_power_min                                       = (-128),
    lte_power_tx_default                                = (  20), ///< C class에서 전송할 수 있는 최대 파워를 기본값으로 한다.
    lte_power_max                                       = ( 127),
    lte_power_unknown                                   = ( 255)
} lte_powerRange;

/* PSID 값 범위 */
typedef enum
{
    lte_dot3Psid_1byte_min                              = (       0),
    lte_dot3Psid_1byte_max                              = (     127),
    lte_dot3Psid_2bytes_min                             = (     128),
    lte_dot3Psid_wsa                                    = (     135),
    lte_dot3Psid_2bytes_max                             = (    16511),
    lte_dot3Psid_3bytes_min                             = (    16512),
    lte_dot3Psid_3bytes_max                             = (  2113663),
    lte_dot3Psid_4bytes_min                             = (  2113664),
    lte_dot3Psid_4bytes_max                             = (270549119),
    lte_dot3Psid_min                                    = (lte_dot3Psid_1byte_min),
    lte_dot3Psid_max                                    = (lte_dot3Psid_4bytes_max),
    lte_dot3Psid_ip                                     = (0xEFFFFFFE),
    lte_dot3Psid_all                                    = (0xFFFFFFFF)
} lte_dot3PsidRange;

/* MAC레벨 사용자 우선순위 범위 */
typedef enum
{
    lte_userPriority_min                                = (0),
    lte_userPriority_max                                = (7),
    lte_userPriority_unknown                            = (8)
} lte_userPriorityRange;

/* DataRate 값 범위(500kbps 단위) */
typedef enum
{
    lte_dataRate_unknown                                = (-999),
    lte_dataRate_min                                    = (   2),
    lte_dataRate_3Mbps                                  = (   6),
    lte_dataRate_4p5Mbps                                = (   9),
    lte_dataRate_6Mbps                                  = (  12),
    lte_dataRate_9Mbps                                  = (  18),
    lte_dataRate_12Mbps                                 = (  24),
    lte_dataRate_18Mbps                                 = (  36),
    lte_dataRate_24Mbps                                 = (  48),
    lte_dataRate_27Mbps                                 = (  54),
    lte_dataRate_max                                    = ( 127)
#define lte_dataRate_default                            lte_dataRate_6Mbps
#define lte_dataRate_wsa_default                        lte_dataRate_default
} lte_dataRateRange;

/* 서명식별자 유형 */
typedef enum
{
	lte_dot2SignerIdentifier_cert                       = (0),
	lte_dot2SignerIdentifier_digest                     = (1),
	lte_dot2SignerIdentifier_self                       = (2),
	lte_dot2SignerIdentifier_secprofile                 = (3),
	lte_dot2SignerIdentifierType_count                  = (4),
	lte_dot2SignerIdentifierType_min                    = (lte_dot2SignerIdentifier_cert),
	lte_dot2SignerIdentifierType_max                    = (lte_dot2SignerIdentifierType_count - 1)
} lte_dot2SignerIdentifierType;

/**
 * @brief api 버전 정보 반환
 * lte API의 문자열 버전 정보를 반환한다.
 *
 * @retval api 버전정보 문자열
 */
EXTERN const char *get_api_version();

/**
 * @brief lte API 초기화
 * lte API를 초기화하는 함수이다. lteInit을 수행후 lte관련 API를 사용가능하다.
 *
 * @param channel (IN)  사용할 채널 설정
 * @param level (IN)  출력 설정할 디버그 레벨
 *
 * @retval apiResult 형식의 결과 코드
 */
EXTERN apiResult lteInit(int channel, apiDbgLevel level);

/**
 * @brief lte API 초기화 해제
 * lte API를 초기화 해제하는 함수이다. 호출 이후에는 lte관련 API를 사용할 수 없다.
 *
 * @retval none
 */
EXTERN void lteDeinit();

/**
 * @brief WSM 서비스를 등록한다
 * 수신한 WSM 중 API로 취득할 PSID를 등록한다. dot3Psid_all을 등록할 경우 모든 PSID를 취득한다.
 *
 * @param psid (IN)  WSM 서비스 등록할 psid
 *
 * @retval apiResult 형식의 결과 코드
 */
EXTERN apiResult lteWsmServiceRegisterAdd(lte_dot3PsidRange psid);

/**
 * @brief WSM 서비스를 등록한다
 * 수신한 WSM 중 API로 취득할 PSID를 등록한다. dot3Psid_all을 등록할 경우 모든 PSID를 취득한다.
 *
 * @param psid (IN)  WSM 서비스 등록할 psid
 *
 * @retval apiResult 형식의 결과 코드
 */
EXTERN apiResult lteWsmServiceRegisterDelete(lte_dot3PsidRange psid);

/**
 * @brief 서명 관리자 등록
 *
 * @param wsc_name (IN)  wsc 이름
 * @param lcm_name (IN)  lcm 이름
 * @param leap_seconds (IN)  윤초(초)
 * @param latitude  (IN)  현재 위치의 위도(J2735 형식)
 * @param longitude (IN)  현재 위치의 경도(J2735 형식)
 * @param elevation (IN)  현재 위치의 고도(J2735 형식)
 * @param contry_code (IN)  현재 위치의 국가코드(ISO 3166-1)
 * @param psid (IN) 수신된 WSM의 PSID

 * @return securityResult
 */
EXTERN securityResult lteSecurityContextAdd(
    char *wsc_name,
    char *lcm_name,
    int leap_seconds,
    int latitude,
    int longitude,
    short elevation,
    short contry_code,
    lte_dot3PsidRange psid);

/**
 * @brief 서명 관리자 등록해제
 *
 * @param psid (IN)  수신된 WSM의 PSID
 *
 * @return 수신된 데이터(WSM)의 크기
 */
EXTERN securityResult lteSecurityContextDelete(lte_dot3PsidRange psid);

/**
 * @brief Unsigned WSM 송신
 *
 * @param psid (IN)  송신 PSID
 * @param txpower (IN)  송신 출력
 * @param priority (IN)  우선순위
 * @param datarate (IN)  Datarate
 * @param dataunit (IN)  송신할 데이터
 * @param dataunit_len (IN)  송신할 데이터의 길이
 * @param dest_mac (IN)  수신자의 MAC 주소
 *
 * @return apiResult 형식의 결과 코드
 */
EXTERN apiResult lteUnsignedWsmTransmit(
    lte_dot3PsidRange psid,
    lte_powerRange txpower,
    lte_userPriorityRange priority,
    lte_dataRateRange datarate,
    unsigned char* dataunit,
    unsigned int dataunit_len,
    unsigned char* dest_mac);

/**
 * @brief Signed WSM 송신
 * @details 서WSM을 수신하고 수신된 WSM의 payload와 PSID를 반환한다.
 *
 * @param singer_type (IN)  서명 형식
 * @param psid (IN)  송신 PSID
 * @param txpower (IN)  송신 출력
 * @param priority (IN)  우선순위
 * @param datarate (IN)  Datarate
 * @param dataunit (IN)  송신할 데이터
 * @param dataunit_len (IN)  송신할 데이터의 길이
 * @param dest_mac (IN)  수신자의 MAC 주소
 *
 * @return apiResult 형식의 결과 코드
 */
EXTERN apiResult lteSignedWsmTransmit(
    lte_dot2SignerIdentifierType signer_type,
    lte_dot3PsidRange psid,
    lte_powerRange txpower,
    lte_userPriorityRange priority,
    lte_dataRateRange datarate,
    unsigned char* dataunit,
    unsigned int dataunit_len,
    unsigned char* dest_mac);

/**
 * @brief Unsigned WSM 수신
 * @details WSM을 수신하고 수신된 WSM의 payload와 PSID를 반환한다.
 *
 * @param psid (OUT) 수신된 WSM의 PSID
 * @param buffer (OUT)  payload가 저장될 버퍼
 * @param buffer_size (IN)  수신된 데이터 저장할 버퍼 최대크기
 *
 * @return 성공시 수신된 데이터(WSM)의 크기, 오류 발생시 음수 반환
 */
EXTERN int lteUnsignedWsmReceive(
    lte_dot3PsidRange* psid,
    unsigned char* buffer,
    int buffer_size);

/**
 * @brief Signed WSM 수신
 * @details WSM을 수신하고 수신된 WSM의 payload와 PSID, 서명 처리결과를 반환한다.
 * 수신된 WSM의 PSID 인증정보가 등록되어 있지 않으면 서명 처리결과에 오류를 반환한다
 *
 * @param psid (OUT)  수신된 WSM의 PSID
 * @param buffer (OUT)  payload가 저장될 버퍼
 * @param buffer_size (IN)  수신된 데이터 저장할 버퍼 최대크기
 * @param signer_type (OUT)  서명 유형
 * @param result (OUT)  서명처리 결과
 *
 * @return 성공시 수신된 데이터(WSM)의 크기, 오류 발생시 음수 반환
 */
int lteSignedWsmReceive(
    lte_dot3PsidRange* psid,
    unsigned char* buffer,
    int buffer_size,
    lte_dot2SignerIdentifierType *signer_type,
    securityResult *result);

#ifdef __cplusplus
}
#endif

#endif
