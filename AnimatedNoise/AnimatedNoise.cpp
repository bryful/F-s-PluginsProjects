/*
	AnimatedNoise
*/

#include "AnimatedNoise.h"

void (*mosicSub)(PrmTbl *prm);
void (*noiseSub)(PrmTbl *prm);

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
// Aboutダイアログ
static PF_Err About (	PF_InData		*in_data,
						PF_OutData		*out_data,
						PF_ParamDef		*params[],
						PF_LayerDef		*output )
{
	PF_Err	err				= PF_Err_NONE;
	CFsAE ae;
	err = ae.About(in_data,out_data,params,output);
	return err;
}

//-------------------------------------------------------------------------------------------------
static PF_Err 
GlobalSetup (
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{
	PF_Err	err				= PF_Err_NONE;
	CFsAE ae;
	err = ae.GlobalSetup(in_data,out_data,params,output);
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err GlobalSetdown(
	PF_InData	*in_data)
{
	PF_Err	err				= PF_Err_NONE;
	CFsAE ae;
	err = ae.GlobalSetdown(in_data);
	return PF_Err_NONE;
}
//-------------------------------------------------------------------------------------------------
static PF_Err SequenceSetup (
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{

	return PF_Err_NONE;
}
//-------------------------------------------------------------------------------------------------
static PF_Err SequenceSetdown (
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{
	return PF_Err_NONE;
}


//-------------------------------------------------------------------------------------------------
static PF_Err SequenceResetup (
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{
	return PF_Err_NONE;
}

//-------------------------------------------------------------------------------------------------
//AfterEffextsにパラメータを通達する
//Param_Utils.hを参照のこと
static PF_Err ParamsSetup (PF_InData		*in_data,
					PF_OutData		*out_data,
					PF_ParamDef		*params[],
					PF_LayerDef		*output )
{
	PF_Err			err = PF_Err_NONE;
	PF_ParamDef		def;

	//----------------------------------------------------------------
	//１個目のパラメータ
	//チェックボックス
	AEFX_CLR_STRUCT(def);
	def.flags		=	PF_ParamFlag_SUPERVISE			|
						PF_ParamFlag_CANNOT_INTERP;
						
	//def.ui_flags	=	PF_PUI_STD_CONTROL_ONLY; 
	PF_ADD_CHECKBOX("毎フレームでノイズ変化",
					"ON",
					TRUE,
					0,
					ID_ANIMATED_CB
					);

	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	def.ui_flags = PF_PUI_DISABLED;
	PF_ADD_SLIDER(	"ノイズの動き",	//パラメータの名前
					0 , 				//数値入力する場合の最小値
					F_RAND_MAX,			//数値入力する場合の最大値
					0,				//スライダーの最小値 
					F_RAND_MAX/10,			//スライダーの最大値
					0,				//デフォルトの値
					ID_ANIMATED_ADD
					);
	//----------------------------------------------------------------
	//２個目のパラメータ
	//整数のスライダーバー
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(	"ノイズの量",	//パラメータの名前
					0, 				//数値入力する場合の最小値
					100,			//数値入力する場合の最大値
					0,				//スライダーの最小値 
					100,			//スライダーの最大値
					10,				//デフォルトの値
					ID_VALUE_ADD
					);
	//----------------------------------------------------------------
	//３個目のパラメータ
	//固定小数のスライダーバー
	AEFX_CLR_STRUCT(def);
	PF_ADD_FIXED(	"ノイズの強さ",	//パラメータの名前
					0, 				//数値入力する場合の最小値
					100,			//数値入力する場合の最大値
					0,				//スライダーの最小値 
					100,			//スライダーの最大値
					20,				//デフォルトの値
					2,//数値表示に関するフラグ 
					0,
					0,
					ID_LENGTH_FIXED
					);
	//----------------------------------------------------------------
	//４個目のパラメータ
	//チェックボックス
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX("カラーノイズ",
					"ON",
					(PF_ParamValue)FALSE,
					0,
					ID_COLOR_CB
					);

	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FIXED(	"ブロックの量",	//パラメータの名前
					0, 				//数値入力する場合の最小値
					300,			//数値入力する場合の最大値
					0,				//スライダーの最小値 
					5,				//スライダーの最大値
					1,				//デフォルトの値
					1,//数値表示に関するフラグ 
					0,
					0,
					ID_BLOCK_VALUE_FIXED
					);
	//----------------------------------------------------------------
	//整数のスライダーバー
	AEFX_CLR_STRUCT(def);
	PF_ADD_FIXED(	"ブロックの強さ",	//パラメータの名前
					0, 				//数値入力する場合の最小値
					100,			//数値入力する場合の最大値
					0,				//スライダーの最小値 
					50,				//スライダーの最大値
					5,				//デフォルトの値
					1,				//数値表示に関するフラグ 
					0,
					0,
					ID_BLOCK_LENGTH_FIXED
					);
	//----------------------------------------------------------------
	//整数のスライダーバー
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(	"ブロックの横幅(px)",	//パラメータの名前
					3, 				//数値入力する場合の最小値
					512,			//数値入力する場合の最大値
					3,				//スライダーの最小値 
					100,			//スライダーの最大値
					16,				//デフォルトの値
					ID_BLOCK_WIDTH_ADD
					);
	//----------------------------------------------------------------
	//整数のスライダーバー
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(	"ブロックの縦幅(px)",	//パラメータの名前
					3, 				//数値入力する場合の最小値
					512,			//数値入力する場合の最大値
					3,				//スライダーの最小値 
					100,			//スライダーの最大値
					16,				//デフォルトの値
					ID_BLOCK_HEIGHT_ADD
					);
	//----------------------------------------------------------------
	//チェックボックス
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX("カラーブロック",
					"ON",
					FALSE,
					0,
					ID_BLOCK_COLOR_CB
					);

	//----------------------------------------------------------------
	out_data->num_params = 	ID_NUM_PARAMS; 

	return err;
}
//-----------------------------------------------------------------------------------
static PF_Err
HandleChangedParam(
	PF_InData					*in_data,
	PF_OutData					*out_data,
	PF_ParamDef					*params[],
	PF_LayerDef					*outputP,
	PF_UserChangedParamExtra	*extraP)
{

	PF_Err				err					= PF_Err_NONE,
						err2				= PF_Err_NONE;
	try{
		CFsAE ae;
		err =ae.HandleChangedParam(in_data,out_data,params,outputP,extraP,ID_NUM_PARAMS);
		if (!err){
			PF_Boolean b = FALSE;
			ERR(ae.GetCHECKBOX(ID_ANIMATED_CB,&b));
			ERR(ae.UI_DISABLE(ID_ANIMATED_ADD, b));
			if (!err){
				out_data->out_flags |= PF_OutFlag_FORCE_RERENDER | PF_OutFlag_REFRESH_UI;
			}
		}
	}catch ( PF_Err & errP){
		err = errP;
	}
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
	PF_Err 	err 	= PF_Err_NONE,
			err2 	= PF_Err_NONE;
	//PF_OutFlag_NON_PARAM_VARYの値をout_flagsへ設定して
	//毎フレームごとの描画をするか切り替える。
	CFsAE ae;
	err = ae.QueryDynamicFlags(in_data,out_data,params,extra,ID_NUM_PARAMS);
	if (!err){
		PF_ParamDef def;
		AEFX_CLR_STRUCT(def);
		ERR(ae.checkout_param(ID_ANIMATED_CB,&def));
		ERR(ae.SetOutFlag_NON_PARAM_VARY((PF_Boolean)def.u.bd.value));
		ERR(ae.checkin_param(&def));
	}
	return err;
}

//-------------------------------------------------------------------------------------------------
static void NoiseMain8(PrmTbl *prm)
{
	PF_Pixel8 *data;
	data =(PF_Pixel8 *)prm->data;
	A_long  t;
	A_long  ar,ag,ab;
	A_long  ar2,ag2,ab2;
	A_long  ar3,ag3,ab3;
	
	A_long  v = (A_long )( F_RAND()*(100 +1.0)/(1.0+ F_RAND_MAX));
	if (v>=prm->value) return;
	
	ar =  -1 * PF_MAX_CHAN8 + (A_long )(F_RAND() * (PF_MAX_CHAN8*2+1.0)/(1.0+F_RAND_MAX) ); ar=ar*prm->length >> 16;
	if (prm->colorFlag==TRUE) {
		ag =  -1 * PF_MAX_CHAN8 + (A_long )(F_RAND() * (PF_MAX_CHAN8*2+1.0)/(1.0+F_RAND_MAX) );ag=ag*prm->length >> 16;
		ab =  -1 * PF_MAX_CHAN8 + (A_long )(F_RAND() * (PF_MAX_CHAN8*2+1.0)/(1.0+F_RAND_MAX) );ab=ab*prm->length >> 16;
	}else{
		ag=ab=ar;
		F_RAND();
		F_RAND();
	}
	
	data[prm->target].red	= RoundByteLong(data[prm->target].red	+ ar);
	data[prm->target].green	= RoundByteLong(data[prm->target].green	+ ag);
	data[prm->target].blue	= RoundByteLong(data[prm->target].blue	+ ab);

	ar2=ar/2;
	ag2=ag/2;
	ab2=ab/2;
	ar3=ar/4;
	ag3=ag/4;
	ab3=ab/4;

	t=prm->target - 1;
	data[t].red		= RoundByteLong(data[t].red		+ ar2);
	data[t].green	= RoundByteLong(data[t].green	+ ag2);
	data[t].blue	= RoundByteLong(data[t].blue	+ ab2);

	t=prm->target +1;
	data[t].red		= RoundByteLong(data[t].red		+ ar2);
	data[t].green	= RoundByteLong(data[t].green	+ ag2);
	data[t].blue	= RoundByteLong(data[t].blue	+ ab2);

	t=prm->target - prm->width;
	data[t].red		= RoundByteLong(data[t].red		+ ar2);
	data[t].green	= RoundByteLong(data[t].green	+ ag2);
	data[t].blue	= RoundByteLong(data[t].blue	+ ab2);

	t=prm->target +prm->width;
	data[t].red		= RoundByteLong(data[t].red		+ ar2);
	data[t].green	= RoundByteLong(data[t].green	+ ag2);
	data[t].blue	= RoundByteLong(data[t].blue	+ ab2);

	t=prm->target - prm->width-1;
	data[t].red		= RoundByteLong(data[t].red		+ ar3);
	data[t].green	= RoundByteLong(data[t].green	+ ag3);
	data[t].blue	= RoundByteLong(data[t].blue	+ ab3);

	t=prm->target - prm->width+1;
	data[t].red		= RoundByteLong(data[t].red		+ ar3);
	data[t].green	= RoundByteLong(data[t].green	+ ag3);
	data[t].blue	= RoundByteLong(data[t].blue	+ ab3);

	t=prm->target + prm->width-1;
	data[t].red		= RoundByteLong(data[t].red		+ ar3);
	data[t].green	= RoundByteLong(data[t].green	+ ag3);
	data[t].blue	= RoundByteLong(data[t].blue	+ ab3);
	
	t=prm->target + prm->width+1;
	data[t].red		= RoundByteLong(data[t].red		+ ar3);
	data[t].green	= RoundByteLong(data[t].green	+ ag3);
	data[t].blue	= RoundByteLong(data[t].blue	+ ab3);


}
//-------------------------------------------------------------------------------------------------
static void NoiseMain16(PrmTbl *prm)
{
	PF_Pixel16 *data16;
	data16 =(PF_Pixel16 *)prm->data;
	A_long  t;
	A_long  ar,ag,ab;
	A_long  ar2,ag2,ab2;
	A_long  ar3,ag3,ab3;
	
	A_long  v = (A_long )( F_RAND()*(100 +1.0)/(1.0+ F_RAND_MAX));
	if (v>=prm->value) return;
	
	ar =  -1 * PF_MAX_CHAN16 + (A_long )(F_RAND() * (PF_MAX_CHAN16*2+1.0)/(1.0+F_RAND_MAX) ); ar=ar*prm->length >> 16;
	if (prm->colorFlag==TRUE) {
		ag =  -1 * PF_MAX_CHAN16 + (A_long )(F_RAND() * (PF_MAX_CHAN16*2+1.0)/(1.0+F_RAND_MAX) );ag=ag*prm->length >> 16;
		ab =  -1 * PF_MAX_CHAN16 + (A_long )(F_RAND() * (PF_MAX_CHAN16*2+1.0)/(1.0+F_RAND_MAX) );ab=ab*prm->length >> 16;
	}else{
		ag=ab=ar;
		F_RAND();
		F_RAND();
	}
	
	data16[prm->target].red		= RoundShort(data16[prm->target].red + ar);
	data16[prm->target].green	= RoundShort(data16[prm->target].green + ag);
	data16[prm->target].blue	= RoundShort(data16[prm->target].blue + ab);

	ar2=ar/2;
	ag2=ag/2;
	ab2=ab/2;
	ar3=ar/4;
	ag3=ag/4;
	ab3=ab/4;

	t=prm->target - 1;
	data16[t].red		= RoundShort(data16[t].red	+ ar2);
	data16[t].green		= RoundShort(data16[t].green	+ ag2);
	data16[t].blue		= RoundShort(data16[t].blue	+ ab2);

	t=prm->target +1;
	data16[t].red		= RoundShort(data16[t].red	+ ar2);
	data16[t].green		= RoundShort(data16[t].green	+ ag2);
	data16[t].blue		= RoundShort(data16[t].blue	+ ab2);

	t=prm->target - prm->width;
	data16[t].red		= RoundShort(data16[t].red	+ ar2);
	data16[t].green		= RoundShort(data16[t].green	+ ag2);
	data16[t].blue		= RoundShort(data16[t].blue	+ ab2);

	t=prm->target +prm->width;
	data16[t].red		= RoundShort(data16[t].red	+ ar2);
	data16[t].green		= RoundShort(data16[t].green	+ ag2);
	data16[t].blue		= RoundShort(data16[t].blue	+ ab2);

	t=prm->target - prm->width-1;
	data16[t].red		= RoundShort(data16[t].red	+ ar3);
	data16[t].green		= RoundShort(data16[t].green	+ ag3);
	data16[t].blue		= RoundShort(data16[t].blue	+ ab3);

	t=prm->target - prm->width+1;
	data16[t].red		= RoundShort(data16[t].red	+ ar3);
	data16[t].green		= RoundShort(data16[t].green	+ ag3);
	data16[t].blue		= RoundShort(data16[t].blue	+ ab3);

	t=prm->target + prm->width-1;
	data16[t].red		= RoundShort(data16[t].red	+ ar3);
	data16[t].green		= RoundShort(data16[t].green	+ ag3);
	data16[t].blue		= RoundShort(data16[t].blue	+ ab3);
	
	t=prm->target + prm->width+1;
	data16[t].red		= RoundShort(data16[t].red	+ ar3);
	data16[t].green		= RoundShort(data16[t].green	+ ag3);
	data16[t].blue		= RoundShort(data16[t].blue	+ ab3);

}
static void NoiseMain32(PrmTbl *prm)
{
	PF_PixelFloat *data;
	data =(PF_PixelFloat *)prm->data;
	A_long  t;
	PF_FpShort  ar,ag,ab;
	PF_FpShort  ar2,ag2,ab2;
	PF_FpShort  ar3,ag3,ab3;
	
	A_long  v = (A_long )( F_RAND()*(100 +1.0)/(1.0+ F_RAND_MAX));
	if (v>=prm->value) return;
	
	ar =  (PF_FpShort)(-1 + ((F_RAND() * 2.0+1.0)/(1.0+F_RAND_MAX))); ar = (PF_FpShort)(ar*prm->length / 65536);
	if (prm->colorFlag==TRUE) {
		ag =  (PF_FpShort)(-1 + ((F_RAND() * 2.0+1.0)/(1.0+F_RAND_MAX))); ag = (PF_FpShort)(ag*prm->length / 65536);
		ab =  (PF_FpShort)(-1 + ((F_RAND() * 2.0+1.0)/(1.0+F_RAND_MAX))); ab = (PF_FpShort)(ab*prm->length / 65536);
	}else{
		ag = ab = ar;
		F_RAND();
		F_RAND();
	}
	
	data[prm->target].red	= RoundFpShortDouble(data[prm->target].red + ar);
	data[prm->target].green	= RoundFpShortDouble(data[prm->target].green + ag);
	data[prm->target].blue	= RoundFpShortDouble(data[prm->target].blue + ab);

	ar2=ar/2;
	ag2=ag/2;
	ab2=ab/2;

	ar3=ar/4;
	ag3=ag/4;
	ab3=ab/4;

	t=prm->target - 1;
	data[t].red		= RoundFpShortDouble(data[t].red	+ ar2);
	data[t].green	= RoundFpShortDouble(data[t].green	+ ag2);
	data[t].blue	= RoundFpShortDouble(data[t].blue	+ ab2);

	t=prm->target +1;
	data[t].red		= RoundFpShortDouble(data[t].red	+ ar2);
	data[t].green	= RoundFpShortDouble(data[t].green	+ ag2);
	data[t].blue	= RoundFpShortDouble(data[t].blue	+ ab2);

	t=prm->target - prm->width;
	data[t].red		= RoundFpShortDouble(data[t].red	+ ar2);
	data[t].green	= RoundFpShortDouble(data[t].green	+ ag2);
	data[t].blue	= RoundFpShortDouble(data[t].blue	+ ab2);

	t=prm->target +prm->width;
	data[t].red		= RoundFpShortDouble(data[t].red	+ ar2);
	data[t].green	= RoundFpShortDouble(data[t].green	+ ag2);
	data[t].blue	= RoundFpShortDouble(data[t].blue	+ ab2);

	t=prm->target - prm->width-1;
	data[t].red		= RoundFpShortDouble(data[t].red	+ ar3);
	data[t].green	= RoundFpShortDouble(data[t].green	+ ag3);
	data[t].blue	= RoundFpShortDouble(data[t].blue	+ ab3);

	t=prm->target - prm->width+1;
	data[t].red		= RoundFpShortDouble(data[t].red	+ ar3);
	data[t].green	= RoundFpShortDouble(data[t].green	+ ag3);
	data[t].blue	= RoundFpShortDouble(data[t].blue	+ ab3);

	t=prm->target + prm->width-1;
	data[t].red		= RoundFpShortDouble(data[t].red	+ ar3);
	data[t].green	= RoundFpShortDouble(data[t].green	+ ag3);
	data[t].blue	= RoundFpShortDouble(data[t].blue	+ ab3);
	
	t=prm->target + prm->width+1;
	data[t].red		= RoundFpShortDouble(data[t].red	+ ar3);
	data[t].green	= RoundFpShortDouble(data[t].green	+ ag3);
	data[t].blue	= RoundFpShortDouble(data[t].blue	+ ab3);
}
//-------------------------------------------------------------------------------------------------
static void NoiseBlock8(PrmTbl *prm)
{
	PF_Pixel *data;
	data = (PF_Pixel *)prm->data;
	
	A_long  temp,w2,h2,x0,y0,x1,y1;
	A_long  target,i,j,offset;
	A_long  r,g,b;
	A_long  ar,ag,ab;
	PF_Pixel col;

	if (prm->w<0) { x0=0;}else{x0=prm->w;}
	if (prm->h<0) { y0=0;}else{y0=prm->h;}
	if ((x0>=prm->width)||(y0>=prm->height)) return;

	temp=prm->block_width/2 +1;
	w2=temp+temp*F_RAND()/F_RAND_MAX;
	temp=prm->block_height/2 +1;
	h2=temp+temp*F_RAND()/F_RAND_MAX;
	
	x1=prm->w+w2;
	if (x1>prm->width) x1=prm->width;
	y1=prm->h+h2;
	if (y1>prm->height) y1=prm->height;
	if ((x1<=0)||(y1<=0)) return;
	w2=x1-x0; h2=y1-y0;
	if ((w2<=0)||(h2<=0)) return;

	
	target=x0+y0*prm->width;
	offset=prm->width-w2;
	r=g=b=0;
	for (j=y0;j<y1;j++){
		for (i=x0;i<x1;i++){
			r+=data[target].red;
			g+=data[target].green;
			b+=data[target].blue;
			target++;
		}
		target+=offset;
	}
	r/=(w2*h2);
	g/=(w2*h2);
	b/=(w2*h2);

	ar=PF_MAX_CHAN8 - F_RAND()*PF_MAX_CHAN8*2/F_RAND_MAX; ar=ar*prm->block_length >> 16;
	if (prm->block_colorFlag==TRUE) {
		ag=PF_MAX_CHAN8 - F_RAND()*PF_MAX_CHAN8*2/F_RAND_MAX; ag=ag*prm->block_length >> 16;
		ab=PF_MAX_CHAN8 - F_RAND()*PF_MAX_CHAN8*2/F_RAND_MAX; ab=ab*prm->block_length >> 16;
	}else{
		ag=ab=ar;
		F_RAND();
		F_RAND();
	}
	target=x0+y0*prm->width;
	col.red		= RoundByteLong(r+ar);
	col.green	= RoundByteLong(g+ag);
	col.blue	= RoundByteLong(b+ab);
	for (j=y0;j<y1;j++){
		for (i=x0;i<x1;i++){
			data[target].red	=col.red;
			data[target].green	=col.green;
			data[target].blue	=col.blue;
			target++;
		}
		target+=offset;
	}
}
//-------------------------------------------------------------------------------------------------
static void NoiseBlock16(PrmTbl *prm)
{
	PF_Pixel16 *data16;
	data16 = (PF_Pixel16 *)prm->data;
	A_long  temp,w2,h2,x0,y0,x1,y1;
	A_long  target,i,j,offset;
	A_long  r,g,b;
	A_long  ar,ag,ab;
	PF_Pixel16 col;

	if (prm->w<0) { x0=0;}else{x0=prm->w;}
	if (prm->h<0) { y0=0;}else{y0=prm->h;}
	if ((x0>=prm->width)||(y0>=prm->height)) return;

	temp=prm->block_width/2 +1;
	w2=temp+temp*F_RAND()/F_RAND_MAX;
	temp=prm->block_height/2 +1;
	h2=temp+temp*F_RAND()/F_RAND_MAX;
	
	x1=prm->w+w2;
	if (x1>prm->width) x1=prm->width;
	y1=prm->h+h2;
	if (y1>prm->height) y1=prm->height;
	if ((x1<=0)||(y1<=0)) return;
	w2=x1-x0; h2=y1-y0;
	if ((w2<=0)||(h2<=0)) return;

	
	target=x0+y0*prm->width;
	offset=prm->width-w2;
	r=g=b=0;
	for (j=y0;j<y1;j++){
		for (i=x0;i<x1;i++){
			r+=data16[target].red;
			g+=data16[target].green;
			b+=data16[target].blue;
			target++;
		}
		target+=offset;
	}
	r/=(w2*h2);
	g/=(w2*h2);
	b/=(w2*h2);

	ar=PF_MAX_CHAN16 - F_RAND()*PF_MAX_CHAN16*2/F_RAND_MAX; ar=ar*prm->block_length >> 16;
	if (prm->block_colorFlag==TRUE) {
		ag=PF_MAX_CHAN16 - F_RAND()*PF_MAX_CHAN16*2/F_RAND_MAX; ag=ag*prm->block_length >> 16;
		ab=PF_MAX_CHAN16 - F_RAND()*PF_MAX_CHAN16*2/F_RAND_MAX; ab=ab*prm->block_length >> 16;
	}else{
		ag=ab=ar;
		F_RAND();
		F_RAND();
	}
	target=x0+y0*prm->width;
	col.red		= RoundShort(r+ar);
	col.green	= RoundShort(g+ag);
	col.blue	= RoundShort(b+ab);
	for (j=y0;j<y1;j++){
		for (i=x0;i<x1;i++){
			data16[target].red	=col.red;
			data16[target].green	=col.green;
			data16[target].blue	=col.blue;
			target++;
		}
		target+=offset;
	}
}
//-------------------------------------------------------------------------------------------------
static void NoiseBlock32(PrmTbl *prm)
{
	PF_PixelFloat *data;
	data = (PF_PixelFloat *)prm->data;
	A_long  temp,w2,h2,x0,y0,x1,y1;
	A_long  target,i,j,offset;
	PF_FpShort  r,g,b;
	PF_FpShort  ar,ag,ab;
	PF_PixelFloat col;

	if (prm->w<0) { x0=0;}else{x0=prm->w;}
	if (prm->h<0) { y0=0;}else{y0=prm->h;}
	if ((x0>=prm->width)||(y0>=prm->height)) return;

	temp = prm->block_width/2 +1;
	w2 = temp + temp * F_RAND()/F_RAND_MAX;
	temp = prm->block_height/2 +1;
	h2 = temp+temp*F_RAND()/F_RAND_MAX;
	
	x1=prm->w+w2;
	if (x1>prm->width) x1=prm->width;
	y1=prm->h+h2;
	if (y1>prm->height) y1=prm->height;
	if ((x1<=0)||(y1<=0)) return;
	w2=x1-x0; h2=y1-y0;
	if ((w2<=0)||(h2<=0)) return;

	
	target=x0+y0*prm->width;
	offset=prm->width-w2;
	r=g=b=0;
	for (j=y0;j<y1;j++){
		for (i=x0;i<x1;i++){
			r += data[target].red;
			g += data[target].green;
			b += data[target].blue;
			target++;
		}
		target+=offset;
	}
	r/=(w2*h2);
	g/=(w2*h2);
	b/=(w2*h2);

	ar = (PF_FpShort)(1 - ((PF_FpShort)F_RAND()*2/F_RAND_MAX)); ar = (PF_FpShort)(ar * prm->block_length /65536);
	if (prm->block_colorFlag==TRUE) {
		ag = (PF_FpShort)(1 - (PF_FpShort)F_RAND()*2/F_RAND_MAX); ag = (PF_FpShort)(ag * prm->block_length /65536);
		ab = (PF_FpShort)(1 - (PF_FpShort)F_RAND()*2/F_RAND_MAX); ab = (PF_FpShort)(ab * prm->block_length /65536);
	}else{
		ag = ab = ar;
		F_RAND();
		F_RAND();
	}
	target=x0+y0*prm->width;
	col.red		= RoundFpShortDouble(r+ar);
	col.green	= RoundFpShortDouble(g+ag);
	col.blue	= RoundFpShortDouble(b+ab);
	for (j=y0;j<y1;j++){
		for (i=x0;i<x1;i++){
			data[target].red	=col.red;
			data[target].green	=col.green;
			data[target].blue	=col.blue;
			target++;
		}
		target+=offset;
	}

}
//-------------------------------------------------------------------------------------------------
static PF_Err GetParams(CFsAE *ae, PrmTbl *infoP)
{
	PF_Err		err 		= PF_Err_NONE;

	ERR(ae->GetCHECKBOX(ID_ANIMATED_CB,&infoP->animFlag));
	ERR(ae->GetADD(ID_ANIMATED_ADD,&infoP->anim));
	ERR(ae->GetADD(ID_VALUE_ADD,&infoP->value));
	ERR(ae->GetFIXED(ID_LENGTH_FIXED,&infoP->length));
	if (!err) infoP->length /= 100;
	ERR(ae->GetCHECKBOX(ID_COLOR_CB,&infoP->colorFlag));
	ERR(ae->GetFIXED(ID_BLOCK_VALUE_FIXED,&infoP->block_value));
	ERR(ae->GetFIXED(ID_BLOCK_LENGTH_FIXED,&infoP->block_length));
	if (!err) infoP->block_length /= 100;
	ERR(ae->GetADD(ID_BLOCK_WIDTH_ADD,&infoP->block_width));
	ERR(ae->GetADD(ID_BLOCK_HEIGHT_ADD,&infoP->block_height));
	ERR(ae->GetCHECKBOX(ID_BLOCK_COLOR_CB,&infoP->block_colorFlag));

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
	Exec (CFsAE *ae , PrmTbl *infoP)
{
	PF_Err	err = PF_Err_NONE;

	//画面をコピー
	ERR(ae->CopyInToOut());

	infoP->data		= ae->output->data;
	infoP->width	= ae->out->widthTrue();
	infoP->height = ae->output->height;
	infoP->frame	= ae->frame();

	if (infoP->animFlag == TRUE) {
		infoP->seed =infoP->frame * 2;
	}else{
		infoP->seed = infoP->anim;
	}
	A_long v = ( infoP->value * infoP->block_value) >>16;

	switch(ae->pixelFormat()){
	case PF_PixelFormat_ARGB128:
		mosicSub = NoiseBlock32;
		noiseSub = NoiseMain32;
		break;
	case PF_PixelFormat_ARGB64:
		mosicSub = NoiseBlock16;
		noiseSub = NoiseMain16;
		break;
	case PF_PixelFormat_ARGB32:
		mosicSub = NoiseBlock8;
		noiseSub = NoiseMain8;
		break;
	}
	if (v>0){
		F_SRAND(infoP->seed);
		for (int i=0;i<v;i++){
			infoP->w =  F_RAND2(-infoP->block_width,ae->out->width());
			infoP->h =  F_RAND2(-infoP->block_height,ae->out->height());
			mosicSub(infoP);
		}
	}
	if (infoP->value>0){
		F_SRAND(infoP->seed);
		infoP->target	= 1 + infoP->width;
		for (infoP->h=1; infoP->h<(infoP->height-1); infoP->h++) {
			for (infoP->w=1; infoP->w<(infoP->width-1); infoP->w++) {
				noiseSub(infoP);
				infoP->target++;
			}
			infoP->target+=2;
		}
	}
	
	return err;
}

//-------------------------------------------------------------------------------------------------
//レンダリングのメイン
static PF_Err Render (	PF_InData		*in_data,
						PF_OutData		*out_data,
						PF_ParamDef		*params[],
						PF_LayerDef		*output )
{
	PF_Err		err 		= PF_Err_NONE;

	CFsAE ae(in_data,out_data,params,output,ID_NUM_PARAMS);
	err =ae.resultErr();
	if (!err){
		PrmTbl info;
		ERR(GetParams(&ae,&info));
		ERR(Exec(&ae,&info));
	}

	return err;
}
//-----------------------------------------------------------------------------------
#if defined(SUPPORT_SMARTFX)
static PF_Err
PreRender(
	PF_InData			*in_dataP,
	PF_OutData			*out_dataP,
	PF_PreRenderExtra	*extraP)
{
	PF_Err		err 		= PF_Err_NONE;
	CFsAE ae(in_dataP,out_dataP,extraP,sizeof(PrmTbl),ID_NUM_PARAMS);
	err = ae.resultErr();
	if (!err){

		PrmTbl *infoP = reinterpret_cast<PrmTbl*>(ae.LockPreRenderData());
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
//-----------------------------------------------------------------------------------
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
		PrmTbl *infoP = reinterpret_cast<PrmTbl*>(ae.LockPreRenderData());
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
//-----------------------------------------------------------------------------------
static PF_Err 
RespondtoAEGP ( 	
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output,
	void*			extraP)
{
	PF_Err			err = PF_Err_NONE;
	
	AEGP_SuiteHandler suites(in_data->pica_basicP);
	
	suites.ANSICallbacksSuite1()->sprintf(	out_data->return_msg, 
											"%s",	
											reinterpret_cast<A_char*>(extraP));

	return err;
}
//-----------------------------------------------------------------------------------
DllExport	PF_Err 
EntryPointFunc (
	PF_Cmd			cmd,
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output,
	void			*extraP)
{
	PF_Err		err = PF_Err_NONE;
	
	try
	{
		CFsAE ae;
		switch (cmd) {
			case PF_Cmd_ABOUT:
				err = About(in_data,out_data,params,output);
				break;
			case PF_Cmd_GLOBAL_SETUP:
				err = GlobalSetup(	in_data,out_data,params,output);
				break;
			case PF_Cmd_GLOBAL_SETDOWN:
				err = GlobalSetdown(in_data);
				break;
			case PF_Cmd_PARAMS_SETUP:
				err = ParamsSetup(	in_data,out_data,params,output);
				break;
			case PF_Cmd_SEQUENCE_SETUP:
				err = SequenceSetup(in_data,out_data,params,output);
				break;
			case PF_Cmd_SEQUENCE_SETDOWN:
				err = SequenceSetdown(in_data,out_data,params,output);
				break;
			case PF_Cmd_SEQUENCE_RESETUP:
				err = SequenceResetup(in_data,out_data,params,output);
				break;
			case PF_Cmd_RENDER:
					err = Render(in_data,out_data,params,output);
					break;
#if defined(SUPPORT_SMARTFX)
			case PF_Cmd_SMART_PRE_RENDER:
				err = PreRender(in_data,out_data,reinterpret_cast<PF_PreRenderExtra*>(extraP));
				break;
			case PF_Cmd_SMART_RENDER:
				err = SmartRender(	in_data,out_data,reinterpret_cast<PF_SmartRenderExtra*>(extraP));
				break;
#endif
			case PF_Cmd_COMPLETELY_GENERAL:
				err = RespondtoAEGP(in_data,out_data,params,output,extraP);
				break;
			case PF_Cmd_DO_DIALOG:
				//err = PopDialog(in_data,out_data,params,output);
				break;		
			case PF_Cmd_USER_CHANGED_PARAM:
				err = HandleChangedParam(	in_data,
											out_data,
											params,
											output, 
											reinterpret_cast<PF_UserChangedParamExtra*>(extraP));
				break;
			case PF_Cmd_QUERY_DYNAMIC_FLAGS:
				err = QueryDynamicFlags(in_data,
										out_data,
										params,
										reinterpret_cast<PF_UserChangedParamExtra*>(extraP));
				break;
		}
	}
	catch(PF_Err &thrown_err){
		err = thrown_err;
	}
	return err;
}

//-------------------------------------------------------------------------------------------------
