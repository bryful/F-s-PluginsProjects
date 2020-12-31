//-----------------------------------------------------------------------------------
/*
	F's Plugins-cs4 for VS2008
*/
//-----------------------------------------------------------------------------------

#include "EdgeBlur.h"

typedef struct edgeBlurPrmSub8
{
	A_long		blur;
	A_long		offset;
	A_long		tbl[451];
	PF_Pixel8	*data;
	A_u_char	*scanline;
	A_long		width;
	A_long		height;
	A_long		widthTrue;
	PF_InData	*in_data;
}edgeBlurPrmSub8;
//----------------------------------------------------------------------------
PF_Boolean makeTbl(edgeBlurPrm *prm, edgeBlurPrmSub8 *sub)
{
	PF_InData *in_data;
	in_data = prm->in_data;
	A_long range	= prm->blur;
	if ( range<=0) return FALSE; 
	double zone	= (double)range/3;
	for (A_long i = 0; i <= range; i++) {
		sub->tbl[i] = (A_long)(PF_EXP(-i * i / (2 * zone * zone)) * 0x100);
	}
	return TRUE;
}
//----------------------------------------------------------------------------
void horMin(edgeBlurPrmSub8 *sub)
{
	A_long hor =0;
	A_long i;
	A_long x=0, y=0;
	hor =0;
	for ( y=0; y<sub->height; y++)
	{
		hor = sub->widthTrue * y;
		//スキャンラインへ転送
		for ( x=0; x<sub->width; x++) sub->scanline[x] = sub->data[hor + x].alpha;

		A_long minIndex = -1;
		A_u_char minValue = 0xFF;
		for ( x=0; x<sub->width; x++){
			A_long start	= x - sub->offset;
			A_long end		= x + sub->offset;
			if ( (start<0)||(end>=sub->width)){
				minValue = 0;
				minIndex = -1;
			}else if((minIndex>=start)&&(minIndex<=end)){
				if ( sub->scanline[end]<=sub->scanline[minIndex]){
					minValue = sub->scanline[end];
					minIndex = end;
				}else{
					minValue = sub->scanline[minIndex];
				}
			}else{
				minValue = 0xFF;
				for ( i= start; i<=end; i++){
					if ( sub->scanline[i] == 0){
						minValue = 0;
						minIndex = i;
						break;
					}
					if ( minValue > sub->scanline[i])minValue = sub->scanline[i]; 
				}
			}
			sub->data[hor + x].alpha = minValue;
		}
	}

}
//----------------------------------------------------------------------------
void verMin(edgeBlurPrmSub8 *sub)
{
	A_long hor =0;
	A_long i;
	A_long x=0, y=0;
	hor =0;
	for ( x=0; x<sub->width; x++)
	{
		hor = x;
		//スキャンラインへ転送
		for ( y=0; y<sub->height; y++) {
			sub->scanline[y] = sub->data[hor].alpha;
			hor += sub->widthTrue;
		}

		A_long minIndex = -1;
		A_u_char minValue = 0xFF;
		hor =x;
		for ( y=0; y<sub->height; y++){
			A_long start	= y - sub->offset;
			A_long end		= y + sub->offset;
			if ( (start<0)||(end>=sub->height)){
				minValue = 0;
				minIndex = -1;
			}else if((minIndex>=start)&&(minIndex<=end)){
				if ( sub->scanline[end]<=sub->scanline[minIndex]){
					minValue = sub->scanline[end];
					minIndex = end;
				}else{
					minValue = sub->scanline[minIndex];
				}
			}else{
				minValue = 0xFF;
				for ( i= start; i<=end; i++){
					if ( sub->scanline[i] == 0){
						minValue = 0;
						minIndex = i;
						break;
					}
					if ( minValue > sub->scanline[i])minValue = sub->scanline[i]; 
				}
			}
			sub->data[hor].alpha = minValue;
			hor += sub->widthTrue;
		}
	}

}
//----------------------------------------------------------------------------

void horBlur(edgeBlurPrmSub8 *sub)
{
	A_long hor =0;
	A_long ir,il,i;
	A_long x=0, y=0;
	A_long count, gauss,sa;
	hor =0;
	for ( y=0; y<sub->height; y++)
	{
		hor = sub->widthTrue * y;
		//スキャンラインへ転送
		for ( x=0; x<sub->width; x++) sub->scanline[x] = sub->data[hor + x].alpha;
		for ( x=0; x<sub->width; x++){
			count = gauss = 0;
			sa = 0;
			//真ん中
			gauss = sub->tbl[0];
			sa += sub->scanline[x] * gauss;
			count += gauss;
			for (i=1; i<=sub->blur;i++)
			{
				il = x-i;
				gauss = sub->tbl[i];
				if ( il>=0) {
					sa += sub->scanline[il] * gauss;
				}
				count += gauss;
				ir = x + i;
				if ( ir<sub->width) {
					sa += sub->scanline[ir] * gauss;
				}
				count += gauss;
			}
			if (count>0){
				sub->data[hor + x].alpha = RoundByteLong(sa/count);
			}
		}
	}

}
//----------------------------------------------------------------------------
void verBlur(edgeBlurPrmSub8 *sub)
{
	A_long hor =0;
	A_long ir,il,i;
	A_long x=0, y=0;
	A_long count, gauss,sa;
	hor =0;
	for ( x=0; x<sub->width; x++)
	{
		hor = x;
		//スキャンラインへ転送
		for ( y=0; y<sub->height; y++) {
			sub->scanline[y] = sub->data[hor].alpha;
			hor += sub->widthTrue;
		}
		hor = x;
		for ( y=0; y<sub->height; y++){
			count = gauss = 0;
			sa = 0;
			//真ん中
			gauss = sub->tbl[0];
			sa += sub->scanline[y] * gauss;
			count += gauss;
			for (i=1; i<=sub->blur;i++)
			{
				il = y-i;
				gauss = sub->tbl[i];
				if ( il>=0) {
					sa += sub->scanline[il] * gauss;
				}
				count += gauss;
				ir = y + i;
				if ( ir<sub->width) {
					sa += sub->scanline[ir] * gauss;
				}
				count += gauss;
			}
			if (count>0){
				sub->data[hor].alpha = RoundByteLong(sa/count);
			}
			hor += sub->widthTrue;
		}
	}

}

//----------------------------------------------------------------------------

PF_Err EdgeBlurSub8(edgeBlurPrm *prm)
{
	PF_Err err = PF_Err_NONE;
	if ( (prm->data == NULL)||(prm->scanlineH == NULL)){
		return PF_Err_INTERNAL_STRUCT_DAMAGED;
	}
	edgeBlurPrmSub8 sub;
	sub.data = (PF_Pixel8 *)prm->data;
	sub.scanline =*(A_u_char **)prm->scanlineH; 
	sub.width = prm->width;
	sub.height = prm->height;
	sub.widthTrue = prm->widthTrue;
	sub.in_data	 = prm->in_data;

	sub.blur = prm->blur;
	sub.offset = prm->offset;
	
	if ( sub.offset>0){
		horMin(&sub);
		verMin(&sub);
	}
	if ( makeTbl(prm, &sub) == FALSE)  return err;
	
	horBlur(&sub);
	verBlur(&sub);
	
	
	return err;
}
//----------------------------------------------------------------------------
