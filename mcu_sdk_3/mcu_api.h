/**********************************Copyright (c)**********************************
**                       版权所有 (C), 2015-2020, 涂鸦科技
**
**                             http://www.tuya.com
**
*********************************************************************************/
/**
 * @file    mcu_api.h
 * @author  涂鸦综合协议开发组
 * @version v1.0.7
 * @date    2020.11.9
 * @brief   用户需要主动调用的函数都在该文件内
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
 * @brief  hex转bcd
 * @param[in] {Value_H} 高字节
 * @param[in] {Value_L} 低字节
 * @return 转换完成后数据
 */
unsigned char hex_to_bcd(unsigned char Value_H,unsigned char Value_L);

/**
 * @brief  求字符串长度
 * @param[in] {str} 字符串地址
 * @return 数据长度
 */
unsigned long my_strlen(unsigned char *str);

/**
 * @brief  把src所指内存区域的前count个字节设置成字符c
 * @param[out] {src} 待设置的内存首地址
 * @param[in] {ch} 设置的字符
 * @param[in] {count} 设置的内存长度
 * @return 待设置的内存首地址
 */
void *my_memset(void *src,unsigned char ch,unsigned short count);

/**
 * @brief  内存拷贝
 * @param[out] {dest} 目标地址
 * @param[in] {src} 源地址
 * @param[in] {count} 拷贝数据个数
 * @return 数据处理完后的源地址
 */
void *my_memcpy(void *dest, const void *src, unsigned short count);

/**
 * @brief  字符串拷贝
 * @param[in] {dest} 目标地址
 * @param[in] {src} 源地址
 * @return 数据处理完后的源地址
 */
char *my_strcpy(char *dest, const char *src);

/**
 * @brief  字符串比较
 * @param[in] {s1} 字符串 1
 * @param[in] {s2} 字符串 2
 * @return 大小比较值
 * -         0:s1=s2
 * -         <0:s1<s2
 * -         >0:s1>s2
 */
int my_strcmp(char *s1 , char *s2);

/**
 * @brief  将int类型拆分四个字节
 * @param[in] {number} 4字节原数据
 * @param[out] {value} 处理完成后4字节数据
 * @return Null
 */
void int_to_byte(unsigned long number,unsigned char value[4]);

/**
 * @brief  将4字节合并为1个32bit变量
 * @param[in] {value} 4字节数组
 * @return 合并完成后的32bit变量
 */
unsigned long byte_to_int(const unsigned char value[4]);


/**
 * @brief  raw型dp数据上传
 * @param[in] {dpid} dpid号
 * @param[in] {value} 当前dp值指针
 * @param[in] {len} 数据长度
 * @return Null
 * @note   Null
 */
unsigned char mcu_dp_raw_update(unsigned char dpid,const unsigned char value[],unsigned short len);

/**
 * @brief  bool型dp数据上传
 * @param[in] {dpid} dpid号
 * @param[in] {value} 当前dp值
 * @return Null
 * @note   Null
 */
unsigned char mcu_dp_bool_update(unsigned char dpid,unsigned char value);

/**
 * @brief  value型dp数据上传
 * @param[in] {dpid} dpid号
 * @param[in] {value} 当前dp值
 * @return Null
 * @note   Null
 */
unsigned char mcu_dp_value_update(unsigned char dpid,unsigned long value);

/**
 * @brief  string型dp数据上传
 * @param[in] {dpid} dpid号
 * @param[in] {value} 当前dp值指针
 * @param[in] {len} 数据长度
 * @return Null
 * @note   Null
 */
unsigned char mcu_dp_string_update(unsigned char dpid,const unsigned char value[],unsigned short len);

/**
 * @brief  enum型dp数据上传
 * @param[in] {dpid} dpid号
 * @param[in] {value} 当前dp值
 * @return Null
 * @note   Null
 */
unsigned char mcu_dp_enum_update(unsigned char dpid,unsigned char value);

/**
 * @brief  fault型dp数据上传
 * @param[in] {dpid} dpid号
 * @param[in] {value} 当前dp值
 * @return Null
 * @note   Null
 */
unsigned char mcu_dp_fault_update(unsigned char dpid,unsigned long value);

/**
 * @brief  mcu获取bool型下发dp值
 * @param[in] {value} dp数据缓冲区地址
 * @param[in] {len} dp数据长度
 * @return 当前dp值
 * @note   Null
 */
unsigned char mcu_get_dp_download_bool(const unsigned char value[],unsigned short len);

