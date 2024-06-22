/**********************************Copyright (c)**********************************
**                       版权所有 (C), 2015-2020, 涂鸦科技
**
**                             http://www.tuya.com
**
*********************************************************************************/
/**
 * @file    lock_api.c
 * @author  涂鸦综合协议开发组
 * @version v1.0.7
 * @date    2020.11.9
 * @brief   门锁下发/上报数据处理函数，此文件下函数无须用户修改,用户需要调用的文件都在该文件内
 */


#define LOCK_API_GLOBAL

#include "wifi.h"

#ifdef LOCK_API_ENABLE
/**
 * @brief  MCU请求获取云端临时密码
 * @param  Null
 * @return Null
 * @note   MCU主动调用完成后在 temp_pass_handle 函数内可获取临时密码和有效期
 */
void mcu_get_temp_pass(void)
{ 
    wifi_uart_write_frame(TEMP_PASS_CMD, 0);
}

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
                      const unsigned char pass[], unsigned char pass_len)
{ 
    //#error "请自行完成临时密码相关处理代码,并删除该行"

    /*
    suc_flag为是否获取密码成功标志，为 0 表示失败，为 1 表示成功
   */
    /*
    gl_time为密码有效期格林时间
    gl_time[0]为年份 , 0x00 表 示2000 年
    gl_time[1]为月份，从 1 开始到12 结束
    gl_time[2]为日期，从 1 开始到31 结束
    gl_time[3]为时钟，从 0 开始到23 结束
    gl_time[4]为分钟，从 0 开始到59 结束
    gl_time[5]为秒钟，从 0 开始到59 结束

    pass指向临时密码数据(ascll码)，长度pass_len
   */
    if (suc_flag == 1) {
        //获取临时密码数据成功
    }else {
        //获取临时密码数据出错
    }
}

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
                        unsigned char admin_len, unsigned char admin_pass[])
{ 
    unsigned char length = 0;
    unsigned char i = 0;

    length = set_wifi_uart_buffer(length,(unsigned char *)time, 6);
    length = set_wifi_uart_buffer(length,(unsigned char *)user_pass, 8);
    length = set_wifi_uart_byte(length, admin_num);

    if (admin_num > 0) {
        length = set_wifi_uart_byte(length, admin_len);

        for(i=0;i<admin_num;i++) {
            length = set_wifi_uart_buffer(length,(unsigned char *)(admin_pass+admin_len*i), admin_len);
        }
    }

    wifi_uart_write_frame(PASS_CHECK_CMD, length);
}

/**
 * @brief  MCU请求校验动态密码返回函数
 * @param[in] {status} 校验标志(0:成功;1:失败;2:未激活;3:长度错误)
 * @return Null
 * @note   MCU主动调用 dynamic_pass_check 成功后,该函数内可获取校验结果
 */
void pass_check_handle(unsigned char status)
{ 
    //#error "请自行完成动态密码校验结果处理代码,并删除该行"

    switch (status) {
        case 0: {
            //密码核对通过
            break;
        }

        case 1: {
            //密码核对失败
            break;
        }

        case 2: {
            //设备未激活
            break;
        }

        case 3: {
            //数据长度错误
            break;
        }

        default:
        break;
    }
}

/**
 * @brief  MCU请求获取云端多组临时密码
 * @param  Null
 * @return Null
 * @note   MCU主动调用完成后在 mul_temp_pass_data 函数内可获取临时密码参数和有效期时限
 */
void mcu_get_mul_temp_pass(void)
{ 
    wifi_uart_write_frame(MUL_TEMP_PASS_CMD, 0);
}


/**
 * @brief  MCU请求多组临时密码处理函数
 * @param[in] {suc_flag} 请求标志(1:成功;0:失败)
 * @param[in] {pass_ser} 当前组密码编号(实际编号须+900)
 * @param[in] {pass_valcnt} 当前组密码有效次数(0:不限次数;1:一次性)
 * @param[in] {pass_sta} 密码当前状态(0:有效;1:被删除无效)
 * @param[in] {gl_start} 密码生效日期格林时间(从低到高6位，分别为年月日时分秒)
 * @param[in] {gl_end} 密码失效日期格林时间(从低到高6位，分别为年月日时分秒)
 * @param[in] {pass} 临时密码数据(ascll码表示，长度pass_len)
 * @param[in] {pass_len} 临时密码数据长度
 * @return Null
 * @note   MCU主动调用 mcu_get_mul_temp_pass 成功后，该函数内可多次分别获取每组的临时密码与有效期限
 */
