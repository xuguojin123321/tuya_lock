
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include "log.h"
#include "usart.h"
#include "rtc.h"

int logd(const char* filename, int lines, const char* functions,const char*format, ...) {
	va_list argPtr;
    char buf[256];
	int count;

    memset(buf,0,sizeof(buf));
	va_start(argPtr, format);                  /*  ��ȡ�ɱ�����б�  */
	count = vsprintf(buf, format, argPtr);  /*  ����Ϣ�������׼�������豸  */
	printf("[%d/%d-%d:%d:%d]",calendar.w_month,calendar.w_date,calendar.hour,calendar.min,calendar.sec);
    printf("[%s][%d][%s]:%s\r\n",filename,lines,functions,buf);
	va_end(argPtr);                            /*  �ɱ�����б����  */
    return 0;
}