/**********************************Copyright (c)**********************************
**                       版权所�? (C), 2015-2020, 涂鸦科技
**
**                             http://www.tuya.com
**
*********************************************************************************/
/**
 * @file    system.h
 * @author  涂鸦综合协�??开发组
 * @version v2.5.6
 * @date    2020.12.16
 * @brief   串口数据处理，用户无需关心该文件实现内�?
 */

/****************************** 免责声明 ！！�? *******************************
由于MCU类型和编译环境�?��?��?�样，所以�?�代码仅供参考，用户请自行把控最终代码质量，
涂鸦不�?�MCU功能结果负责�?
******************************************************************************/

#ifndef __SYSTEM_H_
#define __SYSTEM_H_

#ifdef SYSTEM_GLOBAL
  #define SYSTEM_EXTERN
#else
  #define SYSTEM_EXTERN   extern
#endif

//=============================================================================
//Byte order of the frame
//=============================================================================
#define         HEAD_FIRST                      0
#define         HEAD_SECOND                     1        
#define         PROTOCOL_VERSION                2
#define         FRAME_TYPE                      3
#define         LENGTH_HIGH                     4
#define         LENGTH_LOW                      5
#define         DATA_START                      6

//=============================================================================
//Data frame type
//=============================================================================
#define         HEAT_BEAT_CMD                   0                               //心跳�?
#define         PRODUCT_INFO_CMD                1                               //产品信息
#define         WORK_MODE_CMD                   2                               //查�??MCU 设定的模块工作模�?	
#define         WIFI_STATE_CMD                  3                               //wifi工作状�?	
#define         WIFI_RESET_CMD                  4                               //重置wifi
#define         WIFI_MODE_CMD                   5                               //选择smartconfig/AP模式	
#define         DATA_QUERT_CMD                  6                               //命令下发
#define         STATE_UPLOAD_CMD                7                               //状态上�?	 
#define         STATE_QUERY_CMD                 8                               //状态查�?   
#define         UPDATE_START_CMD                0x0a                            //升级开�?
#define         UPDATE_TRANS_CMD                0x0b                            //升级传输
#define         GET_ONLINE_TIME_CMD             0x0c                            //获取系统时间(格林威治时间)
#define         FACTORY_MODE_CMD                0x0d                            //进入产测模式    
#define         WIFI_TEST_CMD                   0x0e                            //wifi功能测试
#define         GET_LOCAL_TIME_CMD              0x1c                            //获取�?地时�?
#define         WEATHER_OPEN_CMD                0x20                            //打开天气          
#define         WEATHER_DATA_CMD                0x21                            //天气数据
#define         STATE_UPLOAD_SYN_CMD            0x22                            //状态上报（同�?�）
#define         STATE_UPLOAD_SYN_RECV_CMD       0x23                            //状态上报结果通知（同步）
#define         HEAT_BEAT_STOP                  0x25                            //关闭WIFI模组心跳
#define         STREAM_TRANS_CMD                0x28                            //流数�?传输
#define         GET_WIFI_STATUS_CMD             0x2b                            //获取当前wifi联网状�?
#define         WIFI_CONNECT_TEST_CMD           0x2c                            //wifi功能测试(连接指定�?�?)
#define         GET_MAC_CMD                     0x2d                            //获取模块mac
#define         GET_IR_STATUS_CMD               0x2e                            //红�?�状态通知
#define         IR_TX_RX_TEST_CMD               0x2f                            //红�?�进入收发产�?
#define         MAPS_STREAM_TRANS_CMD           0x30                            //流数�?传输(�?持�?�张地图)
#define         FILE_DOWNLOAD_START_CMD         0x31                            //文件下载�?�?
#define         FILE_DOWNLOAD_TRANS_CMD         0x32                            //文件下载数据传输
#define         MODULE_EXTEND_FUN_CMD           0x34                            //模块拓展服务
#define         BLE_TEST_CMD                    0x35                            //蓝牙功能性测试（�?描指定蓝牙信标）
#define         GET_VOICE_STATE_CMD             0x60                            //获取�?音状态码
#define         MIC_SILENCE_CMD                 0x61                            //MIC静音设置
#define         SET_SPEAKER_VOLUME_CMD          0x62                            //speaker音量设置
#define         VOICE_TEST_CMD                  0x63                            //�?音模组音频产�?
#define         VOICE_AWAKEN_TEST_CMD           0x64                            //�?音模组唤醒产�?
#define         VOICE_EXTEND_FUN_CMD            0x65                            //�?音模组扩展功�?


