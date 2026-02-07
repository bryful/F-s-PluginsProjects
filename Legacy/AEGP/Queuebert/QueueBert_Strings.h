// QueueBert_Strings.h

#pragma once

typedef enum {
	StrID_NONE, 
	StrID_Name,
	StrID_Pronounce,
	StrID_Troubles,
	StrID_NUMTYPES
} StrIDType;

char	*GetStringPtr(int strNum);

#ifdef AE_OS_WIN
#define STR_PATH L"C:\\whee.mov"
#else
#define STR_PATH L"Kinesis:Users:bbullis:Desktop:whee.mov"
#endif
