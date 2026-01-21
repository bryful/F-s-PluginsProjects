#include "RimFill.h"
#define AE_CLAMP(val, min, max)  ((val) < (min) ? (min) : ((val) > (max) ? (max) : (val)))


//-----------------------------------------------------------------------------------
static PF_Err
RimFillH16(
	void* refconPV,
	A_long thread_idxL,
	A_long y,
	A_long itrtL)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo* infoP = static_cast<ParamInfo*>(refconPV);
	PF_InData* in_data = infoP->in_data;
	A_long bw = infoP->lineWidth;
	A_long w = infoP->scrW;
	A_long w2 = w +(24-(w%24));

	PF_EffectWorld wld;
	AEFX_CLR_STRUCT(wld);
	ERR((*in_data->utils->new_world)(in_data->effect_ref, w2, 8, PF_NewWorldFlag_DEEP_PIXELS, &wld));
	if (err) return err;

	PF_Pixel16* dst = (PF_Pixel16*)infoP->dst->data + y*infoP->scrWt;
	PF_Pixel16* line = (PF_Pixel16*)wld.data;
	A_long* lineD = (A_long*)(line +w2);
	A_long* lineT = lineD+w2;

	//lineに複写
	for (A_long i=0; i<w;i++) {
		line[i] = dst[i];
		if (infoP->isWhite == TRUE) {
			if (dst[i].red >= PF_MAX_CHAN16 && dst[i].green >= PF_MAX_CHAN16 && dst[i].blue >= PF_MAX_CHAN16) {
				line[i] = { 0,0,0,0 };
			}
		}
		lineD[i] = 0;
		if (line[i].alpha == 0) {
			lineT[i] = 1;
		}
		else {
			lineT[i] = 0;
		}
	}
	//境界検出
	for (A_long i = 0; i < w-1; i++) {
		if (line[i].alpha != PF_MAX_CHAN16 && line[i + 1].alpha == PF_MAX_CHAN16)
		{
			lineD[i] =1;
		};
	}
	for (A_long i = w-1; i > 0; i--) {
		if (line[i].alpha != PF_MAX_CHAN16 && line[i - 1].alpha == PF_MAX_CHAN16)
		{
			lineD[i] = 2;
		};
	}

	PF_Pixel16 col = infoP->customColor16;
	for (A_long x = 0; x < w; x++)
	{
		if (lineD[x] == 1) {
			if (infoP->isCustomColor == FALSE)
			{
				col = line[x+1];
			}
			A_long xl = AE_CLAMP(x - bw, 0, infoP->scrW - 1);
			for(A_long i = x; i > xl; i--) {
				if (lineT[i] == 0) break;
				dst[i] = col;
			}
		}
		else if (lineD[x] == 2) {
			if (infoP->isCustomColor == FALSE)
			{
				col = line[x - 1];
			}
			A_long xr = AE_CLAMP(x + bw, 0, infoP->scrW - 1);
			for (A_long i = x; i < xr; i++) {
				if (lineT[i] == 0) break;
					dst[i] = col;
			}
		}
		
	}
	ERR((*in_data->utils->dispose_world)(in_data->effect_ref, &wld));

	return err;
}
//-----------------------------------------------------------------------------------
static PF_Err
RimFillV16(
	void* refconPV,
	A_long thread_idxL,
	A_long x,
	A_long itrtL)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo* infoP = static_cast<ParamInfo*>(refconPV);
	PF_InData* in_data = infoP->in_data;
	A_long bw = infoP->lineWidth;
	//A_long wwwww = infoP->scrW;
	A_long wt = infoP->scrWt;
	A_long h = infoP->scrH;

	A_long h2 = h + (24 - (h % 24));

	PF_EffectWorld wld;
	AEFX_CLR_STRUCT(wld);
	ERR((*in_data->utils->new_world)(in_data->effect_ref, h2, 8, PF_NewWorldFlag_DEEP_PIXELS, &wld));
	if (err) return err;

	PF_Pixel16* dst = (PF_Pixel16*)infoP->dst->data + x;
	PF_Pixel16* line = (PF_Pixel16*)wld.data;
	A_long* lineD = (A_long*)(line + h2);
	A_long* lineT = lineD + h2;

	//lineに複写
	for (A_long i = 0; i < h; i++) {
		line[i] = dst[i*wt];
		if (infoP->isWhite == TRUE) {
			if (line[i].red >= PF_MAX_CHAN16 && line[i].green >= PF_MAX_CHAN16 && line[i].blue >= PF_MAX_CHAN16) {
				line[i] = { 0,0,0,0 };
			}
		}
		lineD[i] = 0;
		if (line[i].alpha == 0) {
			lineT[i] = 1;
		}
		else {
			lineT[i] = 0;
		}
	}
	//境界検出
	for (A_long i = 0; i < h - 1; i++) {
		if (line[i].alpha != PF_MAX_CHAN16 && line[i + 1].alpha == PF_MAX_CHAN16)
		{
			lineD[i] = 1;
		};
	}
	for (A_long i = h - 1; i > 0; i--) {
		if (line[i].alpha != PF_MAX_CHAN16 && line[i - 1].alpha == PF_MAX_CHAN16)
		{
			lineD[i] = 2;
		};
	}

	PF_Pixel16 col = infoP->customColor16;
	for (A_long y = 0; y < h; y++)
	{
		if (lineD[y] == 1) {
			if (infoP->isCustomColor == FALSE)
			{
				col = line[y + 1];
			}
			A_long yt = AE_CLAMP(y - bw, 0, h - 1);
			for (A_long i = y; i > yt; i--) {
				if (lineT[i] == 0) break;
				dst[i*wt] = col;
			}
		}
		else if (lineD[y] == 2) {
			if (infoP->isCustomColor == FALSE)
			{
				col = line[y - 1];
			}
			A_long yb = AE_CLAMP(y + bw, 0, h - 1);
			for (A_long i = y; i < yb; i++) {
				if (lineT[i] == 0) break;
				dst[ i * wt] = col;
			}
		}

	}
	ERR((*in_data->utils->dispose_world)(in_data->effect_ref, &wld));

	return err;
}
static PF_Err RimFill_SubImpl(
    CFsAE* ae,
	ParamInfo* infoP
)
{
    PF_Err err = PF_Err_NONE;

    if (infoP->lineWidth == 0) return err;
	infoP->scrW = ae->out->width();
	infoP->scrWt = ae->out->widthTrue();
	infoP->scrH = ae->out->height();
	infoP->in_data = ae->in_data;
	infoP->dst = ae->output;

    if (!err) {
		PF_Iterate8Suite1* iterS = nullptr;
		AEFX_SuiteScoper<PF_Iterate8Suite1> iter_scope(
			ae->in_data,
			kPFIterate8Suite,
			kPFIterate8SuiteVersion1,
			ae->out_data
		);
        switch (ae->pixelFormat())
        {
        case PF_PixelFormat_ARGB32:
			//ERR(ae->iterate16(&infoP, RimFill8));

            break;
        case PF_PixelFormat_ARGB64:
			ERR(iter_scope->iterate_generic(
				infoP->scrH,			 // iterationsL: 実行回数（＝行数）
				infoP,                  // refconPV: ユーザー定義データ
				RimFillH16              // fn_func: コールバック関数
			));
			ERR(iter_scope->iterate_generic(
				infoP->scrW,			 // iterationsL: 実行回数（＝行数）
				infoP,                  // refconPV: ユーザー定義データ
				RimFillV16              // fn_func: コールバック関数
			));
            break;
        case PF_PixelFormat_ARGB128:
			//ERR(ae->iterate32(&infoP, RimFill32));

            break;
        default:
            err = PF_Err_BAD_CALLBACK_PARAM;
            break;
        }
    }
    return err;
}


PF_Err RimFill_Sub(
    CFsAE* ae,
	ParamInfo* infoP
)
{
    return RimFill_SubImpl(ae, infoP);
}
