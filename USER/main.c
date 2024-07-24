#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "FreeRTOS.h"
#include "task.h"
#include "sys.h"
#include "log.h"
#include "key.h"
#include "oled.h"
#include "FPM383C.h"
#include "iwdg.h"
#include "rtc.h"
#include "wifi.h"
#include "rc522.h"
#include "stmflash.h"
/***************
STM32F103C8T6最小单片机的freertos指纹锁
作者：XGJ
data : 2024/06/22 
***************/

#define START_TASK_PRIO			1
#define START_STK_SIZE			1024+128
TaskHandle_t StartTask_Handler;
void start_task(void *pvParameters);

#define LED0_TASK_PRIO			3
#define LED0_STK_SIZE				256
TaskHandle_t LED0Task_Handler;
void led0_task(void *pvParameters);

#define KEY_TASK_PRIO			2
#define KEY_STK_SIZE				256
TaskHandle_t KEYTask_Handler;
void key_task(void *pvParameters);

#define TUYA_TASK_PRIO			4
#define TUYA_STK_SIZE				256
TaskHandle_t TUYATask_Handler;
void tuya_task(void *pvParameters);

#define user_max_num 5
#define FLASH_SAVE_ADDR  0x0800F000 	//设置FLASH 保存地址(必须为偶数，且其值要大于本代码所占用FLASH的大小+0X08000000)
uint8_t g_finger_mode = 0;
uint8_t g_card_mode = 0;
typedef struct 
{
    char key_password[7];
    char id_card[4];
}password;

password g_password[user_max_num];

static void flash_read()
{
    int i = 0;
	for(i=0;i<user_max_num;i++)
	{
		memset(&g_password[i],0,sizeof(g_password[i]));
	}
	STMFLASH_Read(FLASH_SAVE_ADDR,(u16*)g_password,sizeof(g_password[0])*user_max_num);//储存数据 
	for(i=0;i<user_max_num;i++)
	{
		if(strlen(g_password[i].key_password) != 0)
		{
			LOGD("===%s\n",g_password[i].key_password);
		}
        if(strlen(g_password[i].id_card) != 0)
        {
            cardhex_print(g_password[i].id_card);
        }
	}
}

static void flash_reset()
{
    int i = 0;
	for(i=0;i<user_max_num;i++)
	{
		memset(&g_password[i],0,sizeof(g_password[i]));
	}
    STMFLASH_Write(FLASH_SAVE_ADDR,(u16*)g_password,sizeof(g_password[0])*user_max_num);//储存数据 
    LOGD("flash reset success");	
}
void show_welcome()
{
    OLED_ShowCHinese(0,0,0);
    OLED_ShowCHinese(16,0,1);
    OLED_ShowCHinese(32,0,2);
    OLED_ShowCHinese(48,0,3);
    OLED_ShowCHinese(64,0,4);
}
int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	delay_init();
	uart_init(115200);
	KEY_UserConfig();
	LED_Init();
	OLED_Init();
	OLED_Clear();
    flash_read();
    RC522_Init();
    PcdReset ();//RC522复位
    M500PcdConfigISOType ( 'A' );//设置工作方式
	usart3_init(115200);
	wifi_protocol_init();
	while(RTC_Init())		//RTC初始化	，一定要初始化成功
	{ 
		LOGD("RTC ERROR!   ");	
		delay_ms(800);
		LOGD("RTC Trying...");	
	}	
	FPM383C_Init();
	IWDG_Init(4,625*3);     //与分频数为64,重载值为625,溢出时间为1s	   
	LOGD("init success\n");
	LED0=0;
    OPEN = 0;
    BEEP = 0;
    delay_ms(200);
    BEEP = 1;
    show_welcome();
    g_card_mode = 0;
    g_finger_mode = 0;
	xTaskCreate((TaskFunction_t		)start_task,
							(const char*			)"start_task",
							(uint16_t					)START_STK_SIZE,
							(void*						)NULL,
							(UBaseType_t			)START_TASK_PRIO,
							(TaskHandle_t*		)&StartTask_Handler);

   // while(1){};
	vTaskStartScheduler();
}

