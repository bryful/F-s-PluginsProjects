#include "NamiGarasu.h"


void AlphaBlur32(CFsGraph *g)
{
	PF_PixelFloat *data;
	data = (PF_PixelFloat *)g->data();
	A_long wt = g->widthTrue();
	A_long ow = g->offsetWidth();

	A_long w = g->width()-1;
	A_long h = g->height()-1;
	
	A_long x,y;
	PF_FpLong v;

	//まず、真ん中
	A_long adr=wt+1;
	A_long adr2;
	for(y=1;y<h;y++){
		for(x=1;x<w;x++){
			adr2 = adr-wt-1;
			v = 0;
			v += data[adr2].red *2;
			adr2++;
			v += data[adr2].red *4;
			adr2++;
			v += data[adr2].red *2;
			adr2+=(wt-2);
			v += data[adr2].red *4;
			adr2++;
			v += data[adr2].red *8;
			adr2++;
			v += data[adr2].red *4;
			adr2+=(wt-2);
			v += data[adr2].red *2;
			adr2++;
			v += data[adr2].red *4;
			adr2++;
			v += data[adr2].red *2;
			v = v/32;
			data[adr].alpha = RoundFpShortDouble(v);

			adr++;
		}
		adr+=(ow+2);
	}
	//上下
	adr = 1;
	A_long adrH = h*wt+1;
	for(x=1;x<w;x++){
		v=0;
		adr2 = adr-1;
		v+=data[adr2].red*4;
		adr2++;
		v+=data[adr2].red*8;
		adr2++;
		v+=data[adr2].red*4;
		adr2+=(wt-2);
		v+=data[adr2].red*2;
		adr2++;
		v+=data[adr2].red*4;
		adr2++;
		v+=data[adr2].red*2;
		v = v/24;
		data[adr].alpha = RoundFpShortDouble(v);

		v=0;
		adr2 = adrH-1;
		v+=data[adr2].red*4;
		adr2++;
		v+=data[adr2].red*8;
		adr2++;
		v+=data[adr2].red*4;
		adr2-=(wt+2);
		v+=data[adr2].red*2;
		adr2++;
		v+=data[adr2].red*4;
		adr2++;
		v+=data[adr2].red*2;
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
		v+=data[adr2].red*4;
		adr2++;
		v+=data[adr2].red*2;
		adr2+=(wt-1);
		v+=data[adr2].red*8;
		adr2++;
		v+=data[adr2].red*4;
		adr2+=(wt-1);
		v+=data[adr2].red*4;
		adr2++;
		v+=data[adr2].red*2;
		v = v/24;
		data[adr].alpha = RoundFpShortDouble(v);

		v=0;
		adr2 = adrH-wt-1;
		v+=data[adr2].red*2;
		adr2++;
		v+=data[adr2].red*4;
		adr2+=(wt-1);
		v+=data[adr2].red*4;
		adr2++;
		v+=data[adr2].red*8;
		adr2+=(wt-2);
		v+=data[adr2].red*2;
		adr2++;
		v+=data[adr2].red*4;
		v = v/24;
		data[adrH].alpha = RoundFpShortDouble(v);

		adr+=wt;
		adrH+=wt;
	}
	//左上
	v=0;
	adr=0;
	v += data[adr].red *8;
	adr++;
	v += data[adr].red *4;
	adr+=(wt-1);
	v += data[adr].red *4;
	adr++;
	v += data[adr].red *2;
	v = v / 18;
	data[0].alpha = RoundFpShortDouble(v);
	//右上
	v=0;
	adr = w-2;
	v += data[adr].red *4;
	adr++;
	v += data[adr].red *8;
	adr+=(wt-1);
	v += data[adr].red *2;
	adr++;
	v += data[adr].red *4;
	v = v / 18;
	data[w-2].alpha = RoundFpShortDouble(v);
	//左下
	v=0;
	adr=(h-2)*wt;
	v += data[adr].red *4;
	adr++;
	v += data[adr].red *2;
	adr+=(wt-1);
	v += data[adr].red *8;
	adr++;
	v += data[adr].red *4;
	v = v / 18;
	data[(h-1)*wt].alpha = RoundFpShortDouble(v);
	//右下
	v=0;
	adr=(h-2)*wt + (w-1);
	v += data[adr].red *2;
	adr++;
	v += data[adr].red *4;
	adr+=(wt-1);
	v += data[adr].red *4;
	adr++;
	v += data[adr].red *8;
	v = v / 18;
	data[(h-1)*wt+(w-1)].alpha = RoundFpShortDouble(v);

}
