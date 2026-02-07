#include "TargetGradRadical.h"
//-----------------------------------------------------------------------------------


/**
 * 8bit比較用のインライン関数
 */
static inline PF_Boolean IsMatch(ColorFilterInfo* ci, PF_Pixel c) {
    for (A_long i = 0; i < ci->color_count; i++) {
        if (c.red == ci->match_colors[i].red &&
            c.green == ci->match_colors[i].green &&
            c.blue == ci->match_colors[i].blue) {

            return TRUE;
        }
    }
    return FALSE;
}

/**
 * 8-bit環境 (ARGB32)
 */
static PF_Err ExtractColor8(
    void* refcon, 
    A_long x, 
    A_long y, 
    PF_Pixel8* in, 
    PF_Pixel8* out) {
    ColorFilterInfo* ci = (ColorFilterInfo*)refcon;
    if (IsMatch(ci, *in)) {
        *out = *in;
    }
    else {
        out->red = out->green = out->blue = out->alpha = 0;
    }
    return PF_Err_NONE;
}

/**
 * 16-bit環境 (ARGB64)
 * 計算式: (double)V / 128 + 0.5 を適用
 */
static PF_Err ExtractColor16(
    void* refcon, A_long x,
    A_long y, 
    PF_Pixel16* in, 
    PF_Pixel16* out) {
    ColorFilterInfo* ci = (ColorFilterInfo*)refcon;

	PF_Pixel c = NF_Pixel16To8(*in);

    if (IsMatch(ci, c)) {
        *out = *in;
    }
    else {
        out->red = out->green = out->blue = out->alpha = 0;
    }
    return PF_Err_NONE;
}

/**
 * 32-bit Float環境 (ARGB128)
 */
