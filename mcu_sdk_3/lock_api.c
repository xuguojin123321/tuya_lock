/**********************************Copyright (c)**********************************
**                       ��Ȩ���� (C), 2015-2020, Ϳѻ�Ƽ�
**
**                             http://www.tuya.com
**
*********************************************************************************/
/**
 * @file    lock_api.c
 * @author  Ϳѻ�ۺ�Э�鿪����
 * @version v1.0.7
 * @date    2020.11.9
 * @brief   �����·�/�ϱ����ݴ����������ļ��º��������û��޸�,�û���Ҫ���õ��ļ����ڸ��ļ���
 */


#define LOCK_API_GLOBAL

#include "wifi.h"

#ifdef LOCK_API_ENABLE
/**
 * @brief  MCU�����ȡ�ƶ���ʱ����
 * @param  Null
 * @return Null
 * @note   MCU����������ɺ��� temp_pass_handle �����ڿɻ�ȡ��ʱ�������Ч��
 */
void mcu_get_temp_pass(void)
{ 
    wifi_uart_write_frame(TEMP_PASS_CMD, 0);
}

/**
 * @brief  MCU������ʱ���뷵�غ���
 * @param[in] {suc_flag} �����־(1:�ɹ�;0:ʧ��)
 * @param[in] {gl_time} ������Ч�ڸ���ʱ��(�ӵ͵���6λ���ֱ�Ϊ������ʱ����)
 * @param[in] {pass} ��ʱ��������(ascll���ʾ������pass_len)
 * @param[in] {pass_len} ��ʱ�������ݳ���
 * @return Null
 * @note   MCU�������� mcu_get_temp_pass �ɹ���,�ú����ڿ���ʱ�������Ч��
 */
void temp_pass_handle(unsigned char suc_flag, const unsigned char gl_time[], 
                      const unsigned char pass[], unsigned char pass_len)
{ 
    //#error "�����������ʱ������ش������,��ɾ������"

    /*
    suc_flagΪ�Ƿ��ȡ����ɹ���־��Ϊ 0 ��ʾʧ�ܣ�Ϊ 1 ��ʾ�ɹ�
   */
    /*
    gl_timeΪ������Ч�ڸ���ʱ��
    gl_time[0]Ϊ��� , 0x00 �� ʾ2000 ��
    gl_time[1]Ϊ�·ݣ��� 1 ��ʼ��12 ����
    gl_time[2]Ϊ���ڣ��� 1 ��ʼ��31 ����
    gl_time[3]Ϊʱ�ӣ��� 0 ��ʼ��23 ����
    gl_time[4]Ϊ���ӣ��� 0 ��ʼ��59 ����
    gl_time[5]Ϊ���ӣ��� 0 ��ʼ��59 ����

    passָ����ʱ��������(ascll��)������pass_len
   */
    if (suc_flag == 1) {
        //��ȡ��ʱ�������ݳɹ�
    }else {
        //��ȡ��ʱ�������ݳ���
    }
}

/**
 * @brief  MCU����̬����У��
 * @param[in] {time} ����ǰ����ʱ��(�ӵ͵���6λ���ֱ�Ϊ������ʱ����)
 * @param[in] {user_pass} �û�����Ķ�̬����(�ӵ͵���8λ��������ASCLL���룬��Χ'0'-'9')
 * @param[in] {admin_num} ����Ա��������(0~10����Ϊ0����������Ч)
 * @param[in] {admin_len} ����Ա���볤��(�8���ֽ�)
 * @param[in] {admin_pass} �������Ա����(���������������У�����admin_num*admin_len����ASCLL���룬��Χ'0'-'9')
 * @return Null
 * @note   1:MCU��������
           2:�ɹ���,�� pass_check_handle �����ڿɻ�ȡ��̬����У����
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
 * @brief  MCU����У�鶯̬���뷵�غ���
 * @param[in] {status} У���־(0:�ɹ�;1:ʧ��;2:δ����;3:���ȴ���)
 * @return Null
 * @note   MCU�������� dynamic_pass_check �ɹ���,�ú����ڿɻ�ȡУ����
 */
