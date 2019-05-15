#include "OpticalDiffusion.h"

//ブレンド入れ替え用
static PF_Err (*blendFunc32)(refconType refcon, A_long xL, A_long yL,PF_PixelFloat *inP,PF_PixelFloat *outP);

//-------------------------------------------------------------------------------------------------
//デバンド対策の気休めノイズ
//-------------------------------------------------------------------------------------------------
static PF_Err 
noise32 (
	refconType		refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_PixelFloat	*inP, 
	PF_PixelFloat	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo *	niP		= reinterpret_cast<ParamInfo*>(refcon);

	double v = niP->noiseValue;
	double rnd =  v  -  (v * 2 * (double)F_RAND() / F_RAND_MAX);
	if (rnd!=0){
		if (outP->alpha>0){
			outP->red	= RoundFpShortDouble( outP->red + rnd);
			outP->green	= RoundFpShortDouble( outP->green + rnd);
			outP->blue	= RoundFpShortDouble( outP->blue + rnd);
		}
	}
	return err;
}

//-------------------------------------------------------------------------------------------------
//抽出
//-------------------------------------------------------------------------------------------------
static PF_Err 
extract32 (
	refconType		refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_PixelFloat	*inP, 
	PF_PixelFloat	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo *	niP		= reinterpret_cast<ParamInfo*>(refcon);
	
	//とりあえず消す。
	PF_FpShort a = outP->alpha;
	outP->alpha	= 0;

	//アルファーを考慮して輝度を求める
	//無理やり整数化
	double vb = ( (0.29891 * outP->red) + ( 0.58661 * outP->green) + ( 0.11448 * outP->blue)) * a ;
	A_long v = (A_long)((vb * PF_MAX_CHAN16) + 0.5);
	double v2 = v;

	A_long b0 = FS_CONVERT8TO16(niP->blackPoint - niP->blackSoftness);
	A_long b1 = FS_CONVERT8TO16(niP->blackPoint);
	A_long w0 = FS_CONVERT8TO16(niP->whitePoint);
	A_long w1 = FS_CONVERT8TO16(niP->whitePoint + niP->whiteSoftness);
	if ( v<b0){
		v2 = 0;
	}else if (v<b1){
		v2 =  ((double)v-(double)b0)/((double)b1-(double)b0);
	}else if(v<=w0){
		v2 = 1.0;
	}else if(v<w1){
		v2 = 1.0 - (((double)v- (double)w0)/((double)w1-(double)w0));
	}else{
		v2 = 0;
	}
	if (niP->invert){
		v2 = 1.0 - v2;
	}
	outP->alpha	= RoundFpShortDouble(a * v2);

	//指定した色を残す
	if((niP->extract_color_Count>0)&&(outP->alpha<1.0)){
		PF_PixelFloat p32 = *inP;
		HLSA hlsa = RGBtoHLS(p32);
		for ( int i=0; i<niP->extract_color_Count; i++){
			PF_Fixed ah = (PF_Fixed)(F_ABS(hlsa.H - niP->extract_colors[i].H) * 65536);
			PF_Fixed as = (PF_Fixed)(F_ABS(hlsa.S - niP->extract_colors[i].S) * 65536);
			if ( (ah<=niP->extract_color_Range)&&(as<=niP->extract_color_Range)){
				PF_Fixed al = (PF_Fixed)(F_ABS(hlsa.L - niP->extract_colors[i].L) * 65536);
				if (al<0x4000) {
					PF_FpShort a2 = RoundFpShortDouble( (double)(0x4000 - al) /(double)0x4000);
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
persentBlend32 (
	refconType		refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_PixelFloat	*inP, 
	PF_PixelFloat	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo *	niP		= reinterpret_cast<ParamInfo*>(refcon);

	PF_PixelFloat i = *inP;
	PF_PixelFloat o = *outP;

	if(niP->blendOpacity >= 1){
		//
	}else if (niP->blendOpacity <= 0){
		*outP = i;
	}else{

		PF_FpLong r1 = niP->blendOpacity;
		PF_FpLong r2 = 1 -r1;

		o.alpha = RoundFpShortDouble( (PF_FpLong)o.alpha * r1 + (PF_FpLong)i.alpha * r2);
		if (o.alpha ==0){
			o.blue = o.green = o.red = 0;
		}else{
			//マット合成に直す
			i.red	= RoundFpShortDouble( i.red	 * i.alpha);
			i.green	= RoundFpShortDouble( i.green* i.alpha);
			i.blue	= RoundFpShortDouble( i.blue * i.alpha );

			//マット合成に直す
			o.red	= RoundFpShortDouble( o.red	 * o.alpha);
			o.green	= RoundFpShortDouble( o.green* o.alpha);
			o.blue	= RoundFpShortDouble( o.blue * o.alpha);

			o.red	= RoundFpShortDouble( ((PF_FpLong)o.red   * r1 + (PF_FpLong)i.red   * r2 )/ (PF_FpLong)o.alpha); 
			o.green	= RoundFpShortDouble( ((PF_FpLong)o.green * r1 + (PF_FpLong)i.green * r2 )/ (PF_FpLong)o.alpha); 
			o.blue	= RoundFpShortDouble( ((PF_FpLong)o.blue  * r1 + (PF_FpLong)i.blue  * r2 )/ (PF_FpLong)o.alpha); 
		}

		*outP = o;
	}
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
normalBlend32 (
	refconType		refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_PixelFloat	*inP, 
	PF_PixelFloat	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo *	niP		= reinterpret_cast<ParamInfo*>(refcon);

	PF_PixelFloat i = *inP;
	PF_PixelFloat o = *outP;

	*outP =  PixelBlend32(i,o);
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
ligtenBlend32 (
	refconType		refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_PixelFloat	*inP, 
	PF_PixelFloat	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo *	niP		= reinterpret_cast<ParamInfo*>(refcon);

	PF_PixelFloat i = *inP;
	PF_PixelFloat o = *outP;
	
	
	if (i.alpha <= 0){
		return err;
	}

	o = PixelBlend32(i,o);

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
darkenBlend32 (
	refconType		refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_PixelFloat	*inP, 
	PF_PixelFloat	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo *	niP		= reinterpret_cast<ParamInfo*>(refcon);

	PF_PixelFloat i = *inP;
	PF_PixelFloat o = *outP;

	if (i.alpha <= 0){
		return err;
	}

	o = PixelBlend32(i,o);

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
screenBlend32 (
	refconType		refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_PixelFloat	*inP, 
	PF_PixelFloat	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo *	niP		= reinterpret_cast<ParamInfo*>(refcon);

	PF_PixelFloat i = *inP;
	PF_PixelFloat o = *outP;

	if (i.alpha <= 0){
		return err;
	}else if(o.alpha <=0){
		*outP = i;
		return err;
	}
	PF_FpShort a=  RoundFpShortDouble(i.alpha + o.alpha -  (i.alpha * o.alpha)); 

	if (a == 0){
		o.blue = o.green = o.red = 0;
		o.alpha = 0;
	}else{
		//マット合成に直す
		i.red	= RoundFpShortDouble( i.red	  * i.alpha);
		i.green	= RoundFpShortDouble( i.green * i.alpha);
		i.blue	= RoundFpShortDouble( i.blue  * i.alpha);

		o.red	= RoundFpShortDouble( o.red	  * o.alpha);
		o.green	= RoundFpShortDouble( o.green * o.alpha);
		o.blue	= RoundFpShortDouble( o.blue  * o.alpha);

		if (a>=1.0){
			o.red	= RoundFpShortDouble(i.red   + o.red   -  (i.red   * o.red)  ); 
			o.green = RoundFpShortDouble(i.green + o.green -  (i.green * o.green)); 
			o.blue  = RoundFpShortDouble(i.blue  + o.blue -   (i.blue  * o.blue) ); 
			o.alpha = a;
		}else{
			o.red	=  RoundFpShortDouble((i.red   + o.red   -  (i.red   * o.red)  )/a); 
			o.green =  RoundFpShortDouble((i.green + o.green -  (i.green * o.green))/a); 
			o.blue  =  RoundFpShortDouble((i.blue  + o.blue -   (i.blue  * o.blue) )/a); 
			o.alpha = a;
		}

	}
	*outP = o;

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
multiplyBlend32 (
	refconType		refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_PixelFloat	*inP, 
	PF_PixelFloat	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo *	niP		= reinterpret_cast<ParamInfo*>(refcon);

	PF_PixelFloat i = *inP;
	PF_PixelFloat o = *outP;

	if (i.alpha <= 0){
		return err;
	}else if(o.alpha <=0){
		*outP = i;
		return err;
	}


	PF_FpShort a	=  RoundFpShortDouble(i.alpha + o.alpha -  (i.alpha * o.alpha)); 
	if (a ==0){
		o.blue = o.green = o.red = 0;
		o.alpha = 0;
		
	}else{
		//白マットへ
		i.red	= RoundFpShortDouble( (i.red  * i.alpha) + 1.0 - i.alpha);
		i.green	= RoundFpShortDouble( (i.green* i.alpha) + 1.0 - i.alpha);
		i.blue	= RoundFpShortDouble( (i.blue * i.alpha) + 1.0 - i.alpha);

		o.red	= RoundFpShortDouble( (o.red  * o.alpha) + 1.0 - o.alpha);
		o.green	= RoundFpShortDouble( (o.green* o.alpha) + 1.0 - o.alpha);
		o.blue	= RoundFpShortDouble( (o.blue * o.alpha) + 1.0 - o.alpha);

		if (a >= 1.0){
			o.red	=  RoundFpShortDouble(i.red   * o.red  ); 
			o.green =  RoundFpShortDouble(i.green * o.green); 
			o.blue  =  RoundFpShortDouble(i.blue  * o.blue ); 
		}else{
			o.red	= RoundFpShortDouble(((i.red  * o.red  ) - 1.0 + a ) /a);
			o.green	= RoundFpShortDouble(((i.green* o.green) - 1.0 + a ) /a);
			o.blue	= RoundFpShortDouble(((i.blue * o.blue ) - 1.0 + a ) /a);
		}
		o.alpha = a;
	}

	*outP = o;

	return err;
}
//******************************************************************
PF_Err DF32(CFsAE *ae , ParamInfo *infoP)
{
	PF_Err	err = PF_Err_NONE;
	ERR(ae->CopyInToOut());
	
	if (infoP->blendOpacity<=0){
		return err;
	}

	//画像抽出
	if (infoP->extractEnabled){
		ERR(ae->iterate32((refconType)infoP,extract32));
	}
	
	//最大･最小
	//最大･最小
	if (infoP->minimax1!=0){
		ae->out->Minimax32(infoP->minimax1);
	}
	if (infoP->minimax2!=0){
		ae->out->Minimax32(infoP->minimax2);
	}
	//ぼかし
	if (infoP->blur>0){
		ae->out->gblur32(infoP->blur);
	}
	//ブレンド
	if (infoP->blendMode != BLEND_MODE::none){
		blendFunc32 = NULL;

		switch(infoP->blendMode){
		case BLEND_MODE::Normal:
			blendFunc32 = normalBlend32;
			break;
		case BLEND_MODE::Lighten:
			blendFunc32 = ligtenBlend32;
			break;
		case BLEND_MODE::Darken:
			blendFunc32 = darkenBlend32;
			break;
		case BLEND_MODE::Screen:
			blendFunc32 = screenBlend32;
			break;
		case BLEND_MODE::Multiply:
			blendFunc32 = multiplyBlend32;
			break;
		}
		if (blendFunc32 != NULL){
			ERR(ae->iterate32((refconType)infoP,blendFunc32));
		}
	}
	if (infoP->blendOpacity<1){
		ERR(ae->iterate32((refconType)infoP,persentBlend32));
	}
	if (infoP->noiseValue>0){
		F_SRAND(ae->frame());
		ERR(ae->iterate32((refconType)infoP,noise32));
	}
	
	
	return err;	
}
//******************************************************************
