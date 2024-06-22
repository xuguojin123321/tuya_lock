/**********************************Copyright (c)**********************************
**                       版权所有 (C), 2015-2020, 涂鸦科技
**
**                             http://www.tuya.com
**
*********************************************************************************/
/**
 * @file    protocol.h
 * @author  涂鸦综合协议开发组
 * @version v1.0.7
 * @date    2020.11.9
 * @brief                
 *                       *******非常重要，一定要看哦！！！********
 *          1. 用户在此文件中实现数据下发/上报功能
 *          2. DP的ID/TYPE及数据处理函数都需要用户按照实际定义实现
 *          3. 当开始某些宏定义后需要用户实现代码的函数内部有#err提示,完成函数后请删除该#err
 */


#ifndef __PROTOCOL_H_
#define __PROTOCOL_H_



/******************************************************************************
                            用户相关信息配置
******************************************************************************/

/******************************************************************************
                          配置是否支持锁固件开发                  
如需要支持MCU们锁类开发,请开启该宏
MCU可调用lock_api.c文件内的们锁类专用接口实现相关功能
                        ********WARNING!!!**********
门锁类开发默认支持固件升级,固件升级包为256字节
如需要开启该功能,串口接收缓冲区会变大
******************************************************************************/
#define         LOCK_API_ENABLE                         //开启lock_api接口

#ifdef LOCK_API_ENABLE
#define         SUPPORT_MCU_FIRM_UPDATE                 //开启MCU固件升级功能(锁类开发，默认支持)
#endif

/******************************************************************************
                            1:修改产品信息                
******************************************************************************/
#define PRODUCT_KEY "eklpdflaieuocnu1"    //开发平台创建产品后生成的16位字符产品唯一标识

#define MCU_VER "1.0.0"                                 //用户的软件版本,用于MCU固件升级,MCU升级版本需修改

/*  选择smart模式和AP模式,该宏都注释将保持smart模式和AP模式互相切换  */
#define CONFIG_MODE_CHOOSE        0         //模块同时支持AP连接配网和EZ配网无需用户切换,对应的配网状态0x06
//#define CONFIG_MODE_CHOOSE        1         //仅只有AP配网模式

/*  设备能力选择  */
//#define PICTURE_UPLOAD_ENABLE      //设备是否需要支持 拍照/图片上传 功能？ 关闭宏定义表示不支持, 开启表示支持
//#define CAP_COMMU_MODE_ENABLE          //设备采用哪种通信方式上传图片？ 关闭表示串口,   开启表示SPI
//#define WIFI_RESET_NOTICE_ENABLE        //设备是否支持模块重置状态通知？ 关闭表示不支持, 开启表示支持

/*  保活选择  */
//#define LOCK_KEEP_ALIVE        //如果是保活门锁，请打开该宏

/******************************************************************************
                         2:定义收发缓存:
                    如当前使用MCU的RAM不够,可修改为24
******************************************************************************/
#ifndef SUPPORT_MCU_FIRM_UPDATE
#define WIFI_UART_RECV_BUF_LMT          16              //串口数据接收缓存区大小,如MCU的RAM不够,可缩小
#define WIFI_DATA_PROCESS_LMT           24              //串口数据处理缓存区大小,根据用户DP数据大小量定,必须大于24
#else
#define WIFI_UART_RECV_BUF_LMT          128             //串口数据接收缓存区大小,如MCU的RAM不够,可缩小
#define WIFI_DATA_PROCESS_LMT           300             //固件升级缓冲区,需大缓存,如单包大小选择256，则缓存必须大于260
#endif

#define WIFIR_UART_SEND_BUF_LMT         48              //根据用户DP数据大小量定,必须大于48

/******************************************************************************
                      3:MCU是否需要支持校时功能                     
如需要请开启该宏,并在Protocol.c文件内mcu_write_rtctime实现代码
mcu_write_rtctime内部有#err提示,完成函数后请删除该#err
mcu在wifi模块正确联网后可调用mcu_get_system_time()函数发起校时功能
******************************************************************************/
//#define     SUPPORT_MCU_RTC_CHECK               //开启校时功能

/******************************************************************************
                      13:MCU是否需要支持获取格林时间功能                 
如需要请开启该宏,并在Protocol.c文件内mcu_get_greentime实现代码
mcu_get_greentime内部有#err提示,完成函数后请删除该#err
mcu在wifi模块正确联网后可调用mcu_get_green_time()函数发起校时功能
******************************************************************************/
//#define     SUPPORT_GREEN_TIME                  //开启格林时间功能
#ifdef LOCK_API_ENABLE
#ifndef SUPPORT_GREEN_TIME
#define         SUPPORT_GREEN_TIME                //开启格林时间功能
#endif
#endif

