/**********************************Copyright (c)**********************************
**                       ��Ȩ���� (C), 2015-2020, Ϳѻ�Ƽ�
**
**                             http://www.tuya.com
**
*********************************************************************************/
/**
 * @file    protocol.c
 * @author  Ϳѻ�ۺ�Э�鿪����
 * @version v1.0.7
 * @date    2020.11.9
 * @brief                
 *                       *******�ǳ���Ҫ��һ��Ҫ��Ŷ������********
 *          1. �û��ڴ��ļ���ʵ�������·�/�ϱ�����
 *          2. DP��ID/TYPE�����ݴ���������Ҫ�û�����ʵ�ʶ���ʵ��
 *          3. ����ʼĳЩ�궨�����Ҫ�û�ʵ�ִ���ĺ����ڲ���#err��ʾ,��ɺ�������ɾ����#err
 */


#include "wifi.h"

/******************************************************************************
                                ��ֲ��֪:
1:MCU������while��ֱ�ӵ���mcu_api.c�ڵ�wifi_uart_service()����
2:����������ʼ����ɺ�,���鲻���йش����ж�,�������ж�,���ж�ʱ������,���жϻ����𴮿����ݰ���ʧ
3:�������ж�/��ʱ���ж��ڵ����ϱ�����
******************************************************************************/

         
/******************************************************************************
                              ��һ��:��ʼ��
1:����Ҫʹ�õ�wifi����ļ����ļ���include "wifi.h"
2:��MCU��ʼ���е���mcu_api.c�ļ��е�wifi_protocol_init()����
3:��MCU���ڵ��ֽڷ��ͺ�������protocol.c�ļ���uart_transmit_output������,��ɾ��//#error
4:��MCU���ڽ��պ����е���mcu_api.c�ļ��ڵ�uart_receive_input����,�������յ����ֽ���Ϊ��������
5:��Ƭ������whileѭ�������mcu_api.c�ļ��ڵ�wifi_uart_service()����
******************************************************************************/

/******************************************************************************
                        1:dp���ݵ��������Ͷ��ձ�
          **��Ϊ�Զ����ɴ���,���ڿ���ƽ̨������޸�����������MCU_SDK**         
******************************************************************************/
const DOWNLOAD_CMD_S download_cmd[] =
{
  {DPID_UNLOCK_FINGERPRINT, DP_TYPE_VALUE},
  {DPID_UNLOCK_PASSWORD, DP_TYPE_VALUE},
  {DPID_UNLOCK_CARD, DP_TYPE_VALUE},
  {DPID_UNLOCK_KEY, DP_TYPE_VALUE},
  {DPID_RESIDUAL_ELECTRICITY, DP_TYPE_VALUE},
  {DPID_UNLOCK_APP, DP_TYPE_VALUE},
  {DPID_REMOTE_NO_DP_KEY, DP_TYPE_RAW},
  {DPID_LOCK_MOTOR_STATE, DP_TYPE_BOOL},
  {DPID_INITIATIVE_MESSAGE, DP_TYPE_RAW},
};



/******************************************************************************
                           2:���ڵ��ֽڷ��ͺ���
�뽫MCU���ڷ��ͺ�������ú�����,�������յ���������Ϊ�������봮�ڷ��ͺ���
******************************************************************************/

/**
 * @brief  ���ڷ�������
 * @param[in] {value} ����Ҫ���͵�1�ֽ�����
 * @return Null
 * @note   �뽫MCU���ڷ��ͺ�������ú�����,�������յ���������Ϊ�������봮�ڷ��ͺ���
 */
void uart_transmit_output(unsigned char value)
{
    //#error "�뽫MCU���ڷ��ͺ�������ú���,��ɾ������"
    printf("===%c\r\n",value);
    UART3_SendByte(value);
/*
    //ʾ��:
    extern void Uart_PutChar(unsigned char value);
    Uart_PutChar(value);	                                //���ڷ��ͺ���
*/
}
/******************************************************************************
                           �ڶ���:ʵ�־����û�����
1:APP�·����ݴ���
2:�����ϱ�����
******************************************************************************/

