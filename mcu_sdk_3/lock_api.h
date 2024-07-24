/**********************************Copyright (c)**********************************
**                       版权所有 (C), 2015-2020, 涂鸦科技
**
**                             http://www.tuya.com
**
*********************************************************************************/
/**
 * @file    lock_api.h
 * @author  涂鸦综合协议开发组
 * @version v1.0.7
 * @date    2020.11.9
 * @brief   门锁下发/上报数据处理函数，此文件下函数无须用户修改,用户需要调用的文件都在该文件内
 */
 
 
#ifndef __LOCK_API_H_
#define __LOCK_API_H_


#ifdef LOCK_API_GLOBAL
  #define LOCK_API_EXTERN
#else
  #define LOCK_API_EXTERN   extern
#endif

#include "wifi.h"

#define DECODE_MAX_LEN                          16                              //离线动态密码解密后的最大长度
  
  
//=============================================================================
//门锁数据帧类型
//=============================================================================
#define         TEMP_PASS_CMD                   0x11                            //请求云端临时密码（只支持单组）
#define         PASS_CHECK_CMD                  0x12                            //动态密码校验
#define         MUL_TEMP_PASS_CMD               0x13                            //请求云端临时密码（支持多组）
#define         SCHEDULE_TEMP_PASS_CMD          0x14                            //请求云端临时密码（带schedule列表）
#define         OFFLINE_DYN_PW_CMD              0x16                            //离线动态密码
#define         PICTURE_EVENT_STATE_CMD         0x60                            //事件状态通知
#define         PICTURE_UPLOAD_CMD              0x61                            //图片上传
#define         PICTURE_UPLOAD_RETURN_CMD       0x62                            //图片上传结果反馈
#define         PICTURE_UPLOAD_STATE_GET_CMD    0x63                            //图片上传状态获取
#ifdef PHOTO_LOCK_PICTURE_UPLOAD_ENABLE
#define         PHOTO_LOCK_PICTURE_UPLOAD_CMD   0x64                            //拍照门锁图片上传相关功能
#endif


//=============================================================================
//子命令
//=============================================================================
#ifdef PHOTO_LOCK_PICTURE_UPLOAD_CMD
#define         EVENT_TRIGGER_CAPTURE_NOTICE_SUBCMD         0x00                //事件触发抓拍通知
#endif


#ifdef LOCK_API_ENABLE
/**
 * @brief  MCU请求获取云端临时密码
 * @param  Null
 * @return Null
 * @note   MCU主动调用完成后在 temp_pass_handle 函数内可获取临时密码和有效期
 */
void mcu_get_temp_pass(void);

/**
 * @brief  MCU请求临时密码返回函数
 * @param[in] {suc_flag} 请求标志(1:成功;0:失败)
 * @param[in] {gl_time} 密码有效期格林时间(从低到高6位，分别为年月日时分秒)
 * @param[in] {pass} 临时密码数据(ascll码表示，长度pass_len)
 * @param[in] {pass_len} 临时密码数据长度
 * @return Null
 * @note   MCU主动调用 mcu_get_temp_pass 成功后,该函数内可临时密码和有效期
 */
void temp_pass_handle(unsigned char suc_flag, const unsigned char gl_time[], 
                      const unsigned char pass[], unsigned char pass_len);


/**
 * @brief  MCU请求动态密码校验
 * @param[in] {time} 请求当前格林时间(从低到高6位，分别为年月日时分秒)
 * @param[in] {user_pass} 用户输入的动态密码(从低到高8位，密码用ASCLL传入，范围'0'-'9')
 * @param[in] {admin_num} 管理员密码组数(0~10，若为0则后面参数无效)
 * @param[in] {admin_len} 管理员密码长度(最长8个字节)
 * @param[in] {admin_pass} 多组管理员密码(多组密码依次排列，长度admin_num*admin_len，用ASCLL传入，范围'0'-'9')
 * @return Null
 * @note   1:MCU主动调用
           2:成功后,在 pass_check_handle 函数内可获取动态密码校验结果
 */
void dynamic_pass_check(unsigned char time[], unsigned char user_pass[], unsigned char admin_num,
                        unsigned char admin_len, unsigned char admin_pass[]);

/**
 * @brief  MCU请求校验动态密码返回函数
 * @param[in] {status} 校验标志(0:成功;1:失败;2:未激活;3:长度错误)
 * @return Null
 * @note   MCU主动调用 dynamic_pass_check 成功后,该函数内可获取校验结果
 */
void pass_check_handle(unsigned char status);

/**
 * @brief  MCU请求获取云端多组临时密码
 * @param  Null
 * @return Null
 * @note   MCU主动调用完成后在 mul_temp_pass_data 函数内可获取临时密码参数和有效期时限
 */
void mcu_get_mul_temp_pass(void);

/**
 * @brief  MCU请求多组临时密码返回函数
 * @param[in] {data} 返回数据
 * @return Null
 * @note   Null
 */
void mul_temp_pass_handle(const unsigned char data[]);

/**
 * @brief  MCU请求获取云端临时密码(带schedule列表)
 * @param  Null
 * @return Null
 * @note   MCU主动调用完成后在 mul_temp_pass_data 函数内可获取结果
 */
void mcu_get_schedule_temp_pass(void);

/**
 * @brief  MCU请求临时密码(带schedule列表)返回
 * @param[in] {data} 返回数据
 * @return Null
 * @note   Null
 */
void schedule_temp_pass_handle(const unsigned char data[]);

