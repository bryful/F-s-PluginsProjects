//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------


#include "NamiGarasu.h"


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
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FIXED(	STR_Y,	//パラメータの名前
					0, 				//数値入力する場合の最小値
					100,			//数値入力する場合の最大値
					0,				//スライダーの最小値 
					50,			//スライダーの最大値
					0,				//デフォルトの値
					1,				//数値表示に関するフラグ 
					0,
					0,
					ID_VALE
					);

	//----------------------------------------------------------------
	//角度
	AEFX_CLR_STRUCT(def);
	def.flags = PF_ParamFlag_CANNOT_TIME_VARY;//これをつけるとキーフレームが撃てなくなる
	PF_ADD_ANGLE(STR_ROT,45,ID_ROT); 
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	def.flags = PF_ParamFlag_CANNOT_TIME_VARY;//これをつけるとキーフレームが撃てなくなる
	PF_ADD_FIXED(	STR_SPEED,	//パラメータの名前
					0, 				//数値入力する場合の最小値
					32000,			//数値入力する場合の最大値
					0,				//スライダーの最小値 
					100,			//スライダーの最大値
					20,				//デフォルトの値
					1,				//数値表示に関するフラグ 
					0,
					0,
					ID_SPEED
					);

	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FIXED(	STR_LEVEL,	//パラメータの名前
					0, 				//数値入力する場合の最小値
					100,			//数値入力する場合の最大値
					0,				//スライダーの最小値 
					100,			//スライダーの最大値
					25,				//デフォルトの値
					1,				//数値表示に関するフラグ 
					0,
					0,
					ID_LEVEL
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FIXED(	STR_NOISE,	//パラメータの名前
					0, 				//数値入力する場合の最小値
					100,			//数値入力する場合の最大値
					0,				//スライダーの最小値 
					100,				//スライダーの最大値
					25,				//デフォルトの値
					1,				//数値表示に関するフラグ 
					0,
					0,
					ID_NOISE
					);
	//****************************************************************
	AEFX_CLR_STRUCT(def);	
	PF_ADD_TOPIC(STR_A, ID_TOPIC_A);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	def.flags = PF_ParamFlag_CANNOT_TIME_VARY;//これをつけるとキーフレームが撃てなくなる
	PF_ADD_SLIDER(	STR_A_SIZE,	//パラメータの名前
					1, 		//数値入力する場合の最小値
					6,			//数値入力する場合の最大値
					1,				//スライダーの最小値 
					6,			//スライダーの最大値
					2,				//デフォルトの値
					ID_A_SIZE
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	def.flags = PF_ParamFlag_CANNOT_TIME_VARY;//これをつけるとキーフレームが撃てなくなる
	PF_ADD_SLIDER(	STR_A_VALUE,	//パラメータの名前
					0, 				//数値入力する場合の最小値
					200,			//数値入力する場合の最大値
					0,				//スライダーの最小値 
					100,				//スライダーの最大値
					40,				//デフォルトの値
					ID_A_VALUE
					);


	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	def.flags = PF_ParamFlag_CANNOT_TIME_VARY;//これをつけるとキーフレームが撃てなくなる
	PF_ADD_FLOAT_SLIDER(STR_A_STRONG,	//Name
						0,						//VALID_MIN
						100,					//VALID_MAX
						0,						//SLIDER_MIN
						100,					//SLIDER_MAX
						1,						//CURVE_TOLERANCE
						35,					//DFLT
						1,						//PREC
						0,						//DISP
						0,						//WANT_PHASE
						ID_A_STRONG
						);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	def.flags = PF_ParamFlag_CANNOT_TIME_VARY;//これをつけるとキーフレームが撃てなくなる
	PF_ADD_FLOAT_SLIDER(STR_A_SPEED,	//Name
						-500,					//VALID_MIN
						500,					//VALID_MAX
						0,						//SLIDER_MIN
						100,					//SLIDER_MAX
						1,						//CURVE_TOLERANCE
						50,					//DFLT
						1,						//PREC
						0,						//DISP
						0,						//WANT_PHASE
						ID_A_SPEED
						);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	def.flags = PF_ParamFlag_CANNOT_TIME_VARY;//これをつけるとキーフレームが撃てなくなる
	PF_ADD_SLIDER(	STR_A_SEED,	//パラメータの名前
					0, 			//数値入力する場合の最小値
					30000,		//数値入力する場合の最大値
					0,			//スライダーの最小値 
					10000,		//スライダーの最大値
					0,			//デフォルトの値
					ID_A_SEED
					);

	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_END_TOPIC(ID_TOPIC_A_END);
	//****************************************************************
	AEFX_CLR_STRUCT(def);	
	PF_ADD_TOPIC(STR_B, ID_TOPIC_B);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	def.flags = PF_ParamFlag_CANNOT_TIME_VARY;//これをつけるとキーフレームが撃てなくなる
	PF_ADD_SLIDER(	STR_B_SIZE,	//パラメータの名前
					1, 		//数値入力する場合の最小値
					6,			//数値入力する場合の最大値
					1,				//スライダーの最小値 
					6,			//スライダーの最大値
					3,				//デフォルトの値
					ID_B_SIZE
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	def.flags = PF_ParamFlag_CANNOT_TIME_VARY;//これをつけるとキーフレームが撃てなくなる
	PF_ADD_SLIDER(	STR_B_VALUE,	//パラメータの名前
					0, 				//数値入力する場合の最小値
					200,			//数値入力する場合の最大値
					0,				//スライダーの最小値 
					100,			//スライダーの最大値
					20,				//デフォルトの値
					ID_B_VALUE
					);


	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	def.flags = PF_ParamFlag_CANNOT_TIME_VARY;//これをつけるとキーフレームが撃てなくなる
	PF_ADD_FLOAT_SLIDER(STR_B_STRONG,	//Name
						0,						//VALID_MIN
						100,					//VALID_MAX
						0,						//SLIDER_MIN
						100,					//SLIDER_MAX
						1,						//CURVE_TOLERANCE
						50,					//DFLT
						1,						//PREC
						0,						//DISP
						0,						//WANT_PHASE
						ID_B_STRONG
						);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	def.flags = PF_ParamFlag_CANNOT_TIME_VARY;//これをつけるとキーフレームが撃てなくなる
	PF_ADD_FLOAT_SLIDER(STR_B_SPEED,	//Name
						-500,					//VALID_MIN
						500,					//VALID_MAX
						0,						//SLIDER_MIN
						100,					//SLIDER_MAX
						1,						//CURVE_TOLERANCE
						75,					//DFLT
						1,						//PREC
						0,						//DISP
						0,						//WANT_PHASE
						ID_B_SPEED
						);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	def.flags = PF_ParamFlag_CANNOT_TIME_VARY;//これをつけるとキーフレームが撃てなくなる
	PF_ADD_SLIDER(	STR_B_SEED,	//パラメータの名前
					0, 			//数値入力する場合の最小値
					30000,		//数値入力する場合の最大値
					0,			//スライダーの最小値 
					10000,		//スライダーの最大値
					1000,		//デフォルトの値
					ID_B_SEED
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_END_TOPIC(ID_TOPIC_B_END);
	//****************************************************************
	AEFX_CLR_STRUCT(def);	
	PF_ADD_TOPIC(STR_C, ID_TOPIC_C);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	def.flags = PF_ParamFlag_CANNOT_TIME_VARY;//これをつけるとキーフレームが撃てなくなる
	PF_ADD_SLIDER(	STR_C_SIZE,	//パラメータの名前
					1, 		//数値入力する場合の最小値
					6,			//数値入力する場合の最大値
					1,				//スライダーの最小値 
					6,			//スライダーの最大値
					6,				//デフォルトの値
					ID_C_SIZE
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	def.flags = PF_ParamFlag_CANNOT_TIME_VARY;//これをつけるとキーフレームが撃てなくなる
	PF_ADD_SLIDER(	STR_C_VALUE,	//パラメータの名前
					0, 				//数値入力する場合の最小値
					200,			//数値入力する場合の最大値
					0,				//スライダーの最小値 
					100,			//スライダーの最大値
					30,			//デフォルトの値
					ID_C_VALUE
					);


	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	def.flags = PF_ParamFlag_CANNOT_TIME_VARY;//これをつけるとキーフレームが撃てなくなる
	PF_ADD_FLOAT_SLIDER(STR_C_STRONG,	//Name
						0,						//VALID_MIN
						100,					//VALID_MAX
						0,						//SLIDER_MIN
						100,					//SLIDER_MAX
						1,						//CURVE_TOLERANCE
						50,					//DFLT
						1,						//PREC
						0,						//DISP
						0,						//WANT_PHASE
						ID_C_STRONG
						);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	def.flags = PF_ParamFlag_CANNOT_TIME_VARY;//これをつけるとキーフレームが撃てなくなる
	PF_ADD_FLOAT_SLIDER(STR_C_SPEED,	//Name
						-500,						//VALID_MIN
						500,					//VALID_MAX
						0,						//SLIDER_MIN
						100,					//SLIDER_MAX
						1,						//CURVE_TOLERANCE
						100,						//DFLT
						1,						//PREC
						0,						//DISP
						0,						//WANT_PHASE
						ID_C_SPEED
						);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	def.flags = PF_ParamFlag_CANNOT_TIME_VARY;//これをつけるとキーフレームが撃てなくなる
	PF_ADD_SLIDER(	STR_C_SEED,	//パラメータの名前
					0, 			//数値入力する場合の最小値
					30000,		//数値入力する場合の最大値
					0,			//スライダーの最小値 
					10000,		//スライダーの最大値
					2000,			//デフォルトの値
					ID_C_SEED
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_END_TOPIC(ID_TOPIC_C_END);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	def.flags = PF_ParamFlag_CANNOT_TIME_VARY;//これをつけるとキーフレームが撃てなくなる
	PF_ADD_CHECKBOX(STR_DISPMAP,
					"on",
					FALSE,
					0,
					ID_DISPMAP
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FIXED(	STR_LENGTH_X,	//パラメータの名前
					0, 				//数値入力する場合の最小値
					1000,			//数値入力する場合の最大値
					0,				//スライダーの最小値 
					100,			//スライダーの最大値
					100,				//デフォルトの値
					1,				//数値表示に関するフラグ 
					0,
					0,
					ID_LENGTH_X
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FIXED(	STR_LENGTH_Y,	//パラメータの名前
					0, 				//数値入力する場合の最小値
					1000,			//数値入力する場合の最大値
					0,				//スライダーの最小値 
					100,			//スライダーの最大値
					100,				//デフォルトの値
					1,				//数値表示に関するフラグ 
					0,
					0,
					ID_LENGTH_Y
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
	PF_Err				err					= PF_Err_NONE;
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
	PF_Err 	err 	= PF_Err_NONE;
	return err;
}
//-------------------------------------------------------------------------------------------------
/*
	アルファーチャンネルに描かれたマップを表示
*/
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
static PF_Err 
dispMap8 (
	refconType	refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel	*inP, 
	PF_Pixel	*outP)
{
	PF_Err				err					= PF_Err_NONE;
	outP->red =
	outP->green =
	outP->blue = outP->alpha;
	outP->alpha = inP->alpha;
	return err;
}
static PF_Err 
dispMap16 (
	refconType	refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel16	*inP, 
	PF_Pixel16	*outP)
{
	PF_Err				err					= PF_Err_NONE;
	outP->red =
	outP->green =
	outP->blue = outP->alpha;
	outP->alpha = inP->alpha;
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
dispMap32 (
	refconType	refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_PixelFloat	*inP, 
	PF_PixelFloat	*outP)
{
	PF_Err				err					= PF_Err_NONE;
	outP->red =
	outP->green =
	outP->blue = outP->alpha;
	outP->alpha = inP->alpha;
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
inToAlphap8 (
	refconType	refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel	*inP, 
	PF_Pixel	*outP)
{
	PF_Err				err					= PF_Err_NONE;
	outP->alpha = inP->alpha;
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
inToAlphap16 (
	refconType	refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel16	*inP, 
	PF_Pixel16	*outP)
{
	PF_Err				err					= PF_Err_NONE;
	outP->alpha = inP->alpha;
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
inToAlphap32 (
	refconType	refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_PixelFloat	*inP, 
	PF_PixelFloat	*outP)
{
	PF_Err				err					= PF_Err_NONE;
	outP->alpha = inP->alpha;
	return err;
}
//-------------------------------------------------------------------------------------------------
/*
	グレーで塗りつぶし	
*/
//-------------------------------------------------------------------------------------------------
static PF_Err 
clearHalf8 (
	refconType	refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel	*inP, 
	PF_Pixel	*outP)
{
	PF_Err				err					= PF_Err_NONE;
	ParamInfo *	niP		= reinterpret_cast<ParamInfo*>(refcon);
	PF_Pixel back = {PF_HALF_CHAN8,PF_HALF_CHAN8,PF_HALF_CHAN8,PF_HALF_CHAN8};
	*outP =back;
	if (niP->noise>0){
		if (xorShift()>0x3fff) {
			A_long  a =  (A_long)(((double)PF_MAX_CHAN8 -  2 * (double)PF_MAX_CHAN8 * xorShiftDouble()) * niP->noise);

			outP->alpha	=  RoundByteLong((A_long)PF_HALF_CHAN8  + a);
		}else{
			xorShiftDouble();
		}
	}else{
			xorShiftDouble();
			xorShiftDouble();
	}

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
clearHalf16 (
	refconType	refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel16	*inP, 
	PF_Pixel16	*outP)
{
	PF_Err				err					= PF_Err_NONE;
	ParamInfo *	niP		= reinterpret_cast<ParamInfo*>(refcon);
	PF_Pixel16 back = {PF_HALF_CHAN16,PF_HALF_CHAN16,PF_HALF_CHAN16,PF_HALF_CHAN16};
	*outP =back;
	if (niP->noise>0){
		if (xorShift()>0x3fff) {
			A_long  a =  (A_long)(((double)PF_MAX_CHAN16 -  2 * (double)PF_MAX_CHAN16 * xorShiftDouble()) * niP->noise);

			outP->alpha	=  RoundShort((A_long)PF_HALF_CHAN16  + a);
		}else{
			xorShiftDouble();
		}

	}else{
			xorShiftDouble();
			xorShiftDouble();
	}
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
clearHalf32 (
	refconType	refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_PixelFloat	*inP, 
	PF_PixelFloat	*outP)
{
	PF_Err				err					= PF_Err_NONE;
	PF_PixelFloat back = {PF_HALF_CHAN32,PF_HALF_CHAN32,PF_HALF_CHAN32,PF_HALF_CHAN32};
	ParamInfo *	niP		= reinterpret_cast<ParamInfo*>(refcon);
	*outP =back;
	if (niP->noise>0){
		if (xorShift()>0x3fff) {
			double  a =  ((1.0 -  2.0 * xorShiftDouble()) * niP->noise);

			outP->alpha	=  RoundFpShortDouble(PF_HALF_CHAN32  + a);
		}else{
			xorShiftDouble();
		}

	}else{
			xorShiftDouble();
			xorShiftDouble();
	}
	return err;
}
//-------------------------------------------------------------------------------------------------
/*
	redチャンネルだけグレーに
*/
//-------------------------------------------------------------------------------------------------
static PF_Err 
clearRed8 (
	refconType	refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel	*inP, 
	PF_Pixel	*outP)
{
	PF_Err				err					= PF_Err_NONE;
	outP->red =PF_HALF_CHAN8;
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
clearRed16 (
	refconType	refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel16	*inP, 
	PF_Pixel16	*outP)
{
	PF_Err				err					= PF_Err_NONE;
	outP->red =PF_HALF_CHAN16;
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
clearRed32 (
	refconType	refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_PixelFloat	*inP, 
	PF_PixelFloat	*outP)
{
	PF_Err				err					= PF_Err_NONE;
	outP->red =PF_HALF_CHAN32;
	return err;
}
//-------------------------------------------------------------------------------------------------
static void Draw8(ParamInfo *infoP,DrawInfo *di,int idx)
{
	int rndTable[6][8]=
	{
		{0,0,0,1,0,1,0,1},
		{2,3,2,1,2,1,2,1},
		{4,5,4,3,4,5,2,3},
		{6,7,6,7,6,5,4,5},
		{8,8,8,9,5,7,7,6},
		{10,10,10,10,10,9,8,7}
	};
	if((infoP->map[idx].value>0)&&(infoP->map[idx].strong>0)){
		init_xorShift(infoP->map[idx].seed);
		di->strong = infoP->map[idx].strong;
		for ( int i=0; i<infoP->map[idx].value; i++){
			
			di->x =  xorShift()*di->scr.width /0x7FFF;
			di->y =  xorShift()*di->scr.height /0x7FFF;
			
			int r = (int)((double)xorShift()*7.0/0x7FFF + 0.5);
			if (r<0) r=0; else if(r>7) r = 7;
			di->sizeIdx = rndTable[infoP->map[idx].size][r];
			drawSq8(di);
			di->isW =  (di->isW +1) %2;
		}
		
	}
}
//-------------------------------------------------------------------------------------------------
static void Draw16(ParamInfo *infoP,DrawInfo *di,int idx)
{
	int rndTable[6][8]=
	{
		{0,0,0,1,0,1,0,1},
		{2,3,2,1,2,1,2,1},
		{4,5,4,3,4,5,2,3},
		{6,7,6,7,6,5,4,5},
		{8,8,8,9,5,7,7,6},
		{10,10,10,10,10,9,8,7}
	};
	if((infoP->map[idx].value>0)&&(infoP->map[idx].strong>0)){
		init_xorShift(infoP->map[idx].seed);
		di->strong = infoP->map[idx].strong;
		for ( int i=0; i<infoP->map[idx].value; i++){
			
			di->x =  xorShift()*di->scr.width /0x7FFF;
			di->y =  xorShift()*di->scr.height /0x7FFF;
			
			int r = (int)((double)xorShift()*7.0/0x7FFF + 0.5);
			if (r<0) r=0; else if(r>7) r = 7;
			di->sizeIdx = rndTable[infoP->map[idx].size][r];
			drawSq16(di);
			di->isW =  (di->isW +1) %2;
		}
		
	}
}
//-------------------------------------------------------------------------------------------------
static void Draw32(ParamInfo *infoP,DrawInfo *di,int idx)
{
	int rndTable[6][8]=
	{
		{0,0,0,1,0,1,0,1},
		{2,3,2,1,2,1,2,1},
		{4,5,4,3,4,5,2,3},
		{6,7,6,7,6,5,4,5},
		{8,8,8,9,5,7,7,6},
		{10,10,10,10,10,9,8,7}
	};
	if((infoP->map[idx].value>0)&&(infoP->map[idx].strong>0)){
		init_xorShift(infoP->map[idx].seed);
		di->strong = infoP->map[idx].strong;
		for ( int i=0; i<infoP->map[idx].value; i++){
			
			di->x =  xorShift()*di->scr.width /0x7FFF;
			di->y =  xorShift()*di->scr.height /0x7FFF;
			
			int r = (int)((double)xorShift()*7.0/0x7FFF + 0.5);
			if (r<0) r=0; else if(r>7) r = 7;
			di->sizeIdx = rndTable[infoP->map[idx].size][r];
			drawSq32(di);
			di->isW =  (di->isW +1) %2;
		}
		
	}
}
//-------------------------------------------------------------------------------------------------
/*
*/
//-------------------------------------------------------------------------------------------------
static PF_Err 
alphatoToRed8 (
	refconType	refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel	*inP, 
	PF_Pixel	*outP)
{
	PF_Err				err					= PF_Err_NONE;
	
	outP->red = outP->alpha;
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
alphatoToRed16 (
	refconType	refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel16	*inP, 
	PF_Pixel16	*outP)
{
	PF_Err				err					= PF_Err_NONE;
	
	outP->red = outP->alpha;
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
alphatoToRed32 (
	refconType	refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_PixelFloat	*inP, 
	PF_PixelFloat	*outP)
{
	PF_Err				err					= PF_Err_NONE;
	
	outP->red = outP->alpha;
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err GetParams(CFsAE *ae, ParamInfo *infoP)
{
	PF_Err		err 		= PF_Err_NONE;

	long valueTable[6] = { 128,32,16,4,2,1};

	PF_Fixed f = 0;
	double mv = 0;
	ERR(ae->GetFIXED(ID_VALE,&f));
	if (!err){
		mv = (double)f/6553600.0;
	}
	ERR(ae->GetFIXED(ID_LENGTH_X,&infoP->lengthX));
	infoP->lengthX = (PF_Fixed)((double)infoP->lengthX *mv + 0.5); 
	ERR(ae->GetFIXED(ID_LENGTH_Y,&infoP->lengthY));
	infoP->lengthY = (PF_Fixed)((double)infoP->lengthY *mv + 0.5); 
	f = 0;
	ERR(ae->GetFIXED(ID_LEVEL,&f));
	if (!err){
		infoP->level = (double)f/6553600.0;
		infoP->level *= mv;
	}
	f = 0;
	ERR(ae->GetFIXED(ID_NOISE,&f));
	if (!err){
		infoP->noise = (double)f/6553600.0;
		infoP->noise *= mv;
	}
	
	ERR(ae->GetANGLE(ID_ROT,&infoP->rot));
	infoP->rot = RoundAngle360(infoP->rot);
	ERR(ae->GetFIXED(ID_SPEED,&infoP->speed));
	
	//**********************
	ERR(ae->GetADD(ID_A_SIZE,&infoP->map[0].size));
	infoP->map[0].size -=1;
	if (infoP->map[0].size<0) infoP->map[0].size = 0;
	else if (infoP->map[0].size>5) infoP->map[0].size = 5;
	ERR(ae->GetADD(ID_A_VALUE,&infoP->map[0].value));
	infoP->map[0].value *= valueTable[infoP->map[0].size];
	
	ERR(ae->GetFLOAT(ID_A_STRONG,&infoP->map[0].strong));
	infoP->map[0].strong /= 100;
	ERR(ae->GetFLOAT(ID_A_SPEED,&infoP->map[0].speed));
	infoP->map[0].speed /= 100;
	ERR(ae->GetADD(ID_A_SEED,&infoP->map[0].seed));

	//**********************
	ERR(ae->GetADD(ID_B_SIZE,&infoP->map[1].size));
	infoP->map[1].size -=1;
	if (infoP->map[1].size<0) infoP->map[1].size = 0;
	else if (infoP->map[1].size>5) infoP->map[1].size = 5;
	ERR(ae->GetADD(ID_B_VALUE,&infoP->map[1].value));
	infoP->map[1].value *= valueTable[infoP->map[1].size];

	ERR(ae->GetFLOAT(ID_B_STRONG,&infoP->map[1].strong));
	infoP->map[1].strong /= 100;
	ERR(ae->GetFLOAT(ID_B_SPEED,&infoP->map[1].speed));
	infoP->map[1].speed /= 100;
	ERR(ae->GetADD(ID_B_SEED,&infoP->map[1].seed));

	//**********************
	ERR(ae->GetADD(ID_C_SIZE,&infoP->map[2].size));
	infoP->map[2].size -=1;
	if (infoP->map[2].size<0) infoP->map[2].size = 0;
	else if (infoP->map[2].size>5) infoP->map[2].size = 5;
	ERR(ae->GetADD(ID_C_VALUE,&infoP->map[2].value));
	infoP->map[2].value *= valueTable[infoP->map[2].size];
	
	ERR(ae->GetFLOAT(ID_C_STRONG,&infoP->map[2].strong));
	infoP->map[2].strong /= 100;
	ERR(ae->GetFLOAT(ID_C_SPEED,&infoP->map[2].speed));
	infoP->map[2].speed /= 100;
	ERR(ae->GetADD(ID_C_SEED,&infoP->map[2].seed));
	//**********************

	ERR(ae->GetCHECKBOX(ID_DISPMAP,&infoP->disp_map));

	//ERR(ae->GetFIXEDPOINT(ID_POINT,&infoP->point));

	return err;
}
//-------------------------------------------------------------------------------------------------
static DrawInfo InitDrawInfo(CFsAE *ae , ParamInfo *infoP)
{
	DrawInfo di;
	di.scr.data = ae->out->data();
	di.scr.width =ae->out->width();
	di.scr.widthTrue =ae->out->widthTrue();
	di.scr.height =ae->out->height();
	di.x = 0;
	di.y = 0;
	di.strong = 1;
	di.isW = 0;
	di.scale = ae->downSaleValue();

	return di;
}
//-------------------------------------------------------------------------------------------------
static ShiftInfo InitShiftInfo(CFsAE *ae , ParamInfo *infoP)
{
	ShiftInfo si;
	si.scr.data = ae->out->data();
	si.scr.width =ae->out->width();
	si.scr.widthTrue =ae->out->widthTrue();
	si.scr.height =ae->out->height();
	return si;

}
//-------------------------------------------------------------------------------------------------
static PF_Err 
	Exec8 (CFsAE *ae , ParamInfo *infoP)
{
	PF_Err	err = PF_Err_NONE;

	DrawInfo di = InitDrawInfo(ae,infoP);

	PF_Fixed len = infoP->speed * ae->frame()/ di.scale;
	CRotCalc rt(ae->in_data);

	ShiftInfo si = InitShiftInfo(ae,infoP);
	
	F_SRAND(ae->frame());
	ERR(ae->iterate8((refconType)infoP,clearHalf8));
	
	int idx = 0;
	
	F_SRAND(infoP->map[idx].seed);
	if ( (infoP->map[idx].strong>0)&&(infoP->map[idx].value>0) ){
		Draw8(infoP,&di,idx);
		rt.SetRotLength(infoP->rot, (PF_Fixed)((double)len * infoP->map[idx].speed));
		si.shift_x = rt.x() % (si.scr.width << 16);
		si.shift_y = rt.y() % (si.scr.height << 16);
		ERR(ae->iterate8((refconType)&si,shiftRed8));
	}

	idx = 1;
	F_SRAND(infoP->map[idx].seed);
	if ( (infoP->map[idx].strong>0)&&(infoP->map[idx].value>0) ){
		ERR(ae->iterate8((refconType)NULL,clearRed8));
		Draw8(infoP,&di,idx);
		rt.SetRotLength(infoP->rot, (PF_Fixed)((double)len * infoP->map[idx].speed));
		si.shift_x = rt.x() % (si.scr.width << 16);
		si.shift_y = rt.y() % (si.scr.height << 16);
		ERR(ae->iterate8((refconType)&si,shiftRed8));
	}

	idx = 2;
	F_SRAND(infoP->map[idx].seed);
	if ( (infoP->map[idx].strong>0)&&(infoP->map[idx].value>0) ){
		ERR(ae->iterate8((refconType)NULL,clearRed8));
		Draw8(infoP,&di,idx);
		rt.SetRotLength(infoP->rot, (PF_Fixed)((double)len * infoP->map[idx].speed));
		si.shift_x = rt.x() % (si.scr.width << 16);
		si.shift_y = rt.y() % (si.scr.height << 16);
		ERR(ae->iterate8((refconType)&si,shiftRed8));
	}

	ERR(ae->iterate8((refconType)NULL,alphatoToRed8));
	AlphaBlur8(ae->out);
	ERR(ae->iterate8((refconType)NULL,alphatoToRed8));
	AlphaBlur8(ae->out);

	if (infoP->disp_map == TRUE){
		ERR(ae->iterate8((refconType)NULL,dispMap8));
	}else{
		si.scr.data = ae->in->data();
		si.scr.width =ae->in->width();
		si.scr.widthTrue =ae->in->widthTrue();
		si.scr.height =ae->in->height();

		si.shift_x = infoP->lengthX;
		si.shift_y = infoP->lengthY;
		si.level   = infoP->level; 
		ERR(ae->iterate8((refconType)&si,imageMap8));

	}
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
	Exec16 (CFsAE *ae , ParamInfo *infoP)
{
	PF_Err	err = PF_Err_NONE;

	DrawInfo di = InitDrawInfo(ae,infoP);

	PF_Fixed len = infoP->speed * ae->frame()/ di.scale;
	CRotCalc rt(ae->in_data);

	ShiftInfo si = InitShiftInfo(ae,infoP);
	
	F_SRAND(ae->frame());
	ERR(ae->iterate16((refconType)infoP,clearHalf16));
	
	int idx = 0;
	
	F_SRAND(infoP->map[idx].seed);
	if ( (infoP->map[idx].strong>0)&&(infoP->map[idx].value>0) ){
		Draw16(infoP,&di,idx);
		rt.SetRotLength(infoP->rot, (PF_Fixed)((double)len * infoP->map[idx].speed));
		si.shift_x = rt.x() % (si.scr.width << 16);
		si.shift_y = rt.y() % (si.scr.height << 16);
		ERR(ae->iterate16((refconType)&si,shiftRed16));
	}

	idx = 1;
	F_SRAND(infoP->map[idx].seed);
	if ( (infoP->map[idx].strong>0)&&(infoP->map[idx].value>0) ){
		ERR(ae->iterate16((refconType)NULL,clearRed16));
		Draw16(infoP,&di,idx);
		rt.SetRotLength(infoP->rot, (PF_Fixed)((double)len * infoP->map[idx].speed));
		si.shift_x = rt.x() % (si.scr.width << 16);
		si.shift_y = rt.y() % (si.scr.height << 16);
		ERR(ae->iterate16((refconType)&si,shiftRed16));
	}

	idx = 2;
	F_SRAND(infoP->map[idx].seed);
	if ( (infoP->map[idx].strong>0)&&(infoP->map[idx].value>0) ){
		ERR(ae->iterate16((refconType)NULL,clearRed16));
		Draw16(infoP,&di,idx);
		rt.SetRotLength(infoP->rot, (PF_Fixed)((double)len * infoP->map[idx].speed));
		si.shift_x = rt.x() % (si.scr.width << 16);
		si.shift_y = rt.y() % (si.scr.height << 16);
		ERR(ae->iterate16((refconType)&si,shiftRed16));
	}

	ERR(ae->iterate16((refconType)NULL,alphatoToRed16));
	AlphaBlur16(ae->out);
	ERR(ae->iterate16((refconType)NULL,alphatoToRed16));
	AlphaBlur16(ae->out);

	if (infoP->disp_map == TRUE){
		ERR(ae->iterate16((refconType)NULL,dispMap16));
	}else{
		si.scr.data = ae->in->data();
		si.scr.width =ae->in->width();
		si.scr.widthTrue =ae->in->widthTrue();
		si.scr.height =ae->in->height();

		si.shift_x = infoP->lengthX;
		si.shift_y = infoP->lengthY;
		si.level   = infoP->level; 
		ERR(ae->iterate16((refconType)&si,imageMap16));

	}
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
	Exec32 (CFsAE *ae , ParamInfo *infoP)
{
	PF_Err	err = PF_Err_NONE;

	DrawInfo di = InitDrawInfo(ae,infoP);

	PF_Fixed len = infoP->speed * ae->frame()/ di.scale;
	CRotCalc rt(ae->in_data);

	ShiftInfo si = InitShiftInfo(ae,infoP);
	
	F_SRAND(ae->frame());
	ERR(ae->iterate32((refconType)infoP,clearHalf32));
	
	int idx = 0;
	
	F_SRAND(infoP->map[idx].seed);
	if ( (infoP->map[idx].strong>0)&&(infoP->map[idx].value>0) ){
		Draw32(infoP,&di,idx);
		rt.SetRotLength(infoP->rot, (PF_Fixed)((double)len * infoP->map[idx].speed));
		si.shift_x = rt.x() % (si.scr.width << 16);
		si.shift_y = rt.y() % (si.scr.height << 16);
		ERR(ae->iterate32((refconType)&si,shiftRed32));
	}

	idx = 1;
	F_SRAND(infoP->map[idx].seed);
	if ( (infoP->map[idx].strong>0)&&(infoP->map[idx].value>0) ){
		ERR(ae->iterate32((refconType)NULL,clearRed32));
		Draw32(infoP,&di,idx);
		rt.SetRotLength(infoP->rot, (PF_Fixed)((double)len * infoP->map[idx].speed));
		si.shift_x = rt.x() % (si.scr.width << 16);
		si.shift_y = rt.y() % (si.scr.height << 16);
		ERR(ae->iterate32((refconType)&si,shiftRed32));
	}

	idx = 2;
	F_SRAND(infoP->map[idx].seed);
	if ( (infoP->map[idx].strong>0)&&(infoP->map[idx].value>0) ){
		ERR(ae->iterate32((refconType)NULL,clearRed32));
		Draw32(infoP,&di,idx);
		rt.SetRotLength(infoP->rot, (PF_Fixed)((double)len * infoP->map[idx].speed));
		si.shift_x = rt.x() % (si.scr.width << 16);
		si.shift_y = rt.y() % (si.scr.height << 16);
		ERR(ae->iterate32((refconType)&si,shiftRed32));
	}


	ERR(ae->iterate32((refconType)NULL,alphatoToRed32));
	AlphaBlur32(ae->out);
	ERR(ae->iterate32((refconType)NULL,alphatoToRed32));
	AlphaBlur32(ae->out);

	if (infoP->disp_map == TRUE){
		ERR(ae->iterate32((refconType)NULL,dispMap32));
	}else{
		si.scr.data = ae->in->data();
		si.scr.width =ae->in->width();
		si.scr.widthTrue =ae->in->widthTrue();
		si.scr.height =ae->in->height();

		si.shift_x = infoP->lengthX;
		si.shift_y = infoP->lengthY;
		si.level   = infoP->level; 
		ERR(ae->iterate32((refconType)&si,imageMap32));

	}
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
	Exec (CFsAE *ae , ParamInfo *infoP)
{
	PF_Err	err = PF_Err_NONE;
	
	bool b = false;
	if ((infoP->lengthX<=0)&&(infoP->lengthY<=0)) b  = true;
	if ( (infoP->map[0].value <=0)&&(infoP->map[1].value <=0)&&(infoP->map[2].value <=0)&&(infoP->noise<=0)&&(infoP->level<=0)) b = true;
	if ( (infoP->map[0].strong <=0)&&(infoP->map[1].strong <=0)&&(infoP->map[2].strong <=0)&&(infoP->noise<=0)&&(infoP->level<=0)) b = true;

	if (b){
		if (infoP->disp_map){
			PF_Pixel back = {PF_MAX_CHAN8,PF_HALF_CHAN8,PF_HALF_CHAN8,PF_HALF_CHAN8};
			ae->out->clear(back);
			switch(ae->pixelFormat())
			{
			case PF_PixelFormat_ARGB128:
				ERR(ae->iterate32((refconType)NULL,inToAlphap32));
				break;
			case PF_PixelFormat_ARGB64:
				ERR(ae->iterate16((refconType)NULL,inToAlphap16));
				break;
			case PF_PixelFormat_ARGB32:
				ERR(ae->iterate8((refconType)NULL,inToAlphap8));
				break;
			}

		}else{
			ae->CopyInToOut();
		}
		return err;
	}

	switch(ae->pixelFormat())
	{
	case PF_PixelFormat_ARGB128:
		ERR(Exec32(ae ,infoP));
		break;
	case PF_PixelFormat_ARGB64:
		ERR(Exec16(ae ,infoP));
		break;
	case PF_PixelFormat_ARGB32:
		ERR(Exec8(ae ,infoP));
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