/******************************************************************************
                            1:���������ϱ�����
��ǰ��������ȫ�������ϱ�(�������·�/���ϱ���ֻ�ϱ�)
  ��Ҫ�û�����ʵ�����ʵ��:
  1:��Ҫʵ�ֿ��·�/���ϱ����ݵ��ϱ�
  2:��Ҫʵ��ֻ�ϱ����ݵ��ϱ�
�˺���ΪMCU�ڲ��������
�û�Ҳ�ɵ��ô˺���ʵ��ȫ�������ϱ�
******************************************************************************/

//�Զ������������ϱ�����

/**
 * @brief  ϵͳ����dp����Ϣ�ϴ�,ʵ��APP��muc����ͬ��
 * @param  Null
 * @return Null
 * @note   �˺���SDK�ڲ�����ã�MCU����ʵ�ָú����������ϱ����ܣ�����ֻ�ϱ��Ϳ��ϱ����·�������
 */
void all_data_update(void)
{
    //#error "���ڴ˴�����·����ϱ����ݼ�ֻ�ϱ�����ʾ��,������ɺ�ɾ������"
    /* 
    //�˴���Ϊƽ̨�Զ����ɣ��밴��ʵ�������޸�ÿ�����·����ϱ�������ֻ�ϱ�����
    mcu_dp_value_update(DPID_UNLOCK_FINGERPRINT,��ǰָ�ƽ���); //VALUE�������ϱ�;
    mcu_dp_value_update(DPID_UNLOCK_PASSWORD,��ǰ��ͨ�������); //VALUE�������ϱ�;
    mcu_dp_value_update(DPID_UNLOCK_CARD,��ǰ��Ƭ����); //VALUE�������ϱ�;
    mcu_dp_value_update(DPID_UNLOCK_KEY,��ǰԿ�׽���); //VALUE�������ϱ�;
    mcu_dp_value_update(DPID_RESIDUAL_ELECTRICITY,��ǰʣ�����); //VALUE�������ϱ�;
    mcu_dp_value_update(DPID_UNLOCK_APP,��ǰAppԶ�̽���); //VALUE�������ϱ�;
    mcu_dp_raw_update(DPID_REMOTE_NO_DP_KEY,��ǰ����Զ�̿���ָ��,��ǰ����Զ�̿������ݳ���); //RAW�������ϱ�;
    mcu_dp_bool_update(DPID_LOCK_MOTOR_STATE,��ǰ����״̬); //BOOL�������ϱ�;
    mcu_dp_raw_update(DPID_INITIATIVE_MESSAGE,��ǰ������Ϣ����ָ��,��ǰ������Ϣ�������ݳ���); //RAW�������ϱ�;

    */
}


/******************************************************************************
                                WARNING!!!    
                            2:���������ϱ�����
�Զ�������ģ�庯��,�������û�����ʵ�����ݴ���
******************************************************************************/
/*****************************************************************************
�������� : dp_download_remote_no_dp_key_handle
�������� : ���DPID_REMOTE_NO_DP_KEY�Ĵ�����
������� : value:����Դ����
        : length:���ݳ���
���ز��� : �ɹ�����:SUCCESS/ʧ�ܷ���:ERROR
ʹ��˵�� : ���·����ϱ�����,��Ҫ�ڴ��������ݺ��ϱ���������app
*****************************************************************************/
static unsigned char dp_download_remote_no_dp_key_handle(const unsigned char value[], unsigned short length)
{
    //ʾ��:��ǰDP����ΪRAW
    unsigned char ret;
    /*
    //RAW type data processing
    
    */
    
    //There should be a report after processing the DP
    ret = mcu_dp_raw_update(DPID_REMOTE_NO_DP_KEY,value,length);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}
