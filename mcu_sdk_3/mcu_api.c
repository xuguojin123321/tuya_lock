/**********************************Copyright (c)**********************************
**                       版权所有 (C), 2015-2020, 涂鸦科技
**
**                             http://www.tuya.com
**
*********************************************************************************/
/**
 * @file    mcu_api.c
 * @author  涂鸦综合协议开发组
 * @version v1.0.7
 * @date    2020.11.9
 * @brief   用户需要主动调用的函数都在该文件内
 */


#define MCU_API_GLOBAL

#include "wifi.h"

extern const DOWNLOAD_CMD_S download_cmd[];

/**
 * @brief  hex转bcd
 * @param[in] {Value_H} 高字节
 * @param[in] {Value_L} 低字节
 * @return 转换完成后数据
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
 * @param[in] {str} 字符串地址
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
 * @brief  把src所指内存区域的前count个字节设置成字符c
 * @param[out] {src} 待设置的内存首地址
 * @param[in] {ch} 设置的字符
 * @param[in] {count} 设置的内存长度
 * @return 待设置的内存首地址
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
 * @param[out] {dest} 目标地址
 * @param[in] {src} 源地址
 * @param[in] {count} 拷贝数据个数
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
 * @brief  字符串拷贝
 * @param[in] {dest} 目标地址
 * @param[in] {src} 源地址
 * @return 数据处理完后的源地址
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
 * @brief  字符串比较
 * @param[in] {s1} 字符串 1
 * @param[in] {s2} 字符串 2
 * @return 大小比较值
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
 * @param[in] {number} 4字节原数据
 * @param[out] {value} 处理完成后4字节数据
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
 * @brief  将4字节合并为1个32bit变量
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
 * @param[in] {dpid} dpid号
 * @param[in] {value} 当前dp值指针
 * @param[in] {len} 数据长度
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
 * @brief  bool型dp数据上传
 * @param[in] {dpid} dpid号
 * @param[in] {value} 当前dp值
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
 * @brief  value型dp数据上传
 * @param[in] {dpid} dpid号
 * @param[in] {value} 当前dp值
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
 * @brief  string型dp数据上传
 * @param[in] {dpid} dpid号
 * @param[in] {value} 当前dp值指针
 * @param[in] {len} 数据长度
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
 * @brief  enum型dp数据上传
 * @param[in] {dpid} dpid号
 * @param[in] {value} 当前dp值
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
 * @brief  fault型dp数据上传
 * @param[in] {dpid} dpid号
 * @param[in] {value} 当前dp值
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
 * @brief  mcu获取bool型下发dp值
 * @param[in] {value} dp数据缓冲区地址
 * @param[in] {len} dp数据长度
 * @return 当前dp值
 * @note   Null
 */
unsigned char mcu_get_dp_download_bool(const unsigned char value[],unsigned short len)
{
    return(value[0]);
}

/**
 * @brief  mcu获取enum型下发dp值
 * @param[in] {value} dp数据缓冲区地址
 * @param[in] {len} dp数据长度
 * @return 当前dp值
 * @note   Null
 */
unsigned char mcu_get_dp_download_enum(const unsigned char value[],unsigned short len)
{
    return(value[0]);
}

/**
 * @brief  mcu获取value型下发dp值
 * @param[in] {value} dp数据缓冲区地址
 * @param[in] {len} dp数据长度
 * @return 当前dp值
 * @note   Null
 */
unsigned long mcu_get_dp_download_value(const unsigned char value[],unsigned short len)
{
    return(byte_to_int(value));
}

/**
 * @brief  串口接收数据暂存处理
 * @param[in] {value} 串口收到的1字节数据
 * @return Null
 * @note   在MCU串口处理函数中调用该函数,并将接收到的数据作为参数传入
 */
