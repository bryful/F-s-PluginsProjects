//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------

#include "SelectedBlur.h"

typedef struct blurPrmSub32
{
	A_long		value;
	A_long		tbl[1024];//451
	PF_PixelFloat	*data;
	PF_PixelFloat	*scanline;
	A_long		width;
	A_long		height;
	A_long		widthTrue;
}blurPrmSub32;
//----------------------------------------------------------------------------
PF_Boolean makeTbl32(blurPrm *prm, blurPrmSub32 *sub)
{
	PF_InData *in_data;
	in_data = prm->in_data;
	A_long range	= prm->blur_value;
	if ( range<=0) return FALSE; 
	double zone	= (double)range/3;
	for (A_long i = 0; i <= range; i++) {
		sub->tbl[i] = (A_long)(PF_EXP(-i * i / (2 * zone * zone))*0x100);
	}
	return TRUE;
}
//----------------------------------------------------------------------------
void horBlur32(blurPrmSub32 *sub)
{
	PF_PixelFloat c;
	A_long hor =0;
	A_long ix,i;
	A_long x=0, y=0;
	A_long count, gauss;
	double sr, sg, sb;
	hor =0;
	for ( y=0; y<sub->height; y++)
	{
		hor = sub->widthTrue * y;
		//スキャンラインへ転送
		for ( x=0; x<sub->width; x++) sub->scanline[x] = sub->data[hor + x];
		for ( x=0; x<sub->width; x++){
			c = sub->scanline[x];
			if ( c.alpha>= 1.0)
			{
				sr = sg = sb = 0; 
				count = gauss = 0;
				//真ん中
				gauss = sub->tbl[0];
				sr += c.red * gauss;
				sg += c.green * gauss;
				sb += c.blue * gauss;
				count += gauss;
				//左
				for (i=1; i<=sub->value;i++)
				{
					ix = x-i;
					if ( ix<0) break;
					c = sub->scanline[ix];
					if ( c.alpha ==0) break;
					gauss = sub->tbl[i];
					sr += c.red * gauss;
					sg += c.green * gauss;
					sb += c.blue * gauss;
					count += gauss;
				}
				//左
				for (i=1; i<=sub->value;i++)
				{
					ix = x+i;
					if ( ix>=sub->width) break;
					c = sub->scanline[ix];
					if ( c.alpha ==0) break;
					gauss = sub->tbl[i];
					sr += c.red * gauss;
					sg += c.green * gauss;
					sb += c.blue * gauss;
					count += gauss;
				}
				if (count>0){
					c.red	= RoundFpShortDouble(sr/count);                
					c.green = RoundFpShortDouble(sg/count);                
					c.blue	= RoundFpShortDouble(sb/count);
					c.alpha = 1;
					sub->data[hor + x] = c;
				}
			}
		}
	}

}
//----------------------------------------------------------------------------
void verBlur32(blurPrmSub32 *sub)
{
	PF_PixelFloat c;
	A_long hor =0;
	A_long x=0, y=0;
	A_long ix,i;
	A_long count, gauss;
	double sr, sg, sb;
	hor =0;
	for ( x=0; x<sub->width; x++)
	{
		//スキャンラインへ転送
		hor = x;
		for ( y=0; y<sub->height; y++) {
			sub->scanline[y] = sub->data[hor];
			hor += sub->widthTrue;
		}
		hor = x;
		for ( y=0; y<sub->height; y++){
			c = sub->scanline[y];
			if ( c.alpha>=1)
			{
				sr = sg = sb = 0; 
				count = gauss = 0;
				//真ん中
				gauss = sub->tbl[0];
				sr += c.red * gauss;
				sg += c.green * gauss;
				sb += c.blue * gauss;
				count += gauss;
				//左
				for (i=1; i<=sub->value;i++)
				{
					ix = y-i;
					if ( ix<0) break;
					c = sub->scanline[ix];
					if ( c.alpha ==0) break;
					gauss = sub->tbl[i];
					sr += c.red * gauss;
					sg += c.green * gauss;
					sb += c.blue * gauss;
					count += gauss;
				}
				//左
				for (i=1; i<=sub->value;i++)
				{
					ix = y+i;
					if ( ix>=sub->height) break;
					c = sub->scanline[ix];
					if ( c.alpha ==0) break;
					gauss = sub->tbl[i];
					sr += c.red * gauss;
					sg += c.green * gauss;
					sb += c.blue * gauss;
					count += gauss;
				}
				if (count>0){
					c.red	= RoundFpShortDouble(sr/count);                
					c.green = RoundFpShortDouble(sg/count);                
					c.blue	= RoundFpShortDouble(sb/count);
					c.alpha = 1;
					sub->data[hor] = c;
				}
			}
			hor +=sub->widthTrue;;
		}
	}
}
//----------------------------------------------------------------------------

PF_Err BlurSub32(blurPrm *prm)
{
	PF_Err err = PF_Err_NONE;
	if ( (prm->data == NULL)||(prm->scanlineH == NULL)){
		return PF_Err_INTERNAL_STRUCT_DAMAGED;
	}
	blurPrmSub32 sub;
	sub.data = (PF_PixelFloat *)prm->data;
	sub.scanline =*(PF_PixelFloat**)prm->scanlineH; 
	sub.width = prm->width;
	sub.height = prm->height;
	sub.widthTrue = prm->widthTrue;

	sub.value = prm->blur_value;
	if ( makeTbl32(prm, &sub) == TRUE)  {
		horBlur32(&sub);
		verBlur32(&sub);

		//ボケの破綻をごまかす処理
		sub.value = prm->blur_value/4;
		if ( makeTbl32(prm, &sub) == TRUE) horBlur32(&sub);
		sub.value = prm->blur_value/16;
		if ( makeTbl32(prm, &sub) == TRUE) verBlur32(&sub);
		sub.value = prm->blur_value/64;
		if ( makeTbl32(prm, &sub) == TRUE) horBlur32(&sub);
	}
	
	
	return err;
}
//----------------------------------------------------------------------------
