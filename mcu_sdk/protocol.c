/**********************************Copyright (c)**********************************
**                       版权所有 (C), 2015-2020, 涂鸦科技
**
**                             http://www.tuya.com
**
*********************************************************************************/
/**
 * @file    protocol.c
 * @author  涂鸦综合协议开发组
 * @version v2.5.6
 * @date    2020.12.16
 * @brief                
 *                       *******非常重要，一定要看哦！！！********
 *          1. 用户在此文件中实现数据下发/上报功能
 *          2. DP的ID/TYPE及数据处理函数都需要用户按照实际定义实现
 *          3. 当开始某些宏定义后需要用户实现代码的函数内部有#err提示,完成函数后请删除该#err
 */

/****************************** 免责声明 ！！！ *******************************
由于MCU类型和编译环境多种多样，所以此代码仅供参考，用户请自行把控最终代码质量，
涂鸦不对MCU功能结果负责。
******************************************************************************/

/******************************************************************************
                                移植须知:
1:MCU必须在while中直接调用mcu_api.c内的wifi_uart_service()函数
2:程序正常初始化完成后,建议不进行关串口中断,如必须关中断,关中断时间必须短,关中断会引起串口数据包丢失
3:请勿在中断/定时器中断内调用上报函数
******************************************************************************/

#include "wifi.h"
#include "led.h"
#include "log.h"

#ifdef WEATHER_ENABLE
/**
 * @var    weather_choose
 * @brief  天气数据参数选择数组
 * @note   用户可以自定义需要的参数，注释或者取消注释即可，注意更改
 */
const char *weather_choose[WEATHER_CHOOSE_CNT] = {
    "temp",
    "humidity",
    "condition",
    "pm25",
    /*"pressure",
    "realFeel",
    "uvi",
    "tips",
    "windDir",
    "windLevel",
    "windSpeed",
    "sunRise",
    "sunSet",
    "aqi",
    "so2 ",
    "rank",
    "pm10",
    "o3",
    "no2",
    "co",
    "conditionNum",*/
};
#endif


/******************************************************************************
                              第一步:初始化
1:在需要使用到wifi相关文件的文件中include "wifi.h"
2:在MCU初始化中调用mcu_api.c文件中的wifi_protocol_init()函数
3:将MCU串口单字节发送函数填入protocol.c文件中uart_transmit_output函数内,并删除#error
4:在MCU串口接收函数中调用mcu_api.c文件内的uart_receive_input函数,并将接收到的字节作为参数传入
5:单片机进入while循环后调用mcu_api.c文件内的wifi_uart_service()函数
******************************************************************************/

/******************************************************************************
                        1:dp数据点序列类型对照表
          **此为自动生成代码,如在开发平台有相关修改请重新下载MCU_SDK**         
******************************************************************************/
const DOWNLOAD_CMD_S download_cmd[] =
{
  {DPID_UNLOCK_FINGERPRINT, DP_TYPE_VALUE},
  {DPID_UNLOCK_PASSWORD, DP_TYPE_VALUE},
  {DPID_UNLOCK_CARD, DP_TYPE_VALUE},
  {DPID_UNLOCK_KEY, DP_TYPE_VALUE},
  {DPID_RESIDUAL_ELECTRICITY, DP_TYPE_VALUE},
  {DPID_UNLOCK_APP, DP_TYPE_VALUE},
  {DPID_REMOTE_NO_DP_KEY, DP_TYPE_RAW},
  {DPID_LOCK_MOTOR_STATE, DP_TYPE_BOOL},
  {DPID_INITIATIVE_MESSAGE, DP_TYPE_RAW},
};



/******************************************************************************
                           2:串口单字节发送函数
请将MCU串口发送函数填入该函数内,并将接收到的数据作为参数传入串口发送函数
******************************************************************************/

/**
 * @brief  串口发送数据
 * @param[in] {value} 串口要发送的1字节数据
 * @return Null
 */
void uart_transmit_output(unsigned char value)
{
    printf("===%c\r\n",value);
//    #error "请将MCU串口发送函数填入该函数,并删除该行"
    UART3_SendByte(value);
/*
    //Example:
    extern void Uart_PutChar(unsigned char value);
    Uart_PutChar(value);	                                //串口发送函数
*/
}