void start_task(void *pvParameters)
{
	taskENTER_CRITICAL();
	
	xTaskCreate((TaskFunction_t		)led0_task,
							(const char*			)"led0_task",
							(uint16_t					)LED0_STK_SIZE,
							(void*						)NULL,
							(UBaseType_t			)LED0_TASK_PRIO,
							(TaskHandle_t*		)&LED0Task_Handler);
	
	xTaskCreate((TaskFunction_t		)key_task,
							(const char*			)"key_task",
							(uint16_t					)KEY_STK_SIZE,
							(void*						)NULL,
							(UBaseType_t			)KEY_TASK_PRIO,
							(TaskHandle_t*		)&KEYTask_Handler);
	xTaskCreate((TaskFunction_t		)tuya_task,
							(const char*			)"tuya_task",
							(uint16_t					)TUYA_STK_SIZE,
							(void*						)NULL,
							(UBaseType_t			)TUYA_TASK_PRIO,
							(TaskHandle_t*		)&TUYATask_Handler);
	vTaskDelete(StartTask_Handler);
	taskEXIT_CRITICAL();

}
static void cardid_check(char *cardid);
void led0_task(void *pvParameters)
{
    char *card = NULL;
	LOGD("led task start\r\n");
	while(1)
	{
		if(g_finger_mode == 0 && FPM383C_Loop() == 0)
        {
            open_door();
        }
		vTaskDelay(50);
        if(g_card_mode == 0)
        {
            card = RC522_GetCard();  //射频卡处理程序
            if(card != NULL)
            {
                cardid_check(card);
            }
        }
	}
}
void tuya_task(void *pvParameters)
{
    uint8_t time = 0;
	LOGD("tuya task start\r\n");
	while(1)
	{
        time++;
		if(time >= 3)
        {
           // LED0 = ~LED0;
            IWDG_Feed();//喂狗
            time = 0;
        }
		wifi_uart_service();
		//LOGD("tuya task runing\n");
		vTaskDelay(50);
	}
}
static void cardid_check(char *cardid)
{
    int i = 0;
    int j = 0;
    int count = 0;
    for(j=0;j<user_max_num;j++)
    {
        for(i=0;i<4;i++)
        {
            if(g_password[j].id_card[i] == cardid[i])
            {
                count ++;
            }
        }
        if(count == 4)
        {
            LOGD("open door by");
            cardhex_print(cardid);
            open_door();
            return;
        }
        count = 0;
    }
    LOGD("no this card");
    cardhex_print(cardid);
    return;
}
static void key_password_add()
{
    int i = 0,j=0;
    char buf[7] = {0};
    int8_t value = -1;
    int count = 0;
    int time = 1000;//界面停留5s 
    LOGD("add key password");
    while(time > 0)
    {
        KEY_Read(&value);
        if(value == 10)
        {
            return;
        }
        if(value != -1)
        {
            buf[count] = '0' + value;
            LOGD("value %c", buf[count]);
            OLED_Clear();
            OLED_ShowString(10*count,2,"*",16);
            count++;
            LOGD("count %d",count);
            value = -1;
        }
        if(count == 6)
        {
            for(i=0;i<user_max_num;i++)
            {
                if(strlen(g_password[i].key_password) == 0)
                {
                    snprintf(g_password[i].key_password,sizeof(g_password[i].key_password),"%s",buf);
                    LOGD("set key success %d %d",strlen(buf),strlen(g_password[i].key_password));
                    LOGD("set key success %s",buf);
                    LOGD("set key success %s",g_password[i].key_password);
                    STMFLASH_Write(FLASH_SAVE_ADDR,(u16*)g_password,sizeof(g_password[0])*user_max_num);//储存数据 
                    return;
                }
            }
            LOGD("key buf null");
            return;
        }
        vTaskDelay(10);
        time--;
    }
    if(time == 0)
    {
        LOGD("time out");
    }
}
static void idcard_add()
{
    int i = 0;
    int8_t value = 0;
    int time = 500;//界面停留5s 
    char *card = NULL;
    OLED_Clear();
    OLED_ShowString(0,4,"input card",16);
    g_card_mode = 1;
    while(time > 0)
    {
        KEY_Read(&value);
        if(value == 10)
        {
            g_card_mode = 0;
            return;
        }
        card = RC522_GetCard();
        if(card != NULL)
        {
            for(i=0;i<user_max_num;i++)
            {
                if(strlen(g_password[i].id_card) == 0)
                {
                    my_memcpy(g_password[i].id_card, card,sizeof(card));
                    LOGD("add card id %02x%02x%02x%02x\r\n",g_password[i].id_card[0],g_password[i].id_card[1],g_password[i].id_card[2],g_password[i].id_card[3]);
                    STMFLASH_Write(FLASH_SAVE_ADDR,(u16*)g_password,sizeof(g_password[0])*user_max_num);//储存数据 
                    g_card_mode = 0;
                    return;
                }
            }
        }
        vTaskDelay(10);
        time--;
    }
    if(time == 0)
    {
        LOGD(">>>超时退出<<<");
    }
    g_card_mode = 0;
    return;
}
static void finger_add()
{
    int8_t value = -1;
    int time = 500;//界面停留5s 
    LOGD("————注册指纹模式————");
    g_finger_mode = 1;
    //清除之前数据的残留，为下一次接收数据准备 

    LOGD(">>>请先输入指纹ID<<<");
    
    OLED_Clear();
    OLED_ShowString(0,4,"input finger value",16);
    
    while(time > 0)
    {
        KEY_Read(&value);
        if(value == 10)
        {
            g_finger_mode = 0;
            return;
        }
        if(value != -1)
        {
            LOGD(">>>输入的指纹ID：%d<<<",value);
            FPM383C_Add(value);
            break;
        }
        vTaskDelay(10);
        time--;
    }
    if(time == 0)
    {
        LOGD(">>>超时退出<<<");
    }
    g_finger_mode = 0;
    /* 将状态标志位置位，传给下面的Switch函数调用对应的功能  */
}
static void add_type(int8_t type)
{
    switch(type)
    {
        case 1://添加密码
            key_password_add();
            break;
        case 2://添加刷卡 
            idcard_add();
            break;
        case 3://指纹
            finger_add();
            break;
        default:
            break;
    }
}

