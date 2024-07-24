/**********************************Copyright (c)**********************************
**                       ��Ȩ���� (C), 2015-2020, Ϳѻ�Ƽ�
**
**                             http://www.tuya.com
**
*********************************************************************************/
/**
 * @file    lock_api.h
 * @author  Ϳѻ�ۺ�Э�鿪����
 * @version v1.0.7
 * @date    2020.11.9
 * @brief   �����·�/�ϱ����ݴ����������ļ��º��������û��޸�,�û���Ҫ���õ��ļ����ڸ��ļ���
 */
 
 
#ifndef __LOCK_API_H_
#define __LOCK_API_H_


#ifdef LOCK_API_GLOBAL
  #define LOCK_API_EXTERN
#else
  #define LOCK_API_EXTERN   extern
#endif

#include "wifi.h"

#define DECODE_MAX_LEN                          16                              //���߶�̬������ܺ����󳤶�
  
  
//=============================================================================
//��������֡����
//=============================================================================
#define         TEMP_PASS_CMD                   0x11                            //�����ƶ���ʱ���루ֻ֧�ֵ��飩
#define         PASS_CHECK_CMD                  0x12                            //��̬����У��
#define         MUL_TEMP_PASS_CMD               0x13                            //�����ƶ���ʱ���루֧�ֶ��飩
#define         SCHEDULE_TEMP_PASS_CMD          0x14                            //�����ƶ���ʱ���루��schedule�б�
#define         OFFLINE_DYN_PW_CMD              0x16                            //���߶�̬����
#define         PICTURE_EVENT_STATE_CMD         0x60                            //�¼�״̬֪ͨ
#define         PICTURE_UPLOAD_CMD              0x61                            //ͼƬ�ϴ�
#define         PICTURE_UPLOAD_RETURN_CMD       0x62                            //ͼƬ�ϴ��������
#define         PICTURE_UPLOAD_STATE_GET_CMD    0x63                            //ͼƬ�ϴ�״̬��ȡ
#ifdef PHOTO_LOCK_PICTURE_UPLOAD_ENABLE
#define         PHOTO_LOCK_PICTURE_UPLOAD_CMD   0x64                            //��������ͼƬ�ϴ���ع���
#endif


//=============================================================================
//������
//=============================================================================
#ifdef PHOTO_LOCK_PICTURE_UPLOAD_CMD
#define         EVENT_TRIGGER_CAPTURE_NOTICE_SUBCMD         0x00                //�¼�����ץ��֪ͨ
#endif


#ifdef LOCK_API_ENABLE
/**
 * @brief  MCU�����ȡ�ƶ���ʱ����
 * @param  Null
 * @return Null
 * @note   MCU����������ɺ��� temp_pass_handle �����ڿɻ�ȡ��ʱ�������Ч��
 */
void mcu_get_temp_pass(void);

/**
 * @brief  MCU������ʱ���뷵�غ���
 * @param[in] {suc_flag} �����־(1:�ɹ�;0:ʧ��)
 * @param[in] {gl_time} ������Ч�ڸ���ʱ��(�ӵ͵���6λ���ֱ�Ϊ������ʱ����)
 * @param[in] {pass} ��ʱ��������(ascll���ʾ������pass_len)
 * @param[in] {pass_len} ��ʱ�������ݳ���
 * @return Null
 * @note   MCU�������� mcu_get_temp_pass �ɹ���,�ú����ڿ���ʱ�������Ч��
 */
void temp_pass_handle(unsigned char suc_flag, const unsigned char gl_time[], 
                      const unsigned char pass[], unsigned char pass_len);


/**
 * @brief  MCU����̬����У��
 * @param[in] {time} ����ǰ����ʱ��(�ӵ͵���6λ���ֱ�Ϊ������ʱ����)
 * @param[in] {user_pass} �û�����Ķ�̬����(�ӵ͵���8λ��������ASCLL���룬��Χ'0'-'9')
 * @param[in] {admin_num} ����Ա��������(0~10����Ϊ0����������Ч)
 * @param[in] {admin_len} ����Ա���볤��(�8���ֽ�)
 * @param[in] {admin_pass} �������Ա����(���������������У�����admin_num*admin_len����ASCLL���룬��Χ'0'-'9')
 * @return Null
 * @note   1:MCU��������
           2:�ɹ���,�� pass_check_handle �����ڿɻ�ȡ��̬����У����
 */
