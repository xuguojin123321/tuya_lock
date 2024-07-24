/**********************************Copyright (c)**********************************
**                       ��Ȩ���� (C), 2015-2020, Ϳѻ�Ƽ�
**
**                             http://www.tuya.com
**
*********************************************************************************/
/**
 * @file    mcu_api.c
 * @author  Ϳѻ�ۺ�Э�鿪����
 * @version v1.0.7
 * @date    2020.11.9
 * @brief   �û���Ҫ�������õĺ������ڸ��ļ���
 */


#define MCU_API_GLOBAL

#include "wifi.h"

extern const DOWNLOAD_CMD_S download_cmd[];

/**
 * @brief  hexתbcd
 * @param[in] {Value_H} ���ֽ�
 * @param[in] {Value_L} ���ֽ�
 * @return ת����ɺ�����
 */
unsigned char hex_to_bcd(unsigned char Value_H,unsigned char Value_L)
{
    unsigned char bcd_value;
    
    if((Value_H >= '0') && (Value_H <= '9'))
        Value_H -= '0';
    else if((Value_H >= 'A') && (Value_H <= 'F'))
        Value_H = Value_H - 'A' + 10;
    else if((Value_H >= 'a') && (Value_H <= 'f'))
        Value_H = Value_H - 'a' + 10;
     
    bcd_value = Value_H & 0x0f;
    
    bcd_value <<= 4;
    if((Value_L >= '0') && (Value_L <= '9'))
        Value_L -= '0';
    else if((Value_L >= 'A') && (Value_L <= 'F'))
        Value_L = Value_L - 'a' + 10;
    else if((Value_L >= 'a') && (Value_L <= 'f'))
        Value_L = Value_L - 'a' + 10;
    
    bcd_value |= Value_L & 0x0f;

    return bcd_value;
}

/**
 * @brief  ���ַ�������
 * @param[in] {str} �ַ�����ַ
 * @return ���ݳ���
 */
unsigned long my_strlen(unsigned char *str)  
{
    unsigned long len = 0;
    if(str == NULL) { 
        return 0;
    }
    
    for(len = 0; *str ++ != '\0'; ) {
        len ++;
    }
    
    return len;
}

/**
 * @brief  ��src��ָ�ڴ������ǰcount���ֽ����ó��ַ�c
 * @param[out] {src} �����õ��ڴ��׵�ַ
 * @param[in] {ch} ���õ��ַ�
 * @param[in] {count} ���õ��ڴ泤��
 * @return �����õ��ڴ��׵�ַ
 */
void *my_memset(void *src,unsigned char ch,unsigned short count)
{
    unsigned char *tmp = (unsigned char *)src;
    
    if(src == NULL) {
        return NULL;
    }
    
    while(count --) {
        *tmp ++ = ch;
    }
    
    return src;
}

/**
 * @brief  �ڴ濽��
 * @param[out] {dest} Ŀ���ַ
 * @param[in] {src} Դ��ַ
 * @param[in] {count} �������ݸ���
 * @return ���ݴ�������Դ��ַ
 */
void *my_memcpy(void *dest, const void *src, unsigned short count)  
{  
    unsigned char *pdest = (unsigned char *)dest;  
    const unsigned char *psrc  = (const unsigned char *)src;  
    unsigned short i;
    
    if(dest == NULL || src == NULL) { 
        return NULL;
    }
    
    if((pdest <= psrc) || (pdest > psrc + count)) {  
        for(i = 0; i < count; i ++) {  
            pdest[i] = psrc[i];  
        }
    }else {
        for(i = count; i > 0; i --) {  
            pdest[i - 1] = psrc[i - 1];  
        }
    }  
    
    return dest;  
}

/**
 * @brief  �ַ�������
 * @param[in] {dest} Ŀ���ַ
 * @param[in] {src} Դ��ַ
 * @return ���ݴ�������Դ��ַ
 */
char *my_strcpy(char *dest, const char *src)  
{
    char *p = NULL;
	p = dest;
    while(*src!='\0') {
        *dest++ = *src++;
    }
    *dest = '\0';
    return p;
}

/**
 * @brief  �ַ����Ƚ�
 * @param[in] {s1} �ַ��� 1
 * @param[in] {s2} �ַ��� 2
 * @return ��С�Ƚ�ֵ
 * -         0:s1=s2
 * -         <0:s1<s2
 * -         >0:s1>s2
 */