#ifdef OFFLINE_DYN_PW_ENABLE
/**
 * @brief  离线动态密码
 * @param[in] {green_time} 格林时间
                green_time[0]为年份，0x00 表示2000 年
                green_time[1]为月份，从 1 开始到12 结束
                green_time[2]为日期，从 1 开始到31 结束
                green_time[3]为时钟，从 0 开始到23 结束
                green_time[4]为分钟，从 0 开始到59 结束
                green_time[5]为秒钟，从 0 开始到59 结束
 * @param[in] {pw} 离线动态密码
 * @param[in] {pw_len} 离线动态密码长度
 * @return Null
 * @note   MCU需要自行调用后，可在offline_dynamic_password_result函数中对结果进行处理
 */
void offline_dynamic_password(unsigned char green_time[],unsigned char pw[],unsigned char pw_len);

/**
 * @brief  离线动态密码结果
 * @param[in] {result_data} 结果数据
 * @return Null
 * @note   MCU需要先自行调用offline_dynamic_password函数后，在此函数对接收的结果进行处理
 */
void offline_dynamic_password_result(unsigned char result_data[]);
#endif

#ifdef PICTURE_UPLOAD_ENABLE
/**
 * @brief  图片上传事件状态通知
 * @param[in] {event} 事件信息编码
 * @param[in] {picture} 是否携带图片信息
 * @param[in] {pic_num} 上传图片张数(<=10)
 * @return Null
 * @note   MCU需要自行调用后，可在 picture_event_state_notice_result 函数中对结果进行处理
 */
void picture_event_state_notice(unsigned short event, unsigned char picture, unsigned char pic_num);

/**
 * @brief  图片上传事件状态通知结果
 * @param[in] {result} 结果
 * @return Null
 * @note   MCU需要先自行调用 picture_event_state_notice 函数后，在此函数对接收的结果进行处理
 */
void picture_event_state_notice_result(unsigned char result);

/**
 * @brief  图片上传
 * @param[in] {p_time} 时间数据
 * @param[in] {picture_id} 图片id号
 * @param[in] {pic_total_num} 图片总包数
 * @param[in] {pic_num} 当前包序号（从0开始）
 * @param[in] {p_pic_data} 图片数据
 * @param[in] {pic_data_len} 图片数据长度
 * @return Null
 * @note   MCU需要自行调用后，可在 picture_upload_result 函数中对结果进行处理
 */
void picture_upload(unsigned char p_time[], unsigned short picture_id, unsigned short pic_total_num, unsigned short pic_num, \
                                            unsigned char p_pic_data[], unsigned char pic_data_len);

/**
 * @brief  图片上传结果
 * @param[in] {result} 结果
 * @return Null
 * @note   MCU需要先自行调用 picture_upload 函数后，在此函数对接收的结果进行处理
 */
void picture_upload_result(unsigned char result);

/**
 * @brief  图片上传结果反馈
 * @param[in] {p_value} 结果数据
 * @param[in] {data_len} 结果数据长度
 * @return Null
 * @note   MCU需要自行对接收的结果进行处理
 */
void picture_upload_return(unsigned char p_value[], unsigned short data_len);

/**
 * @brief  图片上传状态获取
 * @param  Null
 * @return Null
 * @note   MCU需要自行调用后，可在 picture_upload_state_get_result 函数中对结果进行处理
 */
void picture_upload_state_get(void);

/**
 * @brief  图片上传状态获取结果
 * @param[in] {p_value} 结果数据
 * @param[in] {data_len} 结果数据长度
 * @return Null
 * @note   MCU需要先自行调用 picture_upload_state_get 函数后，在此函数对接收的结果进行处理
 */
void picture_upload_state_get_result(unsigned char p_value[], unsigned short data_len);
#endif

#ifdef LOCK_KEEP_ALIVE
/**
 * @brief  保活IO唤醒
 * @param[in] Null
 * @return Null
 * @note   MCU需要自行实现逻辑代码
 */
void keep_alive_awake(void);

/**
 * @brief  获取WIFI状态
 * @param  Null
 * @return Null
 * @note   MCU需要自行调用后，可在 get_wifi_state_result 函数中对结果进行处理
 */
void mcu_get_wifi_state(void);

/**
 * @brief  mcu收到WiFi状态
 * @param[in] {p_data} 数据
 * @return Null
 * @note   MCU需要先自行调用 mcu_get_wifi_state 函数后，在此函数对接收的结果进行处理
 */
void get_wifi_state_result(const unsigned char p_data[]);
#endif

#ifdef PHOTO_LOCK_PICTURE_UPLOAD_ENABLE
/**
 * @brief  MCU发送事件触发抓拍通知
 * @param[in] {event_infor_num} 事件信息编码
 * @param[in] {capture_flag} 是否抓拍图片
 * @param[in] {capture_type} 抓拍类型
 * @param[in] {capture_num} 抓拍图片张数
 * @param[in] {time_flag} 时间标志位  0:不带时间 1:本地时间 2:格林时间
 * @param[in] {p_time} 时间数据
 * @return Null
 * @note   MCU需要自行调用后，可在 event_trigger_capture_notice_result 函数中对结果进行处理
 */
void mcu_event_trigger_capture_notice(unsigned short event_infor_num, unsigned char capture_flag, unsigned char capture_type,
                                        unsigned char capture_num, unsigned char time_flag, unsigned char p_time[]);
                                            
/**
 * @brief  拍照门锁图片上传相关功能
 * @param[in] {p_data} 数据
 * @return Null
 * @note   
 */
void photo_lock_picture_upload_func(const unsigned char p_data[]);
#endif

#endif

#endif