/******************************************************************************
                           第二步:实现具体用户函数
1:APP下发数据处理
2:数据上报处理
******************************************************************************/

/******************************************************************************
                            1:所有数据上报处理
当前函数处理全部数据上报(包括可下发/可上报和只上报)
  需要用户按照实际情况实现:
  1:需要实现可下发/可上报数据点上报
  2:需要实现只上报数据点上报
此函数为MCU内部必须调用
用户也可调用此函数实现全部数据上报
******************************************************************************/

//自动化生成数据上报函数

/**
 * @brief  系统所有dp点信息上传,实现APP和muc数据同步
 * @param  Null
 * @return Null
 * @note   此函数SDK内部需调用，MCU必须实现该函数内数据上报功能，包括只上报和可上报可下发型数据
 */
void all_data_update(void)
{
//    #error "请在此处理可下发可上报数据及只上报数据示例,处理完成后删除该行"
    /*
    //此代码为平台自动生成，请按照实际数据修改每个可下发可上报函数和只上报函数
    mcu_dp_value_update(DPID_UNLOCK_FINGERPRINT,当前指纹解锁); //VALUE型数据上报;
    mcu_dp_value_update(DPID_UNLOCK_PASSWORD,当前普通密码解锁); //VALUE型数据上报;
    mcu_dp_value_update(DPID_UNLOCK_CARD,当前卡片解锁); //VALUE型数据上报;
    mcu_dp_value_update(DPID_UNLOCK_KEY,当前钥匙解锁); //VALUE型数据上报;
    mcu_dp_value_update(DPID_RESIDUAL_ELECTRICITY,当前剩余电量); //VALUE型数据上报;
    mcu_dp_value_update(DPID_UNLOCK_APP,当前App远程解锁); //VALUE型数据上报;
    mcu_dp_raw_update(DPID_REMOTE_NO_DP_KEY,当前免密远程开门指针,当前免密远程开门数据长度); //RAW型数据上报;
    mcu_dp_bool_update(DPID_LOCK_MOTOR_STATE,当前落锁状态); //BOOL型数据上报;
    mcu_dp_raw_update(DPID_INITIATIVE_MESSAGE,当前主动消息推送指针,当前主动消息推送数据长度); //RAW型数据上报;

    */
}

/*****************************************************************************
函数名称 : dp_download_remote_no_dp_key_handle
功能描述 : 针对DPID_REMOTE_NO_DP_KEY的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char dp_download_remote_no_dp_key_handle(const unsigned char value[], unsigned short length)
{
    //示例:当前DP类型为RAW
    unsigned char ret;

    LOGD("123321");
    OPEN = 1;
    vTaskDelay(200);
    OPEN = 0;

    /*
    //RAW type data processing
    
    */
    
    //There should be a report after processing the DP
    ret = mcu_dp_raw_update(DPID_REMOTE_NO_DP_KEY,value,length);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}
/******************************************************************************
                                WARNING!!!    
                            2:所有数据上报处理
自动化代码模板函数,具体请用户自行实现数据处理
******************************************************************************/
/*****************************************************************************
函数名称 : dp_download_lock_motor_state_handle
功能描述 : 针对DPID_LOCK_MOTOR_STATE的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char dp_download_lock_motor_state_handle(const unsigned char value[], unsigned short length)
{
    //示例:当前DP类型为BOOL
    unsigned char ret;
    //0:off/1:on
    unsigned char lock_motor_state;
    
    lock_motor_state = mcu_get_dp_download_bool(value,length);
    if(lock_motor_state == 0) {
        LOGD("===123\n");
    }else {
        LOGD("===456\n");
        OPEN = 1;
    vTaskDelay(200);
    OPEN = 0;
    }
  
    //There should be a report after processing the DP
    ret = mcu_dp_bool_update(DPID_LOCK_MOTOR_STATE,lock_motor_state);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}
/*****************************************************************************
函数名称 : dp_download_countdown_fan_handle
功能描述 : 针对DPID_COUNTDOWN_FAN的处理函数
输入参数 : value:数据源数据
        : length:数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERROR
使用说明 : 可下发可上报类型,需要在处理完数据后上报处理结果至app
*****************************************************************************/
static unsigned char dp_download_initiative_message_handle(const unsigned char value[], unsigned short length)
{
    //示例:当前DP类型为RAW
    unsigned char ret;
    /*
    //RAW type data processing
    
    */
    //There should be a report after processing the DP
    ret = mcu_dp_raw_update(DPID_INITIATIVE_MESSAGE,value,length);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}