static void mul_temp_pass_data(unsigned char suc_flag, unsigned char pass_ser, 
                               unsigned char pass_valcnt, unsigned char pass_sta, 
                               const unsigned char gl_start[], const unsigned char gl_end[], 
                               const unsigned char pass[], unsigned char pass_len)
{ 
    //#error "请自行完成多组临时密码信息处理代码,并删除该行"

    /*
    suc_flag为是否获取密码成功标志，为 0 表示失败，为 1 表示成功
   */
    /*
    pass_ser为密码编号
    pass_valcnt为密码有效次数
    pass_sta为密码当前状态

    gl_start为密码生效日期格林时间
    gl_start[0]为年份 , 0x00 表 示2000 年
    gl_start[1]为月份，从 1 开始到12 结束
    gl_start[2]为日期，从 1 开始到31 结束
    gl_start[3]为时钟，从 0 开始到23 结束
    gl_start[4]为分钟，从 0 开始到59 结束
    gl_start[5]为秒钟，从 0 开始到59 结束
    gl_end为密码截至日期格林时间，同gl_start

    pass指向临时密码数据(ascll码)，长度pass_len
   */

    /*
    注：获取多组密码成功，该函数会进入多次，
        直至将多组临时密码全部获取完结束；
        若失败则只进入一次。
   */
    if (suc_flag == 1) {
        //获取多组临时密码数据成功
    }else {
        //获取多组临时密码数据出错
    }
}

/**
 * @brief  MCU请求多组临时密码返回函数
 * @param[in] {data} 返回数据
 * @return Null
 * @note   Null
 */
void mul_temp_pass_handle(const unsigned char data[])
{
    unsigned char i = 0;

    unsigned char suc_flag = data[0];
    unsigned char pass_num = data[1];
    unsigned char pass_len = data[2];
    unsigned char offset = 0;

    if (suc_flag == 1) {
        for (i=0;i<pass_num;i++) {
            offset = i*(15+pass_len);
            mul_temp_pass_data(suc_flag, data[offset+3], data[offset+4], data[offset+5], 
                               data+offset+6, data+offset+12, data+offset+18, pass_len);
        }
    }else {
        mul_temp_pass_data(suc_flag, 0, 0, 0, 0, 0, 0, 0);
    }
}

/**
 * @brief  MCU请求获取云端临时密码(带schedule列表)
 * @param  Null
 * @return Null
 * @note   MCU主动调用完成后在 mul_temp_pass_data 函数内可获取结果
 */
void mcu_get_schedule_temp_pass(void)
{ 
    wifi_uart_write_frame(SCHEDULE_TEMP_PASS_CMD, 0);
}

/**
 * @brief  schedule列表数据处理
 * @param[in] {schedule_num} schedule列表数量
 * @param[in] {schedule_data} schedule列表数据
 * @return Null
 * @note   MCU调用 schedule_temp_pass_data 成功后，该函数内可对 schedule 列表数据进行处理
 */
static void schedule_data_process(unsigned char schedule_num, const unsigned char schedule_data[])
{
    //#error "请自行完成带schedule列表处理代码,并删除该行"
    
    unsigned char i = 0;
    unsigned char offset = 0;
    
    for(i=0;i<schedule_num;i++) {
        /*
        在此添加schedule列表数据处理
        schedule_data[offset]: 0：非全天有效，分时间段有效  1：全天有效。后面的起始时间和结束时间为无效数据
        schedule_data[offset+1]: 开始时间（小时）
        schedule_data[offset+2]: 开始时间（分钟）
        schedule_data[offset+3]: 结束时间（小时）
        schedule_data[offset+4]: 结束时间（分钟）
        schedule_data[offset+5]: 周循环   Bit0:周日 Bit1:周一 Bit2:周二 Bit3:周三 Bit4:周四 Bit5:周五 Bit6:周六
                                 若此条schedule周日，周三循环，则为0x09
        */
        
        offset += 6;
    }
}

/**
 * @brief  MCU请求带schedule列表临时密码处理函数
 * @param[in] {suc_flag} 请求标志(1:成功;0:失败)
 * @param[in] {pass_ser} 当前组密码编号(实际编号须+900)
 * @param[in] {pass_valcnt} 当前组密码有效次数(0:不限次数;1:一次性)
 * @param[in] {pass_sta} 密码当前状态(0:有效;1:被删除无效)
 * @param[in] {gl_start} 密码生效日期格林时间(从低到高6位，分别为年月日时分秒)
 * @param[in] {gl_end} 密码失效日期格林时间(从低到高6位，分别为年月日时分秒)
 * @param[in] {pass} 临时密码数据(ascll码表示，长度pass_len)
 * @param[in] {pass_len} 临时密码数据长度
 * @param[in] {schedule_num} schedule列表数量
 * @param[in] {schedule_data} schedule列表数据
 * @return Null
 * @note   MCU主动调用 mcu_get_mul_temp_pass 成功后，该函数内可多次分别获取每组的临时密码与有效期限
 */