static PF_Err ExtractColorFloat(
    void* refcon, 
    A_long x, 
    A_long y, 
    PF_PixelFloat* in, 
    PF_PixelFloat* out)
{
    ColorFilterInfo* ci = (ColorFilterInfo*)refcon;

    PF_Pixel c = NF_Pixel32To8(*in);

    if (IsMatch(ci, c)) {
        *out = *in;
    }
    else {
        out->red = out->green = out->blue = out->alpha = 0.0f;
    }
    return PF_Err_NONE;
}
static PF_Err ExtractAlpha8(
    void* refcon,
    A_long x,
    A_long y,
    PF_Pixel8* in,
    PF_Pixel8* out) {
    ColorFilterInfo* ci = (ColorFilterInfo*)refcon;

    if (in->alpha > 0) {
        *out = *in;

    }
    else {
        out->red = out->green = out->blue = out->alpha = 0;
    }
    return PF_Err_NONE;
}
static PF_Err ExtractAlpha16(
    void* refcon,
    A_long x,
    A_long y,
    PF_Pixel16* in,
    PF_Pixel16* out) {
    ColorFilterInfo* ci = (ColorFilterInfo*)refcon;

    if (in->alpha > 0) {
        *out = *in;
    }
    else {
        out->red = out->green = out->blue = out->alpha = 0;
    }
    return PF_Err_NONE;
}
static PF_Err ExtractAlphaFloat(
    void* refcon,
    A_long x,
    A_long y,
    PF_PixelFloat* in,
    PF_PixelFloat* out) {
    ColorFilterInfo* ci = (ColorFilterInfo*)refcon;
    if (in->alpha > 0) {
        *out = *in;

    }
    else {
        out->red = out->green = out->blue = out->alpha = 0;
    }
    return PF_Err_NONE;
}
/*
static PF_Err SearchExtractArea8(CFsAE* ae, ParamInfo* info)
{
    A_long width = ae->out->width();
    A_long height = ae->out->height();
    A_long offsetWidth = ae->out->widthTrue();
    PF_Pixel8* data = (PF_Pixel8*)ae->out->data();

    for (A_long y = 0; y < height; y++) {
        PF_Pixel8* data2 = data + y * ae->out->widthTrue();
        for (A_long x = 0; x < width; x++) {
            if (data2[x].alpha > 0) {
                if (x < info->area.left) info->area.left = x;
                if (x > info->area.right) info->area.right = x;
                if (y < info->area.top) info->area.top = y;
                if (y > info->area.bottom) info->area.bottom = y;
            }
        }
    }
    return PF_Err_NONE;

}
static PF_Err SearchExtractArea16(CFsAE* ae, ParamInfo* info)
{
    A_long width = ae->out->width();
    A_long height = ae->out->height();
    A_long offsetWidth = ae->out->widthTrue();
    PF_Pixel16* data = (PF_Pixel16*)ae->out->data();

    for (A_long y = 0; y < height; y++) {
        PF_Pixel16* data2 = data + y * ae->out->widthTrue();
        for (A_long x = 0; x < width; x++) {
            if (data2[x].alpha > 0) {
                if (x < info->area.left) info->area.left = x;
                if (x > info->area.right) info->area.right = x;
                if (y < info->area.top) info->area.top = y;
                if (y > info->area.bottom) info->area.bottom = y;
            }
        }
    }
    return PF_Err_NONE;
}
static PF_Err SearchExtractArea32(CFsAE* ae, ParamInfo* info)
{
    A_long width = ae->out->width();
    A_long height = ae->out->height();
    A_long wt = ae->out->widthTrue();
    PF_PixelFloat* data = (PF_PixelFloat*)ae->out->data();
    PF_PixelFloat* data2;
    for (A_long y = 0; y < height; y++) {
        data2 = data + y * wt;
        for (A_long x = 0; x < width; x++) {
            if (data2[x].alpha > 0) {
                if (x < info->area.left) info->area.left = x;
                if (x > info->area.right) info->area.right = x;
                if (y < info->area.top) info->area.top = y;
                if (y > info->area.bottom) info->area.bottom = y;
            }
        }
    }
    return PF_Err_NONE;
}
*/
static PF_Err ExtractColorL(NF_AE* ae, ParamInfo* info)
{
    PF_Err err = PF_Err_NONE;
    PF_InData* in_data = ae->in_data;
	PF_OutData* out_data = ae->out_data;
    PF_EffectWorld* input = ae->input;
    PF_EffectWorld* output = ae->output;
    //PF_ParamDef* params[],
    ColorFilterInfo ci;
    ci.color_count = info->targetColorCount; // 実際の色数
    for(int i=0; i< ci.color_count; i++) {
        ci.match_colors[i] = info->targetColors[i];
	}
    /*
	info->area.left = ae->out->width()+8;
	info->area.top = ae->out->height()+8;
	info->area.right = 0-8;
	info->area.bottom = 0-8;
    */
    A_long num_rows = ae->output->extent_hint.bottom - ae->output->extent_hint.top;
    switch (ae->pixelFormat()) {
    case PF_PixelFormat_ARGB32:
        if (info->targetColorMode == 1) {
            ERR(ae->suitesP->Iterate8Suite1()->iterate(in_data, 0, num_rows, input, NULL, &ci, ExtractColor8, output));
        }else {
            ERR(ae->suitesP->Iterate8Suite1()->iterate(in_data, 0, num_rows, input, NULL, &ci, ExtractAlpha8, output));
        }
       // ERR(SearchExtractArea8(ae, info));
        break;
    case PF_PixelFormat_ARGB64:
        if (info->targetColorMode == 1) {
            ERR(ae->suitesP->Iterate16Suite1()->iterate(in_data, 0, num_rows, input, NULL, &ci, ExtractColor16, output));
        }else {
            ERR(ae->suitesP->Iterate16Suite1()->iterate(in_data, 0, num_rows, input, NULL, &ci, ExtractAlpha16, output));
        }
        //ERR(SearchExtractArea16(ae, info));
        break;
    case PF_PixelFormat_ARGB128:
        if (info->targetColorMode == 1) {
            ERR(ae->suitesP->IterateFloatSuite1()->iterate(in_data, 0, num_rows, input, NULL, &ci, ExtractColorFloat, output));
        }
        else {
            ERR(ae->suitesP->IterateFloatSuite1()->iterate(in_data, 0, num_rows, input, NULL, &ci, ExtractAlphaFloat, output));
        }
        //ERR(SearchExtractArea32(ae, info));
        break;
    }
    return PF_Err_NONE;
}

PF_Err ExtractColor(NF_AE* ae, ParamInfo* info)
{
    return ExtractColorL(ae, info);
}

