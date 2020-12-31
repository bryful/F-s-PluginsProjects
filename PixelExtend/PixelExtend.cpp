//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------


#include "PixelExtend.h"
#include <stdio.h>

//-------------------------------------------------------------------------------------------------
//AfterEffextsにパラメータを通達する
//Param_Utils.hを参照のこと
static PF_Err ParamsSetup (
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output)
{
	PF_Err			err = PF_Err_NONE;
	PF_ParamDef		def;
	char num[255] = { "\0" };

	A_u_char defCol[8][3]=
	{
		{0x00,0x00,0x00},
		{0xFF,0x00,0x00},
		{0x00,0xFF,0x00},
		{0x00,0x00,0xFF},
		{0xFF,0xFF,0x00},
		{0xFF,0x00,0xFF},
		{0x00,0xFF,0xFF},
		{0xFF,0xFF,0xFF}
	};

	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_TOPIC(STR_TARGET_TOPIC, ID_TARGET_TOPIC);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_RANGE,	//Name
		0,						//VALID_MIN
		100,						//VALID_MAX
		0,						//SLIDER_MIN
		10,						//SLIDER_MAX
		1,						//CURVE_TOLERANCE
		0,						//DFLT
		1,						//PREC
		0,						//DISP
		0,						//WANT_PHASE
		ID_RANGE
	);
	//----------------------------------------------------------------
	for (int i = 0; i < 8; i++)
	{
		//----------------------------------------------------------------
#ifdef AE_OS_WIN
		sprintf_s(num, "TargetColorEnabled%d", i);
#else
		sprintf(num, "TargetColorEnabled%d", i);
#endif
		AEFX_CLR_STRUCT(def);
		def.flags = PF_ParamFlag_CANNOT_TIME_VARY;//これをつけるとキーフレームが撃てなくなる
		PF_ADD_CHECKBOX(num,
			STR_ON,
			FALSE,
			0,
			ID_TARGET_COLOR0_ENABLED + i * 2
		);
		//----------------------------------------------------------------
#ifdef AE_OS_WIN
		sprintf_s(num, "TargetColor%d", i);
#else
		sprintf(num, "TargetColor%d", i);
#endif
		//色の指定
		AEFX_CLR_STRUCT(def);
		def.flags = PF_ParamFlag_CANNOT_TIME_VARY;//これをつけるとキーフレームが撃てなくなる
		PF_ADD_COLOR(num,
			defCol[i][0],
			defCol[i][1],
			defCol[i][2],
			ID_TARGET_COLOR0_ENABLED + i * 2 + 1
		);
	}
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_END_TOPIC(ID_TARGET_TOPIC_END);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_TOPIC(STR_DIR_TOPIC, ID_DIR_TOPIC);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(STR_TOP,	//パラメータの名前
		0, 		//数値入力する場合の最小値
		EXPAND_MAX,			//数値入力する場合の最大値
		0,				//スライダーの最小値 
		EXPAND_MAX,			//スライダーの最大値
		0,				//デフォルトの値
		ID_TOP
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(STR_TOP_RIGHT,	//パラメータの名前
		0, 		//数値入力する場合の最小値
		EXPAND_MAX,			//数値入力する場合の最大値
		0,				//スライダーの最小値 
		EXPAND_MAX,			//スライダーの最大値
		0,				//デフォルトの値
		ID_TOP_RIGHT
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(STR_RIGHT,	//パラメータの名前
		0, 		//数値入力する場合の最小値
		EXPAND_MAX,			//数値入力する場合の最大値
		0,				//スライダーの最小値 
		EXPAND_MAX,			//スライダーの最大値
		0,				//デフォルトの値
		ID_RIGHT
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(STR_BOTTOM_RIGHT,	//パラメータの名前
		0, 		//数値入力する場合の最小値
		EXPAND_MAX,			//数値入力する場合の最大値
		0,				//スライダーの最小値 
		EXPAND_MAX,			//スライダーの最大値
		0,				//デフォルトの値
		ID_BOTTOM_RIGHT
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(STR_BOTTOM,	//パラメータの名前
		0, 		//数値入力する場合の最小値
		EXPAND_MAX,			//数値入力する場合の最大値
		0,				//スライダーの最小値 
		EXPAND_MAX,			//スライダーの最大値
		0,				//デフォルトの値
		ID_BOTTOM
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(STR_BOTTOM_LEFT,	//パラメータの名前
		0, 		//数値入力する場合の最小値
		EXPAND_MAX,			//数値入力する場合の最大値
		0,				//スライダーの最小値 
		EXPAND_MAX,			//スライダーの最大値
		0,				//デフォルトの値
		ID_BOTTOM_LEFT
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(STR_LEFT,	//パラメータの名前
		0, 		//数値入力する場合の最小値
		EXPAND_MAX,			//数値入力する場合の最大値
		0,				//スライダーの最小値 
		EXPAND_MAX,			//スライダーの最大値
		0,				//デフォルトの値
		ID_LEFT
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(STR_TOP_LEFT,	//パラメータの名前
		0, 		//数値入力する場合の最小値
		EXPAND_MAX,			//数値入力する場合の最大値
		0,				//スライダーの最小値 
		EXPAND_MAX,			//スライダーの最大値
		0,				//デフォルトの値
		ID_TOP_LEFT
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_END_TOPIC(ID_DIR_TOPIC_END);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_TOPIC(STR_NONE_TOPIC, ID_NONE_TOPIC);
	//----------------------------------------------------------------
	for (int i = 0; i < 8; i++)
	{
		//----------------------------------------------------------------
#ifdef AE_OS_WIN
		sprintf_s(num, "NoneColorEnabled%d", i);
#else
		sprintf(num, "NoneColorEnabled%d", i);
#endif
		AEFX_CLR_STRUCT(def);
		def.flags = PF_ParamFlag_CANNOT_TIME_VARY;//これをつけるとキーフレームが撃てなくなる
		PF_ADD_CHECKBOX(num,
			STR_ON,
			FALSE,
			0,
			ID_NONE_COLOR0_ENABLED + i * 2
		);
		//----------------------------------------------------------------
#ifdef AE_OS_WIN
		sprintf_s(num, "NoneColor%d", i);
#else
		sprintf(num, "NoneColor%d", i);
#endif
		//色の指定
		AEFX_CLR_STRUCT(def);
		def.flags = PF_ParamFlag_CANNOT_TIME_VARY;//これをつけるとキーフレームが撃てなくなる
		PF_ADD_COLOR(num,
			defCol[i][0],
			defCol[i][1],
			defCol[i][2],
			ID_NONE_COLOR0_ENABLED + i * 2 + 1
		);
	}
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_END_TOPIC(ID_NONE_TOPIC_END);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_TARGET_ONLY,
		STR_ON,
		FALSE,
		0,
		ID_TARGET_ONLY
	);
	//----------------------------------------------------------------
	out_data->num_params = 	ID_NUM_PARAMS;

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err
HandleChangedParam(
	PF_InData					*in_data,
	PF_OutData					*out_data,
	PF_ParamDef					*params[],
	PF_LayerDef					*outputP,
	PF_UserChangedParamExtra	*extraP)
{
	PF_Err				err = PF_Err_NONE;
	return err;
}
//-----------------------------------------------------------------------------------
static PF_Err
QueryDynamicFlags(	
	PF_InData		*in_data,	
	PF_OutData		*out_data,	
	PF_ParamDef		*params[],	
	void			*extra)	
{
	PF_Err 	err = PF_Err_NONE;
	return err;
}
//-------------------------------------------------------------------------------------------------
static A_long InColor8(PF_Pixel *table, PF_Pixel c, A_long cnt,A_u_char range)
{
	A_long ret = -1;

	if (cnt <= 0) return ret;

	for (A_long i = 0; i < cnt; i++)
	{
		
		if (compPix8Lv(table[i], c, range) == TRUE)
		{
			ret = i;
			break;
		}
	}
	return ret;
}
//-------------------------------------------------------------------------------------------------
static A_long InColor16(PF_Pixel16 *table, PF_Pixel16 c, A_long cnt, A_u_short range)
{
	A_long ret = -1;

	if (cnt <= 0) return ret;

	for (A_long i = 0; i < cnt; i++)
	{
		if (compPix16Lv(table[i], c, range) == TRUE)
		{
			ret = i;
			break;
		}
	}
	return ret;
}
//-------------------------------------------------------------------------------------------------
static A_long InColor32(PF_Pixel32 *table, PF_Pixel32 c, A_long cnt, PF_FpShort range)
{
	A_long ret = -1;

	if (cnt <= 0) return ret;

	for (A_long i = 0; i < cnt; i++)
	{
		if (compPix32Lv(table[i], c, range) == TRUE)
		{
			ret = i;
			break;
		}
	}
	return ret;
}
//-------------------------------------------------------------------------------------------------
static PF_Err
TargetPixelFrom8(
	refconType		refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel8	*inP,
	PF_Pixel8	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo *	infoP = reinterpret_cast<ParamInfo*>(refcon);
	PF_Pixel w = { PF_MAX_CHAN8,PF_MAX_CHAN8 ,PF_MAX_CHAN8 ,PF_MAX_CHAN8 };
	w.alpha = 0;

	A_long idx = InColor8(infoP->colTable, *inP, infoP->colTableCount, infoP->range8);
	if (idx>=0)
	{
		*outP = infoP->colTable[idx];
	}
	else
	{
		*outP = w;
	}
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err
TargetPixelFrom16(
	refconType		refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel16	*inP,
	PF_Pixel16	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo *	infoP = reinterpret_cast<ParamInfo*>(refcon);
	PF_Pixel16 w = { PF_MAX_CHAN16,PF_MAX_CHAN16 ,PF_MAX_CHAN16 ,PF_MAX_CHAN16 };
	w.alpha = 0;

	A_long idx = InColor16(infoP->colTable16, *inP, infoP->colTableCount, infoP->range16);
	if (idx >= 0)
	{
		*outP = infoP->colTable16[idx];
	}
	else
	{
		*outP = w;
	}
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err
TargetPixelFrom32(
	refconType		refcon,
	A_long		xL,
	A_long		yL,
	PF_PixelFloat	*inP,
	PF_PixelFloat	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo *	infoP = reinterpret_cast<ParamInfo*>(refcon);
	PF_PixelFloat w = { 1,1 ,1 ,1 };
	w.alpha = 0;

	A_long idx = InColor32(infoP->colTable32, *inP, infoP->colTableCount, infoP->range32);
	if (idx >= 0)
	{
		*outP = infoP->colTable32[idx];
	}
	else
	{
		*outP = w;
	}
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err
TranCopy8(
	refconType		refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel8	*inP,
	PF_Pixel8	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo *	infoP = reinterpret_cast<ParamInfo*>(refcon);
	
	if (outP->alpha <PF_MAX_CHAN8)
	{
		*outP = *inP;
	}

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err
TranCopy16(
	refconType		refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel16	*inP,
	PF_Pixel16	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo *	infoP = reinterpret_cast<ParamInfo*>(refcon);

	if (outP->alpha < PF_MAX_CHAN16)
	{
		*outP = *inP;
	}

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err
TranCopy32(
	refconType		refcon,
	A_long		xL,
	A_long		yL,
	PF_PixelFloat	*inP,
	PF_PixelFloat	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo *	infoP = reinterpret_cast<ParamInfo*>(refcon);

	if (outP->alpha < 1.0)
	{
		*outP = *inP;
	}

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err
PixelChk8(
	refconType		refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel8	*inP,
	PF_Pixel8	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo *	infoP = reinterpret_cast<ParamInfo*>(refcon);
	PF_Pixel *target = NULL;
	PF_Pixel *target2 = NULL;
	A_long x = 0;
	A_long y = 0;

	if (outP->alpha != PF_MAX_CHAN8) return err;
	y = yL + 1;
	target = outP + infoP->widthTrue;
	if (y < infoP->height)
	{
		if (target->alpha == PF_MAX_CHAN8) return err;
		x = xL - 1;
		if (x <0 )return err;
		target2 = target - 1;
		if (target2->alpha != PF_MAX_CHAN8) return err;
		x = xL + 1;
		if (x >= infoP->width)return err;
		target2 = target + 1;
		if (target2->alpha != PF_MAX_CHAN8) return err;
		y++;
		target += infoP->widthTrue;
		if (y >= infoP->height) return err;
		if (target->alpha == PF_MAX_CHAN8)
		{
			target2 = outP + infoP->widthTrue;
			*target2 = *outP;
		}
	}
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err
PixelChk16(
	refconType		refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel16	*inP,
	PF_Pixel16	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo *	infoP = reinterpret_cast<ParamInfo*>(refcon);
	PF_Pixel16 *target = NULL;
	PF_Pixel16 *target2 = NULL;
	A_long x = 0;
	A_long y = 0;

	if (outP->alpha != PF_MAX_CHAN16) return err;
	y = yL + 1;
	target = outP + infoP->widthTrue;
	if (y < infoP->height)
	{
		if (target->alpha == PF_MAX_CHAN16) return err;
		x = xL - 1;
		if (x < 0)return err;
		target2 = target - 1;
		if (target2->alpha != PF_MAX_CHAN16) return err;
		x = xL + 1;
		if (x >= infoP->width)return err;
		target2 = target + 1;
		if (target2->alpha != PF_MAX_CHAN16) return err;
		y++;
		target += infoP->widthTrue;
		if (y >= infoP->height) return err;
		if (target->alpha == PF_MAX_CHAN16)
		{
			target2 = outP + infoP->widthTrue;
			*target2 = *outP;
		}
	}
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err
PixelChk32(
	refconType		refcon,
	A_long		xL,
	A_long		yL,
	PF_PixelFloat	*inP,
	PF_PixelFloat	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo *	infoP = reinterpret_cast<ParamInfo*>(refcon);
	PF_PixelFloat *target = NULL;
	PF_PixelFloat *target2 = NULL;
	A_long x = 0;
	A_long y = 0;

	if (outP->alpha != 1.0) return err;
	y = yL + 1;
	target = outP + infoP->widthTrue;
	if (y < infoP->height)
	{
		if (target->alpha == 1.0) return err;
		x = xL - 1;
		if (x < 0)return err;
		target2 = target - 1;
		if (target2->alpha != 1.0) return err;
		x = xL + 1;
		if (x >= infoP->width)return err;
		target2 = target + 1;
		if (target2->alpha != 1.0) return err;
		y++;
		target += infoP->widthTrue;
		if (y >= infoP->height) return err;
		if (target->alpha == 1.0)
		{
			target2 = outP + infoP->widthTrue;
			*target2 = *outP;
		}
	}
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err
NoDrawPixel8(
	refconType		refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel8	*inP,
	PF_Pixel8	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo *	infoP = reinterpret_cast<ParamInfo*>(refcon);
	PF_Pixel w = { PF_MAX_CHAN8,PF_MAX_CHAN8 ,PF_MAX_CHAN8 ,PF_MAX_CHAN8 };
	w.alpha = 0;

	A_long idx = InColor8(infoP->ncolTable, *inP, infoP->ncolTableCount, infoP->range8);
	if (idx >= 0)
	{
		*outP = w;
	}
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err
NoDrawPixel16(
	refconType		refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel16	*inP,
	PF_Pixel16	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo *	infoP = reinterpret_cast<ParamInfo*>(refcon);
	PF_Pixel16 w = { PF_MAX_CHAN16,PF_MAX_CHAN16 ,PF_MAX_CHAN16 ,PF_MAX_CHAN16 };
	w.alpha = 0;

	A_long idx = InColor16(infoP->ncolTable16, *inP, infoP->ncolTableCount, infoP->range16);
	if (idx >= 0)
	{
		*outP = w;
	}
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err
NoDrawPixel32(
	refconType		refcon,
	A_long		xL,
	A_long		yL,
	PF_PixelFloat	*inP,
	PF_PixelFloat	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo *	infoP = reinterpret_cast<ParamInfo*>(refcon);
	PF_PixelFloat w = { 1,1 ,1 ,1 };
	w.alpha = 0;

	A_long idx = InColor32(infoP->ncolTable32, *inP, infoP->ncolTableCount, infoP->range32);
	if (idx >= 0)
	{
		*outP = w;
	}
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
ExpandPixel8 (
	refconType		refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel8	*inP, 
	PF_Pixel8	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo *	infoP		= reinterpret_cast<ParamInfo*>(refcon);

	if(inP->alpha==PF_MAX_CHAN8)
	{
		*outP = *inP;
		return err;
	}
	A_long x = xL;
	A_long y = yL;
	A_long len = 0;
	PF_Pixel8	*target = NULL;
	A_long idx = -1;

#pragma region TopExec
	x = xL;
	y = yL;
	len = infoP->lenTable[LEN_TOP];
	target = inP;
	if (len > 0)
	{
		for (A_long i = 0; i < len; i++)
		{
			y++;
			target += infoP->widthTrue;
			if (y < infoP->height)
			{
				if (target->alpha == PF_MAX_CHAN8)
				{
					*outP = *target;
					break;

				}
			}
			else {
				break;
			}
		}
	}
#pragma endregion

#pragma region TopRightExec
	x = xL;
	y = yL;
	len = infoP->lenTable[LEN_TOP_RIGHT];
	target = inP;
	if (len > 0)
	{
		for (A_long i = 0; i < len; i++)
		{
			x--;
			y++;
			target += infoP->widthTrue -1;
			if ((x>=0)&&(y < infoP->height))
			{
				if (target->alpha == PF_MAX_CHAN8)
				{
					*outP = *target;
					break;

				}
			}
			else {
				break;
			}
		}
	}
#pragma endregion


#pragma region RightExec
	x = xL;
	y = yL;
	len = infoP->lenTable[LEN_RIGHT];
	target = inP;
	if (len > 0)
	{
		for (A_long i = 0; i < len; i++)
		{
			x--;
			target += -1;
			if (x>=0)
			{
				if (target->alpha == PF_MAX_CHAN8)
				{
					*outP = *target;
					break;

				}
			}
			else {
				break;
			}
		}
	}
#pragma endregion

#pragma region BottomRightExec
	x = xL;
	y = yL;
	len = infoP->lenTable[LEN_BOTTOM_RIGHT];
	target = inP;
	if (len > 0)
	{
		for (A_long i = 0; i < len; i++)
		{
			x--;
			y--;
			target += -infoP->widthTrue -1;
			if ((x>=0)&&(y >=0))
			{
				if (target->alpha == PF_MAX_CHAN8)
				{
					*outP = *target;
					break;

				}
			}
			else {
				break;
			}
		}
	}
#pragma endregion


#pragma region BottomExec
	x = xL;
	y = yL;
	len = infoP->lenTable[LEN_BOTTOM];
	target = inP;
	if (len > 0)
	{
		for (A_long i = 0; i < len; i++)
		{
			y--;
			target += -infoP->widthTrue;
			if (y >=0)
			{
				if (target->alpha == PF_MAX_CHAN8)
				{
					*outP = *target;
					break;

				}
			}
			else {
				break;
			}
		}
	}
#pragma endregion

#pragma region BottomLeftExec
	x = xL;
	y = yL;
	len = infoP->lenTable[LEN_BOTTOM_LEFT];
	target = inP;
	if (len > 0)
	{
		for (A_long i = 0; i < len; i++)
		{
			x++;
			y--;
			target += -infoP->widthTrue + 1;
			if ((x<infoP->width)&&(y >= 0 ))
			{
				if (target->alpha == PF_MAX_CHAN8)
				{
					*outP = *target;
					break;

				}
			}
			else {
				break;
			}
		}
	}
#pragma endregion


#pragma region LeftExec
	x = xL;
	y = yL;
	len = infoP->lenTable[LEN_LEFT];
	target = inP;
	if (len > 0)
	{
		for (A_long i = 0; i < len; i++)
		{
			x++;
			target += 1;
			if (x<infoP->width)
			{
				if (target->alpha == PF_MAX_CHAN8)
				{
					*outP = *target;
					break;

				}
			}
			else {
				break;
			}
		}
	}
#pragma endregion

#pragma region TopLeftExec
	x = xL;
	y = yL;
	len = infoP->lenTable[LEN_TOP_LEFT];
	target = inP;
	if (len > 0)
	{
		for (A_long i = 0; i < len; i++)
		{
			x++;
			y++;
			target += infoP->widthTrue + 1;
			if ((x < infoP->width) && (y < infoP->height))
			{
				if (target->alpha == PF_MAX_CHAN8)
				{
					*outP = *target;
					break;

				}
			}
			else {
				break;
			}
		}
	}
#pragma endregion
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err
ExpandPixel16(
	refconType		refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel16	*inP,
	PF_Pixel16	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo *	infoP = reinterpret_cast<ParamInfo*>(refcon);

	if (inP->alpha == PF_MAX_CHAN16)
	{
		*outP = *inP;
		return err;
	}
	A_long x = xL;
	A_long y = yL;
	A_long len = 0;
	PF_Pixel16	*target = NULL;
	A_long idx = -1;

#pragma region TopExec
	x = xL;
	y = yL;
	len = infoP->lenTable[LEN_TOP];
	target = inP;
	if (len > 0)
	{
		for (A_long i = 0; i < len; i++)
		{
			y++;
			target += infoP->widthTrue;
			if (y < infoP->height)
			{
				if (target->alpha == PF_MAX_CHAN16)
				{
					*outP = *target;
					break;

				}
			}
			else {
				break;
			}
		}
	}
#pragma endregion

#pragma region TopRightExec
	x = xL;
	y = yL;
	len = infoP->lenTable[LEN_TOP_RIGHT];
	target = inP;
	if (len > 0)
	{
		for (A_long i = 0; i < len; i++)
		{
			x--;
			y++;
			target += infoP->widthTrue - 1;
			if ((x >= 0) && (y < infoP->height))
			{
				if (target->alpha == PF_MAX_CHAN16)
				{
					*outP = *target;
					break;

				}
			}
			else {
				break;
			}
		}
	}
#pragma endregion


#pragma region RightExec
	x = xL;
	y = yL;
	len = infoP->lenTable[LEN_RIGHT];
	target = inP;
	if (len > 0)
	{
		for (A_long i = 0; i < len; i++)
		{
			x--;
			target += -1;
			if (x >= 0)
			{
				if (target->alpha == PF_MAX_CHAN16)
				{
					*outP = *target;
					break;

				}
			}
			else {
				break;
			}
		}
	}
#pragma endregion

#pragma region BottomRightExec
	x = xL;
	y = yL;
	len = infoP->lenTable[LEN_BOTTOM_RIGHT];
	target = inP;
	if (len > 0)
	{
		for (A_long i = 0; i < len; i++)
		{
			x--;
			y--;
			target += -infoP->widthTrue - 1;
			if ((x >= 0) && (y >= 0))
			{
				if (target->alpha == PF_MAX_CHAN16)
				{
					*outP = *target;
					break;

				}
			}
			else {
				break;
			}
		}
	}
#pragma endregion


#pragma region BottomExec
	x = xL;
	y = yL;
	len = infoP->lenTable[LEN_BOTTOM];
	target = inP;
	if (len > 0)
	{
		for (A_long i = 0; i < len; i++)
		{
			y--;
			target += -infoP->widthTrue;
			if (y >= 0)
			{
				if (target->alpha == PF_MAX_CHAN16)
				{
					*outP = *target;
					break;

				}
			}
			else {
				break;
			}
		}
	}
#pragma endregion

#pragma region BottomLeftExec
	x = xL;
	y = yL;
	len = infoP->lenTable[LEN_BOTTOM_LEFT];
	target = inP;
	if (len > 0)
	{
		for (A_long i = 0; i < len; i++)
		{
			x++;
			y--;
			target += -infoP->widthTrue + 1;
			if ((x < infoP->width) && (y >= 0))
			{
				if (target->alpha == PF_MAX_CHAN16)
				{
					*outP = *target;
					break;

				}
			}
			else {
				break;
			}
		}
	}
#pragma endregion


#pragma region LeftExec
	x = xL;
	y = yL;
	len = infoP->lenTable[LEN_LEFT];
	target = inP;
	if (len > 0)
	{
		for (A_long i = 0; i < len; i++)
		{
			x++;
			target += 1;
			if (x < infoP->width)
			{
				if (target->alpha == PF_MAX_CHAN16)
				{
					*outP = *target;
					break;

				}
			}
			else {
				break;
			}
		}
	}
#pragma endregion

#pragma region TopLeftExec
	x = xL;
	y = yL;
	len = infoP->lenTable[LEN_TOP_LEFT];
	target = inP;
	if (len > 0)
	{
		for (A_long i = 0; i < len; i++)
		{
			x++;
			y++;
			target += infoP->widthTrue + 1;
			if ((x < infoP->width) && (y < infoP->height))
			{
				if (target->alpha == PF_MAX_CHAN16)
				{
					*outP = *target;
					break;

				}
			}
			else {
				break;
			}
		}
	}
#pragma endregion
	return err;
}

//-------------------------------------------------------------------------------------------------
static PF_Err
ExpandPixel32(
	refconType		refcon,
	A_long		xL,
	A_long		yL,
	PF_PixelFloat	*inP,
	PF_PixelFloat	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo *	infoP = reinterpret_cast<ParamInfo*>(refcon);

	if (inP->alpha == PF_MAX_CHAN16)
	{
		*outP = *inP;
		return err;
	}
	A_long x = xL;
	A_long y = yL;
	A_long len = 0;
	PF_PixelFloat	*target = NULL;
	A_long idx = -1;

#pragma region TopExec
	x = xL;
	y = yL;
	len = infoP->lenTable[LEN_TOP];
	target = inP;
	if (len > 0)
	{
		for (A_long i = 0; i < len; i++)
		{
			y++;
			target += infoP->widthTrue;
			if (y < infoP->height)
			{
				if (target->alpha == 1)
				{
					*outP = *target;
					break;

				}
			}
			else {
				break;
			}
		}
	}
#pragma endregion

#pragma region TopRightExec
	x = xL;
	y = yL;
	len = infoP->lenTable[LEN_TOP_RIGHT];
	target = inP;
	if (len > 0)
	{
		for (A_long i = 0; i < len; i++)
		{
			x--;
			y++;
			target += infoP->widthTrue - 1;
			if ((x >= 0) && (y < infoP->height))
			{
				if (target->alpha == 1)
				{
					*outP = *target;
					break;

				}
			}
			else {
				break;
			}
		}
	}
#pragma endregion


#pragma region RightExec
	x = xL;
	y = yL;
	len = infoP->lenTable[LEN_RIGHT];
	target = inP;
	if (len > 0)
	{
		for (A_long i = 0; i < len; i++)
		{
			x--;
			target += -1;
			if (x >= 0)
			{
				if (target->alpha == 1)
				{
					*outP = *target;
					break;

				}
			}
			else {
				break;
			}
		}
	}
#pragma endregion

#pragma region BottomRightExec
	x = xL;
	y = yL;
	len = infoP->lenTable[LEN_BOTTOM_RIGHT];
	target = inP;
	if (len > 0)
	{
		for (A_long i = 0; i < len; i++)
		{
			x--;
			y--;
			target += -infoP->widthTrue - 1;
			if ((x >= 0) && (y >= 0))
			{
				if (target->alpha == 1)
				{
					*outP = *target;
					break;

				}
			}
			else {
				break;
			}
		}
	}
#pragma endregion


#pragma region BottomExec
	x = xL;
	y = yL;
	len = infoP->lenTable[LEN_BOTTOM];
	target = inP;
	if (len > 0)
	{
		for (A_long i = 0; i < len; i++)
		{
			y--;
			target += -infoP->widthTrue;
			if (y >= 0)
			{
				if (target->alpha == 1)
				{
					*outP = *target;
					break;

				}
			}
			else {
				break;
			}
		}
	}
#pragma endregion

#pragma region BottomLeftExec
	x = xL;
	y = yL;
	len = infoP->lenTable[LEN_BOTTOM_LEFT];
	target = inP;
	if (len > 0)
	{
		for (A_long i = 0; i < len; i++)
		{
			x++;
			y--;
			target += -infoP->widthTrue + 1;
			if ((x < infoP->width) && (y >= 0))
			{
				if (target->alpha == 1)
				{
					*outP = *target;
					break;

				}
			}
			else {
				break;
			}
		}
	}
#pragma endregion


#pragma region LeftExec
	x = xL;
	y = yL;
	len = infoP->lenTable[LEN_LEFT];
	target = inP;
	if (len > 0)
	{
		for (A_long i = 0; i < len; i++)
		{
			x++;
			target += 1;
			if (x < infoP->width)
			{
				if (target->alpha == 1)
				{
					*outP = *target;
					break;

				}
			}
			else {
				break;
			}
		}
	}
#pragma endregion

#pragma region TopLeftExec
	x = xL;
	y = yL;
	len = infoP->lenTable[LEN_TOP_LEFT];
	target = inP;
	if (len > 0)
	{
		for (A_long i = 0; i < len; i++)
		{
			x++;
			y++;
			target += infoP->widthTrue + 1;
			if ((x < infoP->width) && (y < infoP->height))
			{
				if (target->alpha == 1)
				{
					*outP = *target;
					break;

				}
			}
			else {
				break;
			}
		}
	}
#pragma endregion
	return err;
}

//-------------------------------------------------------------------------------------------------
static PF_Err GetParams(CFsAE *ae, ParamInfo *infoP)
{
	PF_Err		err 		= PF_Err_NONE;


	infoP->colTableCount = 0;
	infoP->ncolTableCount = 0;
	A_long cnt = 0;
	PF_Boolean ok = TRUE;
	PF_Pixel col = { 0,0,0,0 };
	PF_FpLong vb=0;
	ERR(ae->GetFLOAT(ID_RANGE, &vb));
	if (err == PF_Err_NONE)
	{
		infoP->range8 = (A_u_char)(PF_MAX_CHAN8 * vb / 100 + 0.5);
		infoP->range16 = (A_u_short)(PF_MAX_CHAN16 * vb / 100 + 0.5);
		infoP->range32 = (PF_FpShort)(vb / 100);
	}

	for (A_long i = 0; i < 8; i++)
	{
		ERR(ae->GetCHECKBOX(ID_TARGET_COLOR0_ENABLED + i*2 , &ok));
		if ((err == PF_Err_NONE)&&(ok == TRUE))
		{
			ERR(ae->GetCOLOR(ID_TARGET_COLOR0_ENABLED + i * 2 + 1,&col));
			if (err == PF_Err_NONE) {
				if (InColor8(infoP->colTable, col, cnt, 0) < 0)
				{
					infoP->colTable[cnt] = col;
					infoP->colTable[cnt].alpha = PF_MAX_CHAN8;
					infoP->colTable16[cnt] = CONV8TO16(col);
					infoP->colTable16[cnt].alpha = PF_MAX_CHAN16;
					infoP->colTable32[cnt] = CONV8TO32(col);
					infoP->colTable32[cnt].alpha = 1.0;
					cnt++;
				}
			}
		}
	}
	infoP->colTableCount = cnt;

	cnt = 0;
	A_long v;
	for (A_long i = 0; i < LEN_COUNT; i++) {
		infoP->lenTable[cnt] = 0;
	}
	for (A_long i = 0; i < LEN_COUNT; i++)
	{
		ERR(ae->GetADD(ID_TOP + i, &v));
		v = ae->downScaleNoClip(v);
		if (v < 0) v = 0;
		else if (v > EXPAND_MAX) v = EXPAND_MAX;
		infoP->lenTable[cnt] = v;
		cnt++;
	}

	cnt = 0;
	for (A_long i = 0; i < 8; i++)
	{
		ERR(ae->GetCHECKBOX(ID_NONE_COLOR0_ENABLED + i*2, &ok));
		if ((err == PF_Err_NONE) && (ok == TRUE))
		{
			ERR(ae->GetCOLOR(ID_NONE_COLOR0_ENABLED + i * 2 + 1, &col));
			if (err == PF_Err_NONE) {
				if (InColor8(infoP->colTable, col, cnt, 0) < 0)
				{
					if (InColor8(infoP->ncolTable, col, cnt, 0) < 0)
					{
						infoP->ncolTable[cnt] = col;
						infoP->ncolTable[cnt].alpha = PF_MAX_CHAN8;
						infoP->ncolTable16[cnt] = CONV8TO16(col);
						infoP->ncolTable16[cnt].alpha = PF_MAX_CHAN16;
						infoP->ncolTable32[cnt] = CONV8TO32(col);
						infoP->ncolTable32[cnt].alpha = 1.0;
						cnt++;
					}
				}
			}
		}
	}
	infoP->ncolTableCount = cnt;

	ERR(ae->GetCHECKBOX(ID_TARGET_ONLY, &infoP->IsTargetOnly));

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
	Exec (CFsAE *ae , ParamInfo *infoP)
{
	PF_Err	err = PF_Err_NONE;

	
	if (infoP->colTableCount <= 0)
	{
		ERR(ae->CopyInToOut());
		return err;
	}
	//値がすべて０ならおわる
	PF_Boolean GoExpand = FALSE;
	for (A_long i = 0; i < LEN_COUNT; i++)
	{
		if (infoP->lenTable[i] > 0)
		{
			GoExpand = TRUE;
			break;
		}
	}

	infoP->width = ae->out->width();
	infoP->widthTrue = ae->out->widthTrue();
	infoP->height = ae->out->height();

	ae->NewTmpWorld();
	ae->tmp->clear();
	ae->out->clear();
	switch(ae->pixelFormat())
	{
	case PF_PixelFormat_ARGB128:
		ERR(ae->CopyInToOut());
		break;
	case PF_PixelFormat_ARGB64:
		ERR(ae->iterate16InputToTemp((refconType)infoP, TargetPixelFrom16));
		if (GoExpand == TRUE) {
			ERR(ae->iterate16TempToOutput((refconType)infoP, ExpandPixel16));
			ERR(ae->iterate16((refconType)infoP, PixelChk16));
			ERR(ae->iterate16((refconType)infoP, NoDrawPixel16));
		}
		else {
			ERR(ae->Copy(ae->tmpP, ae->output));
		}
		if (infoP->IsTargetOnly == FALSE) {
			ERR(ae->iterate16((refconType)infoP, TranCopy16));
		}
		break;
	case PF_PixelFormat_ARGB32:
		ERR(ae->iterate8InputToTemp((refconType)infoP, TargetPixelFrom8));
		if (GoExpand == TRUE) {
			ERR(ae->iterate8TempToOutput((refconType)infoP, ExpandPixel8));
			ERR(ae->iterate8((refconType)infoP, PixelChk8));
			ERR(ae->iterate8((refconType)infoP, NoDrawPixel8));

		}
		else {
			ERR(ae->Copy(ae->tmpP, ae->output));
		}
		if (infoP->IsTargetOnly == FALSE) {
			ERR(ae->iterate8((refconType)infoP, TranCopy8));
		}

		break;
	}
	return err;
}

//-------------------------------------------------------------------------------------------------
//レンダリングのメイン
/*
	SmartFXに対応していないホスト(After Effects7以前のもの)はこの関数が呼び出されて描画する
	この関数を書いておけば一応v6.5対応になる
*/
static PF_Err 
Render ( 
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{

	PF_Err	err = PF_Err_NONE;
	PF_Handle		pixelTable = NULL;
	
	CFsAE ae(in_data,out_data,params,output,ID_NUM_PARAMS);
	err =ae.resultErr();
	if (!err){
		ParamInfo info;
		ERR(GetParams(&ae,&info));
		ERR(Exec(&ae,&info));
	}
	return err;
}
//-----------------------------------------------------------------------------------
/*
	SmartFX対応の場合、まずこの関数が呼ばれてパラメータの獲得を行う
*/
#if defined(SUPPORT_SMARTFX)
static PF_Err
PreRender(
	PF_InData			*in_data,
	PF_OutData			*out_data,
	PF_PreRenderExtra	*extraP)
{
	PF_Err		err 		= PF_Err_NONE;
	CFsAE ae(in_data,out_data,extraP,sizeof(ParamInfo),ID_NUM_PARAMS);
	err = ae.resultErr();
	if (!err){

		ParamInfo *infoP = reinterpret_cast<ParamInfo*>(ae.LockPreRenderData());
		if (infoP){
			ae.SetHostPreRenderData();
			ERR(GetParams(&ae,infoP));
			ERR(ae.UnSetPreRenderData());
			ae.UnlockPreRenderData();
		}else{
			err = PF_Err_OUT_OF_MEMORY;
		}
	}
	return err;
}
#endif
//-----------------------------------------------------------------------------------
#if defined(SUPPORT_SMARTFX)
static PF_Err
SmartRender(
	PF_InData				*in_data,
	PF_OutData				*out_data,
	PF_SmartRenderExtra		*extraP)
{
	PF_Err			err		= PF_Err_NONE,
					err2 	= PF_Err_NONE;

	CFsAE ae(in_data,out_data,extraP,ID_NUM_PARAMS);
	err = ae.resultErr();
	if (!err){
		ParamInfo *infoP = reinterpret_cast<ParamInfo*>(ae.LockPreRenderData());
		if (infoP){
			ERR(Exec(&ae,infoP));
			ERR2(ae.UnsetSmartRender());
			ae.UnlockPreRenderData();
		}else{
			err = PF_Err_OUT_OF_MEMORY;
		}
	}
	return err;
}
#endif

#include "Fs_Entry.h"