/******************************************************************************
                      4:MCU是否需要支持wifi功能测试                     
如需要请开启该宏,并且mcu在需要wifi功能测试处调用mcu_api.c文件内mcu_start_wifitest
并在protocol.c文件wifi_test_result函数内查看测试结果,
wifi_test_result内部有#err提示,完成函数后请删除该#err
******************************************************************************/
#define     WIFI_TEST_ENABLE                    //开启WIFI产测功能

/******************************************************************************
                          5:MCU是否支持获取dp缓存指令
对于某些带设置性或控制功能的传感器，需要新增 dp 下发功能。当在设备离线时，面板
有指令下发，这条指令会缓存在云端，等待设备主动获取。缓存命令的为增量方式，对于
已经获取过的命令，再次获取时不会下发

如需要支持获取dp缓存指令,请开启该宏
******************************************************************************/
//#define     DP_CACHE_SUPPORT                   //获取dp缓存指令

/******************************************************************************
                          6:MCU是否支持离线动态密码
在门锁离线的时候，用户可以通过app创建自定义的临时密码，此时模块就算处于离线状态
也可验证密码的正确性，有效时间控制依赖于格林时间
                          
如需要支持离线动态密码功能,请开启该宏，并且还需要开启获取格林时间的宏
******************************************************************************/
//#define     OFFLINE_DYN_PW_ENABLE              //支持离线动态密码

/******************************************************************************
                          7:MCU是否支持上报MCU的SN号
可将MCU的SN号上报至平台
如需要支持获取dp缓存指令,请开启该宏
******************************************************************************/
//#define     REPORTED_MCU_SN_ENABLE              //支持上报MCU的SN号

/******************************************************************************
                          8:MCU是否支持状态快速上报
如需要支持状态快速上报,请开启该宏
******************************************************************************/
//#define     STATE_FAST_UPLOAD_ENABLE            //支持状态快速上报

/******************************************************************************
                          9:MCU是否支持拍照门锁图片上传相关功能
如需要支持拍照门锁图片上传相关功能,请开启该宏
******************************************************************************/
//#define     PHOTO_LOCK_PICTURE_UPLOAD_ENABLE    //支持拍照门锁图片上传相关功能



/******************************************************************************
                        1:dp数据点序列号重新定义
          **此为自动生成代码,如在开发平台有相关修改请重新下载MCU_SDK**         
******************************************************************************/
//指纹解锁(只上报)
//备注:【可选】指纹在门锁上录入，录入完成后由硬件分配id，发生指纹解锁后，门锁将上报该dp点。请用记录型上报
#define DPID_UNLOCK_FINGERPRINT 1
//普通密码解锁(只上报)
//备注:【可选】普通密码（非临时密码）在门锁上录入，录入完成后由硬件分配id，发生普通密码解锁后，门锁将上报该dp点。普通密码指在门锁硬件上创建的密码，不包含通过app创建的临时密码。请用记录型上报
#define DPID_UNLOCK_PASSWORD 2
//卡片解锁(只上报)
//备注:【可选】卡片在门锁上录入，录入完成后由硬件分配id，发生卡片解锁后，门锁将上报该dp点。请用记录型上报
#define DPID_UNLOCK_CARD 5
//钥匙解锁(只上报)
//备注:【可选】如硬件能检测出机械钥匙开门，则固定上报 dp值为1，表示发生了机械钥匙开门。该开门记录请有记录型数据上报
#define DPID_UNLOCK_KEY 7
//剩余电量(只上报)
//备注:【可选，但必须二选一】电量值和电量状态，二选一。上报电量数值
#define DPID_RESIDUAL_ELECTRICITY 12
//App远程解锁(只上报)
//备注:【可选】当发生app远程开门的事件后，门锁上报远程开门的记录。使用记录型数据上报
#define DPID_UNLOCK_APP 15
//免密远程开门(可下发可上报)
//备注:【可选】远程开门的新加密机制，该DP需要在远程开门回复时同时发送
#define DPID_REMOTE_NO_DP_KEY 50
//落锁状态(可下发可上报)
//备注:
#define DPID_LOCK_MOTOR_STATE 55
//主动消息推送(可下发可上报)
//备注:用于传输图片以及视频的系统dp，创建pid时需要勾选该能力，否则将不支持传输图片或视频。
#define DPID_INITIATIVE_MESSAGE 212



