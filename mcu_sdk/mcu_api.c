/**********************************Copyright (c)**********************************
**                       版权所�? (C), 2015-2020, 涂鸦科技
**
**                             http://www.tuya.com
**
*********************************************************************************/
/**
 * @file    mcu_api.c
 * @author  涂鸦综合协�??开发组
 * @version v2.5.6
 * @date    2020.12.16
 * @brief   用户需要主动调用的函数都在该文件内
 */

/****************************** 免责声明 ！！�? *******************************
由于MCU类型和编译环境�?��?��?�样，所以�?�代码仅供参考，用户请自行把控最终代码质量，
涂鸦不�?�MCU功能结果负责�?
******************************************************************************/

#define MCU_API_GLOBAL

#include "wifi.h"

/**
 * @brief  hex转bcd
 * @param[in] {Value_H} 高字�?
 * @param[in] {Value_L} 低字�?
 * @return �?换完成后数据
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
 * @brief  求字符串长度
 * @param[in] {str} 字�?�串地址
 * @return 数据长度
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
 * @brief  把src所指内存区域的前count�?字节设置成字�?c
 * @param[out] {src} 待�?�置的内存�?�地址
 * @param[in] {ch} 设置的字�?
 * @param[in] {count} 设置的内存长�?
 * @return 待�?�置的内存�?�地址
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
 * @brief  内存拷贝
 * @param[out] {dest} �?标地址
 * @param[in] {src} 源地址
 * @param[in] {count} 拷贝数据�?�?
 * @return 数据处理完后的源地址
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
 * @brief  字�?�串拷贝
 * @param[in] {dest} �?标地址
 * @param[in] {src} 源地址
 * @return 数据处理完后的源地址
 */
char *my_strcpy(char *dest, const char *src)  
{
	char *p = NULL;
    if((NULL == dest) || (NULL == src)) {
        return NULL;
    }
    
    p = dest;
    while(*src!='\0') {
        *dest++ = *src++;
    }
    *dest = '\0';
    return p;
}

/**
 * @brief  字�?�串比较
 * @param[in] {s1} 字�?�串 1
 * @param[in] {s2} 字�?�串 2
 * @return 大小比较�?
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
 * @brief  将int类型拆分四个字节
 * @param[in] {number} 4字节原数�?
 * @param[out] {value} 处理完成�?4字节数据
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
 * @brief  �?4字节合并�?1�?32bit变量
 * @param[in] {value} 4字节数组
 * @return 合并完成后的32bit变量
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
 * @brief  raw型dp数据上传
 * @param[in] {dpid} dpid�?
 * @param[in] {value} 当前dp值指�?
 * @param[in] {len} 数据长度
 * @return Null
 * @note   Null
 */
unsigned char mcu_dp_raw_update(unsigned char dpid,const unsigned char value[],unsigned short len)
{
    unsigned short send_len = 0;
    
    if(stop_update_flag == ENABLE)
        return SUCCESS;
    //
    send_len = set_wifi_uart_byte(send_len,dpid);
    send_len = set_wifi_uart_byte(send_len,DP_TYPE_RAW);
    //
    send_len = set_wifi_uart_byte(send_len,len / 0x100);
    send_len = set_wifi_uart_byte(send_len,len % 0x100);
    //
    send_len = set_wifi_uart_buffer(send_len,(unsigned char *)value,len);
    
    wifi_uart_write_frame(STATE_UPLOAD_CMD,send_len);
    
    return SUCCESS;
}

/**
 * @brief  bool型dp数据上传
 * @param[in] {dpid} dpid�?
 * @param[in] {value} 当前dp�?
 * @return Null
 * @note   Null
 */
unsigned char mcu_dp_bool_update(unsigned char dpid,unsigned char value)
{
    unsigned short send_len = 0;
    
    if(stop_update_flag == ENABLE)
        return SUCCESS;
    
    send_len = set_wifi_uart_byte(send_len,dpid);
    send_len = set_wifi_uart_byte(send_len,DP_TYPE_BOOL);
    //
    send_len = set_wifi_uart_byte(send_len,0);
    send_len = set_wifi_uart_byte(send_len,1);
    //
    if(value == FALSE) {
        send_len = set_wifi_uart_byte(send_len,FALSE);
    }else {
        send_len = set_wifi_uart_byte(send_len,1);
    }
    
    wifi_uart_write_frame(STATE_UPLOAD_CMD,  send_len);
    
    return SUCCESS;
}

/**
 * @brief  value型dp数据上传
 * @param[in] {dpid} dpid�?
 * @param[in] {value} 当前dp�?
 * @return Null
 * @note   Null
 */
