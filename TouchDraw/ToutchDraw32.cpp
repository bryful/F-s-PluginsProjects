#include "TouchDraw.h"

//-------------------------------------------------------
inline long lineLen(PF_Fixed a,PF_Fixed b)
{
	double tx,ty,tl;
	tx = FIX_2_FLOAT(a);
	ty = FIX_2_FLOAT(b);
	tl  = sqrt(tx*tx + ty*ty);
	return (long)tl;
}
//-------------------------------------------------------
void drawTouchSub32(TouchDrawInfo *p)
{
	if (p->opacity<=0) return;

	PF_Fixed rndi = p->length_i;
	if(rndi<=0) { rndi=0;}
	else if (rndi>(1L<<16)) {rndi = (1L<<16);} 
	
	PF_Fixed rndo = p->length_o;
	if(rndo<=0) { rndo=0;}
	if ( (rndi<=0)&&(rndo<=0) ) return;

	PF_Fixed x,y,dx,dy,dl,ix,iy;
	
	long len;
	long xF =1;
	long yF =1;
	x =  (p->center.x - p->start.x);
	if (x<0) {
		xF = -1;
		x = ABS(x);
	}
	y =  (p->center.y - p->start.y);
	if (y<0) {
		yF = -1;
		y = ABS(y);
	}
	//if (rndi>0) {
		ix = (x>>16) * rndi;
		iy = (y>>16) * rndi;
		
		p->pp.pos	= p->start;
		p->pp.opacity = p->opacity;
		len = lineLen(ix,iy);
		if (len>0) {
			dx = ix / len; dx *=xF;
			dy = iy / len; dy *=yF;
			dl = p->pp.opacity /len;

			long i;
			for ( i=0; i<len;i++){
				if (PsetAlpha32(&(p->pp))==FALSE) break;
				//PsetAlphaB(&(p->pp));
				p->pp.pos.x += dx;
				p->pp.pos.y += dy;
				p->pp.opacity -= dl;
			}
		}
	//}
	//if (rndo>0) {
		ix = (x>>16) * rndo;
		iy = (y>>16) * rndo;
		
		p->pp.pos	= p->start;
		p->pp.opacity = p->opacity;
		len = lineLen(ix,iy);
		if (len>0) {
			dx = ix / len; dx *= xF;
			dy = iy / len; dy *= yF;
			dl = p->pp.opacity /len;

			long i;
			for ( i=0; i<len;i++){
				if (PsetAlpha32(&(p->pp))==FALSE) break;
				//PsetAlphaB(&(p->pp));
				p->pp.pos.x -= dx;
				p->pp.pos.y -= dy;
				p->pp.opacity -= dl;
			}
		}
	//}
}
//-------------------------------------------------------
void drawTouch32(CFsGraph *g,ParamInfo	*prm)
{
	
	A_long w = g->width();
	A_long h = g->height();
	A_long ow = g->offsetWidth();
	
	TouchDrawInfo td;
	td.pp.data			= g->data();
	td.pp.height		= g->height();
	td.pp.width			= g->width();
	td.pp.widthTrue		= g->widthTrue();
	td.center			= prm->center;
	
	PF_PixelFloat *oD;
	oD = (PF_PixelFloat *)g->data();
	
	//ターゲットランダム
	F_SRAND(prm->seed);
	A_long t0;
	t0 = (A_long)(F_RAND_MAX * prm->target_value);
	//長さランダム
	PF_Fixed ir0,ir1;
	ir1 = (prm->length_i_max  >>8) * (prm->length_i_rnd >>8);
	ir0 = prm->length_i_max - ir1;
	if (ir0<0) ir0=0;

	PF_Fixed or0,or1;
	or1 = (prm->length_o_max  >>8) * (prm->length_o_rnd >>8);
	or0 = prm->length_o_max - or1;
	if (or0<0) or0=0;

	//不透明度ランダム
	PF_Fixed p0,p1;
	p1 = (prm->opacity >>8) * (prm->opacity_rnd >>8);
	p0 = prm->opacity -p1;
	if (p0<0) p0=0;
	
	PF_Fixed xx,yy;
	long ln0 = prm->point_length * -1;
	long ln1 = prm->point_length * 2;
	long i,j;
	long k;
	long targetOut = 0;
	for (j=0;j<h;j++){
		yy = j<<16;
		for (i=0;i<w;i++){
			xx = i<<16;
			if (oD[targetOut].blue == 1){
				if ((A_long)F_RAND()<t0){
					for (k=0;k<=prm->point_count;k++){
						td.start.x = xx + ln0 + (long)(F_RAND() *(ln1 +1.0f)/(F_RAND_MAX+1.0f));
						td.start.y = yy + ln0 + (long)(F_RAND() *(ln1 +1.0f)/(F_RAND_MAX+1.0f));
						
						td.length_i = ir0 + (long)(F_RAND() *(ir1 +1.0f) /(1.0f +F_RAND_MAX));
						td.length_o = or0 + (long)(F_RAND() *(or1 +1.0f) /(1.0f +F_RAND_MAX));
						td.opacity	= p0 + (long)(F_RAND() *(p1 +1.0f) /(1.0f +F_RAND_MAX));;

						drawTouchSub32(&td);
					}

					//oD[targetOut].green = PF_MAX_CHAN8;
				}
			}else{
				F_RAND();
			}
			targetOut++;
		}
		targetOut += ow;
	}

}