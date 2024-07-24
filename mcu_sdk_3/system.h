/**********************************Copyright (c)**********************************
**                       版权所有 (C), 2015-2020, 涂鸦科技
**
**                             http://www.tuya.com
**
*********************************************************************************/
/**
 * @file    system.h
 * @author  涂鸦综合协议开发组
 * @version v1.0.7
 * @date    2020.11.9
 * @brief   串口数据处理，用户无需关心该文件实现内容
 */


#ifndef __SYSTEM_H_
#define __SYSTEM_H_

#ifdef SYSTEM_GLOBAL
  #define SYSTEM_EXTERN
#else
  #define SYSTEM_EXTERN   extern
#endif

//=============================================================================
//帧的字节顺序
//=============================================================================
#define         HEAD_FIRST                      0
#define         HEAD_SECOND                     1        
#define         PROTOCOL_VERSION                2
#define         FRAME_TYPE                      3
#define         LENGTH_HIGH                     4
#define         LENGTH_LOW                      5
#define         DATA_START                      6

//=============================================================================
//数据帧类型
//=============================================================================
#define         PRODUCT_INFO_CMD                0x01                            //产品信息
#define         WIFI_STATE_CMD                  0x02                            //wifi工作状态	
#define         WIFI_RESET_CMD                  0x03                            //重置wifi
#define         WIFI_MODE_CMD                   0x04                            //选择smartconfig/AP模式
#ifdef STATE_FAST_UPLOAD_ENABLE
#define         STATE_UPLOAD_CMD                0x18                            //实时状态快速上报
#define         STATE_RC_UPLOAD_CMD             0x19                            //记录型状态快速上报
#else
#define         STATE_UPLOAD_CMD                0x05                            //实时状态上报
#define         STATE_RC_UPLOAD_CMD             0x08                            //记录型状态上报
#endif
#define         GET_LOCAL_TIME_CMD              0x06                            //获取本地时间
#define         WIFI_TEST_CMD                   0x07                            //wifi功能测试
#define         DATA_QUERT_CMD                  0x09                            //命令下发
#define         WIFI_UG_REQ_CMD                 0x0a                            //MCU请求WIFI模块固件升级
#define         ROUTE_RSSI_CMD                  0x0b                            //查询当前连接路由器信号强度
#define         MCU_UG_REQ_CMD                  0x0c                            //MCU请求MCU固件升级
#define         UPDATE_START_CMD                0x0d                            //MCU升级开始
#define         UPDATE_TRANS_CMD                0x0e                            //MCU升级传输
#define         GET_GL_TIME_CMD                 0x10                            //获取格林时间
#define         GET_DP_CACHE_CMD                0x15                            //获取dp缓存指令
#define         REPORTED_MCU_SN_CMD             0x17                            //上报MCU的SN
#ifdef LOCK_KEEP_ALIVE
#define         GET_WIFI_STATE_CMD              0x1A                            //获取WiFi状态
#endif
#define         WIFI_RESET_NOTICE_CMD           0x25                            //模块重置状态通知



//=============================================================================
#define MCU_RX_VER              0x00                                            //MCU接收协议版本号
#define MCU_TX_VER              0x00                                            //MCU发送协议版本号
#define PROTOCOL_HEAD           0x07                                            //固定协议头长度
#define FIRM_UPDATA_SIZE        256                                             //升级包大小
#define FRAME_FIRST             0x55
#define FRAME_SECOND            0xaa

//============================================================================= 
SYSTEM_EXTERN unsigned char wifi_data_process_buf[PROTOCOL_HEAD + WIFI_DATA_PROCESS_LMT];       //串口数据处理缓存
SYSTEM_EXTERN unsigned char volatile wifi_uart_rx_buf[PROTOCOL_HEAD + WIFI_UART_RECV_BUF_LMT];  //串口接收缓存
SYSTEM_EXTERN unsigned char wifi_uart_tx_buf[PROTOCOL_HEAD + WIFIR_UART_SEND_BUF_LMT];          //串口发送缓存
//
SYSTEM_EXTERN volatile unsigned char *queue_in;
SYSTEM_EXTERN volatile unsigned char *queue_out;

SYSTEM_EXTERN unsigned char stop_update_flag;

#ifndef WIFI_CONTROL_SELF_MODE
SYSTEM_EXTERN unsigned char reset_wifi_flag;                                                  //重置wifi标志(TRUE:成功/FALSE:失败)
SYSTEM_EXTERN unsigned char set_wifimode_flag;                                                //设置WIFI工作模式标志(TRUE:成功/FALSE:失败)
SYSTEM_EXTERN unsigned char wifi_work_state;                                                  //wifi模块当前工作状态
#endif


/**
 * @brief  写wifi_uart字节
 * @param[in] {dest} 缓存区其实地址
 * @param[in] {byte} 写入字节值
 * @return 写入完成后的总长度
 */
unsigned short set_wifi_uart_byte(unsigned short dest, unsigned char byte);

/**
 * @brief  写wifi_uart_buffer
 * @param[in] {dest} 目标地址
 * @param[in] {src} 源地址
 * @param[in] {len} 数据长度
 * @return 写入结束的缓存地址
 */
unsigned short set_wifi_uart_buffer(unsigned short dest, unsigned char *src, unsigned short len);

/**
 * @brief  计算校验和
 * @param[in] {pack} 数据源指针
 * @param[in] {pack_len} 计算校验和长度
 * @return 校验和
 */
unsigned char get_check_sum(unsigned char *pack, unsigned short pack_len);

/**
 * @brief  向wifi串口发送一帧数据
 * @param[in] {fr_type} 帧类型
 * @param[in] {fr_ver} 帧版本
 * @param[in] {len} 数据长度
 * @return Null
 */
void wifi_uart_write_frame(unsigned char fr_type, unsigned short len);

/**
 * @brief  获取指定DPID在数组中的序号
 * @param[in] {dpid} dpid
 * @param[out] {p_dp_type} dp 类型
 * @return SUCCESS/ERROR
 */
unsigned char get_dp_type(unsigned char dpid, unsigned char *p_dp_type);

/**
 * @brief  数据帧处理
 * @param[in] {offset} 数据起始位
 * @return Null
 */
void data_handle(unsigned short offset);

/**
 * @brief  判断串口接收缓存中是否有数据
 * @param  Null
 * @return 是否有数据
 */
unsigned char get_queue_total_data(void);

/**
 * @brief  读取队列1字节数据
 * @param  Null
 * @return Read the data
 */
unsigned char Queue_Read_Byte(void);

#endif
  
  