void dynamic_pass_check(unsigned char time[], unsigned char user_pass[], unsigned char admin_num,
                        unsigned char admin_len, unsigned char admin_pass[]);

/**
 * @brief  MCU����У�鶯̬���뷵�غ���
 * @param[in] {status} У���־(0:�ɹ�;1:ʧ��;2:δ����;3:���ȴ���)
 * @return Null
 * @note   MCU�������� dynamic_pass_check �ɹ���,�ú����ڿɻ�ȡУ����
 */
void pass_check_handle(unsigned char status);

/**
 * @brief  MCU�����ȡ�ƶ˶�����ʱ����
 * @param  Null
 * @return Null
 * @note   MCU����������ɺ��� mul_temp_pass_data �����ڿɻ�ȡ��ʱ�����������Ч��ʱ��
 */
void mcu_get_mul_temp_pass(void);

/**
 * @brief  MCU���������ʱ���뷵�غ���
 * @param[in] {data} ��������
 * @return Null
 * @note   Null
 */
void mul_temp_pass_handle(const unsigned char data[]);

/**
 * @brief  MCU�����ȡ�ƶ���ʱ����(��schedule�б�)
 * @param  Null
 * @return Null
 * @note   MCU����������ɺ��� mul_temp_pass_data �����ڿɻ�ȡ���
 */
void mcu_get_schedule_temp_pass(void);

/**
 * @brief  MCU������ʱ����(��schedule�б�)����
 * @param[in] {data} ��������
 * @return Null
 * @note   Null
 */
void schedule_temp_pass_handle(const unsigned char data[]);

#ifdef OFFLINE_DYN_PW_ENABLE
/**
 * @brief  ���߶�̬����
 * @param[in] {green_time} ����ʱ��
                green_time[0]Ϊ��ݣ�0x00 ��ʾ2000 ��
                green_time[1]Ϊ�·ݣ��� 1 ��ʼ��12 ����
                green_time[2]Ϊ���ڣ��� 1 ��ʼ��31 ����
                green_time[3]Ϊʱ�ӣ��� 0 ��ʼ��23 ����
                green_time[4]Ϊ���ӣ��� 0 ��ʼ��59 ����
                green_time[5]Ϊ���ӣ��� 0 ��ʼ��59 ����
 * @param[in] {pw} ���߶�̬����
 * @param[in] {pw_len} ���߶�̬���볤��
 * @return Null
 * @note   MCU��Ҫ���е��ú󣬿���offline_dynamic_password_result�����жԽ�����д���
 */
void offline_dynamic_password(unsigned char green_time[],unsigned char pw[],unsigned char pw_len);

/**
 * @brief  ���߶�̬������
 * @param[in] {result_data} �������
 * @return Null
 * @note   MCU��Ҫ�����е���offline_dynamic_password�������ڴ˺����Խ��յĽ�����д���
 */
void offline_dynamic_password_result(unsigned char result_data[]);
#endif

#ifdef PICTURE_UPLOAD_ENABLE
/**
 * @brief  ͼƬ�ϴ��¼�״̬֪ͨ
 * @param[in] {event} �¼���Ϣ����
 * @param[in] {picture} �Ƿ�Я��ͼƬ��Ϣ
 * @param[in] {pic_num} �ϴ�ͼƬ����(<=10)
 * @return Null
 * @note   MCU��Ҫ���е��ú󣬿��� picture_event_state_notice_result �����жԽ�����д���
 */
void picture_event_state_notice(unsigned short event, unsigned char picture, unsigned char pic_num);

/**
 * @brief  ͼƬ�ϴ��¼�״̬֪ͨ���
 * @param[in] {result} ���
 * @return Null
 * @note   MCU��Ҫ�����е��� picture_event_state_notice �������ڴ˺����Խ��յĽ�����д���
 */