unsigned char mcu_dp_value_update(unsigned char dpid,unsigned long value)
{
    unsigned short send_len = 0;
    
    if(stop_update_flag == ENABLE)
        return SUCCESS;
    
    send_len = set_wifi_uart_byte(send_len,dpid);
    send_len = set_wifi_uart_byte(send_len,DP_TYPE_VALUE);
    //
    send_len = set_wifi_uart_byte(send_len,0);
    send_len = set_wifi_uart_byte(send_len,4);
    //
    send_len = set_wifi_uart_byte(send_len,value >> 24);
    send_len = set_wifi_uart_byte(send_len,value >> 16);
    send_len = set_wifi_uart_byte(send_len,value >> 8);
    send_len = set_wifi_uart_byte(send_len,value & 0xff);
    
    wifi_uart_write_frame(STATE_UPLOAD_CMD,send_len);
    
    return SUCCESS;
}

/**
 * @brief  string型dp数据上传
 * @param[in] {dpid} dpid�?
 * @param[in] {value} 当前dp值指�?
 * @param[in] {len} 数据长度
 * @return Null
 * @note   Null
 */
unsigned char mcu_dp_string_update(unsigned char dpid,const unsigned char value[],unsigned short len)
{
    unsigned short send_len = 0;
    
    if(stop_update_flag == ENABLE)
        return SUCCESS;
    //
    send_len = set_wifi_uart_byte(send_len,dpid);
    send_len = set_wifi_uart_byte(send_len,DP_TYPE_STRING);
    //
    send_len = set_wifi_uart_byte(send_len,len / 0x100);
    send_len = set_wifi_uart_byte(send_len,len % 0x100);
    //
    send_len = set_wifi_uart_buffer(send_len,(unsigned char *)value,len);
    
    wifi_uart_write_frame(STATE_UPLOAD_CMD,send_len);
    
    return SUCCESS;
}

/**
 * @brief  enum型dp数据上传
 * @param[in] {dpid} dpid�?
 * @param[in] {value} 当前dp�?
 * @return Null
 * @note   Null
 */
unsigned char mcu_dp_enum_update(unsigned char dpid,unsigned char value)
{
    unsigned short send_len = 0;
    
    if(stop_update_flag == ENABLE)
        return SUCCESS;
    
    send_len = set_wifi_uart_byte(send_len,dpid);
    send_len = set_wifi_uart_byte(send_len,DP_TYPE_ENUM);
    //
    send_len = set_wifi_uart_byte(send_len,0);
    send_len = set_wifi_uart_byte(send_len,1);
    //
    send_len = set_wifi_uart_byte(send_len,value);
    
    wifi_uart_write_frame(STATE_UPLOAD_CMD,send_len);
    
    return SUCCESS;
}

/**
 * @brief  fault型dp数据上传
 * @param[in] {dpid} dpid�?
 * @param[in] {value} 当前dp�?
 * @return Null
 * @note   Null
 */
unsigned char mcu_dp_fault_update(unsigned char dpid,unsigned long value)
{
    unsigned short send_len = 0;
     
    if(stop_update_flag == ENABLE)
        return SUCCESS;
    
    send_len = set_wifi_uart_byte(send_len,dpid);
    send_len = set_wifi_uart_byte(send_len,DP_TYPE_BITMAP);
    //
    send_len = set_wifi_uart_byte(send_len,0);
    
    if((value | 0xff) == 0xff) {
        send_len = set_wifi_uart_byte(send_len,1);
        send_len = set_wifi_uart_byte(send_len,value);
    }else if((value | 0xffff) == 0xffff) {
        send_len = set_wifi_uart_byte(send_len,2);
        send_len = set_wifi_uart_byte(send_len,value >> 8);
        send_len = set_wifi_uart_byte(send_len,value & 0xff);
    }else {
        send_len = set_wifi_uart_byte(send_len,4);
        send_len = set_wifi_uart_byte(send_len,value >> 24);
        send_len = set_wifi_uart_byte(send_len,value >> 16);
        send_len = set_wifi_uart_byte(send_len,value >> 8);
        send_len = set_wifi_uart_byte(send_len,value & 0xff);
    }    
    
    wifi_uart_write_frame(STATE_UPLOAD_CMD,  send_len);

    return SUCCESS;
}

#ifdef MCU_DP_UPLOAD_SYN
/**
 * @brief  raw型dp数据同�?�上�?
 * @param[in] {dpid} dpid�?
 * @param[in] {value} 当前dp值指�?
 * @param[in] {len} 数据长度
 * @return Null
 * @note   Null
 */
unsigned char mcu_dp_raw_update_syn(unsigned char dpid,const unsigned char value[],unsigned short len)
{
    unsigned short send_len = 0;
    
    if(stop_update_flag == ENABLE)
        return SUCCESS;
    //
    send_len = set_wifi_uart_byte(send_len,dpid);
    send_len = set_wifi_uart_byte(send_len,DP_TYPE_RAW);
    //
    send_len = set_wifi_uart_byte(send_len,len / 0x100);
    send_len = set_wifi_uart_byte(send_len,len % 0x100);
    //
    send_len = set_wifi_uart_buffer(send_len,(unsigned char *)value,len);
    
    wifi_uart_write_frame(STATE_UPLOAD_SYN_CMD,send_len);
    
    return SUCCESS;
}

