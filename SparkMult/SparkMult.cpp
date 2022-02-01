//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------


#include "SparkMult.h"


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
	PF_ADD_SLIDER(STR_SEED,	//パラメータの名前
		-30000, 				//数値入力する場合の最小値
		30000,			//数値入力する場合の最大値
		0,				//スライダーの最小値 
		1000,			//スライダーの最大値
		0,				//デフォルトの値
		ID_SEED
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(STR_SEEDPOS,	//パラメータの名前
		-30000, 				//数値入力する場合の最小値
		30000,			//数値入力する場合の最大値
		0,				//スライダーの最小値 
		1000,			//スライダーの最大値
		0,				//デフォルトの値
		ID_SEEDPOS
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(STR_SEEDMOVE,	//パラメータの名前
		-30000, 				//数値入力する場合の最小値
		30000,			//数値入力する場合の最大値
		0,				//スライダーの最小値 
		1000,			//スライダーの最大値
		0,				//デフォルトの値
		ID_SEEDMOVE
	);
	//----------------------------------------------------------------
	//角度
	AEFX_CLR_STRUCT(def);
	PF_ADD_ANGLE(STR_OFFSET, 0, ID_OFFSET);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_WIPE,	//Name
		0,						//VALID_MIN
		200,						//VALID_MAX
		0,						//SLIDER_MIN
		200,						//SLIDER_MAX
		1,						//CURVE_TOLERANCE
		100,						//DFLT
		1,						//PREC
		0,						//DISP
		0,						//WANT_PHASE
		ID_WIPE
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(STR_POINT_COUNT,	//パラメータの名前
		2, 				//数値入力する場合の最小値
		7,			//数値入力する場合の最大値
		2,				//スライダーの最小値 
		7,				//スライダーの最大値
		5,				//デフォルトの値
		ID_POINT_COUNT
	);
	//----------------------------------------------------------------
	//位置の指定
	AEFX_CLR_STRUCT(def);
	PF_ADD_POINT(STR_0_P,		/*"New Center"*/
		0,	// X
		0,	// Y
		0,	// Flag
		ID_0_P
	);
	//----------------------------------------------------------------
	//位置の指定
	AEFX_CLR_STRUCT(def);
	PF_ADD_POINT(STR_1_P,		/*"New Center"*/
		10,	// X
		10,	// Y
		0,	// Flag
		ID_1_P
	);
	//----------------------------------------------------------------
//位置の指定
	AEFX_CLR_STRUCT(def);
	PF_ADD_POINT(STR_2_P,		/*"New Center"*/
		20,	// X
		20,	// Y
		0,	// Flag
		ID_2_P
	);	//----------------------------------------------------------------
	//位置の指定
	AEFX_CLR_STRUCT(def);
	PF_ADD_POINT(STR_3_P,		/*"New Center"*/
		30,	// X
		30,	// Y
		0,	// Flag
		ID_3_P
	);	
	//----------------------------------------------------------------
	//位置の指定
	AEFX_CLR_STRUCT(def);
	PF_ADD_POINT(STR_4_P,		/*"New Center"*/
		40,	// X
		40,	// Y
		0,	// Flag
		ID_4_P
	);
	//----------------------------------------------------------------
	//位置の指定
	AEFX_CLR_STRUCT(def);
	PF_ADD_POINT(STR_5_P,		/*"New Center"*/
		50,	// X
		50,	// Y
		0,	// Flag
		ID_5_P
	);
	//----------------------------------------------------------------
//位置の指定
	AEFX_CLR_STRUCT(def);
	PF_ADD_POINT(STR_6_P,		/*"New Center"*/
		60,	// X
		60,	// Y
		0,	// Flag
		ID_6_P
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(STR_START_RX,	//パラメータの名前
		0, 				//数値入力する場合の最小値
		500,			//数値入力する場合の最大値
		0,				//スライダーの最小値 
		100,				//スライダーの最大値
		10,				//デフォルトの値
		ID_START_RX
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(STR_START_RY,	//パラメータの名前
		0, 				//数値入力する場合の最小値
		500,			//数値入力する場合の最大値
		0,				//スライダーの最小値 
		100,				//スライダーの最大値
		10,				//デフォルトの値
		ID_START_RY
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(STR_MID_RX,	//パラメータの名前
		0, 				//数値入力する場合の最小値
		500,			//数値入力する場合の最大値
		0,				//スライダーの最小値 
		100,				//スライダーの最大値
		10,				//デフォルトの値
		ID_MID_RX
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(STR_MID_RY,	//パラメータの名前
		0, 				//数値入力する場合の最小値
		500,			//数値入力する場合の最大値
		0,				//スライダーの最小値 
		100,				//スライダーの最大値
		10,				//デフォルトの値
		ID_MID_RY
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(STR_LAST_RX,	//パラメータの名前
		0, 				//数値入力する場合の最小値
		500,			//数値入力する場合の最大値
		0,				//スライダーの最小値 
		100,				//スライダーの最大値
		10,				//デフォルトの値
		ID_LAST_RX
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(STR_LAST_RY,	//パラメータの名前
		0, 				//数値入力する場合の最小値
		500,			//数値入力する場合の最大値
		0,				//スライダーの最小値 
		100,				//スライダーの最大値
		10,				//デフォルトの値
		ID_LAST_RY
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_LINESIZE,	//Name
		0,						//VALID_MIN
		300,						//VALID_MAX
		0,						//SLIDER_MIN
		100,						//SLIDER_MAX
		1,						//CURVE_TOLERANCE
		25,						//DFLT
		1,						//PREC
		0,						//DISP
		0,						//WANT_PHASE
		ID_LINESIZE
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(STR_LINEMOVE,	//パラメータの名前
		0, 				//数値入力する場合の最小値
		2000,			//数値入力する場合の最大値
		0,				//スライダーの最小値 
		100,				//スライダーの最大値
		50,				//デフォルトの値
		ID_LINEMOVE
	);	
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(STR_SUBCOUNT,	//パラメータの名前
		0, 				//数値入力する場合の最小値
		10,			//数値入力する場合の最大値
		0,				//スライダーの最小値 
		5,				//スライダーの最大値
		2,				//デフォルトの値
		ID_SUB_COUNT
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(STR_FOLDCOUNT,	//パラメータの名前
		1, 				//数値入力する場合の最小値
		5,			//数値入力する場合の最大値
		1,				//スライダーの最小値 
		5,				//スライダーの最大値
		3,				//デフォルトの値
		ID_FOLD_COUNT
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(STR_DRAWCOUNT,	//パラメータの名前
		1, 				//数値入力する場合の最小値
		50,			//数値入力する場合の最大値
		1,				//スライダーの最小値 
		5,				//スライダーの最大値
		1,				//デフォルトの値
		ID_DRAW_COUNT
	);


	//----------------------------------------------------------------
//色の指定
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(STR_COLOR,
		0xFF,
		0xFF,
		0xFF,
		ID_COLOR
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_BLEND,
		STR_ON,
		FALSE,
		0,
		ID_BLEND
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
static PF_Err
RedBlendIn8(
	refconType	refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel* inP,
	PF_Pixel* outP)
{
	PF_Err			err = PF_Err_NONE;

	A_u_char v = outP->alpha;

	if (v <= 0) {
		*outP = *inP;
	}
	else {
		PF_FpLong v0 = (PF_FpLong)inP->alpha / PF_MAX_CHAN8;
		PF_FpLong r0 = (PF_FpLong)inP->red * v0;
		PF_FpLong g0 = (PF_FpLong)inP->green * v0;
		PF_FpLong b0 = (PF_FpLong)inP->blue * v0;

		PF_FpLong v1 = (PF_FpLong)outP->alpha / PF_MAX_CHAN8;
		PF_FpLong r1 = (PF_FpLong)outP->red * v1;
		PF_FpLong g1 = (PF_FpLong)outP->green * v1;
		PF_FpLong b1 = (PF_FpLong)outP->blue * v1;

		r1 = r0 * (1 - v1) + r1 * v1;
		g1 = g0 * (1 - v1) + g1 * v1;
		b1 = b0 * (1 - v1) + b1 * v1;

		v1 = v0 + v1 - v0 * v1;

		r1 /= v1;
		g1 /= v1;
		b1 /= v1;

		outP->alpha = RoundByteFpLong(v1*PF_MAX_CHAN8);
		outP->red = RoundByteFpLong(r1);
		outP->green = RoundByteFpLong(g1);
		outP->blue = RoundByteFpLong(b1);

	}

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err
RedBlendIn16(
	refconType	refcon,
	A_long		xL,
	A_long		yL,
	PF_Pixel16* inP,
	PF_Pixel16* outP)
{
	PF_Err			err = PF_Err_NONE;

	A_u_short v = outP->alpha;

	if (v <= 0) {
		*outP = *inP;
	}
	else {
		PF_FpLong v0 = (PF_FpLong)inP->alpha / PF_MAX_CHAN16;
		PF_FpLong r0 = (PF_FpLong)inP->red * v0;
		PF_FpLong g0 = (PF_FpLong)inP->green * v0;
		PF_FpLong b0 = (PF_FpLong)inP->blue * v0;

		PF_FpLong v1 = (PF_FpLong)outP->alpha / PF_MAX_CHAN16;
		PF_FpLong r1 = (PF_FpLong)outP->red * v1;
		PF_FpLong g1 = (PF_FpLong)outP->green * v1;
		PF_FpLong b1 = (PF_FpLong)outP->blue * v1;

		r1 = r0 * (1 - v1) + r1 * v1;
		g1 = g0 * (1 - v1) + g1 * v1;
		b1 = b0 * (1 - v1) + b1 * v1;

		v1 = v0 + v1 - v0 * v1;

		r1 /= v1;
		g1 /= v1;
		b1 /= v1;

		outP->alpha = RoundShortFpLong(v1 * PF_MAX_CHAN16);
		outP->red = RoundShortFpLong(r1);
		outP->green = RoundShortFpLong(g1);
		outP->blue = RoundShortFpLong(b1);

	}

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err
RedBlendIn32(
	refconType	refcon,
	A_long		xL,
	A_long		yL,
	PF_PixelFloat* inP,
	PF_PixelFloat* outP)
{
	PF_Err			err = PF_Err_NONE;

	PF_FpShort v = outP->alpha;

	if (v <= 0) {
		*outP = *inP;
	}
	else {
		PF_FpLong v0 = (PF_FpLong)inP->alpha;
		if (v0 > 1) v0 = 1;
		PF_FpLong r0 = (PF_FpLong)inP->red * v0;
		PF_FpLong g0 = (PF_FpLong)inP->green * v0;
		PF_FpLong b0 = (PF_FpLong)inP->blue * v0;

		PF_FpLong v1 = (PF_FpLong)outP->alpha;
		if (v1 > 1) v1 = 1;
		PF_FpLong r1 = (PF_FpLong)outP->red * v1;
		PF_FpLong g1 = (PF_FpLong)outP->green * v1;
		PF_FpLong b1 = (PF_FpLong)outP->blue * v1;

		r1 = r0 * (1 - v1) + r1 * v1;
		g1 = g0 * (1 - v1) + g1 * v1;
		b1 = b0 * (1 - v1) + b1 * v1;

		v1 = v0 + v1 - v0 * v1;

		r1 /= v1;
		g1 /= v1;
		b1 /= v1;

		outP->alpha = RoundFpShortDouble(v1);
		outP->red = RoundFpShortDouble(r1);
		outP->green = RoundFpShortDouble(g1);
		outP->blue = RoundFpShortDouble(b1);

	}

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err GetParams(CFsAE *ae, ParamInfo *infoP)
{
	PF_Err		err 		= PF_Err_NONE;

	for (A_long i = 0; i < 5; i++)
	{
		infoP->point[i].p.x = 0;
		infoP->point[i].p.y = 0;
		infoP->point[i].s = 0;
	}

	PF_InData* in_data = ae->in_data;
	PF_FpLong ds = (PF_FpLong)in_data->downsample_x.num / (PF_FpLong)in_data->downsample_x.den;


	PF_FixedPoint v;

	ERR(ae->GetADD(ID_SEED, &infoP->seed));
	infoP->seed %= 30000;
	if (infoP->seed < 0)infoP->seed += 30000;
	ERR(ae->GetADD(ID_SEEDPOS, &infoP->seedPos));
	infoP->seedPos %= 30000;
	if (infoP->seedPos < 0)infoP->seedPos += 30000;
	ERR(ae->GetADD(ID_SEEDMOVE, &infoP->seedMove));
	infoP->seedMove %= 30000;
	if (infoP->seedMove < 0)infoP->seedMove += 30000;


	PF_Fixed r;
	ERR(ae->GetANGLE(ID_OFFSET, &r));
	if (!err) {
		r = r % (360L << 16);
		if (r < 0) r += (360L << 16);
		infoP->offset = (PF_FpLong)r / 65536;
	}
	ERR(ae->GetFLOAT(ID_WIPE, &infoP->wipe));
	if (!err) {
		infoP->wipe /= 100;
		if (infoP->wipe < 0) infoP->wipe = 0;
		else if (infoP->wipe > 2) infoP->wipe = 2;
	}
	ERR(ae->GetADD(ID_POINT_COUNT, &infoP->pointCount));


	ERR(ae->GetFIXEDPOINT(ID_0_P, &v));
	if (!err) {
		infoP->point[0].p.x = (int)((double)v.x / 65536 + 0.5);
		infoP->point[0].p.y = (int)((double)v.y / 65536 + 0.5);
	}
	ERR(ae->GetFIXEDPOINT(ID_1_P, &v));
	if (!err) {
		infoP->point[1].p.x = (int)((double)v.x / 65536 + 0.5);
		infoP->point[1].p.y = (int)((double)v.y / 65536 + 0.5);
	}
	ERR(ae->GetFIXEDPOINT(ID_2_P, &v));
	if (!err) {
		infoP->point[2].p.x = (int)((double)v.x / 65536 + 0.5);
		infoP->point[2].p.y = (int)((double)v.y / 65536 + 0.5);
	}
	ERR(ae->GetFIXEDPOINT(ID_3_P, &v));
	if (!err) {
		infoP->point[3].p.x = (int)((double)v.x / 65536 + 0.5);
		infoP->point[3].p.y = (int)((double)v.y / 65536 + 0.5);
	}
	ERR(ae->GetFIXEDPOINT(ID_4_P, &v));
	if (!err) {
		infoP->point[4].p.x = (int)((double)v.x / 65536 + 0.5);
		infoP->point[4].p.y = (int)((double)v.y / 65536 + 0.5);
	}
	ERR(ae->GetFIXEDPOINT(ID_5_P, &v));
	if (!err) {
		infoP->point[5].p.x = (int)((double)v.x / 65536 + 0.5);
		infoP->point[5].p.y = (int)((double)v.y / 65536 + 0.5);
	}
	ERR(ae->GetFIXEDPOINT(ID_6_P, &v));
	if (!err) {
		infoP->point[6].p.x = (int)((double)v.x / 65536 + 0.5);
		infoP->point[6].p.y = (int)((double)v.y / 65536 + 0.5);
	}
	
	A_long mx = 0;
	A_long my = 0;
	ERR(ae->GetADD(ID_MID_RX, &mx));
	if (!err) {
		mx = (A_long)((PF_FpLong)mx * ds + 0.5);
	}
	ERR(ae->GetADD(ID_MID_RX, &my));
	if (!err) {
		my = (A_long)((PF_FpLong)my * ds + 0.5);
	}

	for (A_long i = 0; i < 7; i++) 
	{
		infoP->randX[i] = mx;
		infoP->randY[i] = my;
	}


	ERR(ae->GetADD(ID_START_RX, &infoP->randX[0]));
	if (!err) {
		infoP->randX[0] = (A_long)((PF_FpLong)infoP->randX[0] * ds + 0.5);
	}
	ERR(ae->GetADD(ID_START_RY, &infoP->randY[0]));
	if (!err) {
		infoP->randY[0] = (A_long)((PF_FpLong)infoP->randY[0] * ds + 0.5);
	}
	A_long p = infoP->pointCount - 1;
	ERR(ae->GetADD(ID_LAST_RX, &infoP->randX[p]));
	if (!err) {
		infoP->randX[p] = (A_long)((PF_FpLong)infoP->randX[p] * ds + 0.5);
	}
	ERR(ae->GetADD(ID_LAST_RY, &infoP->randY[p]));
	if (!err) {
		infoP->randY[p] = (A_long)((PF_FpLong)infoP->randY[p] * ds + 0.5);
	}


	ERR(ae->GetFLOAT(ID_LINESIZE, &infoP->lineSize));
	if (!err) {
		infoP->lineSize = (A_long)((PF_FpLong)infoP->lineSize * ds + 0.5);
	}
	ERR(ae->GetADD(ID_LINEMOVE, &infoP->lineMove));
	if (!err) {
		infoP->lineMove = (A_long)((PF_FpLong)infoP->lineMove * ds + 0.5);
	}

	ERR(ae->GetADD(ID_SUB_COUNT, &infoP->subCount));
	ERR(ae->GetADD(ID_FOLD_COUNT, &infoP->foldCount));
	ERR(ae->GetADD(ID_DRAW_COUNT, &infoP->drawCount));


	
	ERR(ae->GetCOLOR(ID_COLOR, &infoP->color));
	ERR(ae->GetCHECKBOX(ID_BLEND, &infoP->blend));


	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
	Exec (CFsAE *ae , ParamInfo *infoP)
{
	PF_Err	err = PF_Err_NONE;
	infoP->frame = ae->frame();
	A_long r = infoP->seed + infoP->seedPos;
	r %= 30000; if (r < 0) r += 30000;
	init_xorShift(r);
	r = infoP->seed + infoP->seedMove;
	r %= 30000; if (r < 0) r += 30000;
	init_xorShiftM(r);

	CLineDraw ld(ae->output, ae->in_data,ae->pixelFormat(),2048);

	A_long pc = infoP->pointCount;
	for (A_long dc = 0; dc < infoP->drawCount; dc++)
	{
		ld.Points.Clear();
		PointInfo sub[10];
		for (A_long k = 0; k < pc; k++)
		{
			PointInfo p = infoP->point[k];
			p = ld.Points.Random(p, infoP->randX[k], infoP->randY[k]);
			if ((k == 0) || (k == pc-1))
			{
				p.s = 0;
				xorShiftDouble();
			}
			else {
				p.s = (double)infoP->lineSize * (0.2 + 0.8*xorShiftDouble());
			}
			ld.Points.Push(p);
			sub[k] = p;
		}
		A_long depth = 0;
		if (infoP->foldCount > 0) {
			for (A_long fc = 0; fc < infoP->foldCount; fc++)
			{
				ld.Points.CalcCenterPos(
					infoP->lineSize,
					infoP->lineMove,
					infoP->offset,
					depth
				);
				depth++;

			}
		}
		ld.Points.Wipe(infoP->wipe);
		ld.Line();

		if (infoP->subCount > 0) {
			for (A_long sc = 0; sc < infoP->subCount; sc++)
			{
				ld.Points.Clear();
				for (A_long k = 0; k < pc; k++)
				{
					PointInfo p = sub[k];
					p = ld.Points.Random(p, infoP->randX[k], infoP->randY[k]);
					ld.Points.Push(p);
				}
				A_long depth = 0;
				if (infoP->foldCount > 0) {
					for (A_long fc = 0; fc < infoP->foldCount; fc++)
					{
						ld.Points.CalcCenterPos(
							infoP->lineSize / 3,
							infoP->lineMove * 5 / 4,
							-infoP->offset,
							depth
						);
						depth++;

					}
				}
				ld.Points.Wipe(infoP->wipe);
				ld.Line();
				
			}
			
		}
	}

	ld.Colorize(infoP->color);

	if (ae->pixelFormat() == PF_PixelFormat_ARGB128) {
		if (infoP->blend == TRUE) {
			ERR(ae->iterate32((refconType)infoP, RedBlendIn32));
		}

	}
	else if (ae->pixelFormat() == PF_PixelFormat_ARGB64) {
		if (infoP->blend == TRUE) {
			ERR(ae->iterate16((refconType)infoP, RedBlendIn16));
		}
	}
	else if (ae->pixelFormat() == PF_PixelFormat_ARGB32) {
		
		if (infoP->blend == TRUE) {
			ERR(ae->iterate8((refconType)infoP, RedBlendIn8));
		}
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