static void del_type(int8_t type)
{
    int i = 0;
    switch(type)
    {
        case 1://添加密码
            for(i=0;i<user_max_num;i++)
            {
                my_memset(g_password[i].key_password,0,sizeof(g_password[i].key_password));
                STMFLASH_Write(FLASH_SAVE_ADDR,(u16*)g_password,sizeof(g_password[0])*user_max_num);//储存数据 
            }
            LOGD("del key");
            break;
        case 2://添加刷卡 
            for(i=0;i<user_max_num;i++)
            {
                my_memset(g_password[i].id_card,0,sizeof(g_password[i].id_card));
                STMFLASH_Write(FLASH_SAVE_ADDR,(u16*)g_password,sizeof(g_password[0])*user_max_num);//储存数据 
            }
            LOGD("del card");
            break;
        case 3://指纹
            FPM383C_Empty(5);
            LOGD("del finger");
            break;
        default:
            break;
    }
}
static void user_add()
{
    int8_t value = -1;
    int time = 500;//界面停留5s 
    LOGD("choose user add type");
    
    OLED_Clear();
    OLED_ShowString(0,0,"input add type",16);
    OLED_ShowString(0,2,"1  key",16);
    OLED_ShowString(0,4,"2  card",16);
    OLED_ShowString(0,6,"3  finger",16);
    while(time > 0)
    {
        KEY_Read(&value);
        if(value == 10)
        {
            return;
        }
        if(value != -1)
        {
            add_type(value);
            break;
        }
        vTaskDelay(10);
        time--;
    }
    if(time == 0)
    {
        LOGD("time out");
    }
}

