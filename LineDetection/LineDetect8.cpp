#include "LineDetection.h"

//-------------------------------------------------------------------------------
//RGBの最大値とアルファーの値を適当に8Bitに割り振る
inline A_u_char rgbMax8(PF_Pixel c)
{
	A_u_char m0,m1;
	if ( c.alpha ==0 ) return 0;

	m0 =  MAX( MAX(c.red,c.green),c.blue);
	m1 =  MIN( MIN(c.red,c.green),c.blue);
	return (m0+m1) * (c.alpha+1) >> 9;

}
//-------------------------------------------------------------------------------
inline A_u_char pixDelta8(PointPrm *pp)
{
	PF_Pixel *oD;
	A_long wt = pp->widthTrue;
	//long w = pp->width;
	A_long w1 = pp->width -1;
	A_long h1 = pp->height -1;
	oD = pp->data;
	A_u_char pv = rgbMax8(oD[pp->adr]);
	A_u_char v1 = 0;

	A_long x = pp->x;
	A_long y = pp->y;
	PF_Pixel p;
	if ( (x>0)&&(y>0) ){
		p = oD[pp->adr -1 -wt];
		v1 = MAX(rgbMax8(p)-pv,v1);
	}
	if (y>0){
		p = oD[pp->adr    -wt];
		v1 = MAX(rgbMax8(p)-pv,v1);
	}
	if ( (x<w1)&&(y>0) ){
		p = oD[pp->adr +1 -wt];
		v1 = MAX(rgbMax8(p)-pv,v1);
	}
	if (x>0){
		p = oD[pp->adr -1    ];
		v1 = MAX(rgbMax8(p)-pv,v1);
	}
	if (x<w1){
		p = oD[pp->adr +1    ];
		v1 = MAX(rgbMax8(p)-pv,v1);
	}
	if ( (x>0)&&(y<h1) ){
		p = oD[pp->adr -1 +wt];
		v1 = MAX(rgbMax8(p)-pv,v1);
	}
	if ( (y<h1) ){
		p = oD[pp->adr    +wt];
		v1 = MAX(rgbMax8(p)-pv,v1);
	}
	if ( (x<w1)&&(y<h1) ){
		p = oD[pp->adr +1 +wt];
		v1 = MAX(rgbMax8(p)-pv,v1);
	}

	return v1;
}
//-------------------------------------------------------------------------------
inline A_u_char pixDeltaAlpha8(PointPrm *pp)
{
	PF_Pixel *oD;
	A_long wt = pp->widthTrue;
	//long w = pp->width;
	A_long w1 = pp->width -1;
	long h1 = pp->height -1;
	oD = pp->data;
	A_u_char pv = oD[pp->adr].alpha;
	A_u_char v1 = 0;

	A_long x = pp->x;
	A_long y = pp->y;
	PF_Pixel p;
	if ( (x>0)&&(y>0) ){
		p = oD[pp->adr -1 -wt];
		v1 = MAX(pv - p.alpha,v1);
	}
	if (y>0){
		p = oD[pp->adr    -wt];
		v1 = MAX(pv - p.alpha,v1);
	}
	if ( (x<w1)&&(y>0) ){
		p = oD[pp->adr +1 -wt];
		v1 = MAX(pv - p.alpha,v1);
	}
	if (x>0){
		p = oD[pp->adr -1    ];
		v1 = MAX(pv - p.alpha,v1);
	}
	if (x<w1){
		p = oD[pp->adr +1    ];
		v1 = MAX(pv - p.alpha,v1);
	}
	if ( (x>0)&&(y<h1) ){
		p = oD[pp->adr -1 +wt];
		v1 = MAX(pv - p.alpha,v1);
	}
	if ( (y<h1) ){
		p = oD[pp->adr    +wt];
		v1 = MAX(pv - p.alpha,v1);
	}
	if ( (x<w1)&&(y<h1) ){
		p = oD[pp->adr +1 +wt];
		v1 = MAX(pv - p.alpha,v1);
	}

	return v1;
}
//-------------------------------------------------------------------------------
void FindTargetDelta8(CFsAE *ae,ParamInfo	*prm)
{
	PF_Pixel *iD;
	PF_Pixel *oD;
	iD = (PF_Pixel *)ae->in->data();
	oD = (PF_Pixel *)ae->out->data();
	A_long w	= ae->out->width();
	A_long h	= ae->out->height();
	A_long iw	= ae->in->offsetWidth();
	A_long ow	= ae->out->offsetWidth();

	A_long targetOut =0;
	A_long i,j;

	PointPrm pp;
	pp.data			= ae->in->data();
	pp.height		= ae->in->height();
	pp.width		= ae->in->width();
	pp.widthOffset	= ae->in->offsetWidth();
	pp.widthTrue	= ae->in->widthTrue();
	PF_Pixel c		= prm->line_color;
	PF_Fixed lv_d	= prm->delta_opacity;
	PF_Fixed lv_a	= prm->alpha_opacity;
	//まず、ターゲットの場所を得る。
	//redへフラグを建てる
	pp.adr =0;
	if ( (prm->delta_flg==TRUE)&&(prm->alpha_flg==TRUE)){
		for (j=0;j<h;j++){
			pp.y =j;
			for (i=0;i<w;i++){
				pp.x =i;
				long p0 = pixDelta8(&pp) * lv_d >>16;
				long p1 = pixDeltaAlpha8(&pp) * lv_a >>16;
				c.alpha= RoundByteLong(p0 + p1 - (p0 * p1 >>8));
				oD[targetOut] = c;
				pp.adr++;
				targetOut++;
			}
			pp.adr		+= iw;
			targetOut	+= ow;
		}
	}else if (prm->delta_flg==TRUE) {
		for (j=0;j<h;j++){
			pp.y =j;
			for (i=0;i<w;i++){
				pp.x =i;
				c.alpha= RoundByteLong(pixDelta8(&pp) * lv_d >>16);
				oD[targetOut] = c;
				pp.adr++;
				targetOut++;
			}
			pp.adr		+= iw;
			targetOut	+= ow;
		}
	}else if (prm->alpha_flg==TRUE) {
		for (j=0;j<h;j++){
			pp.y =j;
			for (i=0;i<w;i++){
				pp.x =i;
				c.alpha= RoundByteLong(pixDeltaAlpha8(&pp) * lv_a >>16);
				oD[targetOut] = c;
				pp.adr++;
				targetOut++;
			}
			pp.adr		+= iw;
			targetOut	+= ow;
		}
	}
}