/******************************************************************************
                                WARNING!!!                     
此部分函数用户请勿修改!!
******************************************************************************/

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
unsigned char dp_download_handle(unsigned char dpid,const unsigned char value[], unsigned short length)
{
    /*********************************
    当前函数处理可下发/可上报数据调用                    
    具体函数内需要实现下发数据处理
    完成用需要将处理结果反馈至APP端,否则APP会认为下发失败
    ***********************************/
    unsigned char ret;
    switch(dpid) {
        case DPID_REMOTE_NO_DP_KEY:
            //免密远程开门处理函数
            ret = dp_download_remote_no_dp_key_handle(value,length);
        break;
        case DPID_LOCK_MOTOR_STATE:
            //落锁状态处理函数
            ret = dp_download_lock_motor_state_handle(value,length);
        break;
        case DPID_INITIATIVE_MESSAGE:
            //主动消息推送处理函数
            ret = dp_download_initiative_message_handle(value,length);
        break;

        
        default:
        break;
    }
    return ret;
}

/**
 * @brief  获取所有dp命令总和
 * @param[in] Null
 * @return 下发命令总和
 * @note   该函数用户不能修改
 */
unsigned char get_download_cmd_total(void)
{
    return(sizeof(download_cmd) / sizeof(download_cmd[0]));
}


/******************************************************************************
                                WARNING!!!                     
此代码为SDK内部调用,请按照实际dp数据实现函数内部数据
******************************************************************************/
#ifdef SUPPORT_MCU_RTC_CHECK
/**
 * @brief  MCU校对本地RTC时钟
 * @param[in] {time} 获取到的格林时间数据
 * @return Null
 * @note   MCU需要自行实现该功能
 */
void mcu_write_rtctime(unsigned char time[])
{
    //#error "请自行完成RTC时钟写入代码,并删除该行"
    /*
    time[0]为是否获取时间成功标志，为 0 表示失败，为 1表示成功
    time[1] 为 年 份 , 0x00 表 示2000 年
    time[2]为月份，从 1 开始到12 结束
    time[3]为日期，从 1 开始到31 结束
    time[4]为时钟，从 0 开始到23 结束
    time[5]为分钟，从 0 开始到59 结束
    time[6]为秒钟，从 0 开始到59 结束
    time[7]为星期，从 1 开始到 7 结束，1代表星期一
   */
    if(time[0] == 1) {
      //正确接收到wifi模块返回的本地时钟数据 
     
    }else {
      //获取本地时钟数据出错,有可能是当前wifi模块未联网
    }
}
#endif

#ifdef SUPPORT_GREEN_TIME
/**
 * @brief  MCU查询格林时间返回函数
 * @param[in] {gl_time} 返回的当前格林时间(从低到高7位，分别为年月日时分秒星期)
 * @return Null
 * @note   MCU主动调用 mcu_get_gelin_time 成功后,该函数内可获取格林时间
 */
void mcu_write_gltime(unsigned char gl_time[])
{
    //#error "请自行完成格林时间记录代码,并删除该行"
    /*
    gl_time[0]为是否获取时间成功标志，为 0 表示失败，为 1表示成功
    gl_time[1]为年份 , 0x00 表示 2000 年
    gl_time[2]为月份，从 1 开始到12 结束
    gl_time[3]为日期，从 1 开始到31 结束
    gl_time[4]为时钟，从 0 开始到23 结束
    gl_time[5]为分钟，从 0 开始到59 结束
    gl_time[6]为秒钟，从 0 开始到59 结束
    gl_time[7]为星期，从 1 开始到 7 结束，1代表星期一
   */
    if(gl_time[0] == 1) {
      //正确接收到wifi模块返回的本地时钟数据 
     
    }else {
      //获取本地时钟数据出错,有可能是当前wifi模块未联网
    }
}
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
void wifi_test_result(unsigned char result,unsigned char rssi)
{
    //#error "请自行实现wifi功能测试成功/失败代码,完成后请删除该行"
    if(result == 0) {
        //测试失败
        if(rssi == 0x00) {
            //未扫描到名称为tuya_mdev_test路由器,请检查
        }else if(rssi == 0x01) {
            //模块未授权
        }
    }else {
        //测试成功
        //rssi为信号强度(0-100, 0信号最差，100信号最强)
    }
}
#endif

