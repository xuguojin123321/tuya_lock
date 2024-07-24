/**********************************Copyright (c)**********************************
**                       ��Ȩ���� (C), 2015-2020, Ϳѻ�Ƽ�
**
**                             http://www.tuya.com
**
*********************************************************************************/
/**
 * @file    mcu_api.h
 * @author  Ϳѻ�ۺ�Э�鿪����
 * @version v1.0.7
 * @date    2020.11.9
 * @brief   �û���Ҫ�������õĺ������ڸ��ļ���
 */


#ifndef __MCU_API_H_
#define __MCU_API_H_

#include "wifi.h"

#ifdef MCU_API_GLOBAL
  #define MCU_API_EXTERN
#else
  #define MCU_API_EXTERN   extern
#endif

/**
 * @brief  hexתbcd
 * @param[in] {Value_H} ���ֽ�
 * @param[in] {Value_L} ���ֽ�
 * @return ת����ɺ�����
 */
unsigned char hex_to_bcd(unsigned char Value_H,unsigned char Value_L);

/**
 * @brief  ���ַ�������
 * @param[in] {str} �ַ�����ַ
 * @return ���ݳ���
 */
unsigned long my_strlen(unsigned char *str);

/**
 * @brief  ��src��ָ�ڴ������ǰcount���ֽ����ó��ַ�c
 * @param[out] {src} �����õ��ڴ��׵�ַ
 * @param[in] {ch} ���õ��ַ�
 * @param[in] {count} ���õ��ڴ泤��
 * @return �����õ��ڴ��׵�ַ
 */
void *my_memset(void *src,unsigned char ch,unsigned short count);

/**
 * @brief  �ڴ濽��
 * @param[out] {dest} Ŀ���ַ
 * @param[in] {src} Դ��ַ
 * @param[in] {count} �������ݸ���
 * @return ���ݴ�������Դ��ַ
 */
void *my_memcpy(void *dest, const void *src, unsigned short count);

/**
 * @brief  �ַ�������
 * @param[in] {dest} Ŀ���ַ
 * @param[in] {src} Դ��ַ
 * @return ���ݴ�������Դ��ַ
 */
char *my_strcpy(char *dest, const char *src);

/**
 * @brief  �ַ����Ƚ�
 * @param[in] {s1} �ַ��� 1
 * @param[in] {s2} �ַ��� 2
 * @return ��С�Ƚ�ֵ
 * -         0:s1=s2
 * -         <0:s1<s2
 * -         >0:s1>s2
 */
int my_strcmp(char *s1 , char *s2);

/**
 * @brief  ��int���Ͳ���ĸ��ֽ�
 * @param[in] {number} 4�ֽ�ԭ����
 * @param[out] {value} ������ɺ�4�ֽ�����
 * @return Null
 */
void int_to_byte(unsigned long number,unsigned char value[4]);

/**
 * @brief  ��4�ֽںϲ�Ϊ1��32bit����
 * @param[in] {value} 4�ֽ�����
 * @return �ϲ���ɺ��32bit����
 */
unsigned long byte_to_int(const unsigned char value[4]);


/**
 * @brief  raw��dp�����ϴ�
 * @param[in] {dpid} dpid��
 * @param[in] {value} ��ǰdpֵָ��
 * @param[in] {len} ���ݳ���
 * @return Null
 * @note   Null
 */
unsigned char mcu_dp_raw_update(unsigned char dpid,const unsigned char value[],unsigned short len);

/**
 * @brief  bool��dp�����ϴ�
 * @param[in] {dpid} dpid��
 * @param[in] {value} ��ǰdpֵ
 * @return Null
 * @note   Null
 */
unsigned char mcu_dp_bool_update(unsigned char dpid,unsigned char value);

/**
 * @brief  value��dp�����ϴ�
 * @param[in] {dpid} dpid��
 * @param[in] {value} ��ǰdpֵ
 * @return Null
 * @note   Null
 */
unsigned char mcu_dp_value_update(unsigned char dpid,unsigned long value);

/**
 * @brief  string��dp�����ϴ�
 * @param[in] {dpid} dpid��
 * @param[in] {value} ��ǰdpֵָ��
 * @param[in] {len} ���ݳ���
 * @return Null
 * @note   Null
 */
