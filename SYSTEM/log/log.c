
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
	va_start(argPtr, format);                  /*  获取可变参数列表  */
	count = vsprintf(buf, format, argPtr);  /*  将信息输出到标准出错流设备  */
	printf("[%d/%d-%d:%d:%d]",calendar.w_month,calendar.w_date,calendar.hour,calendar.min,calendar.sec);
    printf("[%s][%d][%s]:%s\r\n",filename,lines,functions,buf);
	va_end(argPtr);                            /*  可变参数列表结束  */
    return 0;
}