void uart_receive_input(unsigned char value)
{
    //#error "请在串口接收中断中调用uart_receive_input(value),串口数据由MCU_SDK处理,用户请勿再另行处理,完成后删除该行" 
    if(1 == queue_out - queue_in) {
        //数据队列满
    }else if((queue_in > queue_out) && ((queue_in - queue_out) >= sizeof(wifi_uart_rx_buf))) {
        //数据队列满
    }else {
        //队列不满
        if(queue_in >= (unsigned char *)(wifi_uart_rx_buf + sizeof(wifi_uart_rx_buf))) {
            queue_in = (unsigned char *)(wifi_uart_rx_buf);
        }
        
        *queue_in ++ = value;
    }
}

/**
 * @brief  wifi串口数据处理服务
 * @param  Null
 * @return Null
 * @note   在MCU主函数while循环中调用该函数
 */
void wifi_uart_service(void)
{
    //#error "请直接在main函数的while(1){}中添加wifi_uart_service(),调用该函数不要加任何条件判断,完成后删除该行"
    static unsigned short rx_in = 0;
    unsigned short offset = 0;
    unsigned short rx_value_len = 0;             //数据帧长度
    
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
        my_memcpy(wifi_data_process_buf,wifi_data_process_buf + offset,rx_in);
    }
}

/**
 * @brief  协议串口初始化函数
 * @param  Null
 * @return Null
 * @note   在MCU初始化代码中调用该函数
 */
void wifi_protocol_init(void)
{
    //#error " 请在main函数中添加wifi_protocol_init()完成wifi协议初始化,并删除该行"
    queue_in = (unsigned char *)wifi_uart_rx_buf;
    queue_out = (unsigned char *)wifi_uart_rx_buf;
    
    stop_update_flag = DISABLE;
    
#ifndef WIFI_CONTROL_SELF_MODE
    wifi_work_state = WIFI_SATE_UNKNOW;
#endif
}


/**
 * @brief  记录型数据组合上报
 * @param[in] {time} 时间数据长度7，首字节表示是否传输标志位，其余依次为年、月、日、时、分、秒
 * @param[in] {dp_node} dp数据结构，需要在调用此函数之前，创建一个此结构体类型的数组，数组长度为dp个数，
                                    将记录型dp数据缓存到此结构体中，dp值需要根据dp的数据类型，存入到对应类型的结构体成员中
 * @param[in] {dp_node_num} dp个数
 * @return 结果
 * -           0(ERROR): 失败
 * -           1(SUCCESS): 成功
 * @note   MCU需要自行调用该函数
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
            //在此添加提示信息：dpid不存在，请检查
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
 * @brief  MCU获取复位wifi成功标志
 * @param  Null
 * @return 复位标志
 * -           0(RESET_WIFI_ERROR):失败
 * -           1(RESET_WIFI_SUCCESS):成功
 * @note   1:MCU主动调用mcu_reset_wifi()后调用该函数获取复位状态
 *         2:如果为模块自处理模式,MCU无须调用该函数
 */
unsigned char mcu_get_reset_wifi_flag(void)
{
    return reset_wifi_flag;
}

/**
 * @brief  MCU主动重置wifi工作模式
 * @param  Null
 * @return Null
 * @note   1:MCU主动调用,通过mcu_get_reset_wifi_flag()函数获取重置wifi是否成功
 *         2:如果为模块自处理模式,MCU无须调用该函数
 */
void mcu_reset_wifi(void)
{
    reset_wifi_flag = RESET_WIFI_ERROR;
    
    wifi_uart_write_frame(WIFI_RESET_CMD, 0);
}

/**
 * @brief  获取设置wifi状态成功标志
 * @param  Null
 * @return wifimode flag
 * -           0(SET_WIFICONFIG_ERROR):失败
 * -           1(SET_WIFICONFIG_SUCCESS):成功
 * @note   1:MCU主动调用mcu_set_wifi_mode()后调用该函数获取复位状态
 *         2:如果为模块自处理模式,MCU无须调用该函数
 */
unsigned char mcu_get_wifimode_flag(void)
{
    return set_wifimode_flag;
}

