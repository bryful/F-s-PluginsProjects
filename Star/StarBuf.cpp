
#include "Star.h"
typedef struct bufMinParam
{
	A_long		*scanline;
	A_long		scanlineLength;	//scanlineの長さ
	A_long		value;	//Max/Minの値
	
	A_long		target;	//scanlineのターゲットインデックス
	A_long		maxIndex;
}bufMinParam;

//-------------------------------------------------------------------------------------------------
void InToBuf8(CFsAE *ae, CFsBuffer *buf,A_long mode)
{
	A_long w = ae->out->width();
	A_long h = ae->out->height();
	A_long iw = ae->in->offsetWidth();
	A_long ow = ae->out->offsetWidth();
	A_long i,j;

	PF_Pixel *src;
	src = (PF_Pixel *)ae->in->data();
	A_long	*dst;
	dst = buf->bufA_long();
	A_long TargetSrc =0;
	A_long TargetDst =0;
	for ( j = 0; j< h; j++) {
		for ( i = 0; i< w; i++) {
			PF_Pixel p = src[TargetSrc];
			if( p.alpha ==0){
				dst[TargetDst] = 0;
			}else{
				A_long v =  FS_CONVERT8TO16(MAX(p.blue,MAX(p.green,p.red)));
				v = v * p.alpha/PF_MAX_CHAN8;
				if ( mode ==1){
					v = v - dst[TargetDst];
					if ( v<0) v=0;
				}
				dst[TargetDst] = v; 
			}
			TargetSrc++;
			TargetDst++;
		}
		TargetSrc +=iw;
		TargetDst +=ow;
	}
}
//-------------------------------------------------------------------------------------------------
void InToBuf16(CFsAE *ae, CFsBuffer *buf)
{
	A_long w = ae->out->width();
	A_long h = ae->out->height();
	A_long iw = ae->in->offsetWidth();
	A_long ow = ae->out->offsetWidth();
	A_long i,j;

	PF_Pixel16 *src;
	src = (PF_Pixel16 *)ae->in->data();
	A_long	*dst;
	dst = buf->bufA_long();
	A_long TargetSrc =0;
	A_long TargetDst =0;
	for ( j = 0; j< h; j++) {
		for ( i = 0; i< w; i++) {
			PF_Pixel16 p = src[TargetSrc];
			if( p.alpha ==0){
				dst[TargetDst] = 0;
			}else{
				A_long v = (MAX(p.blue,MAX(p.green,p.red)));
				dst[TargetDst] = v * p.alpha/PF_MAX_CHAN16; 
			}
			TargetSrc++;
			TargetDst++;
		}
		TargetSrc +=iw;
		TargetDst +=ow;
	}
}
//-------------------------------------------------------------------------------------------------
void InToBuf32(CFsAE *ae, CFsBuffer *buf)
{
	A_long w = ae->out->width();
	A_long h = ae->out->height();
	A_long iw = ae->in->offsetWidth();
	A_long ow = ae->out->offsetWidth();
	A_long i,j;

	PF_PixelFloat *src;
	src = (PF_PixelFloat *)ae->in->data();
	A_long	*dst;
	dst = buf->bufA_long();
	A_long TargetSrc =0;
	A_long TargetDst =0;
	for ( j = 0; j< h; j++) {
		for ( i = 0; i< w; i++) {
			PF_PixelFloat p = src[TargetSrc];
			if( p.alpha ==0){
				dst[TargetDst] = 0;
			}else{
				PF_FpShort v = (MAX(p.blue,MAX(p.green,p.red)));
				if ( v>1.0) v=1.0;
				dst[TargetDst] = (A_long)(v * PF_MAX_CHAN16); 
			}
			TargetSrc++;
			TargetDst++;
		}
		TargetSrc +=iw;
		TargetDst +=ow;
	}
}
//-------------------------------------------------------------------------------------------------
void BufToOut8(CFsAE *ae, CFsBuffer *buf)
{
	A_long w = ae->out->width();
	A_long h = ae->out->height();
	A_long ow = ae->out->offsetWidth();
	A_long i,j;

	A_long	*src;
	src = buf->bufA_long();
	PF_Pixel *dst;
	dst = (PF_Pixel *)ae->out->data();
	A_long TargetSrc =0;
	A_long TargetDst =0;
	for ( j = 0; j< h; j++) {
		for ( i = 0; i< w; i++) {
			dst[TargetDst].alpha = PF_MAX_CHAN8;
			dst[TargetDst].red
			= dst[TargetDst].green
			= dst[TargetDst].blue = RoundByteLong(FS_CONVERT16TO8(src[TargetSrc]));

			TargetSrc++;
			TargetDst++;
		}
		TargetSrc+=ow;
		TargetDst+=ow;
	}
}
//----------------------------------------------------------------------------
static A_long getMin(bufMinParam *mm)
{
	A_long start = mm->target - mm->value;
	if ( start<0) start = 0;
	A_long end = mm->target + mm->value;
	if ( end >= mm->scanlineLength) end = mm->scanlineLength-1;

	if ( (mm->maxIndex>=start)&&(mm->maxIndex<=end))
	{
		if (mm->scanline[end] <=mm->scanline[mm->maxIndex]){
			mm->maxIndex = end;
			return mm->scanline[end];
		}else{
			return mm->scanline[mm->maxIndex];
		}
	}
	A_long idx = start;
	for ( A_long i=start; i<=end; i++)
	{
		if ( mm->scanline[i] <=0){
			idx = i;
			break;
		}else{
			if ( mm->scanline[i] < mm->scanline[idx]) idx = i;
		}
	}
	mm->maxIndex =idx;
	return mm->scanline[idx];
} 
//-------------------------------------------------------------------------------------------------
void MinBuf(CFsAE *ae, CFsBuffer *buf)
{
	PF_Err err = PF_Err_NONE;
	bufMinParam bm;
	bm.value = 10;
	A_long w = ae->out->width();
	if ( ae->out->width()<ae->out->height()) w = ae->out->height();
	CFsBuffer scanline = ae->NewBuffer(w*sizeof(A_long));
	err = scanline.err();

	if (!err){
		A_long *data;
		data = buf->bufA_long();
		bm.scanline	= scanline.bufA_long();
		A_long x=0, y=0;

		bm.scanlineLength = ae->out->width();
		//水平方向
		A_long hor =0; 
		for ( y=0; y<ae->out->height(); y++)
		{
			hor = ae->out->width() * y;
			//スキャンライン分転送
			for ( x=0; x<ae->out->width(); x++){
				bm.scanline[x]	= data[hor + x];
			}
			bm.maxIndex = -1;
			for ( x=0; x<ae->out->width(); x++){
				bm.target = x;
				data[hor + x] = getMin(&bm);
			}
		}
	
		bm.scanlineLength =  ae->out->height();
		for ( x=0; x<ae->out->width(); x++)
		{
			hor = x;
			for ( y=0; y<ae->out->height(); y++){
				bm.scanline[y] = data[hor];
				hor += ae->out->widthTrue();
			}
			hor = x;
			bm.maxIndex = -1;
			for ( y=0; y<ae->out->height(); y++){
				bm.target = y;
				data[hor] = getMin(&bm);
				hor += ae->out->widthTrue();
			}
		}
		scanline.Dispose();
	}
}
//-------------------------------------------------------------------------------------------------
void BufArea8(CFsAE *ae, CFsBuffer *buf)
{
	A_long w = ae->out->width();
	A_long h = ae->out->height();
	A_long wt = ae->out->widthTrue();
	A_long j;

	A_long	*dst;
	dst = buf->bufA_long();
	//------------
	for ( j = 0; j< h; j++) {
		A_long Target = j*wt;
		A_long org = dst[Target];
		A_long idx =0;
		A_long bk=0;
		while(idx<w){
			A_long now = dst[Target+idx];
			if ((org !=now)||(idx == w-1)){
				if ( (idx-bk)>20){
					for ( A_long k= bk+9; k<=idx-9;k++) dst[Target+k]= 0;
				}
				bk = idx;
				org = now;
			}
			idx++;
		}
	}
	/*
	A_long ww = wt * h;
	//------------
	for ( j = 0; j< w; j++) {
		A_long Target = j;
		A_long org = dst[Target];
		A_long idx =0;
		A_long bk=0;
		while(idx<h){
			A_long now = dst[Target];
			if ((org !=now)||(idx == h-1)){
				if ( (idx-bk)>20){
					A_long Target2 = Target + wt*9;
					for ( A_long k= bk+9; k<=idx-9;k++) {
						if ( Target2<ww){
							dst[Target2] = 0;
						}
						Target2 += wt;
					}
				}
				bk = idx;
				org = now;
			}
			Target += wt;
			idx++;
		}
	}
	*/
}
//-------------------------------------------------------------------------------------------------