unsigned char mcu_dp_string_update(unsigned char dpid,const unsigned char value[],unsigned short len);

/**
 * @brief  enum��dp�����ϴ�
 * @param[in] {dpid} dpid��
 * @param[in] {value} ��ǰdpֵ
 * @return Null
 * @note   Null
 */
unsigned char mcu_dp_enum_update(unsigned char dpid,unsigned char value);

/**
 * @brief  fault��dp�����ϴ�
 * @param[in] {dpid} dpid��
 * @param[in] {value} ��ǰdpֵ
 * @return Null
 * @note   Null
 */
unsigned char mcu_dp_fault_update(unsigned char dpid,unsigned long value);

/**
 * @brief  mcu��ȡbool���·�dpֵ
 * @param[in] {value} dp���ݻ�������ַ
 * @param[in] {len} dp���ݳ���
 * @return ��ǰdpֵ
 * @note   Null
 */
unsigned char mcu_get_dp_download_bool(const unsigned char value[],unsigned short len);

/**
 * @brief  mcu��ȡenum���·�dpֵ
 * @param[in] {value} dp���ݻ�������ַ
 * @param[in] {len} dp���ݳ���
 * @return ��ǰdpֵ
 * @note   Null
 */
unsigned char mcu_get_dp_download_enum(const unsigned char value[],unsigned short len);

/**
 * @brief  mcu��ȡvalue���·�dpֵ
 * @param[in] {value} dp���ݻ�������ַ
 * @param[in] {len} dp���ݳ���
 * @return ��ǰdpֵ
 * @note   Null
 */
unsigned long mcu_get_dp_download_value(const unsigned char value[],unsigned short len);

/**
 * @brief  ���ڽ��������ݴ洦��
 * @param[in] {value} �����յ���1�ֽ�����
 * @return Null
 * @note   ��MCU���ڴ������е��øú���,�������յ���������Ϊ��������
 */
void uart_receive_input(unsigned char value);

/**
 * @brief  wifi�������ݴ������
 * @param  Null
 * @return Null
 * @note   ��MCU������whileѭ���е��øú���
 */
void wifi_uart_service(void);

/**
 * @brief  Э�鴮�ڳ�ʼ������
 * @param  Null
 * @return Null
 * @note   ��MCU��ʼ�������е��øú���
 */
void wifi_protocol_init(void);


/**
 * @brief  ��¼����������ϱ�
 * @param[in] {time} ʱ�����ݳ���7�����ֽڱ�ʾ�Ƿ����־λ����������Ϊ�ꡢ�¡��ա�ʱ���֡���
 * @param[in] {dp_node} dp���ݽṹ����Ҫ�ڵ��ô˺���֮ǰ������һ���˽ṹ�����͵����飬���鳤��Ϊdp������
                                    ����¼��dp���ݻ��浽�˽ṹ���У�dpֵ��Ҫ����dp���������ͣ����뵽��Ӧ���͵Ľṹ���Ա��
 * @param[in] {dp_node_num} dp����
 * @return ���
 * -           0(ERROR): ʧ��
 * -           1(SUCCESS): �ɹ�
 * @note   MCU��Ҫ���е��øú���
 */
unsigned char dp_record_combine_update(unsigned char time[], t_DP_NODE dp_node[], unsigned char dp_node_num);


#ifndef WIFI_CONTROL_SELF_MODE
/**
 * @brief  MCU��ȡ��λwifi�ɹ���־
 * @param  Null
 * @return ��λ��־
 * -           0(RESET_WIFI_ERROR):ʧ��
 * -           1(RESET_WIFI_SUCCESS):�ɹ�
 * @note   1:MCU��������mcu_reset_wifi()����øú�����ȡ��λ״̬
 *         2:���Ϊģ���Դ���ģʽ,MCU������øú���
 */
unsigned char mcu_get_reset_wifi_flag(void);

/**
 * @brief  MCU��������wifi����ģʽ
 * @param  Null
 * @return Null
 * @note   1:MCU��������,ͨ��mcu_get_reset_wifi_flag()������ȡ����wifi�Ƿ�ɹ�
 *         2:���Ϊģ���Դ���ģʽ,MCU������øú���
 */
