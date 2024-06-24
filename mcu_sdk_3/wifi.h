/**********************************Copyright (c)**********************************
**                       版权所有 (C), 2015-2020, 涂鸦科技
**
**                             http://www.tuya.com
**
*********************************************************************************/
/**
 * @file    wifi.h
 * @author  涂鸦综合协议开发组
 * @version v1.0.7
 * @date    2020.11.9
 * @brief   用户无需关心该文件实现内容
 */


#ifndef __WIFI_H_
#define __WIFI_H_


//=============================================================================
/*定义常量*/
//=============================================================================
#ifndef TRUE
#define         TRUE                1
#endif
//
#ifndef FALSE
#define         FALSE               0
#endif
//
#ifndef NULL
#define         NULL                ((void *) 0)
#endif

#ifndef SUCCESS
#define         SUCCESS             1
#endif

#ifndef ERROR
#define         ERROR               0
#endif

#ifndef INVALID
#define         INVALID             0xFF
#endif

#ifndef ENABLE
#define         ENABLE              1
#endif
//
#ifndef DISABLE
#define         DISABLE             0
#endif
//=============================================================================
//dp数据点类型
//=============================================================================
#define         DP_TYPE_RAW                     0x00				//RAW型
#define         DP_TYPE_BOOL                    0x01                //布尔型
#define         DP_TYPE_VALUE                   0x02                //数值型
#define         DP_TYPE_STRING                  0x03				//字符串型
#define         DP_TYPE_ENUM                    0x04				//枚举型
#define         DP_TYPE_BITMAP                  0x05				//故障型

//=============================================================================
//WIFI工作状态
//=============================================================================
#define         SMART_CONFIG_STATE              0x00
#define         AP_STATE                        0x01
#define         WIFI_NOT_CONNECTED              0x02
#define         WIFI_CONNECTED                  0x03
#define         WIFI_CONN_CLOUD                 0x04
#define         LOW_POWER_STATE                 0x05
#define         SMART_AND_AP_STATE              0x06
#define         WIFI_SATE_UNKNOW                0xff
//=============================================================================
//wifi配网的方式
//=============================================================================
#define         SMART_CONFIG                    0x0  
#define         AP_CONFIG                       0x1   

//=============================================================================
//wifi复位状态
//=============================================================================
#define         RESET_WIFI_ERROR                0
#define         RESET_WIFI_SUCCESS              1

//=============================================================================
//wifi复位状态
//=============================================================================
#define         SET_WIFICONFIG_ERROR            0
#define         SET_WIFICONFIG_SUCCESS          1

//=============================================================================
//设备能力是否开启选择选择
//=============================================================================
#define         ON                              1                               //设备能力开启
#define         OFF                             0                               //设备能力关闭

//=============================================================================
//MCU固件升级状态
//=============================================================================
#define         FIRM_STATE_UN_SUPPORT           0x00                            //不支持 MCU 升级
#define         FIRM_STATE_WIFI_UN_READY        0x01                            //模块未就绪
#define         FIRM_STATE_GET_ERROR            0x02                            //云端升级信息查询失败
#define         FIRM_STATE_NO                   0x03                            //无需升级（云端无更新版本）
#define         FIRM_STATE_START                0x04                            //需升级，等待模块发起升级操作

//=============================================================================
//WIFI和mcu的工作方式 
//=============================================================================
#define         UNION_WORK                      0x0                             //mcu模块与wifi配合处理
#define         WIFI_ALONE                      0x1                             //wifi模块自处理

//=============================================================================
//系统工作模式
//=============================================================================
#define         NORMAL_MODE                     0x00                            //正常工作状态
#define         FACTORY_MODE                    0x01                            //工厂模式	
#define         UPDATE_MODE                     0x02                            //升级模式	     

//=============================================================================
//下发命令
//=============================================================================
typedef struct {
    unsigned char dp_id;                        //dp序号
    unsigned char dp_type;                      //dp类型
} DOWNLOAD_CMD_S;

#pragma anon_unions

#pragma pack(1)

typedef union {
    struct{
        unsigned char   picture_upload          :1;     //设备是否需要支持图片上传功能？ 0:表示不支持, 1:表示支持
        unsigned char   communication_mode      :1;     //设备采用哪种通信方式上传图片？ 0:表示串口,   1:表示SPI
        unsigned char   operator_door_lock      :1;     //设备是否为运营商门锁？         0:表示不支持, 1:表示支持
        unsigned char   reset_state_notice      :1;     //设备是否支持模块重置状态通知？ 0:表示不支持, 1:表示支持
        unsigned char   useless1                :1;     //预留
        unsigned char   useless2                :1;     //预留
        unsigned char   useless3                :1;     //预留
        unsigned char   useless4                :1;     //预留
    };
    unsigned char whole;
}tCAP_equip;

typedef struct {
    unsigned char dp_id;                        //dp序号
    unsigned char dp_len;                       //dp数据长度  bool、enum、value 类型的dp可不用写此参数
    unsigned char dp_bool_val;                  //bool 型 dp 值
    unsigned char dp_enum_val;                  //enum 型 dp 值
    unsigned int  dp_value_val;                 //value 型 dp 值
    unsigned int  dp_fault_bitmap;              //fault 型 dp 值
    unsigned char *dp_str_val;                  //string 型 dp 值
    unsigned char *dp_raw_val;                  //raw 型 dp 值
    
}t_DP_NODE;

#pragma pack()


#include "protocol.h"
#include "system.h"
#include "mcu_api.h"


#ifdef LOCK_API_ENABLE
#include "lock_api.h"
#endif


#endif