/*****************************************************************************
�������� : dp_download_lock_motor_state_handle
�������� : ���DPID_LOCK_MOTOR_STATE�Ĵ�����
������� : value:����Դ����
        : length:���ݳ���
���ز��� : �ɹ�����:SUCCESS/ʧ�ܷ���:ERROR
ʹ��˵�� : ���·����ϱ�����,��Ҫ�ڴ��������ݺ��ϱ���������app
*****************************************************************************/
static unsigned char dp_download_lock_motor_state_handle(const unsigned char value[], unsigned short length)
{
    //ʾ��:��ǰDP����ΪBOOL
    unsigned char ret;
    //0:off/1:on
    unsigned char lock_motor_state;
    
    lock_motor_state = mcu_get_dp_download_bool(value,length);
    if(lock_motor_state == 0) {
        //bool off
    }else {
        //bool on
    }
  
    //There should be a report after processing the DP
    ret = mcu_dp_bool_update(DPID_LOCK_MOTOR_STATE,lock_motor_state);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}
/*****************************************************************************
�������� : dp_download_initiative_message_handle
�������� : ���DPID_INITIATIVE_MESSAGE�Ĵ�����
������� : value:����Դ����
        : length:���ݳ���
���ز��� : �ɹ�����:SUCCESS/ʧ�ܷ���:ERROR
ʹ��˵�� : ���·����ϱ�����,��Ҫ�ڴ��������ݺ��ϱ���������app
*****************************************************************************/
static unsigned char dp_download_initiative_message_handle(const unsigned char value[], unsigned short length)
{
    //ʾ��:��ǰDP����ΪRAW
    unsigned char ret;
    /*
    //RAW type data processing
    
    */
    
    //There should be a report after processing the DP
    ret = mcu_dp_raw_update(DPID_INITIATIVE_MESSAGE,value,length);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}





/******************************************************************************
                                WARNING!!!                     
���º����û������޸�!!
******************************************************************************/
/**
 * @brief  dp�·�������
 * @param[in] {dpid} dpid ���
 * @param[in] {value} dp���ݻ�������ַ
 * @param[in] {length} dp���ݳ���
 * @return dp������
 * -           0(ERROR): ʧ��
 * -           1(SUCCESS): �ɹ�
 * @note   �ú����û������޸�
 */
unsigned char dp_download_handle(unsigned char dpid,const unsigned char value[], unsigned short length)
{
    /*********************************
    ��ǰ����������·�/���ϱ����ݵ���                    
    ���庯������Ҫʵ���·����ݴ���
    �������Ҫ��������������APP��,����APP����Ϊ�·�ʧ��
    ***********************************/
    unsigned char ret;
    switch(dpid) {
        case DPID_REMOTE_NO_DP_KEY:
            //����Զ�̿��Ŵ�����
            ret = dp_download_remote_no_dp_key_handle(value,length);
        break;
        case DPID_LOCK_MOTOR_STATE:
            //����״̬������
            ret = dp_download_lock_motor_state_handle(value,length);
        break;
        case DPID_INITIATIVE_MESSAGE:
            //������Ϣ���ʹ�����
            ret = dp_download_initiative_message_handle(value,length);
        break;


      default:
      break;
    }
    return ret;
}

/**
 * @brief  ��ȡ����dp�����ܺ�
 * @param[in] Null
 * @return �·������ܺ�
 * @note   �ú����û������޸�
 */
unsigned char get_download_cmd_total(void)
{
    return(sizeof(download_cmd) / sizeof(download_cmd[0]));
}



/******************************************************************************
                                WARNING!!!                     
�˴���ΪSDK�ڲ�����,�밴��ʵ��dp����ʵ�ֺ����ڲ�����
******************************************************************************/
#ifdef SUPPORT_MCU_RTC_CHECK
/**
 * @brief  MCUУ�Ա���RTCʱ��
 * @param[in] {time} ��ȡ���ĸ���ʱ������
 * @return Null
 * @note   MCU��Ҫ����ʵ�ָù���
 */