void pass_check_handle(unsigned char status)
{ 
    //#error "��������ɶ�̬����У�����������,��ɾ������"

    switch (status) {
        case 0: {
            //����˶�ͨ��
            break;
        }

        case 1: {
            //����˶�ʧ��
            break;
        }

        case 2: {
            //�豸δ����
            break;
        }

        case 3: {
            //���ݳ��ȴ���
            break;
        }

        default:
        break;
    }
}

/**
 * @brief  MCU�����ȡ�ƶ˶�����ʱ����
 * @param  Null
 * @return Null
 * @note   MCU����������ɺ��� mul_temp_pass_data �����ڿɻ�ȡ��ʱ�����������Ч��ʱ��
 */
void mcu_get_mul_temp_pass(void)
{ 
    wifi_uart_write_frame(MUL_TEMP_PASS_CMD, 0);
}


/**
 * @brief  MCU���������ʱ���봦����
 * @param[in] {suc_flag} �����־(1:�ɹ�;0:ʧ��)
 * @param[in] {pass_ser} ��ǰ��������(ʵ�ʱ����+900)
 * @param[in] {pass_valcnt} ��ǰ��������Ч����(0:���޴���;1:һ����)
 * @param[in] {pass_sta} ���뵱ǰ״̬(0:��Ч;1:��ɾ����Ч)
 * @param[in] {gl_start} ������Ч���ڸ���ʱ��(�ӵ͵���6λ���ֱ�Ϊ������ʱ����)
 * @param[in] {gl_end} ����ʧЧ���ڸ���ʱ��(�ӵ͵���6λ���ֱ�Ϊ������ʱ����)
 * @param[in] {pass} ��ʱ��������(ascll���ʾ������pass_len)
 * @param[in] {pass_len} ��ʱ�������ݳ���
 * @return Null
 * @note   MCU�������� mcu_get_mul_temp_pass �ɹ��󣬸ú����ڿɶ�ηֱ��ȡÿ�����ʱ��������Ч����
 */
static void mul_temp_pass_data(unsigned char suc_flag, unsigned char pass_ser, 
                               unsigned char pass_valcnt, unsigned char pass_sta, 
                               const unsigned char gl_start[], const unsigned char gl_end[], 
                               const unsigned char pass[], unsigned char pass_len)
{ 
    //#error "��������ɶ�����ʱ������Ϣ�������,��ɾ������"

    /*
    suc_flagΪ�Ƿ��ȡ����ɹ���־��Ϊ 0 ��ʾʧ�ܣ�Ϊ 1 ��ʾ�ɹ�
   */
    /*
    pass_serΪ������
    pass_valcntΪ������Ч����
    pass_staΪ���뵱ǰ״̬

    gl_startΪ������Ч���ڸ���ʱ��
    gl_start[0]Ϊ��� , 0x00 �� ʾ2000 ��
    gl_start[1]Ϊ�·ݣ��� 1 ��ʼ��12 ����
    gl_start[2]Ϊ���ڣ��� 1 ��ʼ��31 ����
    gl_start[3]Ϊʱ�ӣ��� 0 ��ʼ��23 ����
    gl_start[4]Ϊ���ӣ��� 0 ��ʼ��59 ����
    gl_start[5]Ϊ���ӣ��� 0 ��ʼ��59 ����
    gl_endΪ����������ڸ���ʱ�䣬ͬgl_start

    passָ����ʱ��������(ascll��)������pass_len
   */

    /*
    ע����ȡ��������ɹ����ú���������Σ�
        ֱ����������ʱ����ȫ����ȡ�������
        ��ʧ����ֻ����һ�Ρ�
   */
    if (suc_flag == 1) {
        //��ȡ������ʱ�������ݳɹ�
    }else {
        //��ȡ������ʱ�������ݳ���
    }
}

/**
 * @brief  MCU���������ʱ���뷵�غ���
 * @param[in] {data} ��������
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
 * @brief  MCU�����ȡ�ƶ���ʱ����(��schedule�б�)
 * @param  Null
 * @return Null
 * @note   MCU����������ɺ��� mul_temp_pass_data �����ڿɻ�ȡ���
 */
