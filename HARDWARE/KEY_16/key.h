#ifndef __KEY_H
#define __KEY_H


#include "sys.h"
#include "delay.h"

//行 对应键盘上的 R1 R2 R3 R4
#define KEY_Hang1 GPIO_Pin_12
#define KEY_Hang2 GPIO_Pin_13
#define KEY_Hang3 GPIO_Pin_14
#define KEY_Hang4 GPIO_Pin_15

#define HANG1 PBout(12)	// PA8
#define HANG2 PBout(13)	// PD2	
#define HANG3 PBout(14)	// PA8
#define HANG4 PBout(15)	// PD2	
#define KEY_PROT_HANG GPIOB

//列 1 2 3 4   对应键盘上的 C1 C2 C3 C4
#define KEY1 GPIO_Pin_7
#define KEY2 GPIO_Pin_6
#define KEY3 GPIO_Pin_5
#define KEY4 GPIO_Pin_4
#define KEY_PROT_LIE GPIOA

#define key1_Input GPIO_ReadInputDataBit(KEY_PROT_LIE,KEY1) 
#define key2_Input GPIO_ReadInputDataBit(KEY_PROT_LIE,KEY2) 
#define key3_Input GPIO_ReadInputDataBit(KEY_PROT_LIE,KEY3) 
#define key4_Input GPIO_ReadInputDataBit(KEY_PROT_LIE,KEY4) 

void KEY_UserConfig(void);
void KEY_Read(int8_t *key);


#endif