/**
 * @brief  mcu获取enum型下发dp值
 * @param[in] {value} dp数据缓冲区地址
 * @param[in] {len} dp数据长度
 * @return 当前dp值
 * @note   Null
 */
unsigned char mcu_get_dp_download_enum(const unsigned char value[],unsigned short len);

/**
 * @brief  mcu获取value型下发dp值
 * @param[in] {value} dp数据缓冲区地址
 * @param[in] {len} dp数据长度
 * @return 当前dp值
 * @note   Null
 */
unsigned long mcu_get_dp_download_value(const unsigned char value[],unsigned short len);

/**
 * @brief  串口接收数据暂存处理
 * @param[in] {value} 串口收到的1字节数据
 * @return Null
 * @note   在MCU串口处理函数中调用该函数,并将接收到的数据作为参数传入
 */
void uart_receive_input(unsigned char value);

/**
 * @brief  wifi串口数据处理服务
 * @param  Null
 * @return Null
 * @note   在MCU主函数while循环中调用该函数
 */
void wifi_uart_service(void);

/**
 * @brief  协议串口初始化函数
 * @param  Null
 * @return Null
 * @note   在MCU初始化代码中调用该函数
 */
void wifi_protocol_init(void);


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
unsigned char dp_record_combine_update(unsigned char time[], t_DP_NODE dp_node[], unsigned char dp_node_num);


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
unsigned char mcu_get_reset_wifi_flag(void);

/**
 * @brief  MCU主动重置wifi工作模式
 * @param  Null
 * @return Null
 * @note   1:MCU主动调用,通过mcu_get_reset_wifi_flag()函数获取重置wifi是否成功
 *         2:如果为模块自处理模式,MCU无须调用该函数
 */
void mcu_reset_wifi(void);

/**
 * @brief  获取设置wifi状态成功标志
 * @param  Null
 * @return wifimode flag
 * -           0(SET_WIFICONFIG_ERROR):失败
 * -           1(SET_WIFICONFIG_SUCCESS):成功
 * @note   1:MCU主动调用mcu_set_wifi_mode()后调用该函数获取复位状态
 *         2:如果为模块自处理模式,MCU无须调用该函数
 */
unsigned char mcu_get_wifimode_flag(void);

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
void mcu_set_wifi_mode(unsigned char mode);

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
unsigned char mcu_get_wifi_work_state(void);
#endif

#ifdef SUPPORT_MCU_RTC_CHECK
/**
 * @brief  MCU获取本地时间,用于校对本地时钟
 * @param  Null
 * @return Null
 * @note   MCU主动调用完成后在 mcu_write_rtctime 函数内校对rtc时钟
 */
void mcu_get_system_time(void);
#endif

#ifdef SUPPORT_GREEN_TIME
/**
 * @brief  MCU获取格林时间,用于校对本地时钟
 * @param  Null
 * @return Null
 * @note   MCU主动调用完成后在 mcu_write_gltime 函数内记录并计算格林时间，
           用于门锁类时间戳校验
 */
void mcu_get_gelin_time(void);
#endif

#ifdef WIFI_TEST_ENABLE
/**
 * @brief  mcu发起wifi功能测试
 * @param  Null
 * @return Null
 * @note   MCU需要自行调用该功能
 */
void mcu_start_wifitest(void);
#endif

/**
 * @brief  MCU请求wifi固件升级
 * @param  Null
 * @return Null
 * @note   MCU主动调用完成后在 wifi_update_handle 函数内可获取升级当前状态
 */
void wifi_update_request(void);

#ifdef DP_CACHE_SUPPORT
/**
 * @brief  获取dp缓存指令
 * @param[in] {table} dp表，存放用户需要查询的dp，如果用户需要查询所有dp，则table直接填NULL即可
 * @param[in] {dp_num} 需要查询的dp的数量,如果需要查询所有dp，则dp_num填0即可
 * @return Null
 * @note   MCU需要自行调用该功能
 */
void get_dp_cache(unsigned char *table,unsigned char dp_num);
#endif

#ifdef REPORTED_MCU_SN_ENABLE
/**
 * @brief  MCU上报SN
 * @param[in] {sn} SN号
 * @param[in] {sn_len} SN号长度
 * @return Null
 * @note   MCU需要自行调用后，可在 mcu_sn_updata_result 函数中对结果进行处理
 */
void mcu_sn_updata(unsigned char sn[],unsigned char sn_len);
#endif



#endif