void mcu_get_schedule_temp_pass(void)
{ 
    wifi_uart_write_frame(SCHEDULE_TEMP_PASS_CMD, 0);
}

/**
 * @brief  schedule�б����ݴ���
 * @param[in] {schedule_num} schedule�б�����
 * @param[in] {schedule_data} schedule�б�����
 * @return Null
 * @note   MCU���� schedule_temp_pass_data �ɹ��󣬸ú����ڿɶ� schedule �б����ݽ��д���
 */
static void schedule_data_process(unsigned char schedule_num, const unsigned char schedule_data[])
{
    //#error "��������ɴ�schedule�б������,��ɾ������"
    
    unsigned char i = 0;
    unsigned char offset = 0;
    
    for(i=0;i<schedule_num;i++) {
        /*
        �ڴ����schedule�б����ݴ���
        schedule_data[offset]: 0����ȫ����Ч����ʱ�����Ч  1��ȫ����Ч���������ʼʱ��ͽ���ʱ��Ϊ��Ч����
        schedule_data[offset+1]: ��ʼʱ�䣨Сʱ��
        schedule_data[offset+2]: ��ʼʱ�䣨���ӣ�
        schedule_data[offset+3]: ����ʱ�䣨Сʱ��
        schedule_data[offset+4]: ����ʱ�䣨���ӣ�
        schedule_data[offset+5]: ��ѭ��  ?Bit0:����?Bit1:��һ?Bit2:�ܶ�?Bit3:����?Bit4:����?Bit5:����?Bit6:����
                                 ������schedule���գ�����ѭ������Ϊ0x09
        */
        
        offset += 6;
    }
}

/**
 * @brief  MCU�����schedule�б���ʱ���봦����
 * @param[in] {suc_flag} �����־(1:�ɹ�;0:ʧ��)
 * @param[in] {pass_ser} ��ǰ��������(ʵ�ʱ����+900)
 * @param[in] {pass_valcnt} ��ǰ��������Ч����(0:���޴���;1:һ����)
 * @param[in] {pass_sta} ���뵱ǰ״̬(0:��Ч;1:��ɾ����Ч)
 * @param[in] {gl_start} ������Ч���ڸ���ʱ��(�ӵ͵���6λ���ֱ�Ϊ������ʱ����)
 * @param[in] {gl_end} ����ʧЧ���ڸ���ʱ��(�ӵ͵���6λ���ֱ�Ϊ������ʱ����)
 * @param[in] {pass} ��ʱ��������(ascll���ʾ������pass_len)
 * @param[in] {pass_len} ��ʱ�������ݳ���
 * @param[in] {schedule_num} schedule�б�����
 * @param[in] {schedule_data} schedule�б�����
 * @return Null
 * @note   MCU�������� mcu_get_mul_temp_pass �ɹ��󣬸ú����ڿɶ�ηֱ��ȡÿ�����ʱ��������Ч����
 */
static void schedule_temp_pass_data(unsigned char suc_flag, unsigned char pass_ser, 
                               unsigned char pass_valcnt, unsigned char pass_sta, 
                               const unsigned char gl_start[], const unsigned char gl_end[], 
                               const unsigned char pass[], unsigned char pass_len,
                               unsigned char schedule_num, const unsigned char schedule_data[])
{ 
    //#error "��������ɴ�schedule�б���ʱ������Ϣ�������,��ɾ������"

    /*
    suc_flagΪ�Ƿ��ȡ����ɹ���־��Ϊ 0 ��ʾʧ�ܣ�Ϊ 1 ��ʾ�ɹ�
   */
    /*
    ע����ȡ��������ɹ����ú���������Σ�
        ֱ����������ʱ����ȫ����ȡ�������
        ��ʧ����ֻ����һ�Ρ�
   */
    if (suc_flag == 1) {
        //��ȡ��ʱ�������ݳɹ�
        /*
        pass_serΪ������
        pass_valcntΪ������Ч����
        pass_staΪ���뵱ǰ״̬

        gl_startΪ������Ч���ڸ���ʱ��
        gl_start[0]Ϊ��� , 0x00 �� ʾ2000 ��
        gl_start[1]Ϊ�·ݣ��� 1 ��ʼ��12 ����
        gl_start[2]Ϊ���ڣ��� 1 ��ʼ��31 ����
        gl_start[3]Ϊʱ�ӣ��� 0 ��ʼ��23 ����
        gl_start[4]Ϊ���ӣ��� 0 ��ʼ��59 ����
        gl_start[5]Ϊ���ӣ��� 0 ��ʼ��59 ����
        gl_endΪ����������ڸ���ʱ�䣬ͬgl_start

        passָ����ʱ��������(ascll��)������pass_len
        */
        
        //�˴�����������ݴ���
        
        schedule_data_process(schedule_num, schedule_data);
    }else {
        //��ȡ��ʱ�������ݳ���
    }
}