/**
 * @brief  MCU请求wifi固件升级返回函数
 * @param[in] {status} 校验标志
 * @return Null
 * @note   MCU主动调用 wifi_update_request 函数完成后该函数内可获取升级当前状态
 */
void wifi_update_handle(unsigned char status)
{ 
    //#error "请自行完成wifi模块升级状态返回代码,并删除该行"

    switch (status) {
        case 0: {
            //开始检查固件更新
            break;
        }

        case 1: {
            //已经是最新固件
            break;
        }

        case 2: {
            //正在更新固件
            break;
        }

        case 3: {
            //固件更新成功
            break;
        }

        case 4: {
            //固件更新失败
            break;
        }

        default:
        break;
    }
}

#ifdef SUPPORT_MCU_FIRM_UPDATE
/**
 * @brief  MCU请求mcu固件升级
 * @param  Null
 * @return Null
 * @note   MCU主动调用完成后在 mcu_update_handle 函数内可获取升级当前状态
 */
void mcu_update_request(void)
{ 
    //wifi_uart_write_frame(MCU_UG_REQ_CMD, 0);
}

/**
 * @brief  MCU请求mcu固件升级返回函数
 * @param[in] {status} 校验标志
 * @return Null
 * @note   MCU主动调用 mcu_update_request 函数完成后该函数内可获取升级当前状态
 */
void mcu_update_handle(unsigned char status)
{ 
    //#error "请自行完成mcu升级状态处理代码,并删除该行"

    switch (status) {
        case 0: {
            //开始检查固件更新
            break;
        }

        case 1: {
            //已经是最新固件
            break;
        }

        case 2: {
            //正在更新固件
            break;
        }

        case 3: {
            //固件更新成功
            break;
        }

        case 4: {
            //固件更新失败
            break;
        }

        default:
        break;
    }
}

/**
 * @brief  MCU进入固件升级模式
 * @param[in] {value} 固件缓冲区
 * @param[in] {position} 当前数据包在于固件位置
 * @param[in] {length} 当前固件包长度(固件包长度为0时,表示固件包发送完成)
 * @return Null
 * @note   MCU需要自行实现该功能
 */
unsigned char mcu_firm_update_handle(const unsigned char value[],unsigned long position,unsigned short length)
{
    //#error "请自行完成MCU固件升级代码,完成后请删除该行"
    if(length == 0) {
        //文件包数据发送完成
        
    }else {
        //文件包数据处理
      
    }
    
    return SUCCESS;
}
#endif

#ifdef REPORTED_MCU_SN_ENABLE
/**
 * @brief  MCU上报SN
 * @param[in] {result} 状态结果
 * @return Null
 * @note   MCU需要先自行调用mcu_sn_updata函数后，在此函数对接收的结果进行处理
 */
void mcu_sn_updata_result(unsigned char result)
{
    //#error "请自行完成MCU上报SN结果处理代码,并删除该行"
    if(0 == result) {
        //上报成功
    }else {
        //上报失败
    }
}
#endif

#ifdef WIFI_RESET_NOTICE_ENABLE
/**
 * @brief  蓝牙功能性测试结果
 * @param[in] {value} 数据缓冲区
 * @param[in] {length} 数据长度
 * @return Null
 * @note   MCU需要先自行调用mcu_sn_updata函数后，在此函数对接收的结果进行处理
 */
void wifi_reset_notice(unsigned char result)
{
    //#error "请自行完成模块重置状态通知处理代码,并删除该行"
    
    switch(result) {
        case 0x00:
            //模块本地重置
        break;
        
        case 0x01:
            //APP远程重置
        break;
        
        case 0x02:
            //APP恢复出厂重置
        break;
        
        case 0x03:
            //本地数据清除，但设备不离网
        break;
        
        default:break;
    }
    wifi_uart_write_frame(WIFI_RESET_NOTICE_CMD, 0);
}
#endif