static void schedule_temp_pass_data(unsigned char suc_flag, unsigned char pass_ser, 
                               unsigned char pass_valcnt, unsigned char pass_sta, 
                               const unsigned char gl_start[], const unsigned char gl_end[], 
                               const unsigned char pass[], unsigned char pass_len,
                               unsigned char schedule_num, const unsigned char schedule_data[])
{ 
    //#error "请自行完成带schedule列表临时密码信息处理代码,并删除该行"

    /*
    suc_flag为是否获取密码成功标志，为 0 表示失败，为 1 表示成功
   */
    /*
    注：获取多组密码成功，该函数会进入多次，
        直至将多组临时密码全部获取完结束；
        若失败则只进入一次。
   */
    if (suc_flag == 1) {
        //获取临时密码数据成功
        /*
        pass_ser为密码编号
        pass_valcnt为密码有效次数
        pass_sta为密码当前状态

        gl_start为密码生效日期格林时间
        gl_start[0]为年份 , 0x00 表 示2000 年
        gl_start[1]为月份，从 1 开始到12 结束
        gl_start[2]为日期，从 1 开始到31 结束
        gl_start[3]为时钟，从 0 开始到23 结束
        gl_start[4]为分钟，从 0 开始到59 结束
        gl_start[5]为秒钟，从 0 开始到59 结束
        gl_end为密码截至日期格林时间，同gl_start

        pass指向临时密码数据(ascll码)，长度pass_len
        */
        
        //此处添加密码数据处理
        
        schedule_data_process(schedule_num, schedule_data);
    }else {
        //获取临时密码数据出错
    }
}

/**
 * @brief  MCU请求临时密码(带schedule列表)返回
 * @param[in] {data} 返回数据
 * @return Null
 * @note   Null
 */
void schedule_temp_pass_handle(const unsigned char data[])
{
    unsigned char i = 0;

    unsigned char suc_flag = data[0];
    unsigned char pass_num = data[1];
    unsigned char pass_len = data[2];
  //  unsigned char pack_num = data[3] & 0x7f;
  //  unsigned char next_pack_flag = data[3] >> 7;
    unsigned char offset = 4;

    if (suc_flag == 1) {
        for (i=0;i<pass_num;i++) {
            schedule_temp_pass_data(suc_flag, data[offset], data[offset+1], data[offset+2], data+offset+3, 
                               data+offset+9, data+offset+15, pass_len, data[offset+15+pass_len],data+offset+15+pass_len+1);
            offset += 15 + pass_len + 1 + 6*data[offset+15+pass_len];
        }
    }else {
        schedule_temp_pass_data(suc_flag, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    }
}

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
void offline_dynamic_password(unsigned char green_time[],unsigned char pw[],unsigned char pw_len)
{
    unsigned char length = 0;
    
    length = set_wifi_uart_buffer(length,green_time,6);
    length = set_wifi_uart_byte(length,pw_len);
    length = set_wifi_uart_buffer(length,pw,pw_len);
    
    wifi_uart_write_frame(OFFLINE_DYN_PW_CMD,length);
}

/**
 * @brief  离线动态密码结果
 * @param[in] {result_data} 结果数据
 * @return Null
 * @note   MCU需要先自行调用offline_dynamic_password函数后，在此函数对接收的结果进行处理
 */
void offline_dynamic_password_result(unsigned char result_data[])
{
    //#error "请自行完成离线动态密码结果处理代码,并删除该行"
    unsigned char result; //密码正确性
    unsigned char type; //密码类型
    unsigned char decode_len; //解密后数据长度
    unsigned char decode[DECODE_MAX_LEN]; //解密数据
    
    result = result_data[0];
    if(0 == result) {
        //正确
    }else {
        //错误
        return; //错误时，无后续数据
    }
    
    type = result_data[1];
    switch(type) {
        case 0:
            //限时开门密码
        break;
        
        case 1:
            //单次开门密码
        break;
        
        case 2:
            //清除密码
        break;
        
        default:
        break;
    }
    
    decode_len = result_data[2];
    my_memcpy(decode,&result_data[3],decode_len);
    
    //可添加解密数据处理
    
}
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
void picture_event_state_notice(unsigned short event, unsigned char picture, unsigned char pic_num)
{
    unsigned char length = 0;
  
    length = set_wifi_uart_byte(length, (event >> 8) & 0xff);
    length = set_wifi_uart_byte(length, event & 0xff);
    length = set_wifi_uart_byte(length, picture);
    length = set_wifi_uart_byte(length, pic_num);
    
    wifi_uart_write_frame(PICTURE_EVENT_STATE_CMD,length);
}

/**
 * @brief  图片上传事件状态通知结果
 * @param[in] {result} 结果
 * @return Null
 * @note   MCU需要先自行调用 picture_event_state_notice 函数后，在此函数对接收的结果进行处理
 */
void picture_event_state_notice_result(unsigned char result)
{
    //#error "请自行完成图片上传时间状态通知结果处理代码,并删除该行"
    if (0 == result) {
        //信息接收成功
    }else {
        //信息接收失败
    }
}

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
                                            unsigned char p_pic_data[], unsigned char pic_data_len)
{
    unsigned char length = 0;
    
    length = set_wifi_uart_buffer(length, p_time, 7);
    length = set_wifi_uart_byte(length, (picture_id >> 8) & 0xff);
    length = set_wifi_uart_byte(length, picture_id & 0xff);
    length = set_wifi_uart_byte(length, (pic_total_num >> 8) & 0xff);
    length = set_wifi_uart_byte(length, pic_total_num & 0xff);
    length = set_wifi_uart_byte(length, (pic_num >> 8) & 0xff);
    length = set_wifi_uart_byte(length, pic_num & 0xff);
    length = set_wifi_uart_buffer(length, p_pic_data, pic_data_len);
    
    wifi_uart_write_frame(PICTURE_UPLOAD_CMD,length);
}

