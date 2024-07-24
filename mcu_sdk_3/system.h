/**********************************Copyright (c)**********************************
**                       ��Ȩ���� (C), 2015-2020, Ϳѻ�Ƽ�
**
**                             http://www.tuya.com
**
*********************************************************************************/
/**
 * @file    system.h
 * @author  Ϳѻ�ۺ�Э�鿪����
 * @version v1.0.7
 * @date    2020.11.9
 * @brief   �������ݴ����û�������ĸ��ļ�ʵ������
 */


#ifndef __SYSTEM_H_
#define __SYSTEM_H_

#ifdef SYSTEM_GLOBAL
  #define SYSTEM_EXTERN
#else
  #define SYSTEM_EXTERN   extern
#endif

//=============================================================================
//֡���ֽ�˳��
//=============================================================================
#define         HEAD_FIRST                      0
#define         HEAD_SECOND                     1        
#define         PROTOCOL_VERSION                2
#define         FRAME_TYPE                      3
#define         LENGTH_HIGH                     4
#define         LENGTH_LOW                      5
#define         DATA_START                      6

//=============================================================================
//����֡����
//=============================================================================
#define         PRODUCT_INFO_CMD                0x01                            //��Ʒ��Ϣ
#define         WIFI_STATE_CMD                  0x02                            //wifi����״̬	
#define         WIFI_RESET_CMD                  0x03                            //����wifi
#define         WIFI_MODE_CMD                   0x04                            //ѡ��smartconfig/APģʽ
#ifdef STATE_FAST_UPLOAD_ENABLE
#define         STATE_UPLOAD_CMD                0x18                            //ʵʱ״̬�����ϱ�
#define         STATE_RC_UPLOAD_CMD             0x19                            //��¼��״̬�����ϱ�
#else
#define         STATE_UPLOAD_CMD                0x05                            //ʵʱ״̬�ϱ�
#define         STATE_RC_UPLOAD_CMD             0x08                            //��¼��״̬�ϱ�
#endif
#define         GET_LOCAL_TIME_CMD              0x06                            //��ȡ����ʱ��
#define         WIFI_TEST_CMD                   0x07                            //wifi���ܲ���
#define         DATA_QUERT_CMD                  0x09                            //�����·�
#define         WIFI_UG_REQ_CMD                 0x0a                            //MCU����WIFIģ��̼�����
#define         ROUTE_RSSI_CMD                  0x0b                            //��ѯ��ǰ����·�����ź�ǿ��
#define         MCU_UG_REQ_CMD                  0x0c                            //MCU����MCU�̼�����
#define         UPDATE_START_CMD                0x0d                            //MCU������ʼ
#define         UPDATE_TRANS_CMD                0x0e                            //MCU��������
#define         GET_GL_TIME_CMD                 0x10                            //��ȡ����ʱ��
#define         GET_DP_CACHE_CMD                0x15                            //��ȡdp����ָ��
#define         REPORTED_MCU_SN_CMD             0x17                            //�ϱ�MCU��SN
#ifdef LOCK_KEEP_ALIVE
#define         GET_WIFI_STATE_CMD              0x1A                            //��ȡWiFi״̬
#endif
#define         WIFI_RESET_NOTICE_CMD           0x25                            //ģ������״̬֪ͨ



//=============================================================================
#define MCU_RX_VER              0x00                                            //MCU����Э��汾��
#define MCU_TX_VER              0x00                                            //MCU����Э��汾��
#define PROTOCOL_HEAD           0x07                                            //�̶�Э��ͷ����
#define FIRM_UPDATA_SIZE        256                                             //��������С
#define FRAME_FIRST             0x55
#define FRAME_SECOND            0xaa

//============================================================================= 
SYSTEM_EXTERN unsigned char wifi_data_process_buf[PROTOCOL_HEAD + WIFI_DATA_PROCESS_LMT];       //�������ݴ�����
SYSTEM_EXTERN unsigned char volatile wifi_uart_rx_buf[PROTOCOL_HEAD + WIFI_UART_RECV_BUF_LMT];  //���ڽ��ջ���
SYSTEM_EXTERN unsigned char wifi_uart_tx_buf[PROTOCOL_HEAD + WIFIR_UART_SEND_BUF_LMT];          //���ڷ��ͻ���
//
SYSTEM_EXTERN volatile unsigned char *queue_in;
SYSTEM_EXTERN volatile unsigned char *queue_out;

SYSTEM_EXTERN unsigned char stop_update_flag;

#ifndef WIFI_CONTROL_SELF_MODE
SYSTEM_EXTERN unsigned char reset_wifi_flag;                                                  //����wifi��־(TRUE:�ɹ�/FALSE:ʧ��)
SYSTEM_EXTERN unsigned char set_wifimode_flag;                                                //����WIFI����ģʽ��־(TRUE:�ɹ�/FALSE:ʧ��)
SYSTEM_EXTERN unsigned char wifi_work_state;                                                  //wifiģ�鵱ǰ����״̬
#endif


/**
 * @brief  дwifi_uart�ֽ�
 * @param[in] {dest} ��������ʵ��ַ
 * @param[in] {byte} д���ֽ�ֵ
 * @return д����ɺ���ܳ���
 */
unsigned short set_wifi_uart_byte(unsigned short dest, unsigned char byte);

/**
 * @brief  дwifi_uart_buffer
 * @param[in] {dest} Ŀ���ַ
 * @param[in] {src} Դ��ַ
 * @param[in] {len} ���ݳ���
 * @return д������Ļ����ַ
 */
unsigned short set_wifi_uart_buffer(unsigned short dest, unsigned char *src, unsigned short len);

/**
 * @brief  ����У���
 * @param[in] {pack} ����Դָ��
 * @param[in] {pack_len} ����У��ͳ���
 * @return У���
 */
unsigned char get_check_sum(unsigned char *pack, unsigned short pack_len);

/**
 * @brief  ��wifi���ڷ���һ֡����
 * @param[in] {fr_type} ֡����
 * @param[in] {fr_ver} ֡�汾
 * @param[in] {len} ���ݳ���
 * @return Null
 */
void wifi_uart_write_frame(unsigned char fr_type, unsigned short len);

/**
 * @brief  ��ȡָ��DPID�������е����
 * @param[in] {dpid} dpid
 * @param[out] {p_dp_type} dp ����
 * @return SUCCESS/ERROR
 */
unsigned char get_dp_type(unsigned char dpid, unsigned char *p_dp_type);

/**
 * @brief  ����֡����
 * @param[in] {offset} ������ʼλ
 * @return Null
 */
void data_handle(unsigned short offset);

/**
 * @brief  �жϴ��ڽ��ջ������Ƿ�������
 * @param  Null
 * @return �Ƿ�������
 */
unsigned char get_queue_total_data(void);

/**
 * @brief  ��ȡ����1�ֽ�����
 * @param  Null
 * @return Read the data
 */
unsigned char Queue_Read_Byte(void);

#endif
  
  