int my_strcmp(char *s1 , char *s2)
{
    while( *s1 && *s2 && *s1 == *s2 ) {
        s1++;
        s2++;
    }
    return *s1 - *s2;
}

/**
 * @brief  ��int���Ͳ���ĸ��ֽ�
 * @param[in] {number} 4�ֽ�ԭ����
 * @param[out] {value} ������ɺ�4�ֽ�����
 * @return Null
 */
void int_to_byte(unsigned long number,unsigned char value[4])
{
    value[0] = number >> 24;
    value[1] = number >> 16;
    value[2] = number >> 8;
    value[3] = number & 0xff;
}

/**
 * @brief  ��4�ֽںϲ�Ϊ1��32bit����
 * @param[in] {value} 4�ֽ�����
 * @return �ϲ���ɺ��32bit����
 */
unsigned long byte_to_int(const unsigned char value[4])
{
    unsigned long nubmer = 0;

    nubmer = (unsigned long)value[0];
    nubmer <<= 8;
    nubmer |= (unsigned long)value[1];
    nubmer <<= 8;
    nubmer |= (unsigned long)value[2];
    nubmer <<= 8;
    nubmer |= (unsigned long)value[3];
    
    return nubmer;
}


/**
 * @brief  raw��dp�����ϴ�
 * @param[in] {dpid} dpid��
 * @param[in] {value} ��ǰdpֵָ��
 * @param[in] {len} ���ݳ���
 * @return Null
 * @note   Null
 */
unsigned char mcu_dp_raw_update(unsigned char dpid,const unsigned char value[],unsigned short len)
{
    unsigned short length = 0;
    
    if(stop_update_flag == ENABLE)
      return SUCCESS;
    //
    length = set_wifi_uart_byte(length,dpid);
    length = set_wifi_uart_byte(length,DP_TYPE_RAW);
    //
    length = set_wifi_uart_byte(length,len / 0x100);
    length = set_wifi_uart_byte(length,len % 0x100);
    //
    length = set_wifi_uart_buffer(length,(unsigned char *)value,len);
    
    wifi_uart_write_frame(STATE_UPLOAD_CMD,length);
    
    return SUCCESS;
}

/**
 * @brief  bool��dp�����ϴ�
 * @param[in] {dpid} dpid��
 * @param[in] {value} ��ǰdpֵ
 * @return Null
 * @note   Null
 */
unsigned char mcu_dp_bool_update(unsigned char dpid,unsigned char value)
{
    unsigned short length = 0;
    
    if(stop_update_flag == ENABLE)
        return SUCCESS;
    
    length = set_wifi_uart_byte(length,dpid);
    length = set_wifi_uart_byte(length,DP_TYPE_BOOL);
    //
    length = set_wifi_uart_byte(length,0);
    length = set_wifi_uart_byte(length,1);
    //
    if(value == FALSE) {
        length = set_wifi_uart_byte(length,FALSE);
    }else {
        length = set_wifi_uart_byte(length,1);
    }
    
    wifi_uart_write_frame(STATE_UPLOAD_CMD,length);
    
    return SUCCESS;
}

/**
 * @brief  value��dp�����ϴ�
 * @param[in] {dpid} dpid��
 * @param[in] {value} ��ǰdpֵ
 * @return Null
 * @note   Null
 */
unsigned char mcu_dp_value_update(unsigned char dpid,unsigned long value)
{
    unsigned short length = 0;
    
    if(stop_update_flag == ENABLE)
        return SUCCESS;
    
    length = set_wifi_uart_byte(length,dpid);
    length = set_wifi_uart_byte(length,DP_TYPE_VALUE);
    //
    length = set_wifi_uart_byte(length,0);
    length = set_wifi_uart_byte(length,4);
    //
    length = set_wifi_uart_byte(length,value >> 24);
    length = set_wifi_uart_byte(length,value >> 16);
    length = set_wifi_uart_byte(length,value >> 8);
    length = set_wifi_uart_byte(length,value & 0xff);
    
    wifi_uart_write_frame(STATE_UPLOAD_CMD,length);
    
    return SUCCESS;
}

