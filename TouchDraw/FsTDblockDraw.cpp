#include "TouchDraw.h"

#define TARGET_CHNNEL	blue

//-------------------------------------------------------------------------------
void blockDraw8(CFsGraph *g,A_long bSize,A_long bCount)
{
	PF_Pixel *data;
	data			= (PF_Pixel *)g->data();
	A_long height	= g->height();
	A_long width	= g->width();
	A_long wt		= g->widthTrue();
	A_long cnt		= bCount;
	A_long bs0		= bSize /2;
	if (bs0<2) bs0 = 2;
	A_long i,j,k;

	A_long w,h,x,y;
	for (k=0; k<cnt; k++)
	{
		w = F_RAND2( bs0,bSize);
		h = F_RAND2( bs0,bSize);
		x = F_RAND3(width - w);
		y = F_RAND3(height -h);
		A_long x2 = x + w;
		A_long y2 = y + h;
		A_long target = x + (y * wt);
		for (j= y; j<y2;j++){
			for (i= x; i<x2;i++){
				data[target].TARGET_CHNNEL = 0;
				target++;
			}
			target +=(wt - w);
		}
	}
}
//-------------------------------------------------------------------------------
void blockDraw16(CFsGraph *g,A_long bSize,A_long bCount)
{
	PF_Pixel16 *data;
	data			= (PF_Pixel16 *)g->data();
	A_long height	= g->height();
	A_long width	= g->width();
	A_long wt		= g->widthTrue();
	A_long cnt		= bCount;
	A_long bs0		= bSize /2;
	if (bs0<2) bs0 = 2;
	A_long i,j,k;

	A_long w,h,x,y;
	for (k=0; k<cnt; k++)
	{
		w = F_RAND2( bs0,bSize);
		h = F_RAND2( bs0,bSize);
		x = F_RAND3(width - w);
		y = F_RAND3(height -h);
		A_long x2 = x + w;
		A_long y2 = y + h;
		A_long target = x + (y * wt);
		for (j= y; j<y2;j++){
			for (i= x; i<x2;i++){
				data[target].TARGET_CHNNEL = 0;
				target++;
			}
			target +=(wt - w);
		}
	}
}
//-------------------------------------------------------------------------------
void blockDraw32(CFsGraph *g,A_long bSize,A_long bCount)
{
	PF_PixelFloat *data;
	data			= (PF_PixelFloat *)g->data();
	A_long height	= g->height();
	A_long width	= g->width();
	A_long wt		= g->widthTrue();
	A_long cnt		= bCount;
	A_long bs0		= bSize /2;
	if (bs0<2) bs0 = 2;
	A_long i,j,k;

	A_long w,h,x,y;
	for (k=0; k<cnt; k++)
	{
		w = F_RAND2( bs0,bSize);
		h = F_RAND2( bs0,bSize);
		x = F_RAND3(width - w);
		y = F_RAND3(height -h);
		A_long x2 = x + w;
		A_long y2 = y + h;
		A_long target = x + (y * wt);
		for (j= y; j<y2;j++){
			for (i= x; i<x2;i++){
				data[target].TARGET_CHNNEL = 0;
				target++;
			}
			target +=(wt - w);
		}
	}
}
//-------------------------------------------------------------------------------------------------
PF_Err blockDraw(CFsAE *ae , A_long bSize, A_long bCount)
{
	PF_Err	err = PF_Err_NONE;

	switch(ae->pixelFormat())
	{
	case PF_PixelFormat_ARGB128:
		blockDraw32(ae->out,bSize,bCount);
		break;
	case PF_PixelFormat_ARGB64:
		blockDraw16(ae->out,bSize,bCount);
		break;
	case PF_PixelFormat_ARGB32:
		blockDraw8(ae->out,bSize,bCount);
		break;
	}
	return err;
}

