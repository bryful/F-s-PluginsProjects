#include "TouchDrawStraght.h"



//-------------------------------------------------------------------------------
void greenBlur8(CFsGraph *g)
{
	PF_Pixel *data;
	data = g->data();
	long wt = g->widthTrue();
	long ow = g->offsetWidth();
	long w = g->width();
	long h = g->height();
	
	long x,y;
	long v;
	w = g->width()-1;
	h = g->height()-1;

	//まず、真ん中
	A_long adr=wt+1;
	A_long adr2;
	for(y=1;y<h;y++){
		for(x=1;x<w;x++){
			adr2 = adr-wt-1;
			v = 0;
			v += data[adr2].green *2;
			adr2++;
			v += data[adr2].green *4;
			adr2++;
			v += data[adr2].green *2;
			adr2+=(wt-2);
			v += data[adr2].green *4;
			adr2++;
			v += data[adr2].green *8;
			adr2++;
			v += data[adr2].green *4;
			adr2+=(wt-2);
			v += data[adr2].green *2;
			adr2++;
			v += data[adr2].green *4;
			adr2++;
			v += data[adr2].green *2;
			v = v>> 5;
			data[adr].alpha = RoundByteLong(v);

			adr++;
		}
		adr+=(ow+2);
	}
	//上下
	adr = 1;
	long adrH = h*wt+1;
	for(x=1;x<w;x++){
		v=0;
		adr2 = adr-1;
		v+=data[adr2].green*4;
		adr2++;
		v+=data[adr2].green*8;
		adr2++;
		v+=data[adr2].green*4;
		adr2+=(wt-2);
		v+=data[adr2].green*2;
		adr2++;
		v+=data[adr2].green*4;
		adr2++;
		v+=data[adr2].green*2;
		v = v/24;
		data[adr].alpha = RoundByteLong(v);

		v=0;
		adr2 = adrH-1;
		v+=data[adr2].green*4;
		adr2++;
		v+=data[adr2].green*8;
		adr2++;
		v+=data[adr2].green*4;
		adr2-=(wt+2);
		v+=data[adr2].green*2;
		adr2++;
		v+=data[adr2].green*4;
		adr2++;
		v+=data[adr2].green*2;
		v = v/24;
		data[adrH].alpha = RoundByteLong(v);

		adr++;
		adrH++;
	}
	adr = wt;
	adrH = wt + w;
	for(y=1;y<h;y++){
		v=0;
		adr2 = adr-wt;
		v+=data[adr2].green*4;
		adr2++;
		v+=data[adr2].green*2;
		adr2+=(wt-1);
		v+=data[adr2].green*8;
		adr2++;
		v+=data[adr2].green*4;
		adr2+=(wt-1);
		v+=data[adr2].green*4;
		adr2++;
		v+=data[adr2].green*2;
		v = v/24;
		data[adr].alpha = RoundByteLong(v);

		v=0;
		adr2 = adrH-wt-1;
		v+=data[adr2].green*2;
		adr2++;
		v+=data[adr2].green*4;
		adr2+=(wt-1);
		v+=data[adr2].green*4;
		adr2++;
		v+=data[adr2].green*8;
		adr2+=(wt-2);
		v+=data[adr2].green*2;
		adr2++;
		v+=data[adr2].green*4;
		v = v/24;
		data[adrH].alpha = RoundByteLong(v);

		adr+=wt;
		adrH+=wt;
	}
	//左上
	v=0;
	adr=0;
	v += data[adr].green *8;
	adr++;
	v += data[adr].green *4;
	adr+=(wt-1);
	v += data[adr].green *4;
	adr++;
	v += data[adr].green *2;
	v = v / 18;
	data[0].alpha = RoundByteLong(v);
	//右上
	v=0;
	adr = w-2;
	v += data[adr].green *4;
	adr++;
	v += data[adr].green *8;
	adr+=(wt-1);
	v += data[adr].green *2;
	adr++;
	v += data[adr].green *4;
	v = v / 18;
	data[w-2].alpha = RoundByteLong(v);
	//左下
	v=0;
	adr=(h-2)*wt;
	v += data[adr].green *4;
	adr++;
	v += data[adr].green *2;
	adr+=(wt-1);
	v += data[adr].green *8;
	adr++;
	v += data[adr].green *4;
	v = v / 18;
	data[(h-1)*wt].alpha = RoundByteLong(v);
	//右下
	v=0;
	adr=(h-2)*wt + (w-1);
	v += data[adr].green *2;
	adr++;
	v += data[adr].green *4;
	adr+=(wt-1);
	v += data[adr].green *4;
	adr++;
	v += data[adr].green *8;
	v = v / 18;
	data[(h-1)*wt+(w-1)].alpha = RoundByteLong(v);

}
//-------------------------------------------------------------------------------
void greenBlur16(CFsGraph *g)
{
	PF_Pixel16 *data;
	data = (PF_Pixel16 *)g->data();
	long wt = g->widthTrue();
	long ow = g->offsetWidth();
	long w = g->width();
	long h = g->height();
	
	long x,y;
	long v;
	w = g->width()-1;
	h = g->height()-1;

	//まず、真ん中
	long adr=wt+1;
	long adr2;
	
	for(y=1;y<h;y++){
		for(x=1;x<w;x++){
			adr2 = adr-wt-1;
			v = 0;
			v += data[adr2].green *2;
			adr2++;
			v += data[adr2].green *4;
			adr2++;
			v += data[adr2].green *2;
			adr2+=(wt-2);
			v += data[adr2].green *4;
			adr2++;
			v += data[adr2].green *8;
			adr2++;
			v += data[adr2].green *4;
			adr2+=(wt-2);
			v += data[adr2].green *2;
			adr2++;
			v += data[adr2].green *4;
			adr2++;
			v += data[adr2].green *2;
			v = v >> 5;
			data[adr].alpha = RoundShort(v);

			adr++;
		}
		adr+=(ow+2);
	}
	
	//上下
	adr = 1;
	long adrH = h*wt+1;
	for(x=1;x<w;x++){
		v=0;
		adr2 = adr-1;
		v+=data[adr2].green*4;
		adr2++;
		v+=data[adr2].green*8;
		adr2++;
		v+=data[adr2].green*4;
		adr2+=(wt-2);
		v+=data[adr2].green*2;
		adr2++;
		v+=data[adr2].green*4;
		adr2++;
		v+=data[adr2].green*2;
		v = v/24;
		data[adr].alpha = RoundShort(v);

		v=0;
		adr2 = adrH-1;
		v+=data[adr2].green*4;
		adr2++;
		v+=data[adr2].green*8;
		adr2++;
		v+=data[adr2].green*4;
		adr2-=(wt+2);
		v+=data[adr2].green*2;
		adr2++;
		v+=data[adr2].green*4;
		adr2++;
		v+=data[adr2].green*2;
		v = v/24;
		data[adrH].alpha = RoundShort(v);

		adr++;
		adrH++;
	}
	adr = wt;
	adrH = wt + w;
	for(y=1;y<h;y++){
		v=0;
		adr2 = adr-wt;
		v+=data[adr2].green*4;
		adr2++;
		v+=data[adr2].green*2;
		adr2+=(wt-1);
		v+=data[adr2].green*8;
		adr2++;
		v+=data[adr2].green*4;
		adr2+=(wt-1);
		v+=data[adr2].green*4;
		adr2++;
		v+=data[adr2].green*2;
		v = v/24;
		data[adr].alpha = RoundShort(v);

		v=0;
		adr2 = adrH-wt-1;
		v+=data[adr2].green*2;
		adr2++;
		v+=data[adr2].green*4;
		adr2+=(wt-1);
		v+=data[adr2].green*4;
		adr2++;
		v+=data[adr2].green*8;
		adr2+=(wt-2);
		v+=data[adr2].green*2;
		adr2++;
		v+=data[adr2].green*4;
		v = v/24;
		data[adrH].alpha = RoundShort(v);

		adr+=wt;
		adrH+=wt;
	}
	//左上
	v=0;
	adr=0;
	v += data[adr].green *8;
	adr++;
	v += data[adr].green *4;
	adr+=(wt-1);
	v += data[adr].green *4;
	adr++;
	v += data[adr].green *2;
	v = v / 18;
	data[0].alpha = RoundShort(v);
	//右上
	v=0;
	adr = w-2;
	v += data[adr].green *4;
	adr++;
	v += data[adr].green *8;
	adr+=(wt-1);
	v += data[adr].green *2;
	adr++;
	v += data[adr].green *4;
	v = v / 18;
	data[w-2].alpha = RoundShort(v);
	//左下
	v=0;
	adr=(h-2)*wt;
	v += data[adr].green *4;
	adr++;
	v += data[adr].green *2;
	adr+=(wt-1);
	v += data[adr].green *8;
	adr++;
	v += data[adr].green *4;
	v = v / 18;
	data[(h-1)*wt].alpha = RoundShort(v);
	//右下
	v=0;
	adr=(h-2)*wt + (w-1);
	v += data[adr].green *2;
	adr++;
	v += data[adr].green *4;
	adr+=(wt-1);
	v += data[adr].green *4;
	adr++;
	v += data[adr].green *8;
	v = v / 18;
	data[(h-1)*wt+(w-1)].alpha = RoundShort(v);

}
//-------------------------------------------------------------------------------
void greenBlur32(CFsGraph *g)
{
	PF_PixelFloat *data;
	data = (PF_PixelFloat *)g->data();
	long wt = g->widthTrue();
	long ow = g->offsetWidth();
	long w = g->width();
	long h = g->height();
	
	long x,y;
	PF_FpLong v;
	w = g->width()-1;
	h = g->height()-1;

	//まず、真ん中
	long adr=wt+1;
	long adr2;
	
	for(y=1;y<h;y++){
		for(x=1;x<w;x++){
			adr2 = adr-wt-1;
			v = 0;
			v += data[adr2].green *2;
			adr2++;
			v += data[adr2].green *4;
			adr2++;
			v += data[adr2].green *2;
			adr2+=(wt-2);
			v += data[adr2].green *4;
			adr2++;
			v += data[adr2].green *8;
			adr2++;
			v += data[adr2].green *4;
			adr2+=(wt-2);
			v += data[adr2].green *2;
			adr2++;
			v += data[adr2].green *4;
			adr2++;
			v += data[adr2].green *2;
			v = v /32;
			data[adr].alpha = RoundFpShortDouble(v);

			adr++;
		}
		adr+=(ow+2);
	}
	
	//上下
	adr = 1;
	long adrH = h*wt+1;
	for(x=1;x<w;x++){
		v=0;
		adr2 = adr-1;
		v+=data[adr2].green*4;
		adr2++;
		v+=data[adr2].green*8;
		adr2++;
		v+=data[adr2].green*4;
		adr2+=(wt-2);
		v+=data[adr2].green*2;
		adr2++;
		v+=data[adr2].green*4;
		adr2++;
		v+=data[adr2].green*2;
		v = v/24;
		data[adr].alpha = RoundFpShortDouble(v);

		v=0;
		adr2 = adrH-1;
		v+=data[adr2].green*4;
		adr2++;
		v+=data[adr2].green*8;
		adr2++;
		v+=data[adr2].green*4;
		adr2-=(wt+2);
		v+=data[adr2].green*2;
		adr2++;
		v+=data[adr2].green*4;
		adr2++;
		v+=data[adr2].green*2;
		v = v/24;
		data[adrH].alpha = RoundFpShortDouble(v);

		adr++;
		adrH++;
	}
	adr = wt;
	adrH = wt + w;
	for(y=1;y<h;y++){
		v=0;
		adr2 = adr-wt;
		v+=data[adr2].green*4;
		adr2++;
		v+=data[adr2].green*2;
		adr2+=(wt-1);
		v+=data[adr2].green*8;
		adr2++;
		v+=data[adr2].green*4;
		adr2+=(wt-1);
		v+=data[adr2].green*4;
		adr2++;
		v+=data[adr2].green*2;
		v = v/24;
		data[adr].alpha = RoundFpShortDouble(v);

		v=0;
		adr2 = adrH-wt-1;
		v+=data[adr2].green*2;
		adr2++;
		v+=data[adr2].green*4;
		adr2+=(wt-1);
		v+=data[adr2].green*4;
		adr2++;
		v+=data[adr2].green*8;
		adr2+=(wt-2);
		v+=data[adr2].green*2;
		adr2++;
		v+=data[adr2].green*4;
		v = v/24;
		data[adrH].alpha = RoundFpShortDouble(v);

		adr+=wt;
		adrH+=wt;
	}
	//左上
	v=0;
	adr=0;
	v += data[adr].green *8;
	adr++;
	v += data[adr].green *4;
	adr+=(wt-1);
	v += data[adr].green *4;
	adr++;
	v += data[adr].green *2;
	v = v / 18;
	data[0].alpha = RoundFpShortDouble(v);
	//右上
	v=0;
	adr = w-2;
	v += data[adr].green *4;
	adr++;
	v += data[adr].green *8;
	adr+=(wt-1);
	v += data[adr].green *2;
	adr++;
	v += data[adr].green *4;
	v = v / 18;
	data[w-2].alpha = RoundFpShortDouble(v);
	//左下
	v=0;
	adr=(h-2)*wt;
	v += data[adr].green *4;
	adr++;
	v += data[adr].green *2;
	adr+=(wt-1);
	v += data[adr].green *8;
	adr++;
	v += data[adr].green *4;
	v = v / 18;
	data[(h-1)*wt].alpha = RoundFpShortDouble(v);
	//右下
	v=0;
	adr=(h-2)*wt + (w-1);
	v += data[adr].green *2;
	adr++;
	v += data[adr].green *4;
	adr+=(wt-1);
	v += data[adr].green *4;
	adr++;
	v += data[adr].green *8;
	v = v / 18;
	data[(h-1)*wt+(w-1)].alpha = RoundFpShortDouble(v);

}
//-------------------------------------------------------------------------------------------------
PF_Err greenBlur(CFsAE *ae)
{
	PF_Err	err = PF_Err_NONE;
	
	switch(ae->pixelFormat())
	{
	case PF_PixelFormat_ARGB128:
		greenBlur32(ae->out);
		break;
	case PF_PixelFormat_ARGB64:
		greenBlur16(ae->out);
		break;
	case PF_PixelFormat_ARGB32:
		greenBlur8(ae->out);
		break;
	default:
		err = PF_Err_BAD_CALLBACK_PARAM;
		break;
	}
	return err;
}
