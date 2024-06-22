#ifndef __LOG_H
#define __LOG_H

#include "stdio.h"	
#include "sys.h" 

#define filename(x) strrchr(x,'\\')?strrchr(x,'\\')+1:x

#define LOGD(FORMAT,...) logd(filename(__FILE__),__LINE__,__FUNCTION__,FORMAT,##__VA_ARGS__)

#endif