//-------------------------------------------------------------------------------
inline PF_Boolean cmparePix8(PF_Pixel s,PF_Pixel d,A_long rng)
{
	if (s.alpha !=PF_MAX_CHAN8) return FALSE;
	if (rng>0){
		long r  = ABS(s.red - d.red);
		long g  = ABS(s.green - d.green);
		long b  = ABS(s.blue - d.blue);
		return ( (r<=rng)&&(g<=rng)&&(b<=rng) );
	}else{
		return  ( (s.red==d.red)&&(s.green==d.green)&&(s.blue==d.blue) );
	}
}
//-------------------------------------------------------------------------------
inline PF_Boolean cmpareTable8(PF_Pixel s,PF_Pixel *color_tbl, A_long *level_tbl,A_long cnt)
{
	if (cnt<=0) return FALSE;
	long i;
	for(i=0; i<cnt;i++){
		if (cmparePix8(s,color_tbl[i],level_tbl[i])==TRUE) {
			return TRUE;
		}
	}
	return FALSE;
}
/*
//-------------------------------------------------------------------------------
void FindTarget8(CFsAE *ae,ParamInfo	*prm)
{
	PF_Pixel *iD;
	PF_Pixel *oD;
	iD = (PF_Pixel *)ae->in->data();
	oD = (PF_Pixel *)ae->out->data();
	A_long w		= ae->out->width();
	A_long h		= ae->out->height();
	A_long iw		= ae->in->offsetWidth();
	A_long ow		= ae->out->offsetWidth();

	A_long targetIn =0;
	A_long targetOut =0;
	A_long i,j;
	PF_Pixel	color_tbl[COLOR_COUNT];
	A_long		level_tbl[COLOR_COUNT];
	A_long		count = prm->tbl_count;
	if (count<=0) return;
	for (i=0;i<count;i++){
		color_tbl[i] = prm->color_tbl[i];
		level_tbl[i] = PF_MAX_CHAN8 * prm->level_tbl[i] >> 16;
	}
	A_long lv = PF_MAX_CHAN8 * prm->color_opacity >> 16;

	//まず、ターゲットの場所を得る。
	for (j=0;j<h;j++){
		for (i=0;i<w;i++){
			if ( cmpareTable8(iD[targetIn],color_tbl,level_tbl,count)==TRUE){
				long p0 = oD[targetOut].alpha;
				oD[targetOut].alpha = RoundByteLong(p0 + lv - (p0 * lv >>8));

			}
			targetIn++;
			targetOut++;
		}
		targetIn	+= iw;
		targetOut	+= ow;
	}
}
//-------------------------------------------------------------------------------
*/