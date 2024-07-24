/**********************************Copyright (c)**********************************
**                       ��Ȩ���� (C), 2015-2020, Ϳѻ�Ƽ�
**
**                             http://www.tuya.com
**
*********************************************************************************/
/**
 * @file    system.c
 * @author  Ϳѻ�ۺ�Э�鿪����
 * @version v1.0.7
 * @date    2020.11.9
 * @brief   �������ݴ����û�������ĸ��ļ�ʵ������
 */


#define SYSTEM_GLOBAL

#include "wifi.h"
#include "protocol.h"
//
//
extern const DOWNLOAD_CMD_S download_cmd[];

/*�豸����ѡ��*/
/*���ڴ˴�ѡ����Ҫ�������豸��������ONΪ������OFFΪ�ر�*/
tCAP_equip cap_equip = {
#ifndef PICTURE_UPLOAD_ENABLE
    OFF,        //�豸��֧�� ����/ͼƬ���� ����
#else
    ON,         //�豸֧�� ����/ͼƬ���� ����
#endif
#ifndef CAP_COMMU_MODE_ENABLE
    OFF,        //�豸���ô���ͨ�ŷ�ʽ�ϴ�ͼƬ
#else
    ON,         //�豸����SPIͨ�ŷ�ʽ�ϴ�ͼƬ
#endif
    OFF,        //�豸�Ƿ�Ϊ��Ӫ�������� OFF:��ʾ��֧��, ON:��ʾ֧�� ���ݲ�֧�ִ˹��ܣ�
#ifndef WIFI_RESET_NOTICE_ENABLE
    OFF,        //�豸��֧��ģ������״̬֪ͨ
#else
    ON,         //�豸֧��ģ������״̬֪ͨ
#endif
    OFF,        //Ԥ��
    OFF,        //Ԥ��
    OFF,        //Ԥ��
    OFF         //Ԥ��
};

/**
 * @brief  дwifi_uart�ֽ�
 * @param[in] {dest} ��������ʵ��ַ
 * @param[in] {byte} д���ֽ�ֵ
 * @return д����ɺ���ܳ���
 */
unsigned short set_wifi_uart_byte(unsigned short dest, unsigned char byte)
{
    unsigned char *obj = (unsigned char *)wifi_uart_tx_buf + DATA_START + dest;
    
    *obj = byte;
    dest += 1;
    
    return dest;
}

/**
 * @brief  дwifi_uart_buffer
 * @param[in] {dest} Ŀ���ַ
 * @param[in] {src} Դ��ַ
 * @param[in] {len} ���ݳ���
 * @return д������Ļ����ַ
 */
unsigned short set_wifi_uart_buffer(unsigned short dest, unsigned char *src, unsigned short len)
{
    unsigned char *obj = (unsigned char *)wifi_uart_tx_buf + DATA_START + dest;
    
    my_memcpy(obj,src,len);
    
    dest += len;
    return dest;
}

/**
 * @brief  ����У���
 * @param[in] {pack} ����Դָ��
 * @param[in] {pack_len} ����У��ͳ���
 * @return У���
 */
unsigned char get_check_sum(unsigned char *pack, unsigned short pack_len)
{
    unsigned short i;
    unsigned char check_sum = 0;
    
    for(i = 0; i < pack_len; i ++) {
        check_sum += *pack ++;
    }
    
    return check_sum;
}

/**
 * @brief  ���ڷ���һ������
 * @param[in] {in} ���ͻ���ָ��
 * @param[in] {len} ���ݷ��ͳ���
 * @return Null
 */
static void wifi_uart_write_data(unsigned char *in, unsigned short len)
{
    if((NULL == in) || (0 == len)) {
        return;
    }
    
    while(len --) {
        uart_transmit_output(*in);
        in ++;
    }
}

/**
 * @brief  ��wifi���ڷ���һ֡����
 * @param[in] {fr_type} ֡����
 * @param[in] {fr_ver} ֡�汾
 * @param[in] {len} ���ݳ���
 * @return Null
 */