/**
 * @brief  串口发送数据
 * @param[in] {value} 串口要发送的1字节数据
 * @return Null
 * @note   请将MCU串口发送函数填入该函数内,并将接收到的数据作为参数传入串口发送函数
 */
void uart_transmit_output(unsigned char value);

/**
 * @brief  系统所有dp点信息上传,实现APP和muc数据同步
 * @param  Null
 * @return Null
 * @note   此函数SDK内部需调用，MCU必须实现该函数内数据上报功能，包括只上报和可上报可下发型数据
 */
void all_data_update(void);

/**
 * @brief  dp下发处理函数
 * @param[in] {dpid} dpid 序号
 * @param[in] {value} dp数据缓冲区地址
 * @param[in] {length} dp数据长度
 * @return dp处理结果
 * -           0(ERROR): 失败
 * -           1(SUCCESS): 成功
 * @note   该函数用户不能修改
 */
unsigned char dp_download_handle(unsigned char dpid,const unsigned char value[], unsigned short length);

/**
 * @brief  获取所有dp命令总和
 * @param[in] Null
 * @return 下发命令总和
 * @note   该函数用户不能修改
 */
unsigned char get_download_cmd_total(void);


#ifdef SUPPORT_MCU_RTC_CHECK
/**
 * @brief  MCU校对本地RTC时钟
 * @param[in] {time} 获取到的格林时间数据
 * @return Null
 * @note   MCU需要自行实现该功能
 */
void mcu_write_rtctime(unsigned char time[]);
#endif

#ifdef SUPPORT_GREEN_TIME
/**
 * @brief  MCU查询格林时间返回函数
 * @param[in] {gl_time} 返回的当前格林时间(从低到高7位，分别为年月日时分秒星期)
 * @return Null
 * @note   MCU主动调用 mcu_get_gelin_time 成功后,该函数内可获取格林时间
 */
void mcu_write_gltime(unsigned char gl_time[]);
#endif

#ifdef WIFI_TEST_ENABLE
/**
 * @brief  wifi功能测试反馈
 * @param[in] {result} wifi功能测试结果
 * @ref       0: 失败
 * @ref       1: 成功
 * @param[in] {rssi} 测试成功表示wifi信号强度/测试失败表示错误类型
 * @return Null
 * @note   MCU需要自行实现该功能
 */
void wifi_test_result(unsigned char result,unsigned char rssi);
#endif

/**
 * @brief  MCU请求wifi固件升级返回函数
 * @param[in] {status} 校验标志
 * @return Null
 * @note   MCU主动调用 wifi_update_request 函数完成后该函数内可获取升级当前状态
 */
void wifi_update_handle(unsigned char status);

#ifdef SUPPORT_MCU_FIRM_UPDATE
/**
 * @brief  MCU请求mcu固件升级
 * @param  Null
 * @return Null
 * @note   MCU主动调用完成后在 mcu_update_handle 函数内可获取升级当前状态
 */
void mcu_update_request(void);

/**
 * @brief  MCU请求mcu固件升级返回函数
 * @param[in] {status} 校验标志
 * @return Null
 * @note   MCU主动调用 mcu_update_request 函数完成后该函数内可获取升级当前状态
 */
void mcu_update_handle(unsigned char status);

/**
 * @brief  MCU进入固件升级模式
 * @param[in] {value} 固件缓冲区
 * @param[in] {position} 当前数据包在于固件位置
 * @param[in] {length} 当前固件包长度(固件包长度为0时,表示固件包发送完成)
 * @return Null
 * @note   MCU需要自行实现该功能
 */
unsigned char mcu_firm_update_handle(const unsigned char value[],unsigned long position,unsigned short length);
#endif

#ifdef REPORTED_MCU_SN_ENABLE
/**
 * @brief  MCU上报SN
 * @param[in] {result} 状态结果
 * @return Null
 * @note   MCU需要先自行调用mcu_sn_updata函数后，在此函数对接收的结果进行处理
 */
void mcu_sn_updata_result(unsigned char result);
#endif

#ifdef WIFI_RESET_NOTICE_ENABLE
/**
 * @brief  模块重置状态通知
 * @param[in] {result} 状态结果
 * @return Null
 * @note   MCU需要先自行调用mcu_sn_updata函数后，在此函数对接收的结果进行处理
 */
void wifi_reset_notice(unsigned char result);
#endif



#endif