/**
 * @brief  MCU������ʱ����(��schedule�б�)����
 * @param[in] {data} ��������
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
 * @brief  ���߶�̬����
 * @param[in] {green_time} ����ʱ��
                green_time[0]Ϊ��ݣ�0x00 ��ʾ2000 ��
                green_time[1]Ϊ�·ݣ��� 1 ��ʼ��12 ����
                green_time[2]Ϊ���ڣ��� 1 ��ʼ��31 ����
                green_time[3]Ϊʱ�ӣ��� 0 ��ʼ��23 ����
                green_time[4]Ϊ���ӣ��� 0 ��ʼ��59 ����
                green_time[5]Ϊ���ӣ��� 0 ��ʼ��59 ����
 * @param[in] {pw} ���߶�̬����
 * @param[in] {pw_len} ���߶�̬���볤��
 * @return Null
 * @note   MCU��Ҫ���е��ú󣬿���offline_dynamic_password_result�����жԽ�����д���
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
 * @brief  ���߶�̬������
 * @param[in] {result_data} �������
 * @return Null
 * @note   MCU��Ҫ�����е���offline_dynamic_password�������ڴ˺����Խ��յĽ�����д���
 */
void offline_dynamic_password_result(unsigned char result_data[])
{
    //#error "������������߶�̬�������������,��ɾ������"
    unsigned char result; //������ȷ��
    unsigned char type; //��������
    unsigned char decode_len; //���ܺ����ݳ���
    unsigned char decode[DECODE_MAX_LEN]; //��������
    
    result = result_data[0];
    if(0 == result) {
        //��ȷ
    }else {
        //����
        return; //����ʱ���޺�������
    }
    
    type = result_data[1];
    switch(type) {
        case 0:
            //��ʱ��������
        break;
        
        case 1:
            //���ο�������
        break;
        
        case 2:
            //�������
        break;
        
        default:
        break;
    }
    
    decode_len = result_data[2];
    my_memcpy(decode,&result_data[3],decode_len);
    
    //����ӽ������ݴ���
    
}
#endif

#ifdef PICTURE_UPLOAD_ENABLE
/**
 * @brief  ͼƬ�ϴ��¼�״̬֪ͨ
 * @param[in] {event} �¼���Ϣ����
 * @param[in] {picture} �Ƿ�Я��ͼƬ��Ϣ
 * @param[in] {pic_num} �ϴ�ͼƬ����(<=10)
 * @return Null
 * @note   MCU��Ҫ���е��ú󣬿��� picture_event_state_notice_result �����жԽ�����д���
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
 * @brief  ͼƬ�ϴ��¼�״̬֪ͨ���
 * @param[in] {result} ���
 * @return Null
 * @note   MCU��Ҫ�����е��� picture_event_state_notice �������ڴ˺����Խ��յĽ�����д���
 */
void picture_event_state_notice_result(unsigned char result)
{
    //#error "���������ͼƬ�ϴ�ʱ��״̬֪ͨ����������,��ɾ������"
    if (0 == result) {
        //��Ϣ���ճɹ�
    }else {
        //��Ϣ����ʧ��
    }
}