//=============================================================================
#define MCU_RX_VER              0x00                                            //模块发送帧协�??版本�?
#define MCU_TX_VER              0x03                                            //MCU 发送帧协�??版本�?(默�??)
#define PROTOCOL_HEAD           0x07                                            //固定协�??头长�?
#define FRAME_FIRST             0x55                                            //帧头�?一字节
#define FRAME_SECOND            0xaa                                            //帧头�?二字�?
//============================================================================= 
SYSTEM_EXTERN volatile unsigned char wifi_data_process_buf[PROTOCOL_HEAD + WIFI_DATA_PROCESS_LMT];     //串口数据处理缓存
SYSTEM_EXTERN volatile unsigned char wifi_uart_rx_buf[PROTOCOL_HEAD + WIFI_UART_RECV_BUF_LMT];         //串口接收缓存
SYSTEM_EXTERN volatile unsigned char wifi_uart_tx_buf[PROTOCOL_HEAD + WIFIR_UART_SEND_BUF_LMT];        //串口发送缓�?
//
SYSTEM_EXTERN volatile unsigned char *rx_buf_in;
SYSTEM_EXTERN volatile unsigned char *rx_buf_out;

SYSTEM_EXTERN volatile unsigned char stop_update_flag;                                                 //ENABLE:停�??一切数�?上传  DISABLE:恢�?�一切数�?上传

#ifndef WIFI_CONTROL_SELF_MODE
SYSTEM_EXTERN volatile unsigned char reset_wifi_flag;                                                  //重置wifi标志(TRUE:成功/FALSE:失败)
SYSTEM_EXTERN volatile unsigned char set_wifimode_flag;                                                //设置WIFI工作模式标志(TRUE:成功/FALSE:失败)
SYSTEM_EXTERN volatile unsigned char wifi_work_state;                                                  //wifi模块当前工作状�?
#endif

#ifdef WIFI_STREAM_ENABLE
SYSTEM_EXTERN volatile unsigned char stream_status;                                                             //流服务发包返回状�?
SYSTEM_EXTERN volatile unsigned char maps_stream_status;                                                        //多地图流服务发包返回状�?
#endif

/**
 * @brief  写wifi_uart字节
 * @param[in] {dest} 缓存区其实地址
 * @param[in] {byte} 写入字节�?
 * @return 写入完成后的总长�?
 */
unsigned short set_wifi_uart_byte(unsigned short dest, unsigned char byte);

/**
 * @brief  写wifi_uart_buffer
 * @param[in] {dest} �?标地址
 * @param[in] {src} 源地址
 * @param[in] {len} 数据长度
 * @return 写入结束的缓存地址
 */
unsigned short set_wifi_uart_buffer(unsigned short dest, const unsigned char *src, unsigned short len);

/**
 * @brief  计算校验�?
 * @param[in] {pack} 数据源指�?
 * @param[in] {pack_len} 计算校验和长�?
 * @return 校验�?
 */
unsigned char get_check_sum(unsigned char *pack, unsigned short pack_len);

/**
 * @brief  向wifi串口发送一帧数�?
 * @param[in] {fr_type} 帧类�?
 * @param[in] {fr_ver} 帧版�?
 * @param[in] {len} 数据长度
 * @return Null
 */
void wifi_uart_write_frame(unsigned char fr_type,  unsigned short len);

#ifdef WIFI_STREAM_ENABLE
/**
 * @brief  流数�?传输
 * @param[in] {id} 流服务标�?
 * @param[in] {offset} 偏移�?
 * @param[in] {buffer} 数据地址
 * @param[in] {buf_len} 数据长度
 * @return Null
 * @note   Null
 */
unsigned char stream_trans(unsigned short id, unsigned int offset, unsigned char *buffer, unsigned short buf_len);

/**
 * @brief  多地图流数据传输
 * @param[in] {pro_ver} 地图服务协�??版本
 * @param[in] {id} 地图流服务会话ID
 * @param[in] {sub_id} 子地图ID
 * @param[in] {sub_id_pro_mode} 子地图ID数据处理方式
 * @ref           0x00:继续�?�?
 * @ref           0x00:清除上传的数�?
 * @param[in] {offset} 偏移�?
 * @param[in] {buffer} 数据地址
 * @param[in] {buf_len} 数据长度
 * @return Null
 * @note   Null
 */
unsigned char maps_stream_trans(unsigned char pro_ver, unsigned short id, unsigned char sub_id, unsigned char sub_id_pro_mode, 
                                unsigned int offset, unsigned char *buffer, unsigned short buf_len);
#endif

/**
 * @brief  数据帧�?�理
 * @param[in] {offset} 数据起�?�位
 * @return Null
 */
void data_handle(unsigned short offset);

/**
 * @brief  判断串口接收缓存�?�?否有数据
 * @param  Null
 * @return �?否有数据
 */
unsigned char with_data_rxbuff(void);

/**
 * @brief  读取队列1字节数据
 * @param  Null
 * @return Read the data
 */
unsigned char take_byte_rxbuff(void);
#endif
  
  