/**
 * @brief  bool型dp数据同�?�上�?
 * @param[in] {dpid} dpid�?
 * @param[in] {value} 当前dp值指�?
 * @return Null
 * @note   Null
 */
unsigned char mcu_dp_bool_update_syn(unsigned char dpid,unsigned char value)
{
    unsigned short send_len = 0;
    
    if(stop_update_flag == ENABLE)
        return SUCCESS;
    
    send_len = set_wifi_uart_byte(send_len,dpid);
    send_len = set_wifi_uart_byte(send_len,DP_TYPE_BOOL);
    //
    send_len = set_wifi_uart_byte(send_len,0);
    send_len = set_wifi_uart_byte(send_len,1);
    //
    if(value == FALSE) {
        send_len = set_wifi_uart_byte(send_len,FALSE);
    }else {
        send_len = set_wifi_uart_byte(send_len,1);
    }
    
    wifi_uart_write_frame(STATE_UPLOAD_SYN_CMD,  send_len);
    
    return SUCCESS;
}

/**
 * @brief  value型dp数据同�?�上�?
 * @param[in] {dpid} dpid�?
 * @param[in] {value} 当前dp值指�?
 * @return Null
 * @note   Null
 */
unsigned char mcu_dp_value_update_syn(unsigned char dpid,unsigned long value)
{
    unsigned short send_len = 0;
    
    if(stop_update_flag == ENABLE)
        return SUCCESS;
    
    send_len = set_wifi_uart_byte(send_len,dpid);
    send_len = set_wifi_uart_byte(send_len,DP_TYPE_VALUE);
    //
    send_len = set_wifi_uart_byte(send_len,0);
    send_len = set_wifi_uart_byte(send_len,4);
    //
    send_len = set_wifi_uart_byte(send_len,value >> 24);
    send_len = set_wifi_uart_byte(send_len,value >> 16);
    send_len = set_wifi_uart_byte(send_len,value >> 8);
    send_len = set_wifi_uart_byte(send_len,value & 0xff);
    
    wifi_uart_write_frame(STATE_UPLOAD_SYN_CMD,  send_len);
    
    return SUCCESS;
}

/**
 * @brief  string型dp数据同�?�上�?
 * @param[in] {dpid} dpid�?
 * @param[in] {value} 当前dp值指�?
 * @param[in] {len} 数据长度
 * @return Null
 * @note   Null
 */
unsigned char mcu_dp_string_update_syn(unsigned char dpid,const unsigned char value[],unsigned short len)
{
    unsigned short send_len = 0;
    
    if(stop_update_flag == ENABLE)
        return SUCCESS;
    //
    send_len = set_wifi_uart_byte(send_len,dpid);
    send_len = set_wifi_uart_byte(send_len,DP_TYPE_STRING);
    //
    send_len = set_wifi_uart_byte(send_len,len / 0x100);
    send_len = set_wifi_uart_byte(send_len,len % 0x100);
    //
    send_len = set_wifi_uart_buffer(send_len,(unsigned char *)value,len);
    
    wifi_uart_write_frame(STATE_UPLOAD_SYN_CMD,send_len);
    
    return SUCCESS;
}

/**
 * @brief  enum型dp数据同�?�上�?
 * @param[in] {dpid} dpid�?
 * @param[in] {value} 当前dp值指�?
 * @return Null
 * @note   Null
 */
unsigned char mcu_dp_enum_update_syn(unsigned char dpid,unsigned char value)
{
    unsigned short send_len = 0;
    
    if(stop_update_flag == ENABLE)
        return SUCCESS;
    
    send_len = set_wifi_uart_byte(send_len,dpid);
    send_len = set_wifi_uart_byte(send_len,DP_TYPE_ENUM);
    //
    send_len = set_wifi_uart_byte(send_len,0);
    send_len = set_wifi_uart_byte(send_len,1);
    
    send_len = set_wifi_uart_byte(send_len,value);
    
    wifi_uart_write_frame(STATE_UPLOAD_SYN_CMD,send_len);
    
    return SUCCESS;
}

/**
 * @brief  fault型dp数据同�?�上�?
 * @param[in] {dpid} dpid�?
 * @param[in] {value} 当前dp值指�?
 * @return Null
 * @note   Null
 */