static void user_del()
{
    int8_t value = -1;
    int time = 500;//界面停留5s 
    LOGD("choose user del type");
    
    OLED_Clear();
    OLED_ShowString(0,0,"input del type",16);
    OLED_ShowString(0,2,"1  key",16);
    OLED_ShowString(0,4,"2  card",16);
    OLED_ShowString(0,6,"3  finger",16);
    while(time > 0)
    {
        KEY_Read(&value);
        if(value == 10)
        {
            return;
        }
        if(value != -1)
        {
            del_type(value);
            break;
        }
        vTaskDelay(10);
        time--;
    }
    if(time == 0)
    {
        LOGD("time out");
    }
}

static int keypassword_open_process(int8_t keyvalue)
{
    int i = 0,j=0;
    char buf[7] = {0};
    int8_t value = -1;
    int count = 0;
    int time = 1000;//界面停留5s 
    LOGD("open key password");

    if(keyvalue<9)
    {
        buf[0] = '0' + keyvalue;
        count++;
        OLED_ShowString(32,2,"*",16);
    }

    while(time > 0)
    {
        KEY_Read(&value);
        if(value == 10)
        {
            return;
        }
        if(value != -1)
        {
            buf[count] = '0' + value;
            LOGD("value %c", buf[count]);
            LOGD("count %d",count);
            OLED_ShowString(32+10*count,2,"*",16);
            count++;
        }
        if(count == 6)
        {
            for(i=0;i<user_max_num;i++)
            {
                if(strlen(g_password[i].key_password) != 0)
                {
                    if(strcmp(g_password[i].key_password,buf) == 0)
                    {
                        LOGD("open success %s",g_password[i].key_password);
                        return 0;
                    }
                }
            }
            LOGD("open failed %s",buf);
            return;
        }
        vTaskDelay(10);
        time--;
    }
    if(time == 0)
    {
        LOGD("time out");
    }
    return -1;
}
static void key_process(int8_t value)
{
	switch(value)
	{
		case 13:
            {
                OLED_Clear();
                OLED_ShowString(0,0,"config wifi",16);
                if(keypassword_open_process(value) == 0)
                {
                    flash_reset();
                    mcu_reset_wifi();
                    BEEP = 0;
                    vTaskDelay(100);
                    BEEP = 1;
                    vTaskDelay(100);
                    BEEP = 0;
                    vTaskDelay(100);
                    BEEP = 1;
                    mcu_set_wifi_mode(SMART_CONFIG);
                    LOGD("按键配网 SMART_CONFIG %d\r\n",set_wifimode_flag);
                    LOGD("%02x\n",mcu_get_wifi_work_state());
                }
            }
            break;
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        {
            OLED_Clear();
            OLED_ShowString(0,0,"open door",16);
            if(keypassword_open_process(value) == 0)
            {
                open_door();
            }
        }
        break;
		case 12:
        {
            OLED_Clear();
            OLED_ShowString(0,0,"add user",16);
            if(keypassword_open_process(value) == 0)
            {
                user_add();
            }
        }
        break;
        case 14:
        {
            OLED_Clear();
            OLED_ShowString(0,0,"del user",16);
            if(keypassword_open_process(value) == 0)
            {
                user_del();
            }
        }
        break;
	}
}
void key_task(void *pvParameters)
{
	int8_t value = -1;
	char buf[20];
	LOGD("key task start");
	while(1)
	{
		//wifi_uart_service();
		KEY_Read(&value);
		if(g_finger_mode == 0 && value != -1)
		{
			memset(buf,0,sizeof(buf));
			snprintf(buf,sizeof(buf),"key is %d    ",value);
			LOGD(buf);
			//OLED_ShowString(0,0,buf,16);
		    key_process(value);
            OLED_Clear();
            show_welcome();
        }
		vTaskDelay(10);
	}
}