void wifi_uart_write_frame(unsigned char fr_type, unsigned short len)
{
    unsigned char check_sum = 0;
    
    wifi_uart_tx_buf[HEAD_FIRST] = FRAME_FIRST;
    wifi_uart_tx_buf[HEAD_SECOND] = FRAME_SECOND;
    wifi_uart_tx_buf[PROTOCOL_VERSION] = MCU_TX_VER;
    wifi_uart_tx_buf[FRAME_TYPE] = fr_type;
    wifi_uart_tx_buf[LENGTH_HIGH] = len >> 8;
    wifi_uart_tx_buf[LENGTH_LOW] = len & 0xff;
    
    len += PROTOCOL_HEAD;
    check_sum = get_check_sum((unsigned char *)wifi_uart_tx_buf, len - 1);
    wifi_uart_tx_buf[len - 1] = check_sum;
    //
    wifi_uart_write_data((unsigned char *)wifi_uart_tx_buf, len);
}

/**
 * @brief  ��Ʒ��Ϣ�ϴ�
 * @param  Null
 * @return Null
 */
static void product_info_update(void)
{
    unsigned char length = 0;
    unsigned char str[10] = {0};
  
    length = set_wifi_uart_buffer(length, "{\"p\":\"", my_strlen("{\"p\":\""));
    length = set_wifi_uart_buffer(length,(unsigned char *)PRODUCT_KEY,my_strlen((unsigned char *)PRODUCT_KEY));
    length = set_wifi_uart_buffer(length, "\",\"v\":\"", my_strlen("\",\"v\":\""));
    length = set_wifi_uart_buffer(length,(unsigned char *)MCU_VER,my_strlen((unsigned char *)MCU_VER));
    length = set_wifi_uart_buffer(length, "\"", my_strlen("\""));
    
#ifdef CONFIG_MODE_CHOOSE
    sprintf((char *)str,",\"n\":%d",CONFIG_MODE_CHOOSE);
    length = set_wifi_uart_buffer(length, str, my_strlen(str));
#endif
    
    sprintf((char *)str, ",\"cap\":%d", cap_equip.whole);
    length = set_wifi_uart_buffer(length, str, my_strlen(str));
    
    length = set_wifi_uart_buffer(length, "}", my_strlen("}"));
    
    wifi_uart_write_frame(PRODUCT_INFO_CMD, length);
}

/**
 * @brief  ��ȡ�ƶ�DPID�������е����
 * @param[in] {dpid} dpid
 * @return dp���
 */
static unsigned char get_dowmload_dpid_index(unsigned char dpid)
{
    unsigned char index;
    unsigned char total = get_download_cmd_total();
    
    for(index = 0; index < total; index ++) {
        if(download_cmd[index].dp_id == dpid) {
            break;
        }
    }
    
    return index;
}

/**
 * @brief  ��ȡָ��DPID�������е����
 * @param[in] {dpid} dpid
 * @param[out] {p_dp_type} dp ����
 * @return SUCCESS/ERROR
 */
unsigned char get_dp_type(unsigned char dpid, unsigned char *p_dp_type)
{
    unsigned char index;
    unsigned char total = get_download_cmd_total();
    
    for(index = 0; index < total; index ++) {
        if(download_cmd[index].dp_id == dpid) {
            *p_dp_type = download_cmd[index].dp_type;
            return SUCCESS;
        }
    }
    
    return ERROR;
}

/**
 * @brief  �·����ݴ���
 * @param[in] {value} �·�����Դָ��
 * @return �������ݴ�����
 */
static unsigned char data_point_handle(const unsigned char value[])
{
    unsigned char dp_id,index;
    unsigned char dp_type;
    unsigned char ret;
    unsigned short dp_len;
    
    dp_id = value[0];
    dp_type = value[1];
    dp_len = value[2] * 0x100;
    dp_len += value[3];
    
    index = get_dowmload_dpid_index(dp_id);

    if(dp_type != download_cmd[index].dp_type) {
        //������ʾ
        return FALSE;
    }else {
        ret = dp_download_handle(dp_id,value + 4,dp_len);
    }
    
    return ret;
}