unsigned char mcu_dp_fault_update_syn(unsigned char dpid,unsigned long value)
{
    unsigned short send_len = 0;
     
    if(stop_update_flag == ENABLE)
        return SUCCESS;
    
    send_len = set_wifi_uart_byte(send_len,dpid);
    send_len = set_wifi_uart_byte(send_len,DP_TYPE_BITMAP);
    //
    send_len = set_wifi_uart_byte(send_len,0);
    
    if((value | 0xff) == 0xff) {
        send_len = set_wifi_uart_byte(send_len,1);
        send_len = set_wifi_uart_byte(send_len,value);
    }else if((value | 0xffff) == 0xffff) {
        send_len = set_wifi_uart_byte(send_len,2);
        send_len = set_wifi_uart_byte(send_len,value >> 8);
        send_len = set_wifi_uart_byte(send_len,value & 0xff);
    }else {
        send_len = set_wifi_uart_byte(send_len,4);
        send_len = set_wifi_uart_byte(send_len,value >> 24);
        send_len = set_wifi_uart_byte(send_len,value >> 16);
        send_len = set_wifi_uart_byte(send_len,value >> 8);
        send_len = set_wifi_uart_byte(send_len,value & 0xff);
    }    
    
    wifi_uart_write_frame(STATE_UPLOAD_SYN_CMD,send_len);

    return SUCCESS;
}
#endif

/**
 * @brief  mcu获取bool型下发dp�?
 * @param[in] {value} dp数据缓冲区地址
 * @param[in] {len} dp数据长度
 * @return 当前dp�?
 * @note   Null
 */
unsigned char mcu_get_dp_download_bool(const unsigned char value[],unsigned short len)
{
    return(value[0]);
}

/**
 * @brief  mcu获取enum型下发dp�?
 * @param[in] {value} dp数据缓冲区地址
 * @param[in] {len} dp数据长度
 * @return 当前dp�?
 * @note   Null
 */
unsigned char mcu_get_dp_download_enum(const unsigned char value[],unsigned short len)
{
    return(value[0]);
}

/**
 * @brief  mcu获取value型下发dp�?
 * @param[in] {value} dp数据缓冲区地址
 * @param[in] {len} dp数据长度
 * @return 当前dp�?
 * @note   Null
 */
unsigned long mcu_get_dp_download_value(const unsigned char value[],unsigned short len)
{
    return(byte_to_int(value));
}

/**
 * @brief  串口接收数据暂存处理
 * @param[in] {value} 串口收到�?1字节数据
 * @return Null
 * @note   在MCU串口处理函数�?调用该函�?,并将接收到的数据作为参数传入
 */
void uart_receive_input(unsigned char value)
{
//    #error "请在串口接收�?�?�?调用uart_receive_input(value),串口数据由MCU_SDK处理,用户请勿再另行�?�理,完成后删除�?��??" 
    
    if(1 == rx_buf_out - rx_buf_in) {
        //串口接收缓存已满
    }else if((rx_buf_in > rx_buf_out) && ((rx_buf_in - rx_buf_out) >= sizeof(wifi_uart_rx_buf))) {
        //串口接收缓存已满
    }else {
        //串口接收缓存�?�?
        if(rx_buf_in >= (unsigned char *)(wifi_uart_rx_buf + sizeof(wifi_uart_rx_buf))) {
            rx_buf_in = (unsigned char *)(wifi_uart_rx_buf);
        }
        
        *rx_buf_in ++ = value;
    }
}

/**
 * @brief  串口接收多个字节数据暂存处理
 * @param[in] {value} 串口要接收的数据的源地址
 * @param[in] {data_len} 串口要接收的数据的数�?长度
 * @return Null
 * @note   如需要支持一次�?�字节缓存，�?调用该函�?
 */
void uart_receive_buff_input(unsigned char value[], unsigned short data_len)
{
//    #error "请在需要一次缓存�?�个字节串口数据处调用�?�函�?,串口数据由MCU_SDK处理,用户请勿再另行�?�理,完成后删除�?��??" 
    
    unsigned short i = 0;
    for(i = 0; i < data_len; i++) {
        uart_receive_input(value[i]);
    }
}

/**
 * @brief  wifi串口数据处理服务
 * @param  Null
 * @return Null
 * @note   在MCU主函数while�?�?�?调用该函�?
 */