/**
 * @brief  ͼƬ�ϴ�
 * @param[in] {p_time} ʱ������
 * @param[in] {picture_id} ͼƬid��
 * @param[in] {pic_total_num} ͼƬ�ܰ���
 * @param[in] {pic_num} ��ǰ����ţ���0��ʼ��
 * @param[in] {p_pic_data} ͼƬ����
 * @param[in] {pic_data_len} ͼƬ���ݳ���
 * @return Null
 * @note   MCU��Ҫ���е��ú󣬿��� picture_upload_result �����жԽ�����д���
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
 * @brief  ͼƬ�ϴ����
 * @param[in] {result} ���
 * @return Null
 * @note   MCU��Ҫ�����е��� picture_upload �������ڴ˺����Խ��յĽ�����д���
 */
void picture_upload_result(unsigned char result)
{
    //#error "���������ͼƬ�ϴ�����������,��ɾ������"
    if (0 == result) {
        //��Ϣ���ճɹ�
    }else {
        //��Ϣ����ʧ��
    }
}

/**
 * @brief  ͼƬ�ϴ��������
 * @param[in] {p_value} �������
 * @param[in] {data_len} ������ݳ���
 * @return Null
 * @note   MCU��Ҫ���жԽ��յĽ�����д���
 */
void picture_upload_return(unsigned char p_value[], unsigned short data_len)
{
    //#error "���������ͼƬ�ϴ���������������,��ɾ������"
    
    unsigned short pic_id; //ͼƬid��
    unsigned char p_time[6]; //��ǰͼƬ�ϴ���ʱ��
    unsigned char upload_result; //ͼƬ�ϴ����
    
    if(9 != data_len) {
        //���ݳ��ȴ���
        return;
    }
    
    pic_id = (p_value[0] << 8) | p_value[1];
    my_memcpy(p_time, &p_value[2], 6);
    upload_result = p_value[8];
    
    /*
    //�����д��������
    //pic_idΪͼƬid��   
    //p_time[6]���ǵ�ǰͼƬ�ϴ���ʱ�䣬p_time[0]~p_time[5]������ �� �� �� ʱ �� ��
    */
    switch(upload_result) {
        case 0:
            //ͼƬ�ϴ��ɹ�
        break;
        
        case 1:
            //����Ͽ��޷��ϴ��ɹ�
        break;
        
        case 2:
            //ͼƬ̫�󳬳�40k
        break;
        
        case 3:
            //��ʱδ�յ�mcu��ͼƬ��Ϣ
        break;
        
        case 4:
            //����ԭ��ʧ��
        break;
        
        default:break;
    }
    
    unsigned char length = 0;
    
    //���ж��Ƿ��к���ͼƬ�ϴ���ѡ��ظ�����
    
    length = set_wifi_uart_byte(length, 0); //�޺���ͼƬ�ϴ�
    //length = set_wifi_uart_byte(length, 1); //�к���ͼƬ�ϴ�
    
    wifi_uart_write_frame(PICTURE_UPLOAD_RETURN_CMD,length);
}

/**
 * @brief  ͼƬ�ϴ�״̬��ȡ
 * @param  Null
 * @return Null
 * @note   MCU��Ҫ���е��ú󣬿��� picture_upload_state_get_result �����жԽ�����д���
 */
void picture_upload_state_get(void)
{
    wifi_uart_write_frame(PICTURE_UPLOAD_STATE_GET_CMD, 0);
}

/**
 * @brief  ͼƬ�ϴ�״̬��ȡ���
 * @param[in] {p_value} �������
 * @param[in] {data_len} ������ݳ���
 * @return Null
 * @note   MCU��Ҫ�����е��� picture_upload_state_get �������ڴ˺����Խ��յĽ�����д���
 */
void picture_upload_state_get_result(unsigned char p_value[], unsigned short data_len)
{
    //#error "���������ͼƬ�ϴ�״̬��ȡ����������,��ɾ������"
    unsigned char pic_upload_state; //ͼƬ�ϴ�״̬
    unsigned short event_infor; //�¼���Ϣ����
    unsigned short pic_id; //ͼƬid��
    unsigned char p_time[6]; //��ǰͼƬ�ϴ���ʱ��
    
    if(11 != data_len) {
        //���ݳ��ȴ���
        return;
    }
    
    pic_upload_state = p_value[0];
    event_infor = (p_value[1] << 8) | p_value[2];
    pic_id = (p_value[3] << 8) | p_value[4];
    my_memcpy(p_time, &p_value[5], 6);
    
    //���ڴ˴����ȡ��������
    
}