/**
 * @brief  string��dp�����ϴ�
 * @param[in] {dpid} dpid��
 * @param[in] {value} ��ǰdpֵָ��
 * @param[in] {len} ���ݳ���
 * @return Null
 * @note   Null
 */
unsigned char mcu_dp_string_update(unsigned char dpid,const unsigned char value[],unsigned short len)
{
    unsigned short length = 0;
    
    if(stop_update_flag == ENABLE)
        return SUCCESS;
    //
    length = set_wifi_uart_byte(length,dpid);
    length = set_wifi_uart_byte(length,DP_TYPE_STRING);
    //
    length = set_wifi_uart_byte(length,len / 0x100);
    length = set_wifi_uart_byte(length,len % 0x100);
    //
    length = set_wifi_uart_buffer(length,(unsigned char *)value,len);
    
    wifi_uart_write_frame(STATE_UPLOAD_CMD,length);
    
    return SUCCESS;
}

/**
 * @brief  enum��dp�����ϴ�
 * @param[in] {dpid} dpid��
 * @param[in] {value} ��ǰdpֵ
 * @return Null
 * @note   Null
 */
unsigned char mcu_dp_enum_update(unsigned char dpid,unsigned char value)
{
    unsigned short length = 0;
    
    if(stop_update_flag == ENABLE)
        return SUCCESS;
    
    length = set_wifi_uart_byte(length,dpid);
    length = set_wifi_uart_byte(length,DP_TYPE_ENUM);
    //
    length = set_wifi_uart_byte(length,0);
    length = set_wifi_uart_byte(length,1);
    //
    length = set_wifi_uart_byte(length,value);
    
    wifi_uart_write_frame(STATE_UPLOAD_CMD,length);
    
    return SUCCESS;
}

/**
 * @brief  fault��dp�����ϴ�
 * @param[in] {dpid} dpid��
 * @param[in] {value} ��ǰdpֵ
 * @return Null
 * @note   Null
 */
unsigned char mcu_dp_fault_update(unsigned char dpid,unsigned long value)
{
    unsigned short length = 0;
     
    if(stop_update_flag == ENABLE)
        return SUCCESS;
    
    length = set_wifi_uart_byte(length,dpid);
    length = set_wifi_uart_byte(length,DP_TYPE_BITMAP);
    //
    length = set_wifi_uart_byte(length,0);
    
    if((value | 0xff) == 0xff) {
        length = set_wifi_uart_byte(length,1);
        length = set_wifi_uart_byte(length,value);
    }else if((value | 0xffff) == 0xffff) {
        length = set_wifi_uart_byte(length,2);
        length = set_wifi_uart_byte(length,value >> 8);
        length = set_wifi_uart_byte(length,value & 0xff);
    }else {
        length = set_wifi_uart_byte(length,4);
        length = set_wifi_uart_byte(length,value >> 24);
        length = set_wifi_uart_byte(length,value >> 16);
        length = set_wifi_uart_byte(length,value >> 8);
        length = set_wifi_uart_byte(length,value & 0xff);
    }    
    
    wifi_uart_write_frame(STATE_UPLOAD_CMD,length);

    return SUCCESS;
}

/**
 * @brief  mcu��ȡbool���·�dpֵ
 * @param[in] {value} dp���ݻ�������ַ
 * @param[in] {len} dp���ݳ���
 * @return ��ǰdpֵ
 * @note   Null
 */
unsigned char mcu_get_dp_download_bool(const unsigned char value[],unsigned short len)
{
    return(value[0]);
}

/**
 * @brief  mcu��ȡenum���·�dpֵ
 * @param[in] {value} dp���ݻ�������ַ
 * @param[in] {len} dp���ݳ���
 * @return ��ǰdpֵ
 * @note   Null
 */
unsigned char mcu_get_dp_download_enum(const unsigned char value[],unsigned short len)
{
    return(value[0]);
}

/**
 * @brief  mcu��ȡvalue���·�dpֵ
 * @param[in] {value} dp���ݻ�������ַ
 * @param[in] {len} dp���ݳ���
 * @return ��ǰdpֵ
 * @note   Null
 */
unsigned long mcu_get_dp_download_value(const unsigned char value[],unsigned short len)
{
    return(byte_to_int(value));
}