void wifi_uart_service(void)
{
//    #error "请直接在main函数的while(1){}�?添加wifi_uart_service(),调用该函数不要加任何条件判断,完成后删除�?��??" 
    static unsigned short rx_in = 0;
    unsigned short offset = 0;
    unsigned short rx_value_len = 0;
    
    while((rx_in < sizeof(wifi_data_process_buf)) && with_data_rxbuff() > 0) {
        wifi_data_process_buf[rx_in ++] = take_byte_rxbuff();
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
        
        rx_value_len = wifi_data_process_buf[offset + LENGTH_HIGH] * 0x100;
        rx_value_len += (wifi_data_process_buf[offset + LENGTH_LOW] + PROTOCOL_HEAD);
        if(rx_value_len > sizeof(wifi_data_process_buf) + PROTOCOL_HEAD) {
            offset += 3;
            continue;
        }
        
        if((rx_in - offset) < rx_value_len) {
            break;
        }
        
        //数据接收完成
        if(get_check_sum((unsigned char *)wifi_data_process_buf + offset,rx_value_len - 1) != wifi_data_process_buf[offset + rx_value_len - 1]) {
            //校验出错
            //printf("crc error (crc:0x%X  but data:0x%X)\r\n",get_check_sum((unsigned char *)wifi_data_process_buf + offset,rx_value_len - 1),wifi_data_process_buf[offset + rx_value_len - 1]);
            offset += 3;
            continue;
        }
        
        data_handle(offset);
        offset += rx_value_len;
    }//end while

    rx_in -= offset;
    if(rx_in > 0) {
        my_memcpy((char *)wifi_data_process_buf, (const char *)wifi_data_process_buf + offset, rx_in);
    }
}

/**
 * @brief  协�??串口初�?�化函数
 * @param  Null
 * @return Null
 * @note   在MCU初�?�化代码�?调用该函�?
 */
void wifi_protocol_init(void)
{
//    #error " 请在main函数�?添加wifi_protocol_init()完成wifi协�??初�?�化,并删除�?��??"
    rx_buf_in = (unsigned char *)wifi_uart_rx_buf;
    rx_buf_out = (unsigned char *)wifi_uart_rx_buf;
    
    stop_update_flag = DISABLE;
    
#ifndef WIFI_CONTROL_SELF_MODE
    wifi_work_state = WIFI_SATE_UNKNOW;
#endif
}



#ifndef WIFI_CONTROL_SELF_MODE
/**
 * @brief  MCU获取复位wifi成功标志
 * @param  Null
 * @return 复位标志
 * -           0(RESET_WIFI_ERROR):失败
 * -           1(RESET_WIFI_SUCCESS):成功
 * @note   1:MCU主动调用mcu_reset_wifi()后调用�?�函数获取�?�位状�?
 *         2:如果为模块自处理模式,MCU无须调用该函�?
 */
unsigned char mcu_get_reset_wifi_flag(void)
{
    return reset_wifi_flag;
}

/**
 * @brief  MCU主动重置wifi工作模式
 * @param  Null
 * @return Null
 * @note   1:MCU主动调用,通过mcu_get_reset_wifi_flag()函数获取重置wifi�?否成�?
 *         2:如果为模块自处理模式,MCU无须调用该函�?
 */
void mcu_reset_wifi(void)
{
    reset_wifi_flag = RESET_WIFI_ERROR;
    
    wifi_uart_write_frame(WIFI_RESET_CMD,  0);
}

/**
 * @brief  获取设置wifi状态成功标�?
 * @param  Null
 * @return wifimode flag
 * -           0(SET_WIFICONFIG_ERROR):失败
 * -           1(SET_WIFICONFIG_SUCCESS):成功
 * @note   1:MCU主动调用mcu_set_wifi_mode()后调用�?�函数获取�?�位状�?
 *         2:如果为模块自处理模式,MCU无须调用该函�?
 */
unsigned char mcu_get_wifimode_flag(void)
{
    return set_wifimode_flag;
}

/**
 * @brief  MCU设置wifi工作模式
 * @param[in] {mode} 进入的模�?
 * @ref        0(SMART_CONFIG):进入smartconfig模式
 * @ref        1(AP_CONFIG):进入AP模式
 * @return Null
 * @note   1:MCU主动调用
 *         2:成功�?,�?判断set_wifi_config_state�?否为TRUE;TRUE表示为�?�置wifi工作模式成功
 *         3:如果为模块自处理模式,MCU无须调用该函�?
 */
void mcu_set_wifi_mode(unsigned char mode)
{
    unsigned char length = 0;
    
    set_wifimode_flag = SET_WIFICONFIG_ERROR;
    
    length = set_wifi_uart_byte(length, mode);
    
    wifi_uart_write_frame(WIFI_MODE_CMD,  length);
}

/**
 * @brief  MCU主动获取当前wifi工作状�?
 * @param  Null
 * @return wifi work state
 * -          SMART_CONFIG_STATE: smartconfig配置状�?
 * -          AP_STATE: AP配置状�?
 * -          WIFI_NOT_CONNECTED: WIFI配置成功但未连上�?由器
 * -          WIFI_CONNECTED: WIFI配置成功且连上路由器
 * -          WIFI_CONN_CLOUD: WIFI已经连接上云服务�?
 * -          WIFI_LOW_POWER: WIFI处于低功耗模�?
 * -          SMART_AND_AP_STATE: WIFI smartconfig&AP 模式
 * @note   如果为模块自处理模式,MCU无须调用该函�?
 */
