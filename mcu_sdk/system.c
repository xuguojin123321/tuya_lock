/**********************************Copyright (c)**********************************
**                       版权所有 (C), 2015-2020, 涂鸦科技
**
**                             http://www.tuya.com
**
*********************************************************************************/
/**
 * @file    system.c
 * @author  涂鸦综合协议开发组
 * @version v1.0.7
 * @date    2020.11.9
 * @brief   串口数据处理，用户无需关心该文件实现内容
 */


#define SYSTEM_GLOBAL

#include "wifi.h"
#include "protocol.h"
//
//
extern const DOWNLOAD_CMD_S download_cmd[];

/*设备能力选择*/
/*请在此处选择需要开启的设备的能力，ON为开启，OFF为关闭*/
tCAP_equip cap_equip = {
#ifndef PICTURE_UPLOAD_ENABLE
    OFF,        //设备不支持 拍照/图片传输 功能
#else
    ON,         //设备支持 拍照/图片传输 功能
#endif
#ifndef CAP_COMMU_MODE_ENABLE
    OFF,        //设备采用串口通信方式上传图片
#else
    ON,         //设备采用SPI通信方式上传图片
#endif
    OFF,        //设备是否为运营商门锁？ OFF:表示不支持, ON:表示支持 （暂不支持此功能）
#ifndef WIFI_RESET_NOTICE_ENABLE
    OFF,        //设备不支持模块重置状态通知
#else
    ON,         //设备支持模块重置状态通知
#endif
    OFF,        //预留
    OFF,        //预留
    OFF,        //预留
    OFF         //预留
};

/**
 * @brief  写wifi_uart字节
 * @param[in] {dest} 缓存区其实地址
 * @param[in] {byte} 写入字节值
 * @return 写入完成后的总长度
 */
unsigned short set_wifi_uart_byte(unsigned short dest, unsigned char byte)
{
    unsigned char *obj = (unsigned char *)wifi_uart_tx_buf + DATA_START + dest;
    
    *obj = byte;
    dest += 1;
    
    return dest;
}

/**
 * @brief  写wifi_uart_buffer
 * @param[in] {dest} 目标地址
 * @param[in] {src} 源地址
 * @param[in] {len} 数据长度
 * @return 写入结束的缓存地址
 */
unsigned short set_wifi_uart_buffer(unsigned short dest, unsigned char *src, unsigned short len)
{
    unsigned char *obj = (unsigned char *)wifi_uart_tx_buf + DATA_START + dest;
    
    my_memcpy(obj,src,len);
    
    dest += len;
    return dest;
}

/**
 * @brief  计算校验和
 * @param[in] {pack} 数据源指针
 * @param[in] {pack_len} 计算校验和长度
 * @return 校验和
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
 * @brief  串口发送一段数据
 * @param[in] {in} 发送缓存指针
 * @param[in] {len} 数据发送长度
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
 * @brief  向wifi串口发送一帧数据
 * @param[in] {fr_type} 帧类型
 * @param[in] {fr_ver} 帧版本
 * @param[in] {len} 数据长度
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
 * @brief  产品信息上传
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
 * @brief  获取制定DPID在数组中的序号
 * @param[in] {dpid} dpid
 * @return dp序号
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
 * @brief  获取指定DPID在数组中的序号
 * @param[in] {dpid} dpid
 * @param[out] {p_dp_type} dp 类型
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
 * @brief  下发数据处理
 * @param[in] {value} 下发数据源指针
 * @return 返回数据处理结果
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
        //错误提示
        return FALSE;
    }else {
        ret = dp_download_handle(dp_id,value + 4,dp_len);
    }
    
    return ret;
}

/**
 * @brief  数据帧处理
 * @param[in] {offset} 数据起始位
 * @return Null
 */
