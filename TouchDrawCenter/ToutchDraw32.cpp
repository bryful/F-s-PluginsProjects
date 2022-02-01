#include "TouchDrawCenter.h"

//-------------------------------------------------------
inline PF_FpShort mixCh32(PF_FpLong c0, PF_FpLong c1)
{
	PF_FpLong v = c0 + c1 - (c0 * c1);
	if (v<0) { v=0; }else if(v>1){v = 1;}
	return (PF_FpShort)v;
}

//-------------------------------------------------------
/*
	アルファーへ書き込み
*/
inline void PsetAlpha32(CFsGraph *g, PsetAlphaPrm *prm)
{
	PF_FpShort opacity = prm->opacity;
	if (opacity<=0) return;
	if (opacity>1) opacity = 1;
	
	PF_PixelFloat *data;
	data = (PF_PixelFloat *)g->data();
	A_long w	= g->width();
	A_long wt	= g->widthTrue();
	A_long h	= g->height();
	A_long x1,y1,x2,y2;

	PF_FpLong xx2,yy2,xx1,yy1;
	
	x1 = (A_long)prm->x;
	y1 = (A_long)prm->y;
	x2 = x1+1;
	y2 = y1+1;
	if ( (x1>=w)||(y1>=h)||(x2<0)||(y2<0) ) return;

	

	xx2 = prm->x - (PF_FpShort)x1;
	yy2 = prm->y - (PF_FpShort)y1;
	xx1 = 1 - xx2;
	yy1 = 1 - yy2;

	PF_FpLong v = opacity;
	A_long target = x1 + (y1 * wt);

	//まず、ぴったりの時
	if ( (xx2 == 0)&&(yy2 == 0) ) {
		data[target].alpha = mixCh32(data[target].alpha,v);
		data[target].red = data[target].alpha;
	}else{
		//まず左上
		if ( ((x1>=0)&&(y1>=0)) == TRUE){
			data[target].alpha = mixCh32(data[target].alpha,(v * xx1 * yy1));
			data[target].red = data[target].alpha;
		}
		//右上
		target++;
		if ( ((x2<w)&&(y1>=0)) == TRUE){
			data[target].alpha = mixCh32(data[target].alpha,(v * xx2 *yy1));
			data[target].red = data[target].alpha;
		}
		//右下
		target+= wt;
		if ( ((x2<w)&&(y2<h)) == TRUE){
			data[target].alpha = mixCh32(data[target].alpha,(v * xx2 * yy2));
			data[target].red = data[target].alpha;
		}
		//左下
		target--;
		if ( ((x1>=0)&&(y2<h)) == TRUE){
			data[target].alpha = mixCh32(data[target].alpha,(v * xx1 * yy2));
			data[target].red = data[target].alpha;
		}
	}
	
}
//-------------------------------------------------------
static void drawTouchSub32(CFsGraph *g, TouchDrawCenterInfo *p)
{
	if (p->opacity <=0) return;

	PsetAlphaPrm pprm; 
	PF_FpShort rnd = p->length;
	if(rnd<=0) {return;}
	else if (rnd>1) {rnd = 1;} 

	PF_FpShort x,y,dx,dy,dl;
	A_long len;
	PF_FpShort xF =1;
	PF_FpShort yF =1;
	x =  (p->centerX - p->startX);
	if (x<0) {
		xF = -1;
		x *= -1;
	}
	y =  (p->centerY - p->startY);
	if (y<0) {
		yF = -1;
		y *= -1;
	}
	x *= rnd;
	y *= rnd;
	
	if ( (x==0)&&(y==0) ) return;

	pprm.x			= p->startX;
	pprm.y			= p->startY;
	pprm.opacity	= p->opacity;
	A_long i;
	
	if (x>=y){
		len = (A_long)x;
	}else{
		len = (A_long)y;
	}
	dx = x / (PF_FpShort)len; dx *=xF;
	dy = y / (PF_FpShort)len; dy *=yF;
	dl = pprm.opacity /(PF_FpShort)len;

	for ( i=0; i<len;i++){
		PsetAlpha32(g,&pprm);
		pprm.x += dx;
		pprm.y += dy;
		pprm.opacity -= dl;
	}



}
//-------------------------------------------------------
void drawTouch32(CFsGraph *g, ParamInfo *info)
{
	F_SRAND(info->seed);
	A_long w = g->width();
	A_long h = g->height();
	
	
	TouchDrawCenterInfo ti;	
	ti.centerX = info->centeX;
	ti.centerY = info->centeY;	

	//不透明度ランダム
	PF_FpShort p0,p1;
	p0 = info->opacity * (1 - info->opacity_rnd);
	p1 = info->opacity - p0;

	//長さランダム
	PF_FpShort r0,r1;
	r0 = info->length_max * (1 - info->length_rnd);
	r1 = info->length_max - r0;
	
	
	//横ランダム
	PF_FpShort w0 = (PF_FpShort)w / -5;
	PF_FpShort w1 = (PF_FpShort)w + ((PF_FpShort)w/5)-w0;
	//縦ランダム
	PF_FpShort h0 = (PF_FpShort)h / -5;
	PF_FpShort h1 = (PF_FpShort)h + ((PF_FpShort)h/5)-h0;
	
	//
	PF_FpShort lr0 = -info->point_length;
	PF_FpShort lr1 = info->point_length * 2;

	A_long i,j;
	PF_FpShort xx,yy;
	//先ずtop
	if (info->value_top>0){
		ti.startY = (PF_FpShort)((PF_FpShort)h/-20);
		for(i=0;i<info->value_top;i++){
			xx = (PF_FpShort)(w0 + F_RAND_D()*w1);
			for (j=0; j<=info->point_count;j++)
			{
				ti.opacity = (PF_FpShort)(p0 + F_RAND_D()*p1);
				ti.length  = (PF_FpShort)(r0 + F_RAND_D()*r1);
				ti.startX  = (PF_FpShort)(xx + lr0 + F_RAND_D()*lr1);
				drawTouchSub32(g,&ti);
			}
		}
	}
	if (info->value_bottom>0){
		ti.startY = (PF_FpShort)((PF_FpShort)h-1 + (PF_FpShort)h/20);
		for(i=0;i<info->value_bottom;i++){
			xx = (PF_FpShort)(w0 + F_RAND_D()*w1);
			for (j=0; j<=info->point_count;j++)
			{
				ti.opacity = (PF_FpShort)(p0 + F_RAND_D()*p1);
				ti.length  = (PF_FpShort)(r0 + F_RAND_D()*r1);
				ti.startX  = (PF_FpShort)(xx + lr0 + F_RAND_D()*lr1);
				drawTouchSub32(g,&ti);
			}
		}
	}
	if (info->value_left>0){
		ti.startX = (PF_FpShort)((PF_FpShort)w/-20);
		for(i=0;i<info->value_left;i++){
			yy = (PF_FpShort)(h0 + F_RAND_D()*h1);
			for (j=0; j<=info->point_count;j++)
			{
				ti.opacity = (PF_FpShort)(p0 + F_RAND_D()*p1);
				ti.length = (PF_FpShort)(r0 + F_RAND_D()*r1);
				ti.startY = (PF_FpShort)(yy + lr0 + F_RAND_D()*lr0);
				drawTouchSub32(g,&ti);
			}
		}
	}

	if (info->value_right>0){
		ti.startX = (PF_FpShort)((PF_FpShort)w -1 + (PF_FpShort)w/20);
		for(i=0;i< info->value_right;i++){
			yy = (PF_FpShort)(h0 + F_RAND_D()*h1);
			for (j=0; j<=info->point_count;j++)
			{
				ti.opacity	= (PF_FpShort)(p0 + F_RAND_D()*p1);
				ti.length	= (PF_FpShort)(r0 + F_RAND_D()*r1);
				ti.startY	= (PF_FpShort)(yy + lr0 + F_RAND_D()*lr0);
				drawTouchSub32(g,&ti);
			}
		}
	}
}
//-------------------------------------------------------------------------------