unsigned char mcu_get_wifi_work_state(void)
{
    return wifi_work_state;
}
#endif

#ifdef SUPPORT_GREEN_TIME
/**
 * @brief  MCU获取格林时间
 * @param  Null
 * @return Null
 * @note   MCU需要自行调用�?�功�?
 */
void mcu_get_green_time(void)
{
    wifi_uart_write_frame(GET_ONLINE_TIME_CMD,  0);
}
#endif

#ifdef SUPPORT_MCU_RTC_CHECK
/**
 * @brief  MCU获取系统时间,用于校�?�本地时�?
 * @param  Null
 * @return Null
 * @note   MCU主动调用完成后在mcu_write_rtctime函数内校对rtc时钟
 */
void mcu_get_system_time(void)
{
    wifi_uart_write_frame(GET_LOCAL_TIME_CMD,  0);
}
#endif

#ifdef WIFI_TEST_ENABLE
/**
 * @brief  mcu发起wifi功能测试
 * @param  Null
 * @return Null
 * @note   MCU需要自行调用�?�功�?
 */
void mcu_start_wifitest(void)
{
    wifi_uart_write_frame(WIFI_TEST_CMD,  0);
}
#endif

#ifdef WIFI_HEARTSTOP_ENABLE
/**
 * @brief  通知WIFI模组关闭心跳
 * @param  Null
 * @return Null
 * @note   MCU需要自行调用�?�功�?
 */
void wifi_heart_stop(void)
{
    wifi_uart_write_frame(HEAT_BEAT_STOP,  0);
}
#endif

#ifdef GET_WIFI_STATUS_ENABLE
/**
 * @brief  获取当前wifi联网状�?
 * @param  Null
 * @return Null
 * @note   MCU需要自行调用�?�功�?
 */
void mcu_get_wifi_connect_status(void)
{
    wifi_uart_write_frame(GET_WIFI_STATUS_CMD,  0);
}
#endif

#ifdef WIFI_STREAM_ENABLE
/**
 * @brief  流服务发�?
 * @param[in] {id} ID�?
 * @param[in] {buffer} 发送包的地址
 * @param[in] {buf_len} 发送包长度
 * @return 流服务传输结�?
 * -           0(ERROR): 失败
 * -           1(SUCCESS): 成功
 * @note   MCU需要自行实现�?�功�?
 */
unsigned char stream_trans_send(unsigned int id, unsigned char *buffer, unsigned long buf_len)
{
    unsigned long map_offset = 0;
    unsigned int pack_num = 0;
    unsigned int rest_length = 0;

    if(stop_update_flag == ENABLE)
        return ERROR;

    pack_num = buf_len / STREM_PACK_LEN;
    rest_length = buf_len - pack_num * STREM_PACK_LEN;
    if (rest_length > 0) {
        pack_num++;
    }

    int this_len = STREM_PACK_LEN;
    for (int cnt = 0; cnt < pack_num; cnt++,map_offset += this_len) {
        if (cnt == pack_num - 1 && rest_length > 0) {
            this_len = rest_length;
        }else {
            this_len = STREM_PACK_LEN;
        }

        if(ERROR == stream_trans(id, map_offset, buffer + map_offset, this_len)) {
            //mcu正在升级�?，不�?以进行流服务传输
            //printf("is upgrade\n");
            return ERROR;
        }
        
        #error "请根�?提示，自行完善流服务发送代码，完成后�?�删除�?��??"
        /*  如果MCU�?以使用�?�进程或多线程，�?以将此函数放在一�?单独的进程或线程里执行，并打开下面所示的代码  */
        /*
        while(0xff == stream_status); //等待模块回�??
        
        if(0x00 != stream_status) {
            //失败  �?�? stream_trans_send_result 函数�?查看错�??�?
            return ERROR;
        }
        */
        
        /*  如果MCU不支持�?�进程或多线程，使用此函数时每�?�只发一包（数据长度不大于STREM_PACK_LEN），
        发送完之后�? stream_trans_send_result 函数�?处理模块返回的结果，发送成功时再�?�调用�?�函数发送下一包，注意包偏移�?��?�加  */
    }
    
    return SUCCESS;
}

/**
 * @brief  多地图流服务发�?
 * @param[in] {id} 地图流服务会话ID
 * @param[in] {sub_id} 子地图ID
 * @param[in] {sub_id_pro_mode} 子地图ID数据处理方式
 * @ref       0x00: 继续�?�?
 * @ref       0x01: 清除上传的数�?
 * @param[in] {buffer} 数据包发送的地址
 * @param[in] {buf_len} 数据包发送的长度
 * @return 流服务传输结�?
 * -           0(ERROR): 失败
 * -           1(SUCCESS): 成功
 * @note   MCU需要自行实现�?�功�?
 */