void data_handle(unsigned short offset)
{
#ifdef SUPPORT_MCU_FIRM_UPDATE
    unsigned char *firmware_addr = NULL;
    static unsigned long firm_length = 0;                 //MCU升级文件长度
    static unsigned char firm_update_flag;                //MCU升级标志
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
        case PRODUCT_INFO_CMD:                                  //产品信息
            product_info_update();
        break;
        
#ifndef WIFI_CONTROL_SELF_MODE
        case WIFI_STATE_CMD:                                    //wifi工作状态	
            wifi_work_state = wifi_data_process_buf[offset + DATA_START];
            wifi_uart_write_frame(WIFI_STATE_CMD,0);
        break;

        case WIFI_RESET_CMD:                                    //重置wifi(wifi返回成功)
            reset_wifi_flag = RESET_WIFI_SUCCESS;
        break;
          
        case WIFI_MODE_CMD:                                     //选择smartconfig/AP模式(wifi返回成功)	
            set_wifimode_flag = SET_WIFICONFIG_SUCCESS;
        break;
#endif
      
        case DATA_QUERT_CMD:                                    //命令下发
            total_len = wifi_data_process_buf[offset + LENGTH_HIGH] * 0x100;
            total_len += wifi_data_process_buf[offset + LENGTH_LOW];
            
            for(i = 0;i < total_len; ) {
                dp_len = wifi_data_process_buf[offset + DATA_START + i + 2] * 0x100;
                dp_len += wifi_data_process_buf[offset + DATA_START + i + 3];
                //
                ret = data_point_handle((unsigned char *)wifi_data_process_buf + offset + DATA_START + i);
                
                if(SUCCESS == ret) {
                  //成功提示
                }else {
                  //错误提示
                }
                
                i += (dp_len + 4);
            }
        break;
          
#ifdef SUPPORT_MCU_FIRM_UPDATE
        case MCU_UG_REQ_CMD:                                    //请求升级
            mcu_update_handle(wifi_data_process_buf[offset + DATA_START]);
        break;
        
        case UPDATE_START_CMD:                                  //升级开始
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
          
        case UPDATE_TRANS_CMD:                                  //升级传输
            if(firm_update_flag == UPDATE_START_CMD) {
                //停止一切数据上报
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
                    //最后一包
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
                //恢复一切数据上报
                stop_update_flag = DISABLE;    
            }
        break;
#endif      

#ifdef SUPPORT_MCU_RTC_CHECK
        case GET_LOCAL_TIME_CMD:                              //获取本地时间
            mcu_write_rtctime(wifi_data_process_buf + offset + DATA_START);
        break;
#endif
   
#ifdef WIFI_TEST_ENABLE
        case WIFI_TEST_CMD:                                     //wifi功能测试
            wifi_test_result(wifi_data_process_buf[offset + DATA_START],wifi_data_process_buf[offset + DATA_START + 1]);
        break;
#endif

        case WIFI_UG_REQ_CMD:                                   //请求WIFI模块固件升级
            wifi_update_handle(wifi_data_process_buf[offset + DATA_START]);
        break;

#ifdef LOCK_API_ENABLE
        case GET_GL_TIME_CMD:                                   //获取格林时间
            mcu_write_gltime(wifi_data_process_buf + offset + DATA_START);
        break;

        case TEMP_PASS_CMD:                                     //请求云端临时密码（只支持单组）
            if (wifi_data_process_buf[offset + DATA_START] == 1) {
                total_len = wifi_data_process_buf[offset + LENGTH_HIGH] * 0x100;
                total_len += wifi_data_process_buf[offset + LENGTH_LOW];

                temp_pass_handle(wifi_data_process_buf[offset + DATA_START], wifi_data_process_buf + offset + DATA_START + 1,
                                 wifi_data_process_buf + offset + DATA_START + 7, total_len - 7);
            }else {
                temp_pass_handle(wifi_data_process_buf[offset + DATA_START], 0, 0, 0);
            }
        break;

        case PASS_CHECK_CMD:                                    //动态密码校验
            pass_check_handle(wifi_data_process_buf[offset + DATA_START]);
        break;

        case MUL_TEMP_PASS_CMD:                                 //请求云端临时密码（支持多组）
            mul_temp_pass_handle(wifi_data_process_buf + offset + DATA_START);
        break;
        
        case SCHEDULE_TEMP_PASS_CMD:                            //请求云端临时密码（带schedule列表）
            schedule_temp_pass_handle(wifi_data_process_buf + offset + DATA_START);
        break;
#endif

#ifdef DP_CACHE_SUPPORT
        case GET_DP_CACHE_CMD:                                  //获取dp缓存指令
            total_len = wifi_data_process_buf[offset + LENGTH_HIGH] * 0x100;//后面的总字节数
            total_len += wifi_data_process_buf[offset + LENGTH_LOW];
            
            result = wifi_data_process_buf[offset + DATA_START]; //获取指令成功标志
            if(result == 0) {     //获取指令失败
                //用户自行实现获取指令失败后的操作
              
            }else {               //获取指令成功
                dp_num = wifi_data_process_buf[offset + DATA_START + 1];//获取到的dp数量
                
                for(i = 2;i < total_len;) {
                    dp_len = wifi_data_process_buf[offset + DATA_START + i + 2] * 0x100; //value的字节数
                    dp_len += wifi_data_process_buf[offset + DATA_START + i + 3];

                    //处理状态数据单元
                    ret = data_point_handle((unsigned char *)wifi_data_process_buf + offset + DATA_START + i);
                    
                    if(SUCCESS == ret) {
                        //成功提示
                    }else {
                        //错误提示
                    }
                    
                    i += (dp_len + 4);  //dp_len(value) + len(2) + type(1) + dpid(1)
                }
            }
        break;
#endif

#ifdef OFFLINE_DYN_PW_ENABLE
        case OFFLINE_DYN_PW_CMD:                                //离线动态密码
            offline_dynamic_password_result(wifi_data_process_buf+offset + DATA_START);
        break;
#endif
    
#ifdef REPORTED_MCU_SN_ENABLE
        case REPORTED_MCU_SN_CMD:                               //上报MCU的SN
            mcu_sn_updata_result(wifi_data_process_buf[offset + DATA_START]);
        break;
#endif
        
#ifdef LOCK_KEEP_ALIVE
        case GET_WIFI_STATE_CMD:                                //获取WiFi状态
            get_wifi_state_result((unsigned char *)wifi_data_process_buf + offset + DATA_START);
        break;
#endif
        
#ifdef WIFI_RESET_NOTICE_ENABLE
        case WIFI_RESET_NOTICE_CMD:                             //模块重置状态通知
            wifi_reset_notice(wifi_data_process_buf[offset + DATA_START]);
        break;
#endif

#ifdef PICTURE_UPLOAD_ENABLE
        case PICTURE_EVENT_STATE_CMD:                           //事件状态通知
            picture_event_state_notice_result(wifi_data_process_buf[offset + DATA_START]);
        break;
        
        case PICTURE_UPLOAD_CMD:                                //图片上传
            picture_upload_result(wifi_data_process_buf[offset + DATA_START]);
        break;
        
        case PICTURE_UPLOAD_RETURN_CMD:                         //图片上传结果反馈
            total_len = wifi_data_process_buf[offset + LENGTH_HIGH] * 0x100;
            total_len += wifi_data_process_buf[offset + LENGTH_LOW];
            picture_upload_return(wifi_data_process_buf + offset + DATA_START, total_len);
        break;
        
        case PICTURE_UPLOAD_STATE_GET_CMD:                      //图片上传状态获取
            total_len = wifi_data_process_buf[offset + LENGTH_HIGH] * 0x100;
            total_len += wifi_data_process_buf[offset + LENGTH_LOW];
            picture_upload_state_get_result(wifi_data_process_buf + offset + DATA_START, total_len);
        break;
#endif
        
#ifdef PHOTO_LOCK_PICTURE_UPLOAD_ENABLE
        case PHOTO_LOCK_PICTURE_UPLOAD_CMD:                      //拍照门锁图片上传相关功能
            photo_lock_picture_upload_func(wifi_data_process_buf + offset + DATA_START);
        break;
#endif
        
        
        
        default:
        break;
    }
}

/**
 * @brief  判断串口接收缓存中是否有数据
 * @param  Null
 * @return 是否有数据
 */
unsigned char get_queue_total_data(void)
{
    if(queue_in != queue_out)
        return 1;
    else
        return 0;
}

/**
 * @brief  读取队列1字节数据
 * @param  Null
 * @return Read the data
 */
unsigned char Queue_Read_Byte(void)
{
    unsigned char value;
    
    if(queue_out != queue_in) {
        //有数据
        if(queue_out >= (unsigned char *)(wifi_uart_rx_buf + sizeof(wifi_uart_rx_buf))) {
            //数据已经到末尾
            queue_out = (unsigned char *)(wifi_uart_rx_buf);
        }
        
        value = *queue_out++;
    }
    
    return value;
}