/**
 * @brief  图片上传结果
 * @param[in] {result} 结果
 * @return Null
 * @note   MCU需要先自行调用 picture_upload 函数后，在此函数对接收的结果进行处理
 */
void picture_upload_result(unsigned char result)
{
    //#error "请自行完成图片上传结果处理代码,并删除该行"
    if (0 == result) {
        //信息接收成功
    }else {
        //信息接收失败
    }
}

/**
 * @brief  图片上传结果反馈
 * @param[in] {p_value} 结果数据
 * @param[in] {data_len} 结果数据长度
 * @return Null
 * @note   MCU需要自行对接收的结果进行处理
 */
void picture_upload_return(unsigned char p_value[], unsigned short data_len)
{
    //#error "请自行完成图片上传结果反馈处理代码,并删除该行"
    
    unsigned short pic_id; //图片id号
    unsigned char p_time[6]; //当前图片上传的时间
    unsigned char upload_result; //图片上传结果
    
    if(9 != data_len) {
        //数据长度错误
        return;
    }
    
    pic_id = (p_value[0] << 8) | p_value[1];
    my_memcpy(p_time, &p_value[2], 6);
    upload_result = p_value[8];
    
    /*
    //请自行处理反馈结果
    //pic_id为图片id号   
    //p_time[6]中是当前图片上传的时间，p_time[0]~p_time[5]依次是 年 月 日 时 分 秒
    */
    switch(upload_result) {
        case 0:
            //图片上传成功
        break;
        
        case 1:
            //网络断开无法上传成功
        break;
        
        case 2:
            //图片太大超出40k
        break;
        
        case 3:
            //超时未收到mcu的图片信息
        break;
        
        case 4:
            //其它原因失败
        break;
        
        default:break;
    }
    
    unsigned char length = 0;
    
    //请判断是否还有后续图片上传，选择回复数据
    
    length = set_wifi_uart_byte(length, 0); //无后续图片上传
    //length = set_wifi_uart_byte(length, 1); //有后续图片上传
    
    wifi_uart_write_frame(PICTURE_UPLOAD_RETURN_CMD,length);
}

/**
 * @brief  图片上传状态获取
 * @param  Null
 * @return Null
 * @note   MCU需要自行调用后，可在 picture_upload_state_get_result 函数中对结果进行处理
 */
void picture_upload_state_get(void)
{
    wifi_uart_write_frame(PICTURE_UPLOAD_STATE_GET_CMD, 0);
}

/**
 * @brief  图片上传状态获取结果
 * @param[in] {p_value} 结果数据
 * @param[in] {data_len} 结果数据长度
 * @return Null
 * @note   MCU需要先自行调用 picture_upload_state_get 函数后，在此函数对接收的结果进行处理
 */