unsigned char maps_stream_trans_send(unsigned int id, unsigned char sub_id, unsigned char sub_id_pro_mode, unsigned char *buffer, unsigned long buf_len)
{
    unsigned long map_offset = 0;
    unsigned int pack_num = 0;
    unsigned int rest_length = 0;
    unsigned char pro_ver = 0; //地图服务协�??版本 默�?�为0

    if(stop_update_flag == ENABLE)
        return SUCCESS;

    pack_num = buf_len / STREM_PACK_LEN;
    rest_length = buf_len - pack_num * STREM_PACK_LEN;
    if (rest_length > 0) {
        pack_num++;
    }

    int this_len = STREM_PACK_LEN;
    for (int cnt = 0; cnt < pack_num; cnt++,map_offset += this_len) {
        if (cnt == pack_num - 1 && rest_length > 0) {
            this_len = rest_length;
        }else {
            this_len = STREM_PACK_LEN;
        }

        if(ERROR == maps_stream_trans(pro_ver, id, sub_id, sub_id_pro_mode, map_offset, buffer + map_offset, this_len)) {
            //mcu正在升级�?，不�?以进行流服务传输
            //printf("is upgrade\n");
            return ERROR;
        }
        
        #error "请根�?提示，自行完善流服务发送代码，完成后�?�删除�?��??"
        /*  如果MCU�?以使用�?�进程或多线程，�?以将此函数放在一�?单独的进程或线程里执行，并打开下面所示的代码  */
        /*
        while(0xff == maps_stream_status); //等待模块回�??
        
        if(0x00 != maps_stream_status) {
            //失败  �?�? maps_stream_trans_send_result 函数�?查看错�??�?
            return ERROR;
        }
        */
        
        /*  如果MCU不支持�?�进程或多线程，使用此函数时每�?�只发一包（数据长度不大于STREM_PACK_LEN），
        发送完之后�? maps_stream_trans_send_result 函数�?处理模块返回的结果，发送成功时再�?�调用�?�函数发送下一包，注意包偏移�?��?�加  */
    }
    
    return SUCCESS;
}
#endif

#ifdef WIFI_CONNECT_TEST_ENABLE
/**
 * @brief  mcu发起wifi功能测试(连接指定�?�?)
 * @param[in] {ssid_buf} 存放�?由器名称字�?�串数据的地址(ssid长度最大支�?32�?字节)
 * @param[in] {passwd_buffer} 存放�?由器名称字�?�串数据的地址(passwd长度最大支�?64�?字节)
 * @return Null
 * @note   MCU需要自行调用�?�功�?
 */
void mcu_start_connect_wifitest(unsigned char *ssid_buf,unsigned char *passwd_buffer)
{
    unsigned short send_len = 0;

    if( my_strlen(ssid_buf) > 32 || my_strlen(passwd_buffer) > 64) {
        //printf("ssid_buf or passwd_buffer is too long!");
        return;
    }
    
    send_len = set_wifi_uart_buffer(send_len, "{\"ssid\":\"", my_strlen("{\"ssid\":\""));
    send_len = set_wifi_uart_buffer(send_len,ssid_buf,my_strlen(ssid_buf));
    send_len = set_wifi_uart_buffer(send_len, "\",\"password\":\"", my_strlen("\",\"password\":\""));
    send_len = set_wifi_uart_buffer(send_len,passwd_buffer,my_strlen(passwd_buffer));
    send_len = set_wifi_uart_buffer(send_len, "\"}", my_strlen("\"}"));

    wifi_uart_write_frame(WIFI_CONNECT_TEST_CMD,  send_len);
}
#endif

#ifdef GET_MODULE_MAC_ENABLE
/**
 * @brief  获取模块MAC
 * @param  Null
 * @return Null
 * @note   MCU需要自行调用�?�功�?
 */
void mcu_get_module_mac(void)
{
    wifi_uart_write_frame(GET_MAC_CMD,  0);
}
#endif

#ifdef IR_TX_RX_TEST_ENABLE
/**
 * @brief  mcu发起红�?�进入收发产�?
 * @param  Null
 * @return Null
 * @note   MCU需要自行调用�?�功�?
 */
void mcu_start_ir_test(void)
{
    wifi_uart_write_frame(IR_TX_RX_TEST_CMD,  0);
}
#endif

#ifdef MODULE_EXPANDING_SERVICE_ENABLE
/**
 * @brief  打开模块时间服务通知
 * @param[in] {time_type} 时间类型
 * @ref       0x00: 格林时间
 * @ref       0x01: �?地时�?
 * @return Null
 * @note   MCU需要自行调用�?�功�?
 */