void picture_event_state_notice_result(unsigned char result);

/**
 * @brief  ͼƬ�ϴ�
 * @param[in] {p_time} ʱ������
 * @param[in] {picture_id} ͼƬid��
 * @param[in] {pic_total_num} ͼƬ�ܰ���
 * @param[in] {pic_num} ��ǰ����ţ���0��ʼ��
 * @param[in] {p_pic_data} ͼƬ����
 * @param[in] {pic_data_len} ͼƬ���ݳ���
 * @return Null
 * @note   MCU��Ҫ���е��ú󣬿��� picture_upload_result �����жԽ�����д���
 */
void picture_upload(unsigned char p_time[], unsigned short picture_id, unsigned short pic_total_num, unsigned short pic_num, \
                                            unsigned char p_pic_data[], unsigned char pic_data_len);

/**
 * @brief  ͼƬ�ϴ����
 * @param[in] {result} ���
 * @return Null
 * @note   MCU��Ҫ�����е��� picture_upload �������ڴ˺����Խ��յĽ�����д���
 */
void picture_upload_result(unsigned char result);

/**
 * @brief  ͼƬ�ϴ��������
 * @param[in] {p_value} �������
 * @param[in] {data_len} ������ݳ���
 * @return Null
 * @note   MCU��Ҫ���жԽ��յĽ�����д���
 */
void picture_upload_return(unsigned char p_value[], unsigned short data_len);

/**
 * @brief  ͼƬ�ϴ�״̬��ȡ
 * @param  Null
 * @return Null
 * @note   MCU��Ҫ���е��ú󣬿��� picture_upload_state_get_result �����жԽ�����д���
 */
void picture_upload_state_get(void);

/**
 * @brief  ͼƬ�ϴ�״̬��ȡ���
 * @param[in] {p_value} �������
 * @param[in] {data_len} ������ݳ���
 * @return Null
 * @note   MCU��Ҫ�����е��� picture_upload_state_get �������ڴ˺����Խ��յĽ�����д���
 */
void picture_upload_state_get_result(unsigned char p_value[], unsigned short data_len);
#endif

#ifdef LOCK_KEEP_ALIVE
/**
 * @brief  ����IO����
 * @param[in] Null
 * @return Null
 * @note   MCU��Ҫ����ʵ���߼�����
 */
void keep_alive_awake(void);

/**
 * @brief  ��ȡWIFI״̬
 * @param  Null
 * @return Null
 * @note   MCU��Ҫ���е��ú󣬿��� get_wifi_state_result �����жԽ�����д���
 */
void mcu_get_wifi_state(void);

/**
 * @brief  mcu�յ�WiFi״̬
 * @param[in] {p_data} ����
 * @return Null
 * @note   MCU��Ҫ�����е��� mcu_get_wifi_state �������ڴ˺����Խ��յĽ�����д���
 */
void get_wifi_state_result(const unsigned char p_data[]);
#endif

#ifdef PHOTO_LOCK_PICTURE_UPLOAD_ENABLE
/**
 * @brief  MCU�����¼�����ץ��֪ͨ
 * @param[in] {event_infor_num} �¼���Ϣ����
 * @param[in] {capture_flag} �Ƿ�ץ��ͼƬ
 * @param[in] {capture_type} ץ������
 * @param[in] {capture_num} ץ��ͼƬ����
 * @param[in] {time_flag} ʱ���־λ  0:����ʱ�� 1:����ʱ�� 2:����ʱ��
 * @param[in] {p_time} ʱ������
 * @return Null
 * @note   MCU��Ҫ���е��ú󣬿��� event_trigger_capture_notice_result �����жԽ�����д���
 */
void mcu_event_trigger_capture_notice(unsigned short event_infor_num, unsigned char capture_flag, unsigned char capture_type,
                                        unsigned char capture_num, unsigned char time_flag, unsigned char p_time[]);
                                            
/**
 * @brief  ��������ͼƬ�ϴ���ع���
 * @param[in] {p_data} ����
 * @return Null
 * @note   
 */
void photo_lock_picture_upload_func(const unsigned char p_data[]);
#endif

#endif

#endif