void mcu_reset_wifi(void);

/**
 * @brief  ��ȡ����wifi״̬�ɹ���־
 * @param  Null
 * @return wifimode flag
 * -           0(SET_WIFICONFIG_ERROR):ʧ��
 * -           1(SET_WIFICONFIG_SUCCESS):�ɹ�
 * @note   1:MCU��������mcu_set_wifi_mode()����øú�����ȡ��λ״̬
 *         2:���Ϊģ���Դ���ģʽ,MCU������øú���
 */
unsigned char mcu_get_wifimode_flag(void);

/**
 * @brief  MCU����wifi����ģʽ
 * @param[in] {mode} �����ģʽ
 * @ref        0(SMART_CONFIG):����smartconfigģʽ
 * @ref        1(AP_CONFIG):����APģʽ
 * @return Null
 * @note   1:MCU��������
 *         2:�ɹ���,���ж�set_wifi_config_state�Ƿ�ΪTRUE;TRUE��ʾΪ����wifi����ģʽ�ɹ�
 *         3:���Ϊģ���Դ���ģʽ,MCU������øú���
 */
void mcu_set_wifi_mode(unsigned char mode);

/**
 * @brief  MCU������ȡ��ǰwifi����״̬
 * @param  Null
 * @return wifi work state
 * -          SMART_CONFIG_STATE: smartconfig����״̬
 * -          AP_STATE: AP����״̬
 * -          WIFI_NOT_CONNECTED: WIFI���óɹ���δ����·����
 * -          WIFI_CONNECTED: WIFI���óɹ�������·����
 * -          WIFI_CONN_CLOUD: WIFI�Ѿ��������Ʒ�����
 * -          WIFI_LOW_POWER: WIFI���ڵ͹���ģʽ
 * -          SMART_AND_AP_STATE: WIFI smartconfig&AP ģʽ
 * @note   ���Ϊģ���Դ���ģʽ,MCU������øú���
 */
unsigned char mcu_get_wifi_work_state(void);
#endif

#ifdef SUPPORT_MCU_RTC_CHECK
/**
 * @brief  MCU��ȡ����ʱ��,����У�Ա���ʱ��
 * @param  Null
 * @return Null
 * @note   MCU����������ɺ��� mcu_write_rtctime ������У��rtcʱ��
 */
void mcu_get_system_time(void);
#endif

#ifdef SUPPORT_GREEN_TIME
/**
 * @brief  MCU��ȡ����ʱ��,����У�Ա���ʱ��
 * @param  Null
 * @return Null
 * @note   MCU����������ɺ��� mcu_write_gltime �����ڼ�¼���������ʱ�䣬
           ����������ʱ���У��
 */
void mcu_get_gelin_time(void);
#endif

#ifdef WIFI_TEST_ENABLE
/**
 * @brief  mcu����wifi���ܲ���
 * @param  Null
 * @return Null
 * @note   MCU��Ҫ���е��øù���
 */
void mcu_start_wifitest(void);
#endif

/**
 * @brief  MCU����wifi�̼�����
 * @param  Null
 * @return Null
 * @note   MCU����������ɺ��� wifi_update_handle �����ڿɻ�ȡ������ǰ״̬
 */
void wifi_update_request(void);

#ifdef DP_CACHE_SUPPORT
/**
 * @brief  ��ȡdp����ָ��
 * @param[in] {table} dp������û���Ҫ��ѯ��dp������û���Ҫ��ѯ����dp����tableֱ����NULL����
 * @param[in] {dp_num} ��Ҫ��ѯ��dp������,�����Ҫ��ѯ����dp����dp_num��0����
 * @return Null
 * @note   MCU��Ҫ���е��øù���
 */
void get_dp_cache(unsigned char *table,unsigned char dp_num);
#endif

#ifdef REPORTED_MCU_SN_ENABLE
/**
 * @brief  MCU�ϱ�SN
 * @param[in] {sn} SN��
 * @param[in] {sn_len} SN�ų���
 * @return Null
 * @note   MCU��Ҫ���е��ú󣬿��� mcu_sn_updata_result �����жԽ�����д���
 */
void mcu_sn_updata(unsigned char sn[],unsigned char sn_len);
#endif



#endif