void open_module_time_serve(unsigned char time_type)
{
    unsigned short send_len = 0;
    send_len = set_wifi_uart_byte(send_len, 0x01);
    send_len = set_wifi_uart_byte(send_len, time_type);
    wifi_uart_write_frame(MODULE_EXTEND_FUN_CMD,  send_len);
}

/**
 * @brief  主动请求天气服务数据
 * @param  Null
 * @return Null
 * @note   MCU需要自行调用�?�功�?
 */
void request_weather_serve(void)
{
    unsigned short send_len = 0;
    send_len = set_wifi_uart_byte(send_len, 0x03);
    wifi_uart_write_frame(MODULE_EXTEND_FUN_CMD,  send_len);
}

/**
 * @brief  打开模块重置状态通知
 * @param  Null
 * @return Null
 * @note   MCU需要自行调用�?�功�?
 */
void open_module_reset_state_serve(void)
{
    unsigned short send_len = 0;
    send_len = set_wifi_uart_byte(send_len, 0x04);
    wifi_uart_write_frame(MODULE_EXTEND_FUN_CMD,  send_len);
}
#endif

#ifdef BLE_RELATED_FUNCTION_ENABLE
/**
 * @brief  mcu发起蓝牙功能性测试（�?描指定蓝牙信标）
 * @param  Null
 * @return Null
 * @note   MCU需要自行调用�?�功�?
 */
void mcu_start_BLE_test(void)
{
    unsigned short send_len = 0;
    send_len = set_wifi_uart_byte(send_len, 0x01);
    wifi_uart_write_frame(BLE_TEST_CMD,  send_len);
}
#endif

#ifdef VOICE_MODULE_PROTOCOL_ENABLE
/**
 * @brief  获取�?音状态码
 * @param  Null
 * @return Null
 * @note   MCU需要自行调用�?�功�?
 */
void get_voice_state(void)
{
    wifi_uart_write_frame(GET_VOICE_STATE_CMD,  0);
}

/**
 * @brief  MIC静音设置
 * @param[in] {set_val} 静音设置�?
 * @ref       0x00: mic开�?
 * @ref       0x01: mic静音
 * @ref       0xA0: 查�?�静音状�?
 * @return Null
 * @note   MCU需要自行调用�?�功�?
 */
void set_voice_MIC_silence(unsigned char set_val)
{
    unsigned short send_len = 0;
    send_len = set_wifi_uart_byte(send_len, set_val);
    wifi_uart_write_frame(MIC_SILENCE_CMD,  send_len);
}

/**
 * @brief  speaker音量设置
 * @param[in] {set_val} 音量设置�?
 * @ref       0~10: 音量范围
 * @ref       0xA0: 查�?�音量�?
 * @return Null
 * @note   MCU需要自行调用�?�功�?
 */
void set_speaker_voice(unsigned char set_val)
{
    unsigned short send_len = 0;
    send_len = set_wifi_uart_byte(send_len, set_val);
    wifi_uart_write_frame(SET_SPEAKER_VOLUME_CMD,  send_len);
}

/**
 * @brief  音�?�产�?
 * @param[in] {set_val} 音�?�产测�?
 * @ref       0x00: 关闭音�?�产�?
 * @ref       0x01: mic1音�?�环�?测试
 * @ref       0x02: mic2音�?�环�?测试
 * @ref       0xA0: 查�?�当前产测状�?
 * @return Null
 * @note   MCU需要自行调用�?�功�?
 */
void voice_test(unsigned char test_val)
{
    unsigned short send_len = 0;
    send_len = set_wifi_uart_byte(send_len, test_val);
    wifi_uart_write_frame(VOICE_TEST_CMD,  send_len);
}

/**
 * @brief  唤醒产测
 * @param  Null
 * @return Null
 * @note   MCU需要自行调用�?�功�?
 */
void voice_awaken_test(void)
{
    wifi_uart_write_frame(VOICE_AWAKEN_TEST_CMD,  0);
}

/**
 * @brief  �?音模组MCU功能设置
 * @param[in] {play} �?�?/暂停功能 1(�?�?) / 0(暂停)
 * @param[in] {bt_play} 蓝牙开关功�? 1(开) / 0(�?)
 * @return Null
 * @note   MCU需要自行调用�?�功�?
 */
void voice_mcu_fun_set(unsigned char play, unsigned char bt_play)
{
    unsigned short send_len = 0;
    unsigned char str[50];
    
    //MCU设置暂仅�?持”播�?/暂停�? ”蓝牙开关�?
    
    sprintf((char*)str,"{\"play\":%s,\"bt_play\":%s}", play?"true":"false", bt_play?"true":"false");
    
    send_len = set_wifi_uart_byte(send_len, 0x00);
    send_len = set_wifi_uart_buffer(send_len, str, my_strlen(str));
    wifi_uart_write_frame(VOICE_EXTEND_FUN_CMD,  send_len);
}
#endif