void picture_upload_state_get_result(unsigned char p_value[], unsigned short data_len)
{
    //#error "请自行完成图片上传状态获取结果处理代码,并删除该行"
    unsigned char pic_upload_state; //图片上传状态
    unsigned short event_infor; //事件信息编码
    unsigned short pic_id; //图片id号
    unsigned char p_time[6]; //当前图片上传的时间
    
    if(11 != data_len) {
        //数据长度错误
        return;
    }
    
    pic_upload_state = p_value[0];
    event_infor = (p_value[1] << 8) | p_value[2];
    pic_id = (p_value[3] << 8) | p_value[4];
    my_memcpy(p_time, &p_value[5], 6);
    
    //请在此处理获取到的数据
    
}

#endif

#ifdef LOCK_KEEP_ALIVE
/**
 * @brief  保活IO唤醒
 * @param[in] Null
 * @return Null
 * @note   MCU需要自行实现逻辑代码
 */
void keep_alive_awake(void)
{
    //#error "请在此完成保活IO唤醒操作代码，以唤醒模块，并删除该行"
    //此函数用于MCU唤醒模块，唤醒方式为IO低电平唤醒，唤醒后保持低电平，具体IO请参考硬件设计。
    
}

/**
 * @brief  获取WIFI状态
 * @param  Null
 * @return Null
 * @note   MCU需要自行调用后，可在 get_wifi_state_result 函数中对结果进行处理
 */
void mcu_get_wifi_state(void)
{
    wifi_uart_write_frame(GET_WIFI_STATE_CMD, 0);
}

/**
 * @brief  mcu收到WiFi状态
 * @param[in] {p_data} 数据
 * @return Null
 * @note   MCU需要先自行调用 mcu_get_wifi_state 函数后，在此函数对接收的结果进行处理
 */
void get_wifi_state_result(const unsigned char p_data[])
{
    //#error "请自行完成wifi状态处理代码,并删除该行"
    switch(p_data[0]) {
        case 0x00:
            //状态1  smartconfig配置状态
        break;
        
        case 0x01:
            //状态2  AP配置状态
        break;
        
        case 0x02:
            //状态3  WIFI已配置但未连上路由器
        break;
        
        case 0x03:
            //状态4  WIFI已配置且连上路由器
        break;
        
        case 0x04:
            //状态5  已连上路由器且连接到云端
        break;
        
        case 0x05:
            //状态6  WIFI设备处于低功耗模式
        break;
        
        case 0xff:
            //无效状态  初始化未完成
        break;
        
        default:break;
    }
    
    switch(p_data[1]) {
        case 0x00:
            //未激活
        break;
        
        case 0x01:
            //已激活
        break;
        
        default:break;
    } 
}
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
                                        unsigned char capture_num, unsigned char time_flag, unsigned char p_time[])
{
    unsigned short send_len = 0;
    
    send_len = set_wifi_uart_byte(send_len,EVENT_TRIGGER_CAPTURE_NOTICE_SUBCMD);
    send_len = set_wifi_uart_byte(send_len,(event_infor_num >> 8) & 0xff);
    send_len = set_wifi_uart_byte(send_len,event_infor_num & 0xff);
    send_len = set_wifi_uart_byte(send_len,capture_flag);
    send_len = set_wifi_uart_byte(send_len,capture_type);
    send_len = set_wifi_uart_byte(send_len,capture_num);
    send_len = set_wifi_uart_byte(send_len,time_flag);
    send_len = set_wifi_uart_buffer(send_len, p_time, 6);
    
    wifi_uart_write_frame(PHOTO_LOCK_PICTURE_UPLOAD_CMD, send_len);
}


/**
 * @brief  事件触发抓拍通知数据处理
 * @param[in] {result} 结果数据
 * @return Null
 * @note   MCU需要先自行调用 mcu_event_trigger_capture_notice 函数后，在此函数对接收的结果进行处理
 */
static void event_trigger_capture_notice_result(unsigned char result)
{
    //#error "请自行完成事件触发抓拍通知数据处理代码,并删除该行"
    
    if(0 == result) {
        //信息接收成功
    }else {
        //信息接收失败
    }
}

/**
 * @brief  拍照门锁图片上传相关功能
 * @param[in] {p_data} 数据
 * @return Null
 * @note   
 */
void photo_lock_picture_upload_func(const unsigned char p_data[])
{
    event_trigger_capture_notice_result(p_data[0]); //事件触发抓拍通知
}
#endif

#endif