#endif

#ifdef LOCK_KEEP_ALIVE
/**
 * @brief  ����IO����
 * @param[in] Null
 * @return Null
 * @note   MCU��Ҫ����ʵ���߼�����
 */
void keep_alive_awake(void)
{
    //#error "���ڴ���ɱ���IO���Ѳ������룬�Ի���ģ�飬��ɾ������"
    //�˺�������MCU����ģ�飬���ѷ�ʽΪIO�͵�ƽ���ѣ����Ѻ󱣳ֵ͵�ƽ������IO��ο�Ӳ����ơ�
    
}

/**
 * @brief  ��ȡWIFI״̬
 * @param  Null
 * @return Null
 * @note   MCU��Ҫ���е��ú󣬿��� get_wifi_state_result �����жԽ�����д���
 */
void mcu_get_wifi_state(void)
{
    wifi_uart_write_frame(GET_WIFI_STATE_CMD, 0);
}

/**
 * @brief  mcu�յ�WiFi״̬
 * @param[in] {p_data} ����
 * @return Null
 * @note   MCU��Ҫ�����е��� mcu_get_wifi_state �������ڴ˺����Խ��յĽ�����д���
 */
void get_wifi_state_result(const unsigned char p_data[])
{
    //#error "���������wifi״̬�������,��ɾ������"
    switch(p_data[0]) {
        case 0x00:
            //״̬1  smartconfig����״̬
        break;
        
        case 0x01:
            //״̬2  AP����״̬
        break;
        
        case 0x02:
            //״̬3  WIFI�����õ�δ����·����
        break;
        
        case 0x03:
            //״̬4  WIFI������������·����
        break;
        
        case 0x04:
            //״̬5  ������·���������ӵ��ƶ�
        break;
        
        case 0x05:
            //״̬6  WIFI�豸���ڵ͹���ģʽ
        break;
        
        case 0xff:
            //��Ч״̬  ��ʼ��δ���
        break;
        
        default:break;
    }
    
    switch(p_data[1]) {
        case 0x00:
            //δ����
        break;
        
        case 0x01:
            //�Ѽ���
        break;
        
        default:break;
    } 
}
#endif


#ifdef PHOTO_LOCK_PICTURE_UPLOAD_ENABLE
/**
 * @brief  MCU�����¼�����ץ��֪ͨ
 * @param[in] {event_infor_num} �¼���Ϣ����
 * @param[in] {capture_flag} �Ƿ�ץ��ͼƬ
 * @param[in] {capture_type} ץ������
 * @param[in] {capture_num} ץ��ͼƬ����
 * @param[in] {time_flag} ʱ���־λ  0:����ʱ�� 1:����ʱ�� 2:����ʱ��
 * @param[in] {p_time} ʱ������
 * @return Null
 * @note   MCU��Ҫ���е��ú󣬿��� event_trigger_capture_notice_result �����жԽ�����д���
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
 * @brief  �¼�����ץ��֪ͨ���ݴ���
 * @param[in] {result} �������
 * @return Null
 * @note   MCU��Ҫ�����е��� mcu_event_trigger_capture_notice �������ڴ˺����Խ��յĽ�����д���
 */
static void event_trigger_capture_notice_result(unsigned char result)
{
    //#error "����������¼�����ץ��֪ͨ���ݴ������,��ɾ������"
    
    if(0 == result) {
        //��Ϣ���ճɹ�
    }else {
        //��Ϣ����ʧ��
    }
}

/**
 * @brief  ��������ͼƬ�ϴ���ع���
 * @param[in] {p_data} ����
 * @return Null
 * @note   
 */
void photo_lock_picture_upload_func(const unsigned char p_data[])
{
    event_trigger_capture_notice_result(p_data[0]); //�¼�����ץ��֪ͨ
}
#endif

#endif

