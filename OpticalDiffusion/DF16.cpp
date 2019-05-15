#include "OpticalDiffusion.h"

//ブレンド入れ替え用
static PF_Err (*blendFunc16)(refconType refcon, A_long xL, A_long yL,PF_Pixel16 *inP,PF_Pixel16 *outP);

//-------------------------------------------------------------------------------------------------
//デバンド対策の気休めノイズ
//-------------------------------------------------------------------------------------------------
static PF_Err 
noise16 (
	refconType		refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel16	*inP, 
	PF_Pixel16	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo *	niP		= reinterpret_cast<ParamInfo*>(refcon);

	A_long v = (A_long)(niP->noiseValue * PF_MAX_CHAN16);
	A_long rnd =  v  -  (v * 2 * F_RAND() / F_RAND_MAX);
	if (rnd!=0){
		if (outP->alpha>0){
			outP->red	= RoundShort( outP->red + rnd);
			outP->green	= RoundShort( outP->green + rnd);
			outP->blue	= RoundShort( outP->blue + rnd);
		}
	}
	return err;
}

//-------------------------------------------------------------------------------------------------
//抽出
//-------------------------------------------------------------------------------------------------
static PF_Err 
extract16 (
	refconType		refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel16	*inP, 
	PF_Pixel16	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo *	niP		= reinterpret_cast<ParamInfo*>(refcon);
	
	//とりあえず消す。
	A_long a = outP->alpha;
	outP->alpha	= 0;

	//アルファーを考慮して輝度を求める
	A_long v = (A_long)( (( 0.29891 * (PF_FpLong)outP->red) + ( 0.58661 * (PF_FpLong)outP->green) + ( 0.11448 * (PF_FpLong)outP->blue)) * ((PF_FpLong)a / PF_MAX_CHAN16) );
	A_long v2 = v;

	A_long b0 = FS_CONVERT8TO16(niP->blackPoint - niP->blackSoftness);
	A_long b1 = FS_CONVERT8TO16(niP->blackPoint);
	A_long w0 = FS_CONVERT8TO16(niP->whitePoint);
	A_long w1 = FS_CONVERT8TO16(niP->whitePoint + niP->whiteSoftness);
	if ( v<b0){
		v2 = 0;
	}else if (v<b1){
		v2 = PF_MAX_CHAN16 * (v-b0)/(b1-b0);
	}else if(v<=w0){
		v2 = PF_MAX_CHAN16;
	}else if(v<w1){
		v2 = PF_MAX_CHAN16 - (PF_MAX_CHAN16 * (v- w0)/(w1-w0));
	}else{
		v2 = 0;
	}
	if (niP->invert){
		v2 = PF_MAX_CHAN16 - v2;
	}
	outP->alpha	= RoundShort(a * v2 / PF_MAX_CHAN16);

	//指定した色を残す
	if((niP->extract_color_Count>0)&&(outP->alpha<PF_MAX_CHAN16)){
		PF_PixelFloat p32 = CONV16TO32(*inP);
		HLSA hlsa = RGBtoHLS(p32);
		for ( int i=0; i<niP->extract_color_Count; i++){
			PF_Fixed ah = (PF_Fixed)(F_ABS(hlsa.H - niP->extract_colors[i].H) * 65536);
			PF_Fixed as = (PF_Fixed)(F_ABS(hlsa.S - niP->extract_colors[i].S) * 65536);
			if ( (ah<=niP->extract_color_Range)&&(as<=niP->extract_color_Range)){
				PF_Fixed al = (PF_Fixed)(F_ABS(hlsa.L - niP->extract_colors[i].L) * 65536);
				if (al<0x4000) {
					A_u_short a2 = RoundShort( PF_MAX_CHAN16 *(0x4000 - al) /0x4000);
					if ( outP->alpha < a2) outP->alpha = a2; 
					break;
				}
			}
		}
	}

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
persentBlend16 (
	refconType		refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel16	*inP, 
	PF_Pixel16	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo *	niP		= reinterpret_cast<ParamInfo*>(refcon);

	PF_Pixel16 i = *inP;
	PF_Pixel16 o = *outP;

	if(niP->blendOpacity >= 1){
		//
	}else if (niP->blendOpacity <= 0){
		*outP = i;
	}else{
		PF_FpLong r1 = niP->blendOpacity;
		PF_FpLong r2 = 1 -r1;

		o.alpha = RoundShortFpLong( (PF_FpLong)o.alpha * r1 + (PF_FpLong)i.alpha * r2);
		if (o.alpha ==0){
			o.blue = o.green = o.red = 0;
		}else{
			//マット合成に直す
			i.red	= RoundShortFpLong( (A_long)i.red	 * (A_long)i.alpha / PF_MAX_CHAN16);
			i.green	= RoundShortFpLong( (A_long)i.green * (A_long)i.alpha / PF_MAX_CHAN16);
			i.blue	= RoundShortFpLong( (A_long)i.blue * (A_long)i.alpha / PF_MAX_CHAN16);

			//マット合成に直す
			o.red	= RoundShortFpLong( (A_long)o.red	 * (A_long)o.alpha / PF_MAX_CHAN16);
			o.green	= RoundShortFpLong( (A_long)o.green * (A_long)o.alpha / PF_MAX_CHAN16);
			o.blue	= RoundShortFpLong( (A_long)o.blue * (A_long)o.alpha / PF_MAX_CHAN16);

			o.red	= RoundShortFpLong( ((PF_FpLong)o.red   * r1 + (PF_FpLong)i.red   * r2 )*PF_MAX_CHAN16 / (PF_FpLong)o.alpha); 
			o.green	= RoundShortFpLong( ((PF_FpLong)o.green * r1 + (PF_FpLong)i.green * r2 )*PF_MAX_CHAN16 / (PF_FpLong)o.alpha); 
			o.blue	= RoundShortFpLong( ((PF_FpLong)o.blue  * r1 + (PF_FpLong)i.blue  * r2 )*PF_MAX_CHAN16 / (PF_FpLong)o.alpha); 
		}

		*outP = o;

	}
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
normalBlend16 (
	refconType		refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel16	*inP, 
	PF_Pixel16	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo *	niP		= reinterpret_cast<ParamInfo*>(refcon);

	PF_Pixel16 i = *inP;
	PF_Pixel16 o = *outP;

	*outP =  PixelBlend16(i,o);
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
ligtenBlend16 (
	refconType		refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel16	*inP, 
	PF_Pixel16	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo *	niP		= reinterpret_cast<ParamInfo*>(refcon);

	PF_Pixel16 i = *inP;
	PF_Pixel16 o = *outP;
	
	
	if (i.alpha <= 0){
		return err;
	}

	o = PixelBlend16(i,o);

	if (o.red<i.red)
		o.red = i.red;
	if (o.green<i.green)
		o.green = i.green;
	if (o.blue<i.blue)
		o.blue = i.blue;
	
	*outP = o;


	return err;
}


//-------------------------------------------------------------------------------------------------
static PF_Err 
darkenBlend16 (
	refconType		refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel16	*inP, 
	PF_Pixel16	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo *	niP		= reinterpret_cast<ParamInfo*>(refcon);

	PF_Pixel16 i = *inP;
	PF_Pixel16 o = *outP;

	if (i.alpha <= 0){
		return err;
	}

	o = PixelBlend16(i,o);

	if (o.red>i.red)
		o.red = i.red;
	if (o.green>i.green)
		o.green = i.green;
	if (o.blue>i.blue)
		o.blue = i.blue;
	
	*outP = o;


	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
screenBlend16 (
	refconType		refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel16	*inP, 
	PF_Pixel16	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo *	niP		= reinterpret_cast<ParamInfo*>(refcon);

	PF_Pixel16 i = *inP;
	PF_Pixel16 o = *outP;

	if (i.alpha <= 0){
		return err;
	}else if(o.alpha <=0){
		*outP = i;
		return err;
	}
	A_u_short a	=  RoundShort(i.alpha + o.alpha -  (i.alpha * o.alpha)/PF_MAX_CHAN16); 

	if (a == 0){
		o.blue = o.green = o.red = 0;
		o.alpha = 0;
	}else{
		//マット合成に直す
		i.red	= RoundShort( (A_long)i.red	  * (A_long)i.alpha / PF_MAX_CHAN16);
		i.green	= RoundShort( (A_long)i.green * (A_long)i.alpha / PF_MAX_CHAN16);
		i.blue	= RoundShort( (A_long)i.blue  * (A_long)i.alpha / PF_MAX_CHAN16);

		o.red	= RoundShort( (A_long)o.red	  * (A_long)o.alpha / PF_MAX_CHAN16);
		o.green	= RoundShort( (A_long)o.green * (A_long)o.alpha / PF_MAX_CHAN16);
		o.blue	= RoundShort( (A_long)o.blue  * (A_long)o.alpha / PF_MAX_CHAN16);


		if (a==PF_MAX_CHAN16){
			o.red	= RoundShort((i.red   + o.red   -  (i.red   * o.red)  /PF_MAX_CHAN16)); 
			o.green = RoundShort((i.green + o.green -  (i.green * o.green)/PF_MAX_CHAN16)); 
			o.blue  = RoundShort((i.blue  + o.blue -   (i.blue  * o.blue) /PF_MAX_CHAN16)); 
			o.alpha = PF_MAX_CHAN16;
		}else{
			o.red	=  RoundShort((i.red   + o.red   -  (i.red   * o.red)  /PF_MAX_CHAN16)*PF_MAX_CHAN16/a); 
			o.green =  RoundShort((i.green + o.green -  (i.green * o.green)/PF_MAX_CHAN16)*PF_MAX_CHAN16/a); 
			o.blue  =  RoundShort((i.blue  + o.blue -   (i.blue  * o.blue) /PF_MAX_CHAN16)*PF_MAX_CHAN16/a); 
			o.alpha = a;
		}

	}
	*outP = o;

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
multiplyBlend16 (
	refconType		refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel16	*inP, 
	PF_Pixel16	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo *	niP		= reinterpret_cast<ParamInfo*>(refcon);

	PF_Pixel16 i = *inP;
	PF_Pixel16 o = *outP;

	if (i.alpha <= 0){
		return err;
	}else if(o.alpha <=0){
		*outP = i;
		return err;
	}


	A_u_short a	=  RoundShort(i.alpha + o.alpha -  (i.alpha * o.alpha)/PF_MAX_CHAN16); 
	if (a ==0){
		o.blue = o.green = o.red = 0;
		o.alpha = 0;
		
	}else{
		//白マットへ
		i.red	= RoundShort( ((A_long)i.red  * (A_long)i.alpha)/PF_MAX_CHAN16 + PF_MAX_CHAN16-(A_long)i.alpha);
		i.green	= RoundShort( ((A_long)i.green* (A_long)i.alpha)/PF_MAX_CHAN16 + PF_MAX_CHAN16-(A_long)i.alpha);
		i.blue	= RoundShort( ((A_long)i.blue * (A_long)i.alpha)/PF_MAX_CHAN16 + PF_MAX_CHAN16-(A_long)i.alpha);

		o.red	= RoundShort( ((A_long)o.red  * (A_long)o.alpha)/PF_MAX_CHAN16 + PF_MAX_CHAN16-(A_long)o.alpha);
		o.green	= RoundShort( ((A_long)o.green* (A_long)o.alpha)/PF_MAX_CHAN16 + PF_MAX_CHAN16-(A_long)o.alpha);
		o.blue	= RoundShort( ((A_long)o.blue * (A_long)o.alpha)/PF_MAX_CHAN16 + PF_MAX_CHAN16-(A_long)o.alpha);

		if (a == PF_MAX_CHAN16){
			o.red	=  RoundShort(i.red   * o.red  /PF_MAX_CHAN16); 
			o.green =  RoundShort(i.green * o.green/PF_MAX_CHAN16); 
			o.blue  =  RoundShort(i.blue  * o.blue /PF_MAX_CHAN16); 
		}else{
			o.red	= RoundShort(((i.red  * o.red  /PF_MAX_CHAN16) - PF_MAX_CHAN16 + a ) * PF_MAX_CHAN16/a);
			o.green	= RoundShort(((i.green* o.green/PF_MAX_CHAN16) - PF_MAX_CHAN16 + a ) * PF_MAX_CHAN16/a);
			o.blue	= RoundShort(((i.blue * o.blue /PF_MAX_CHAN16) - PF_MAX_CHAN16 + a ) * PF_MAX_CHAN16/a);
		}
		o.alpha = a;
	}

	*outP = o;

	return err;
}
//******************************************************************
PF_Err DF16(CFsAE *ae , ParamInfo *infoP)
{
	PF_Err	err = PF_Err_NONE;
	ERR(ae->CopyInToOut());
	
	if (infoP->blendOpacity<=0){
		return err;
	}

	//画像抽出
	if (infoP->extractEnabled){
		ERR(ae->iterate16((refconType)infoP,extract16));
	}
	
	//最大･最小
	//最大･最小
	if (infoP->minimax1!=0){
		ae->out->Minimax16(infoP->minimax1);
	}
	if (infoP->minimax2!=0){
		ae->out->Minimax16(infoP->minimax2);
	}
	//ぼかし
	if (infoP->blur>0){
		ae->out->gblur16(infoP->blur);
	}
	//ブレンド
	if (infoP->blendMode != BLEND_MODE::none){
		blendFunc16 = NULL;

		switch(infoP->blendMode){
		case BLEND_MODE::Normal:
			blendFunc16 = normalBlend16;
			break;
		case BLEND_MODE::Lighten:
			blendFunc16 = ligtenBlend16;
			break;
		case BLEND_MODE::Darken:
			blendFunc16 = darkenBlend16;
			break;
		case BLEND_MODE::Screen:
			blendFunc16 = screenBlend16;
			break;
		case BLEND_MODE::Multiply:
			blendFunc16 = multiplyBlend16;
			break;
		}
		if (blendFunc16 != NULL){
			ERR(ae->iterate16((refconType)infoP,blendFunc16));
		}
	}
	if (infoP->blendOpacity<1){
		ERR(ae->iterate16((refconType)infoP,persentBlend16));
	}
	if (infoP->noiseValue>0){
		F_SRAND(ae->frame());
		ERR(ae->iterate16((refconType)infoP,noise16));
	}
	
	
	return err;	
}
//******************************************************************
