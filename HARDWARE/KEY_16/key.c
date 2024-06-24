/*
��עCSDN�ʺţ�����_DW	
��ÿ�������Լ�ѧϰ���������⣬һ����ӣ�һ������ɣ�
��ӭ��λ����һ��ѧϰ�������Ҵ�С��������У�

������õĻ����ǵø���һ�����޺͹�עŶ
*/


#include "key.h"
#include "led.h"

void KEY_UserConfig(void){

	GPIO_InitTypeDef GPIO_InitStrcture;//�ṹ��
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB,ENABLE);//ʱ������
	
	//��
	GPIO_InitStrcture.GPIO_Pin = KEY_Hang1 | KEY_Hang2 | KEY_Hang3 | KEY_Hang4;
	GPIO_InitStrcture.GPIO_Speed = GPIO_Speed_50MHz;//50HZ
	GPIO_InitStrcture.GPIO_Mode = GPIO_Mode_Out_PP;//�������
	GPIO_Init(KEY_PROT_HANG,&GPIO_InitStrcture);//��ʼ��
	GPIO_SetBits(KEY_PROT_HANG,KEY_Hang1 | KEY_Hang2 | KEY_Hang3 | KEY_Hang4);// ��  ���ͣ�ɨ��  ���ߣ��ȴ�ɨ��
	
	//�У�����
	GPIO_InitStrcture.GPIO_Pin = KEY1|KEY2|KEY3|KEY4;
	GPIO_InitStrcture.GPIO_Mode = GPIO_Mode_IPU;//��������
	GPIO_Init(KEY_PROT_LIE,&GPIO_InitStrcture);//��ʼ��
	GPIO_SetBits(KEY_PROT_LIE,KEY1|KEY2|KEY3|KEY4);
	
}
//����
void hang1()
{
	HANG1 = 0;
	HANG2 = 1;	
	HANG3 = 1;
	HANG4 = 1;
}
void hang2()
{
	HANG1 = 1;
	HANG2 = 0;	
	HANG3 = 1;
	HANG4 = 1;
}
void hang3()
{
	HANG1 = 1;
	HANG2 = 1;	
	HANG3 = 0;
	HANG4 = 1;
}
void hang4()
{
	HANG1 = 1;
	HANG2 = 1;	
	HANG3 = 1;
	HANG4 = 0;
}
int switch_keyvalue(int8_t value)
{
    switch(value)
    {
        case 1:
        case 2:
        case 3:
            return value;
        case 4:
            return 13;
        case 5:
            return 4;
        case 6:
            return 5;
        case 7:
            return 6;
        case 8:
            return 14;
        case 9:
            return 7;
        case 10:
            return 8;
        case 11:
            return 9;
        case 12:
            return 15;
        case 13:
            return 10;//*
        case 14:
            return 0;
        case 15:
            return 12;//#
        case 16:
            return 16;
        case -1:
            return -1;
    }   

}
void KEY_Read(int8_t *key){
	
	hang1();
	if(key1_Input == RESET||key2_Input == RESET||key3_Input == RESET||key4_Input == RESET){ //0 ����
	
		delay_ms(10);
		if(key1_Input == RESET){
		
			*key=1;
			
			while(!GPIO_ReadInputDataBit(KEY_PROT_LIE,KEY1));//���ּ��
		}
		else if(key2_Input == RESET){
			
			*key=2;
			
			while(!GPIO_ReadInputDataBit(KEY_PROT_LIE,KEY2));//���ּ��
		}
		else if(key3_Input == RESET){
			
			*key=3;
			
			while(!GPIO_ReadInputDataBit(KEY_PROT_LIE,KEY3));//���ּ��
		}
		else if(key4_Input == RESET){
			
			*key=4;
			
			while(!GPIO_ReadInputDataBit(KEY_PROT_LIE,KEY4));//���ּ��
		}
		else{
		    *key = -1;
			GPIO_Write(KEY_PROT_LIE,0X00FF);//��һ������
		}
	
	}
	hang2();
	if(key1_Input == RESET||key2_Input == RESET||key3_Input == RESET||key4_Input == RESET){ //0 ����
	
		delay_ms(10);
		if(key1_Input == RESET){
		
			*key=5;
			
			while(!GPIO_ReadInputDataBit(KEY_PROT_LIE,KEY1));//���ּ��
		}
		else if(key2_Input == RESET){
			
			*key=6;
			
			while(!GPIO_ReadInputDataBit(KEY_PROT_LIE,KEY2));//���ּ��
		}
		else if(key3_Input == RESET){
			
			*key=7;
			
			while(!GPIO_ReadInputDataBit(KEY_PROT_LIE,KEY3));//���ּ��
		}
		else if(key4_Input == RESET){
			
			*key=8;
			
			while(!GPIO_ReadInputDataBit(KEY_PROT_LIE,KEY4));//���ּ��
		}
		else{
		    *key = -1;
			GPIO_Write(KEY_PROT_LIE,0X00FF);//��һ������
		}
	
	}
	hang3();
	if(key1_Input == RESET||key2_Input == RESET||key3_Input == RESET||key4_Input == RESET){ //0 ����
	
		delay_ms(10);
		if(key1_Input == RESET){
		
			*key=9;
			
			while(!GPIO_ReadInputDataBit(KEY_PROT_LIE,KEY1));//���ּ��
		}
		else if(key2_Input == RESET){
			
			*key=10;
			
			while(!GPIO_ReadInputDataBit(KEY_PROT_LIE,KEY2));//���ּ��
		}
		else if(key3_Input == RESET){
			
			*key=11;
			
			while(!GPIO_ReadInputDataBit(KEY_PROT_LIE,KEY3));//���ּ��
		}
		else if(key4_Input == RESET){
			
			*key=12;
			
			while(!GPIO_ReadInputDataBit(KEY_PROT_LIE,KEY4));//���ּ��
		}
		else{
		    *key = -1;
			GPIO_Write(KEY_PROT_LIE,0X00FF);//��һ������
		}
	
	}
	hang4();
	if(key1_Input == RESET||key2_Input == RESET||key3_Input == RESET||key4_Input == RESET){ //0 ����
	
		delay_ms(10);
		if(key1_Input == RESET){
		
			*key=13;
			
			while(!GPIO_ReadInputDataBit(KEY_PROT_LIE,KEY1));//���ּ��
		}
		else if(key2_Input == RESET){
			
			*key=14;
			
			while(!GPIO_ReadInputDataBit(KEY_PROT_LIE,KEY2));//���ּ��
		}
		else if(key3_Input == RESET){
			
			*key=15;
			
			while(!GPIO_ReadInputDataBit(KEY_PROT_LIE,KEY3));//���ּ��
		}
		else if(key4_Input == RESET){
			
			*key=16;
			
			while(!GPIO_ReadInputDataBit(KEY_PROT_LIE,KEY4));//���ּ��
		}
		else{
		    *key = -1;
			GPIO_Write(KEY_PROT_LIE,0X00FF);//��һ������
		}
	
	}
    *key = switch_keyvalue(*key);
	if(*key != -1)
	{
		BEEP = 0;
		delay_ms(50);
		BEEP = 1;
	}
    
}