void mcu_write_rtctime(unsigned char time[])
{
    //#error "���������RTCʱ��д�����,��ɾ������"
    /*
    time[0]Ϊ�Ƿ��ȡʱ��ɹ���־��Ϊ 0 ��ʾʧ�ܣ�Ϊ 1��ʾ�ɹ�
    time[1] Ϊ �� �� , 0x00 �� ʾ2000 ��
    time[2]Ϊ�·ݣ��� 1 ��ʼ��12 ����
    time[3]Ϊ���ڣ��� 1 ��ʼ��31 ����
    time[4]Ϊʱ�ӣ��� 0 ��ʼ��23 ����
    time[5]Ϊ���ӣ��� 0 ��ʼ��59 ����
    time[6]Ϊ���ӣ��� 0 ��ʼ��59 ����
    time[7]Ϊ���ڣ��� 1 ��ʼ�� 7 ������1��������һ
   */
    if(time[0] == 1) {
      //��ȷ���յ�wifiģ�鷵�صı���ʱ������ 
     
    }else {
      //��ȡ����ʱ�����ݳ���,�п����ǵ�ǰwifiģ��δ����
    }
}
#endif

#ifdef SUPPORT_GREEN_TIME
/**
 * @brief  MCU��ѯ����ʱ�䷵�غ���
 * @param[in] {gl_time} ���صĵ�ǰ����ʱ��(�ӵ͵���7λ���ֱ�Ϊ������ʱ��������)
 * @return Null
 * @note   MCU�������� mcu_get_gelin_time �ɹ���,�ú����ڿɻ�ȡ����ʱ��
 */
void mcu_write_gltime(unsigned char gl_time[])
{
    //#error "��������ɸ���ʱ���¼����,��ɾ������"
    /*
    gl_time[0]Ϊ�Ƿ��ȡʱ��ɹ���־��Ϊ 0 ��ʾʧ�ܣ�Ϊ 1��ʾ�ɹ�
    gl_time[1]Ϊ��� , 0x00 ��ʾ 2000 ��
    gl_time[2]Ϊ�·ݣ��� 1 ��ʼ��12 ����
    gl_time[3]Ϊ���ڣ��� 1 ��ʼ��31 ����
    gl_time[4]Ϊʱ�ӣ��� 0 ��ʼ��23 ����
    gl_time[5]Ϊ���ӣ��� 0 ��ʼ��59 ����
    gl_time[6]Ϊ���ӣ��� 0 ��ʼ��59 ����
    gl_time[7]Ϊ���ڣ��� 1 ��ʼ�� 7 ������1��������һ
   */
    if(gl_time[0] == 1) {
      //��ȷ���յ�wifiģ�鷵�صı���ʱ������ 
     
    }else {
      //��ȡ����ʱ�����ݳ���,�п����ǵ�ǰwifiģ��δ����
    }
}
#endif

#ifdef WIFI_TEST_ENABLE
/**
 * @brief  wifi���ܲ��Է���
 * @param[in] {result} wifi���ܲ��Խ��
 * @ref       0: ʧ��
 * @ref       1: �ɹ�
 * @param[in] {rssi} ���Գɹ���ʾwifi�ź�ǿ��/����ʧ�ܱ�ʾ��������
 * @return Null
 * @note   MCU��Ҫ����ʵ�ָù���
 */
void wifi_test_result(unsigned char result,unsigned char rssi)
{
    //#error "������ʵ��wifi���ܲ��Գɹ�/ʧ�ܴ���,��ɺ���ɾ������"
    if(result == 0) {
        //����ʧ��
        if(rssi == 0x00) {
            //δɨ�赽����Ϊtuya_mdev_test·����,����
        }else if(rssi == 0x01) {
            //ģ��δ��Ȩ
        }
    }else {
        //���Գɹ�
        //rssiΪ�ź�ǿ��(0-100, 0�ź���100�ź���ǿ)
    }
}
#endif

