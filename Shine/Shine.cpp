

#include "Shine.h"


// **********************************************************
PF_Err CopyIntoBuf1_8(ParamInfo* infoP)
{
	PF_Err			err = PF_Err_NONE;
	A_long w = infoP->bufP1->width();
	A_long h = infoP->bufP1->height();

	for (A_long y = 0; y < h; y++)
	{
		for (A_long x = 0; x < w; x++)
		{
			PF_Pixel p[4];
			p[0] = PMat8(infoP->inP->GetPix8(2*x, 2*y));
			p[1] = PMat8(infoP->inP->GetPix8(2 * x + 1, 2 * y));
			p[2] = PMat8(infoP->inP->GetPix8(2 * x, 2 * y + 1));
			p[3] = PMat8(infoP->inP->GetPix8(2 * x + 1, 2 * y + 1));


			A_long vmax = p[0].red+ p[0].green + p[0].blue;
			A_long idx = 0;
			for (A_long i = 1; i < 4; i++)
			{
				A_long vv = p[i].red + p[i].green + p[i].blue;
				if (vv > vmax)
				{
					idx = i;
					vmax = vv;
				}
			}
			p[idx].alpha = PF_MAX_CHAN8;
			infoP->bufP1->SetPix8(x, y, p[idx]);
		}

	}



	return err;
}
PF_Err CopyBuf1ToDst_8(ParamInfo* infoP)
{
	PF_Err			err = PF_Err_NONE;
	A_long w = infoP->outP->width();
	A_long h = infoP->outP->height();
	A_long w2 = infoP->bufP1->width();
	A_long h2 = infoP->bufP1->height();

	for (A_long y = 0; y < h; y++)
	{
		PF_FpLong yy = (PF_FpLong)y * (PF_FpLong)h2 / (PF_FpLong)h;
		for (A_long x = 0; x < w; x++)
		{
			PF_FpLong xx = (PF_FpLong)x * (PF_FpLong)w2 / (PF_FpLong)w;

			infoP->outP->SetPix8(
				x, y,
				infoP->bufP1->GetPixD8(xx, yy)
			);
		}
	}
	return err;
}
PF_Err CopyBuf2ToDst_8(ParamInfo* infoP)
{
	PF_Err			err = PF_Err_NONE;
	A_long w = infoP->outP->width();
	A_long h = infoP->outP->height();
	A_long w2 = infoP->bufP2->width();
	A_long h2 = infoP->bufP2->height();

	for (A_long y = 0; y < h; y++)
	{
		PF_FpLong yy = (PF_FpLong)y * (PF_FpLong)h2 / (PF_FpLong)h;
		for (A_long x = 0; x < w; x++)
		{
			PF_FpLong xx = (PF_FpLong)x * (PF_FpLong)w2 / (PF_FpLong)w;

			infoP->outP->SetPix8(
				x, y,
				infoP->bufP2->GetPixD8(xx, yy)
			);
		}
	}
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
sub8 (
	void* refcon,
	A_long		xL, 
	A_long		yL, 
	PF_Pixel8	*inP, 
	PF_Pixel8	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo* infoP = reinterpret_cast<ParamInfo*>(refcon);

	PF_FpLong cx = infoP->pos.x/2;
	PF_FpLong cy = infoP->pos.y/2;

	//sqrt( (x1-x2)^2 + (y1-y2)^2 )
	PF_FpLong lenD = pointLength(infoP->outP->in_data,
		(PF_FpLong)xL,
		(PF_FpLong)yL,
		cx,
		cy
	);
	A_long len = (A_long)(lenD + 0.5);
	if (len > infoP->length/2)
	{
		len = infoP->length/2;
	}
	if (len <= 0) return err;

	PF_FpLong dx = (cx - (PF_FpLong)xL) / (PF_FpLong)lenD;
	PF_FpLong dy = (cy - (PF_FpLong)yL) / (PF_FpLong)lenD;

	PF_FpLong xD = (PF_FpLong)xL;
	PF_FpLong yD = (PF_FpLong)yL;
	PF_FpLong r = 0;
	PF_FpLong g = 0;
	PF_FpLong b = 0;
	for (A_long i = 0; i < len; i++)
	{
		xD += dx;
		yD += dy;
		PF_Pixel c = infoP->bufP1->GetPixD8(xD, yD);
		PF_FpLong av = (PF_FpLong)(len - i) / (PF_FpLong)(len + 1);
		//av /= (len+1);
		r += (PF_FpLong)c.red * av;
		g += (PF_FpLong)c.green * av;
		b += (PF_FpLong)c.blue * av;
	}
	outP->red = RoundByteFpLong((PF_FpLong)(outP->red) + r*infoP->strong);
	outP->green = RoundByteFpLong((PF_FpLong)(outP->green) + g * infoP->strong);
	outP->blue = RoundByteFpLong((PF_FpLong)(outP->blue) + b * infoP->strong);
	outP->alpha = MAX(outP->red, MAX(outP->green, outP->blue));
	//outP->alpha = PF_MAX_CHAN8;
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
NFsNoise16(
	void* refcon,
	A_long		xL, 
	A_long		yL, 
	PF_Pixel16	*inP, 
	PF_Pixel16	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo* infoP = reinterpret_cast<ParamInfo*>(refcon);


	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
NFsNoise32(
	void* refcon,
	A_long		xL, 
	A_long		yL, 
	PF_PixelFloat	*inP, 
	PF_PixelFloat	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo* infoP = reinterpret_cast<ParamInfo*>(refcon);


	return err;
}
//-------------------------------------------------------------------------------------------------
// **********************************************************
PF_Err Shine::ParamsSetup(
	PF_InData* in_dataP,
	PF_OutData* out_dataP,
	PF_ParamDef* paramsP[],
	PF_LayerDef* outputP)
{
	PF_Err err = PF_Err_NONE;
	Init();
	m_cmd = PF_Cmd_PARAMS_SETUP;
	in_data = in_dataP;
	out_data = out_dataP;
	PF_ParamDef		def;
	//----------------------------------------------------------------
	//位置の指定
	AEFX_CLR_STRUCT(def);
	PF_ADD_POINT(STR_POS,		//"New Center"
		50,	// X
		50,	// Y
		0,	// Flag
		ID_POS
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(STR_LENGTH,	//パラメータの名前
		0, 		//数値入力する場合の最小値
		1000,			//数値入力する場合の最大値
		0,				//スライダーの最小値
		200,			//スライダーの最大値
		0,				//デフォルトの値
		ID_LENGTH
	);		
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_STRONG,	//Name
		0,						//VALID_MIN
		100,					//VALID_MAX
		0,						//SLIDER_MIN
		10,					//SLIDER_MAX
		1,						//CURVE_TOLERANCE
		1,						//DFLT
		1,						//PREC
		0,						//DISP
		0,						//WANT_PHASE
		ID_STRONG
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_ISCOLOR,
		STR_ON,
		FALSE,
		0,
		ID_ISCOLOR
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
	/*
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_R,	//Name
						-2,						//VALID_MIN
						2,						//VALID_MAX
						-1,						//SLIDER_MIN
						1,						//SLIDER_MAX
						1,						//CURVE_TOLERANCE
						0,						//DFLT
						1,						//PREC
						0,						//DISP
						0,						//WANT_PHASE
						ID_R
						);

	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	def.flags		=	PF_ParamFlag_SUPERVISE	|
					PF_ParamFlag_CANNOT_TIME_VARY |
					PF_ParamFlag_CANNOT_INTERP;
	PF_ADD_CHECKBOX(STR_NOISE_FRAME1,
					STR_NOISE_FRAME2,
					TRUE,
					0,
					ID_NOISE_FRAME
					);
	//----------------------------------------------------------------
		AEFX_CLR_STRUCT(def);
		def.flags = PF_ParamFlag_START_COLLAPSED;	//これをつけると表示時に開いた状態になる
		PF_ADD_TOPIC(STR_TOPIC, ID_TOPIC);
	//----------------------------------------------------------------
	//色の指定
	AEFX_CLR_STRUCT(def);
	def.flags = PF_ParamFlag_CANNOT_TIME_VARY;//これをつけるとキーフレームが撃てなくなる
	PF_ADD_COLOR(	STR_COLOR,
					0xFF,
					0xFF,
					0xFF,
					ID_COLOR
					);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	//def.flags = PF_ParamFlag_CANNOT_TIME_VARY;//これをつけるとキーフレームが撃てなくなる
	PF_ADD_FIXED(	STR_FIXED_SLIDER,	//パラメータの名前
					0, 				//数値入力する場合の最小値
					100,			//数値入力する場合の最大値
					0,				//スライダーの最小値
					100,			//スライダーの最大値
					100,			//デフォルトの値
					1,				//数値表示に関するフラグ
					0,
					0,
					ID_FIXED_SLIDER
					);
	//----------------------------------------------------------------
	//角度
	AEFX_CLR_STRUCT(def);
	PF_ADD_ANGLE(STR_ANGLE,0,ID_ANGLE);
	//----------------------------------------------------------------

	*/

					
//----------------------------------------------------------------
	out_data->num_params = ID_NUM_PARAMS;
	return err;
};
// **********************************************************
PF_Err Shine::GetParams(ParamInfo* infoP)
{
	PF_Err err = PF_Err_NONE;

	PF_FixedPoint fp;
	ERR(GetPOINT_FIXED(ID_POS, &fp));
	if (!err)
	{
		infoP->pos.x = (A_FpLong)((A_FpLong)fp.x / 65536);
		infoP->pos.y = (A_FpLong)((A_FpLong)fp.y / 65536);
	}
	ERR(GetADD(ID_LENGTH, &infoP->length));
	if (!err)
	{
		if (infoP->length < 0) infoP->length = 0;
	}
	ERR(GetFLOAT(ID_STRONG, &infoP->strong));
	if (!err)
	{
		infoP->strong /= 100;
		if (infoP->strong < 0) infoP->strong = 0;
		if (infoP->strong > 1) infoP->strong = 1;
	}
	ERR(GetCHECKBOX(ID_ISCOLOR, &infoP->isColor));
	ERR(GetCOLOR(ID_COLOR, &infoP->Color));

	return err;
};
// **********************************************************
PF_Err Shine::Exec(ParamInfo* infoP)
{
	PF_Err err = PF_Err_NONE;


	infoP->inP = new NFsWorld(input, in_data, pixelFormat());
	infoP->outP = new NFsWorld(output, in_data, pixelFormat());
	if ((infoP->length <= 0) || (infoP->strong <= 0))
	{
		infoP->outP->Copy(infoP->inP);
	}
	else {
		PF_EffectWorld buf1 = NewEffectWorld(infoP->outP->width() / 2, infoP->outP->height() / 2, pixelFormat());
		PF_EffectWorld buf2 = NewEffectWorld(infoP->outP->width() / 2, infoP->outP->height() / 2, pixelFormat());
		infoP->bufP1 = new NFsWorld(&buf1, in_data, pixelFormat());
		infoP->bufP2 = new NFsWorld(&buf2, in_data, pixelFormat());
		switch (pixelFormat())
		{
		case PF_PixelFormat_ARGB128:
			infoP->outP->Copy(infoP->inP);
			//iterate32(src->world, (void*)infoP, NFsNoise32, dst->world);
			break;
		case PF_PixelFormat_ARGB64:
			infoP->outP->Copy(infoP->inP);
			//iterate16(src->world, (void*)infoP, NFsNoise16, dst->world);
			break;
		case PF_PixelFormat_ARGB32:
			CopyIntoBuf1_8(infoP);
			iterate8(infoP->bufP1->world, (void*)infoP, sub8, infoP->bufP2->world);
			CopyBuf2ToDst_8(infoP);
			infoP->outP->FromMat();
			break;
		default:
			break;
		}

		delete infoP->bufP1;
		delete infoP->bufP2;
	}
	delete infoP->inP;
	delete infoP->outP;
	return err;

};
// **********************************************************
