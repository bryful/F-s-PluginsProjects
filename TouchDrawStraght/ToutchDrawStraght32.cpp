#include "TouchDrawStraght.h"


//-------------------------------------------------------
void drawTouchSub32(TouchDrawStraghtInfo *p)
{
	if (p->opacity<=0) return;
	CRotCalc rc(p->in_data);
	rc.SetRot(p->rot);

	PF_Fixed dx,dy,dl;
	A_long i,len;
	len = p->length_i;
	if (len>0) {
		rc.SetRotLength(p->rot,len<<16);
		p->pp.pos	= p->start;
		p->pp.opacity = p->opacity;
		dx = rc.x() / len;
		dy = rc.y() / len;
		dl = p->pp.opacity / len;
		for ( i=0; i<len;i++){
			if (PsetAlpha32(&(p->pp))==FALSE) break;
			p->pp.pos.x += dx;
			p->pp.pos.y += dy;
			p->pp.opacity -= dl;
		}
	}
	len = p->length_o;
	if (len>0) {
		rc.SetRotLength(p->rot,len<<16);
		p->pp.pos	= p->start;
		p->pp.opacity = p->opacity;
		dx = rc.x() / -len;
		dy = rc.y() / -len;
		dl = p->pp.opacity / len;
		for ( i=0; i<len;i++){
			if (PsetAlpha32(&(p->pp))==FALSE) break;
			p->pp.pos.x += dx;
			p->pp.pos.y += dy;
			p->pp.opacity -= dl;
		}
	}
}
//-------------------------------------------------------
void drawTouchS32(CFsAE *ae,ParamInfo	*prm)
{
	
	A_long w = ae->out->width();
	A_long h = ae->out->height();
	A_long ow = ae->out->offsetWidth();
	
	TouchDrawStraghtInfo td;
	td.pp.data			= ae->out->data();
	td.pp.height		= h;
	td.pp.width			= w;
	td.pp.widthTrue		= ae->out->widthTrue();
	td.rot				= prm->rot;
	td.in_data			= ae->in_data;
	
	PF_PixelFloat *oD;
	oD = (PF_PixelFloat *)ae->out->data();
	

	F_SRAND(prm->seed);
	//ターゲットランダム
	PF_Fixed t0;
	t0 = F_RAND_MAX * prm->target_value >> 16;

	//長さランダム
	PF_Fixed ir0;
	ir0 = prm->length_i_max - (prm->length_i_max * prm->length_i_rnd >>16);
	if (ir0<0) ir0=0;

	PF_Fixed or0;
	or0 = prm->length_o_max - (prm->length_o_max * prm->length_o_rnd >>16);
	if (or0<0) or0=0;

	//不透明度ランダム
	PF_Fixed p0;
	p0 = prm->opacity - (prm->opacity >>8) * (prm->opacity_rnd >>8);
	if (p0<0) p0=0;
	
	PF_Fixed xx,yy;
	A_long i,j;
	A_long k;
	A_long targetOut = 0;
	for (j=0;j<h;j++){
		yy = j<<16;
		for (i=0;i<w;i++){
			xx = i<<16;
			if (oD[targetOut].blue == 1){
				if ((A_long)F_RAND()<t0){
					for (k=0;k<=prm->point_count;k++){
						td.start.x = xx + F_RAND2(-prm->point_length,prm->point_length);
						td.start.y = yy + F_RAND2(-prm->point_length,prm->point_length);
						
						td.length_i = F_RAND2(ir0,prm->length_i_max);
						td.length_o = F_RAND2(or0,prm->length_o_max);
						td.opacity	= F_RAND2(p0,prm->opacity);
						drawTouchSub32(&td);
					}
				}
			}else{
				F_RAND();
			}
			targetOut++;
		}
		targetOut += ow;
	}
}