/**
 * @brief  ���ڽ��������ݴ洦��
 * @param[in] {value} �����յ���1�ֽ�����
 * @return Null
 * @note   ��MCU���ڴ������е��øú���,�������յ���������Ϊ��������
 */
void uart_receive_input(unsigned char value)
{
    //#error "���ڴ��ڽ����ж��е���uart_receive_input(value),����������MCU_SDK����,�û����������д���,��ɺ�ɾ������" 
    if(1 == queue_out - queue_in) {
        //���ݶ�����
    }else if((queue_in > queue_out) && ((queue_in - queue_out) >= sizeof(wifi_uart_rx_buf))) {
        //���ݶ�����
    }else {
        //���в���
        if(queue_in >= (unsigned char *)(wifi_uart_rx_buf + sizeof(wifi_uart_rx_buf))) {
            queue_in = (unsigned char *)(wifi_uart_rx_buf);
        }
        
        *queue_in ++ = value;
    }
}

/**
 * @brief  wifi�������ݴ������
 * @param  Null
 * @return Null
 * @note   ��MCU������whileѭ���е��øú���
 */
void wifi_uart_service(void)
{
    //#error "��ֱ����main������while(1){}�����wifi_uart_service(),���øú�����Ҫ���κ������ж�,��ɺ�ɾ������"
    static unsigned short rx_in = 0;
    unsigned short offset = 0;
    unsigned short rx_value_len = 0;             //����֡����
    
    while((rx_in < sizeof(wifi_data_process_buf)) && get_queue_total_data() > 0) {
        wifi_data_process_buf[rx_in ++] = Queue_Read_Byte();
    }
    
    if(rx_in < PROTOCOL_HEAD)
        return;
    
    while((rx_in - offset) >= PROTOCOL_HEAD) {
        if(wifi_data_process_buf[offset + HEAD_FIRST] != FRAME_FIRST) {
            offset ++;
            continue;
        }
        
        if(wifi_data_process_buf[offset + HEAD_SECOND] != FRAME_SECOND) {
            offset ++;
            continue;
        }  
        
        if(wifi_data_process_buf[offset + PROTOCOL_VERSION] != MCU_RX_VER) {
            offset += 2;
            continue;
        }      
        
        rx_value_len = wifi_data_process_buf[offset + LENGTH_HIGH] * 0x100 + wifi_data_process_buf[offset + LENGTH_LOW] + PROTOCOL_HEAD;
        if(rx_value_len > sizeof(wifi_data_process_buf) + PROTOCOL_HEAD) {
            offset += 3;
            continue;
        }
        
        if((rx_in - offset) < rx_value_len) {
            break;
        }
        
        //���ݽ������
        if(get_check_sum((unsigned char *)wifi_data_process_buf + offset,rx_value_len - 1) != wifi_data_process_buf[offset + rx_value_len - 1]) {
            //У�����
            //printf("crc error (crc:0x%X  but data:0x%X)\r\n",get_check_sum((unsigned char *)wifi_data_process_buf + offset,rx_value_len - 1),wifi_data_process_buf[offset + rx_value_len - 1]);
            offset += 3;
            continue;
        }
        
        data_handle(offset);
        offset += rx_value_len;
    }//end while

    rx_in -= offset;
    if(rx_in > 0) {
        my_memcpy(wifi_data_process_buf,wifi_data_process_buf + offset,rx_in);
    }
}

/**
 * @brief  Э�鴮�ڳ�ʼ������
 * @param  Null
 * @return Null
 * @note   ��MCU��ʼ�������е��øú���
 */