/**
 * @brief  MCU����wifi�̼��������غ���
 * @param[in] {status} У���־
 * @return Null
 * @note   MCU�������� wifi_update_request ������ɺ�ú����ڿɻ�ȡ������ǰ״̬
 */
void wifi_update_handle(unsigned char status)
{ 
    //#error "���������wifiģ������״̬���ش���,��ɾ������"

    switch (status) {
        case 0: {
            //��ʼ���̼�����
            break;
        }

        case 1: {
            //�Ѿ������¹̼�
            break;
        }

        case 2: {
            //���ڸ��¹̼�
            break;
        }

        case 3: {
            //�̼����³ɹ�
            break;
        }

        case 4: {
            //�̼�����ʧ��
            break;
        }

        default:
        break;
    }
}

#ifdef SUPPORT_MCU_FIRM_UPDATE
/**
 * @brief  MCU����mcu�̼�����
 * @param  Null
 * @return Null
 * @note   MCU����������ɺ��� mcu_update_handle �����ڿɻ�ȡ������ǰ״̬
 */
void mcu_update_request(void)
{ 
    wifi_uart_write_frame(MCU_UG_REQ_CMD, 0);
}

/**
 * @brief  MCU����mcu�̼��������غ���
 * @param[in] {status} У���־
 * @return Null
 * @note   MCU�������� mcu_update_request ������ɺ�ú����ڿɻ�ȡ������ǰ״̬
 */
void mcu_update_handle(unsigned char status)
{ 
    //#error "���������mcu����״̬�������,��ɾ������"

    switch (status) {
        case 0: {
            //��ʼ���̼�����
            break;
        }

        case 1: {
            //�Ѿ������¹̼�
            break;
        }

        case 2: {
            //���ڸ��¹̼�
            break;
        }

        case 3: {
            //�̼����³ɹ�
            break;
        }

        case 4: {
            //�̼�����ʧ��
            break;
        }

        default:
        break;
    }
}

/**
 * @brief  MCU����̼�����ģʽ
 * @param[in] {value} �̼�������
 * @param[in] {position} ��ǰ���ݰ����ڹ̼�λ��
 * @param[in] {length} ��ǰ�̼�������(�̼�������Ϊ0ʱ,��ʾ�̼����������)
 * @return Null
 * @note   MCU��Ҫ����ʵ�ָù���
 */
unsigned char mcu_firm_update_handle(const unsigned char value[],unsigned long position,unsigned short length)
{
    //#error "���������MCU�̼���������,��ɺ���ɾ������"
    if(length == 0) {
        //�̼����ݷ������
    }else {
        //�̼����ݴ���
    }
    
    return SUCCESS;
}
#endif

#ifdef REPORTED_MCU_SN_ENABLE
/**
 * @brief  MCU�ϱ�SN
 * @param[in] {result} ״̬���
 * @return Null
 * @note   MCU��Ҫ�����е���mcu_sn_updata�������ڴ˺����Խ��յĽ�����д���
 */
void mcu_sn_updata_result(unsigned char result)
{
    //#error "���������MCU�ϱ�SN����������,��ɾ������"
    if(0 == result) {
        //�ϱ��ɹ�
    }else {
        //�ϱ�ʧ��
    }
}
#endif

#ifdef WIFI_RESET_NOTICE_ENABLE
/**
 * @brief  ģ������״̬֪ͨ
 * @param[in] {result} ״̬���
 * @return Null
 * @note   MCU��Ҫ�����е���mcu_sn_updata�������ڴ˺����Խ��յĽ�����д���
 */
void wifi_reset_notice(unsigned char result)
{
    //#error "���������ģ������״̬֪ͨ�������,��ɾ������"
    
    switch(result) {
        case 0x00:
            //ģ�鱾������
        break;
        
        case 0x01:
            //APPԶ������
        break;
        
        case 0x02:
            //APP�ָ���������
        break;
        
        case 0x03:
            //����������������豸������
        break;
        
        default:break;
    }
    wifi_uart_write_frame(WIFI_RESET_NOTICE_CMD, 0);
}
#endif