/**
 * @brief  ����֡����
 * @param[in] {offset} ������ʼλ
 * @return Null
 */
void data_handle(unsigned short offset)
{
#ifdef SUPPORT_MCU_FIRM_UPDATE
    unsigned char *firmware_addr = NULL;
    static unsigned long firm_length = 0;                 //MCU�����ļ�����
    static unsigned char firm_update_flag;                //MCU������־
    unsigned long dp_len = 0;
#else
    unsigned short dp_len;
#endif
    unsigned char result;
    unsigned char dp_num;

    unsigned char ret;
    unsigned short i,total_len;
    unsigned char cmd_type = wifi_data_process_buf[offset + FRAME_TYPE];
    
    switch(cmd_type)
    {
        case PRODUCT_INFO_CMD:                                  //��Ʒ��Ϣ
            product_info_update();
        break;
        
#ifndef WIFI_CONTROL_SELF_MODE
        case WIFI_STATE_CMD:                                    //wifi����״̬	
            wifi_work_state = wifi_data_process_buf[offset + DATA_START];
            wifi_uart_write_frame(WIFI_STATE_CMD,0);
        break;

        case WIFI_RESET_CMD:                                    //����wifi(wifi���سɹ�)
            reset_wifi_flag = RESET_WIFI_SUCCESS;
        break;
          
        case WIFI_MODE_CMD:                                     //ѡ��smartconfig/APģʽ(wifi���سɹ�)	
            set_wifimode_flag = SET_WIFICONFIG_SUCCESS;
        break;
#endif
      
        case DATA_QUERT_CMD:                                    //�����·�
            total_len = wifi_data_process_buf[offset + LENGTH_HIGH] * 0x100;
            total_len += wifi_data_process_buf[offset + LENGTH_LOW];
            
            for(i = 0;i < total_len; ) {
                dp_len = wifi_data_process_buf[offset + DATA_START + i + 2] * 0x100;
                dp_len += wifi_data_process_buf[offset + DATA_START + i + 3];
                //
                ret = data_point_handle((unsigned char *)wifi_data_process_buf + offset + DATA_START + i);
                
                if(SUCCESS == ret) {
                  //�ɹ���ʾ
                }else {
                  //������ʾ
                }
                
                i += (dp_len + 4);
            }
        break;
          
#ifdef SUPPORT_MCU_FIRM_UPDATE
        case MCU_UG_REQ_CMD:                                    //��������
            mcu_update_handle(wifi_data_process_buf[offset + DATA_START]);
        break;
        
        case UPDATE_START_CMD:                                  //������ʼ
            firm_length = wifi_data_process_buf[offset + DATA_START];
            firm_length <<= 8;
            firm_length |= wifi_data_process_buf[offset + DATA_START + 1];
            firm_length <<= 8;
            firm_length |= wifi_data_process_buf[offset + DATA_START + 2];
            firm_length <<= 8;
            firm_length |= wifi_data_process_buf[offset + DATA_START + 3];
            //
            wifi_uart_write_frame(UPDATE_START_CMD,0);
            firm_update_flag = UPDATE_START_CMD;
        break;
          
        case UPDATE_TRANS_CMD:                                  //��������
            if(firm_update_flag == UPDATE_START_CMD) {
                //ֹͣһ�������ϱ�
                stop_update_flag = ENABLE;                                                 
                
                total_len = wifi_data_process_buf[offset + LENGTH_HIGH] * 0x100;
                total_len += wifi_data_process_buf[offset + LENGTH_LOW];
                
                dp_len = wifi_data_process_buf[offset + DATA_START];
                dp_len <<= 8;
                dp_len |= wifi_data_process_buf[offset + DATA_START + 1];
                dp_len <<= 8;
                dp_len |= wifi_data_process_buf[offset + DATA_START + 2];
                dp_len <<= 8;
                dp_len |= wifi_data_process_buf[offset + DATA_START + 3];
                
                firmware_addr = (unsigned char *)wifi_data_process_buf;
                firmware_addr += (offset + DATA_START + 4);
                if((total_len == 4) && (dp_len == firm_length)) {
                    //���һ��
                    ret = mcu_firm_update_handle(firmware_addr,dp_len,0);
                    
                    firm_update_flag = 0;
                }else if((total_len - 4) <= FIRM_UPDATA_SIZE) {
                    ret = mcu_firm_update_handle(firmware_addr,dp_len,total_len - 4);
                }else {
                    firm_update_flag = 0;
                    ret = ERROR;
                }
                
                if(ret == SUCCESS) {
                    wifi_uart_write_frame(UPDATE_TRANS_CMD,0);
                }
                //�ָ�һ�������ϱ�
                stop_update_flag = DISABLE;    
            }
        break;
#endif      

#ifdef SUPPORT_MCU_RTC_CHECK
        case GET_LOCAL_TIME_CMD:                              //��ȡ����ʱ��
            mcu_write_rtctime(wifi_data_process_buf + offset + DATA_START);
        break;
#endif
   
#ifdef WIFI_TEST_ENABLE
        case WIFI_TEST_CMD:                                     //wifi���ܲ���
            wifi_test_result(wifi_data_process_buf[offset + DATA_START],wifi_data_process_buf[offset + DATA_START + 1]);
        break;
#endif

        case WIFI_UG_REQ_CMD:                                   //����WIFIģ��̼�����
            wifi_update_handle(wifi_data_process_buf[offset + DATA_START]);
        break;

#ifdef LOCK_API_ENABLE
        case GET_GL_TIME_CMD:                                   //��ȡ����ʱ��
            mcu_write_gltime(wifi_data_process_buf + offset + DATA_START);
        break;

        case TEMP_PASS_CMD:                                     //�����ƶ���ʱ���루ֻ֧�ֵ��飩
            if (wifi_data_process_buf[offset + DATA_START] == 1) {
                total_len = wifi_data_process_buf[offset + LENGTH_HIGH] * 0x100;
                total_len += wifi_data_process_buf[offset + LENGTH_LOW];

                temp_pass_handle(wifi_data_process_buf[offset + DATA_START], wifi_data_process_buf + offset + DATA_START + 1,
                                 wifi_data_process_buf + offset + DATA_START + 7, total_len - 7);
            }else {
                temp_pass_handle(wifi_data_process_buf[offset + DATA_START], 0, 0, 0);
            }
        break;

        case PASS_CHECK_CMD:                                    //��̬����У��
            pass_check_handle(wifi_data_process_buf[offset + DATA_START]);
        break;

        case MUL_TEMP_PASS_CMD:                                 //�����ƶ���ʱ���루֧�ֶ��飩
            mul_temp_pass_handle(wifi_data_process_buf + offset + DATA_START);
        break;
        
        case SCHEDULE_TEMP_PASS_CMD:                            //�����ƶ���ʱ���루��schedule�б�
            schedule_temp_pass_handle(wifi_data_process_buf + offset + DATA_START);
        break;
#endif

#ifdef DP_CACHE_SUPPORT
        case GET_DP_CACHE_CMD:                                  //��ȡdp����ָ��
            total_len = wifi_data_process_buf[offset + LENGTH_HIGH] * 0x100;//��������ֽ���
            total_len += wifi_data_process_buf[offset + LENGTH_LOW];
            
            result = wifi_data_process_buf[offset + DATA_START]; //��ȡָ��ɹ���־
            if(result == 0) {     //��ȡָ��ʧ��
                //�û�����ʵ�ֻ�ȡָ��ʧ�ܺ�Ĳ���
              
            }else {               //��ȡָ��ɹ�
                dp_num = wifi_data_process_buf[offset + DATA_START + 1];//��ȡ����dp����
                
                for(i = 2;i < total_len;) {
                    dp_len = wifi_data_process_buf[offset + DATA_START + i + 2] * 0x100; //value���ֽ���
                    dp_len += wifi_data_process_buf[offset + DATA_START + i + 3];

                    //����״̬���ݵ�Ԫ
                    ret = data_point_handle((unsigned char *)wifi_data_process_buf + offset + DATA_START + i);
                    
                    if(SUCCESS == ret) {
                        //�ɹ���ʾ
                    }else {
                        //������ʾ
                    }
                    
                    i += (dp_len + 4);  //dp_len(value) + len(2) + type(1) + dpid(1)
                }
            }
        break;
#endif

#ifdef OFFLINE_DYN_PW_ENABLE
        case OFFLINE_DYN_PW_CMD:                                //���߶�̬����
            offline_dynamic_password_result(wifi_data_process_buf+offset + DATA_START);
        break;
#endif
    
#ifdef REPORTED_MCU_SN_ENABLE
        case REPORTED_MCU_SN_CMD:                               //�ϱ�MCU��SN
            mcu_sn_updata_result(wifi_data_process_buf[offset + DATA_START]);
        break;
#endif
        
#ifdef LOCK_KEEP_ALIVE
        case GET_WIFI_STATE_CMD:                                //��ȡWiFi״̬
            get_wifi_state_result((unsigned char *)wifi_data_process_buf + offset + DATA_START);
        break;
#endif
        
#ifdef WIFI_RESET_NOTICE_ENABLE
        case WIFI_RESET_NOTICE_CMD:                             //ģ������״̬֪ͨ
            wifi_reset_notice(wifi_data_process_buf[offset + DATA_START]);
        break;
#endif

#ifdef PICTURE_UPLOAD_ENABLE
        case PICTURE_EVENT_STATE_CMD:                           //�¼�״̬֪ͨ
            picture_event_state_notice_result(wifi_data_process_buf[offset + DATA_START]);
        break;
        
        case PICTURE_UPLOAD_CMD:                                //ͼƬ�ϴ�
            picture_upload_result(wifi_data_process_buf[offset + DATA_START]);
        break;
        
        case PICTURE_UPLOAD_RETURN_CMD:                         //ͼƬ�ϴ��������
            total_len = wifi_data_process_buf[offset + LENGTH_HIGH] * 0x100;
            total_len += wifi_data_process_buf[offset + LENGTH_LOW];
            picture_upload_return(wifi_data_process_buf + offset + DATA_START, total_len);
        break;
        
        case PICTURE_UPLOAD_STATE_GET_CMD:                      //ͼƬ�ϴ�״̬��ȡ
            total_len = wifi_data_process_buf[offset + LENGTH_HIGH] * 0x100;
            total_len += wifi_data_process_buf[offset + LENGTH_LOW];
            picture_upload_state_get_result(wifi_data_process_buf + offset + DATA_START, total_len);
        break;
#endif
        
#ifdef PHOTO_LOCK_PICTURE_UPLOAD_ENABLE
        case PHOTO_LOCK_PICTURE_UPLOAD_CMD:                      //��������ͼƬ�ϴ���ع���
            photo_lock_picture_upload_func(wifi_data_process_buf + offset + DATA_START);
        break;
#endif
        
        
        
        default:
        break;
    }
}

/**
 * @brief  �жϴ��ڽ��ջ������Ƿ�������
 * @param  Null
 * @return �Ƿ�������
 */
unsigned char get_queue_total_data(void)
{
    if(queue_in != queue_out)
        return 1;
    else
        return 0;
}

/**
 * @brief  ��ȡ����1�ֽ�����
 * @param  Null
 * @return Read the data
 */
unsigned char Queue_Read_Byte(void)
{
    unsigned char value;
    
    if(queue_out != queue_in) {
        //������
        if(queue_out >= (unsigned char *)(wifi_uart_rx_buf + sizeof(wifi_uart_rx_buf))) {
            //�����Ѿ���ĩβ
            queue_out = (unsigned char *)(wifi_uart_rx_buf);
        }
        
        value = *queue_out++;
    }
    
    return value;
}