void wifi_protocol_init(void)
{
    //#error " ����main���������wifi_protocol_init()���wifiЭ���ʼ��,��ɾ������"
    queue_in = (unsigned char *)wifi_uart_rx_buf;
    queue_out = (unsigned char *)wifi_uart_rx_buf;
    
    stop_update_flag = DISABLE;
    
#ifndef WIFI_CONTROL_SELF_MODE
    wifi_work_state = WIFI_SATE_UNKNOW;
#endif
}


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
unsigned char dp_record_combine_update(unsigned char time[], t_DP_NODE dp_node[], unsigned char dp_node_num)
{
    unsigned short length = 0;
    unsigned char i = 0;
    unsigned char dp_type;
    
    if(stop_update_flag == ENABLE)
        return ERROR;
    
    //local_time
    length = set_wifi_uart_buffer(length,(unsigned char *)time,7);
    
    for(i = 0; i < dp_node_num; i++) {
        if(SUCCESS != get_dp_type(dp_node[i].dp_id, &dp_type)) {
            //�ڴ������ʾ��Ϣ��dpid�����ڣ�����
            continue;
        }
        
        length = set_wifi_uart_byte(length, dp_node[i].dp_id);
        length = set_wifi_uart_byte(length, dp_type);
        
        switch(dp_type) {
            case DP_TYPE_RAW:{
                length = set_wifi_uart_byte(length, (dp_node[i].dp_len >> 8) & 0xff);
                length = set_wifi_uart_byte(length, dp_node[i].dp_len & 0xff);
                length = set_wifi_uart_buffer(length, dp_node[i].dp_raw_val ,dp_node[i].dp_len);
            }
            break;
            
            case DP_TYPE_BOOL:{
                length = set_wifi_uart_byte(length, 0);
                length = set_wifi_uart_byte(length, 1);
                length = set_wifi_uart_byte(length, dp_node[i].dp_bool_val);
            }
            break;
            
            case DP_TYPE_VALUE:{
                length = set_wifi_uart_byte(length, 0);
                length = set_wifi_uart_byte(length, 4);
                length = set_wifi_uart_byte(length, (dp_node[i].dp_value_val >> 24) & 0xff);
                length = set_wifi_uart_byte(length, (dp_node[i].dp_value_val >> 16) & 0xff);
                length = set_wifi_uart_byte(length, (dp_node[i].dp_value_val >> 8) & 0xff);
                length = set_wifi_uart_byte(length, dp_node[i].dp_value_val & 0xff);
            }
            break;
            
            case DP_TYPE_STRING:{
                length = set_wifi_uart_byte(length, (dp_node[i].dp_len >> 8) & 0xff);
                length = set_wifi_uart_byte(length, dp_node[i].dp_len & 0xff);
                length = set_wifi_uart_buffer(length, dp_node[i].dp_str_val ,dp_node[i].dp_len);
            }
            break;
            
            case DP_TYPE_ENUM:{
                length = set_wifi_uart_byte(length, 0);
                length = set_wifi_uart_byte(length, 1);
                length = set_wifi_uart_byte(length, dp_node[i].dp_enum_val);
            }
            break;
            
            case DP_TYPE_BITMAP:{
                length = set_wifi_uart_byte(length, 0);

                if(1 == dp_node[i].dp_len) {
                    length = set_wifi_uart_byte(length, dp_node[i].dp_len);
                    length = set_wifi_uart_byte(length, dp_node[i].dp_fault_bitmap);
                }else if(2 == dp_node[i].dp_len) {
                    length = set_wifi_uart_byte(length, dp_node[i].dp_len);
                    length = set_wifi_uart_byte(length, (dp_node[i].dp_fault_bitmap >> 8) & 0xff);
                    length = set_wifi_uart_byte(length, dp_node[i].dp_fault_bitmap & 0xff);
                }else {
                    length = set_wifi_uart_byte(length, 4);
                    length = set_wifi_uart_byte(length, (dp_node[i].dp_fault_bitmap >> 24) & 0xff);
                    length = set_wifi_uart_byte(length, (dp_node[i].dp_fault_bitmap >> 16) & 0xff);
                    length = set_wifi_uart_byte(length, (dp_node[i].dp_fault_bitmap >> 8) & 0xff);
                    length = set_wifi_uart_byte(length, dp_node[i].dp_fault_bitmap & 0xff);
                }
            }
            break;
            
            default:break;
        }
    }
    
    wifi_uart_write_frame(STATE_RC_UPLOAD_CMD,length);
    
    return SUCCESS;
}

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
unsigned char mcu_get_reset_wifi_flag(void)
{
    return reset_wifi_flag;
}

/**
 * @brief  MCU��������wifi����ģʽ
 * @param  Null
 * @return Null
 * @note   1:MCU��������,ͨ��mcu_get_reset_wifi_flag()������ȡ����wifi�Ƿ�ɹ�
 *         2:���Ϊģ���Դ���ģʽ,MCU������øú���
 */