/**
 * @brief  MCU设置wifi工作模式
 * @param[in] {mode} 进入的模式
 * @ref        0(SMART_CONFIG):进入smartconfig模式
 * @ref        1(AP_CONFIG):进入AP模式
 * @return Null
 * @note   1:MCU主动调用
 *         2:成功后,可判断set_wifi_config_state是否为TRUE;TRUE表示为设置wifi工作模式成功
 *         3:如果为模块自处理模式,MCU无须调用该函数
 */
void mcu_set_wifi_mode(unsigned char mode)
{
    unsigned char length = 0;
    
    set_wifimode_flag = SET_WIFICONFIG_ERROR;
    
    length = set_wifi_uart_byte(length, mode);
    
    wifi_uart_write_frame(WIFI_MODE_CMD, length);
}

/**
 * @brief  MCU主动获取当前wifi工作状态
 * @param  Null
 * @return wifi work state
 * -          SMART_CONFIG_STATE: smartconfig配置状态
 * -          AP_STATE: AP配置状态
 * -          WIFI_NOT_CONNECTED: WIFI配置成功但未连上路由器
 * -          WIFI_CONNECTED: WIFI配置成功且连上路由器
 * -          WIFI_CONN_CLOUD: WIFI已经连接上云服务器
 * -          WIFI_LOW_POWER: WIFI处于低功耗模式
 * -          SMART_AND_AP_STATE: WIFI smartconfig&AP 模式
 * @note   如果为模块自处理模式,MCU无须调用该函数
 */
unsigned char mcu_get_wifi_work_state(void)
{
    return wifi_work_state;
}
#endif

#ifdef SUPPORT_MCU_RTC_CHECK
/**
 * @brief  MCU获取本地时间,用于校对本地时钟
 * @param  Null
 * @return Null
 * @note   MCU主动调用完成后在 mcu_write_rtctime 函数内校对rtc时钟
 */
void mcu_get_system_time(void)
{
    wifi_uart_write_frame(GET_LOCAL_TIME_CMD,0);
}
#endif

#ifdef SUPPORT_GREEN_TIME
/**
 * @brief  MCU获取格林时间,用于校对本地时钟
 * @param  Null
 * @return Null
 * @note   MCU主动调用完成后在 mcu_write_gltime 函数内记录并计算格林时间，
           用于门锁类时间戳校验
 */
void mcu_get_gelin_time(void)
{
    wifi_uart_write_frame(GET_GL_TIME_CMD,0);
}
#endif

#ifdef WIFI_TEST_ENABLE
/**
 * @brief  mcu发起wifi功能测试
 * @param  Null
 * @return Null
 * @note   MCU需要自行调用该功能
 */
void mcu_start_wifitest(void)
{
    wifi_uart_write_frame(WIFI_TEST_CMD,0);
}
#endif

/**
 * @brief  MCU请求wifi固件升级
 * @param  Null
 * @return Null
 * @note   MCU主动调用完成后在 wifi_update_handle 函数内可获取升级当前状态
 */
void wifi_update_request(void)
{ 
    wifi_uart_write_frame(WIFI_UG_REQ_CMD, 0);
}

#ifdef DP_CACHE_SUPPORT
/**
 * @brief  获取dp缓存指令
 * @param[in] {table} dp表，存放用户需要查询的dp，如果用户需要查询所有dp，则table直接填NULL即可
 * @param[in] {dp_num} 需要查询的dp的数量,如果需要查询所有dp，则dp_num填0即可
 * @return Null
 * @note   MCU需要自行调用该功能
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
 * @brief  MCU上报SN
 * @param[in] {sn} SN号
 * @param[in] {sn_len} SN号长度
 * @return Null
 * @note   MCU需要自行调用后，可在 mcu_sn_updata_result 函数中对结果进行处理
 */
void mcu_sn_updata(unsigned char sn[],unsigned char sn_len)
{
    unsigned char length = 0;
    
    length = set_wifi_uart_byte(length,sn_len);
    length = set_wifi_uart_buffer(length,sn,sn_len);
    
    wifi_uart_write_frame(REPORTED_MCU_SN_CMD,length);
}
#endif


