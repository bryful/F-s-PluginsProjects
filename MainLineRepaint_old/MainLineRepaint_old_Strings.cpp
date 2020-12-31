/* MainLineRepaint_Strings.cpp */

#include "MainLineRepaint_old.h"

typedef struct {
	unsigned long	index;
	char			str[256];
} TableString;

TableString		g_strs[StrID_NUMTYPES] = {
	StrID_NONE,					"",
	StrID_Name,					"F's MainLineRepaint",
	StrID_Description,			"セル画の主線を無くします",
	StrID_MADEBY,				"bry-ful",
	
	StrID_MY_Main_Color,		"主線の色",

	StrID_ERR_getFsAEParams,	"画像バッファーサイズエラーです。御免なさい。",
	StrID_ERR_getParams,	"パラメータエラーです。御免なさい。",
};


char *GetStringPtr(int strNum)
{
	return g_strs[strNum].str;
}
	