void mcu_reset_wifi(void)
{
    reset_wifi_flag = RESET_WIFI_ERROR;
    
    wifi_uart_write_frame(WIFI_RESET_CMD, 0);
}

/**
 * @brief  ��ȡ����wifi״̬�ɹ���־
 * @param  Null
 * @return wifimode flag
 * -           0(SET_WIFICONFIG_ERROR):ʧ��
 * -           1(SET_WIFICONFIG_SUCCESS):�ɹ�
 * @note   1:MCU��������mcu_set_wifi_mode()����øú�����ȡ��λ״̬
 *         2:���Ϊģ���Դ���ģʽ,MCU������øú���
 */
unsigned char mcu_get_wifimode_flag(void)
{
    return set_wifimode_flag;
}

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
void mcu_set_wifi_mode(unsigned char mode)
{
    unsigned char length = 0;
    
    set_wifimode_flag = SET_WIFICONFIG_ERROR;
    
    length = set_wifi_uart_byte(length, mode);
    
    wifi_uart_write_frame(WIFI_MODE_CMD, length);
}

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
unsigned char mcu_get_wifi_work_state(void)
{
    return wifi_work_state;
}
#endif

#ifdef SUPPORT_MCU_RTC_CHECK
/**
 * @brief  MCU��ȡ����ʱ��,����У�Ա���ʱ��
 * @param  Null
 * @return Null
 * @note   MCU����������ɺ��� mcu_write_rtctime ������У��rtcʱ��
 */
void mcu_get_system_time(void)
{
    wifi_uart_write_frame(GET_LOCAL_TIME_CMD,0);
}
#endif

#ifdef SUPPORT_GREEN_TIME
/**
 * @brief  MCU��ȡ����ʱ��,����У�Ա���ʱ��
 * @param  Null
 * @return Null
 * @note   MCU����������ɺ��� mcu_write_gltime �����ڼ�¼���������ʱ�䣬
           ����������ʱ���У��
 */
void mcu_get_gelin_time(void)
{
    wifi_uart_write_frame(GET_GL_TIME_CMD,0);
}
#endif

#ifdef WIFI_TEST_ENABLE
/**
 * @brief  mcu����wifi���ܲ���
 * @param  Null
 * @return Null
 * @note   MCU��Ҫ���е��øù���
 */
void mcu_start_wifitest(void)
{
    wifi_uart_write_frame(WIFI_TEST_CMD,0);
}
#endif

/**
 * @brief  MCU����wifi�̼�����
 * @param  Null
 * @return Null
 * @note   MCU����������ɺ��� wifi_update_handle �����ڿɻ�ȡ������ǰ״̬
 */
void wifi_update_request(void)
{ 
    wifi_uart_write_frame(WIFI_UG_REQ_CMD, 0);
}

#ifdef DP_CACHE_SUPPORT
/**
 * @brief  ��ȡdp����ָ��
 * @param[in] {table} dp������û���Ҫ��ѯ��dp������û���Ҫ��ѯ����dp����tableֱ����NULL����
 * @param[in] {dp_num} ��Ҫ��ѯ��dp������,�����Ҫ��ѯ����dp����dp_num��0����
 * @return Null
 * @note   MCU��Ҫ���е��øù���
 */
void get_dp_cache(unsigned char *table,unsigned char dp_num)
{
    unsigned char length = 0;
    
    length = set_wifi_uart_byte(length,dp_num);
    if(table != NULL) {
        length = set_wifi_uart_buffer(length,table, dp_num);
    }
    wifi_uart_write_frame(GET_DP_CACHE_CMD,length);
}
#endif

#ifdef REPORTED_MCU_SN_ENABLE
/**
 * @brief  MCU�ϱ�SN
 * @param[in] {sn} SN��
 * @param[in] {sn_len} SN�ų���
 * @return Null
 * @note   MCU��Ҫ���е��ú󣬿��� mcu_sn_updata_result �����жԽ�����д���
 */
void mcu_sn_updata(unsigned char sn[],unsigned char sn_len)
{
    unsigned char length = 0;
    
    length = set_wifi_uart_byte(length,sn_len);
    length = set_wifi_uart_buffer(length,sn,sn_len);
    
    wifi_uart_write_frame(REPORTED_MCU_SN_CMD,length);
}
#endif


