#include "LineDetection.h"

//-------------------------------------------------------------------------------
//RGBの最大値とアルファーの値を適当に8Bitに割り振る
inline PF_FpShort rgbMax32(PF_PixelFloat c)
{
	PF_FpShort m0,m1;
	if ( c.alpha ==0 ) return 0;

	m0 =  MAX( MAX(c.red,c.green),c.blue);
	m1 =  MIN( MIN(c.red,c.green),c.blue);
	
	PF_FpShort  mp = (m0+m1) / 2;
	mp = mp * c.alpha;
	if (mp<0) { mp = 0; }
	return  (PF_FpShort)mp;
}
//-------------------------------------------------------------------------------
inline PF_FpShort pixDelta32(PointPrm *pp)
{
	PF_Pixel32 *oD;
	A_long wt = pp->widthTrue;
	//long w = pp->width;
	A_long w1 = pp->width -1;
	A_long h1 = pp->height -1;
	oD = (PF_Pixel32 *)pp->data;
	PF_FpShort pv = rgbMax32(oD[pp->adr]);
	PF_FpShort v1 = 0;

	A_long x = pp->x;
	A_long y = pp->y;
	PF_PixelFloat p;
	if ( (x>0)&&(y>0) ){
		p = oD[pp->adr -1 -wt];
		v1 = MAX(rgbMax32(p)-pv,v1);
	}
	if (y>0){
		p = oD[pp->adr    -wt];
		v1 = MAX(rgbMax32(p)-pv,v1);
	}
	if ( (x<w1)&&(y>0) ){
		p = oD[pp->adr +1 -wt];
		v1 = MAX(rgbMax32(p)-pv,v1);
	}
	if (x>0){
		p = oD[pp->adr -1    ];
		v1 = MAX(rgbMax32(p)-pv,v1);
	}
	if (x<w1){
		p = oD[pp->adr +1    ];
		v1 = MAX(rgbMax32(p)-pv,v1);
	}
	if ( (x>0)&&(y<h1) ){
		p = oD[pp->adr -1 +wt];
		v1 = MAX(rgbMax32(p)-pv,v1);
	}
	if ( (y<h1) ){
		p = oD[pp->adr    +wt];
		v1 = MAX(rgbMax32(p)-pv,v1);
	}
	if ( (x<w1)&&(y<h1) ){
		p = oD[pp->adr +1 +wt];
		v1 = MAX(rgbMax32(p)-pv,v1);
	}

	return v1;
}
//-------------------------------------------------------------------------------
inline PF_FpShort pixDeltaAlpha32(PointPrm *pp)
{
	PF_Pixel32 *oD;
	A_long wt = pp->widthTrue;
	//long w = pp->width;
	A_long w1 = pp->width -1;
	A_long h1 = pp->height -1;
	oD = (PF_Pixel32 *)pp->data;
	PF_FpShort pv = oD[pp->adr].alpha;
	PF_FpShort v1 = 0;

	A_long x = pp->x;
	A_long y = pp->y;
	PF_Pixel32 p;
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
void FindTargetDelta32(CFsAE *ae,ParamInfo	*prm)
{
	PF_Pixel32 *iD;
	PF_Pixel32 *oD;
	iD = (PF_Pixel32 *)ae->in->data();
	oD = (PF_Pixel32 *)ae->out->data();
	A_long w		= ae->in->width();
	A_long h		= ae->in->height();
	A_long iw		= ae->in->offsetWidth();
	A_long ow		= ae->out->offsetWidth();

	//long targetIn =0;
	A_long targetOut =0;
	A_long i,j;

	PointPrm pp;
	pp.data		= ae->in->data();
	pp.height	= ae->in->height();
	pp.width	= ae->in->width();
	pp.widthOffset = ae->in->offsetWidth();
	pp.widthTrue = ae->in->widthTrue();
	PF_Pixel32 c;
	c = CONV8TO32(prm->line_color);
	c.alpha	= PF_MAX_CHAN16;
	PF_FpShort lv_d = (PF_FpShort)prm->delta_opacity /65536;
	PF_FpShort lv_a = (PF_FpShort)prm->alpha_opacity /65536;
	//まず、ターゲットの場所を得る。
	//redへフラグを建てる
	pp.adr =0;
	if ( (prm->delta_flg==TRUE)&&(prm->alpha_flg==TRUE)){
		for (j=0;j<h;j++){
			pp.y =j;
			for (i=0;i<w;i++){
				pp.x =i;
				PF_FpShort p0 = pixDelta32(&pp) * lv_d;
				PF_FpShort p1 = pixDeltaAlpha32(&pp) * lv_a;
				c.alpha= RoundFpShortDouble(p0 + p1 - (p0 * p1));
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
				c.alpha= RoundFpShortDouble(pixDelta32(&pp) * lv_d);
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
				c.alpha= RoundFpShortDouble(pixDeltaAlpha32(&pp) * lv_a);
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
inline PF_Boolean cmparePix32(PF_Pixel32 s,PF_Pixel32 d,PF_FpShort rng)
{
	if (s.alpha !=1.0) return FALSE;
	if (rng>0){
		PF_FpShort r  = ABS(s.red - d.red);
		PF_FpShort g  = ABS(s.green - d.green);
		PF_FpShort b  = ABS(s.blue - d.blue);
		return ( (r<=rng)&&(g<=rng)&&(b<=rng) );
	}else{
		return  ( (s.red==d.red)&&(s.green==d.green)&&(s.blue==d.blue) );
	}
}
//-------------------------------------------------------------------------------
inline PF_Boolean cmpareTable32(PF_Pixel32 s,PF_Pixel32 *color_tbl, PF_FpShort *level_tbl,A_long cnt)
{
	if (cnt<=0) return FALSE;
	A_long i;
	for(i=0; i<cnt;i++){
		if (cmparePix32(s,color_tbl[i],level_tbl[i])==TRUE) {
			return TRUE;
		}
	}
	return FALSE;
}
/*
//-------------------------------------------------------------------------------
void FindTarget32(CFsAE *ae,ParamInfo	*prm)
{
	PF_Pixel32 *iD;
	PF_Pixel32 *oD;
	iD = (PF_Pixel32 *)ae->in->data();
	oD = (PF_Pixel32 *)ae->out->data();
	A_long w		= ae->in->width();
	A_long h		= ae->in->height();
	A_long iw		= ae->in->offsetWidth();
	A_long ow		= ae->out->offsetWidth();
	//long iwt	= aprm->inWidth;
	//long owt	= aprm->outWidth;

	A_long targetIn =0;
	A_long targetOut =0;
	A_long i,j;
	PF_Pixel32	color_tbl[COLOR_COUNT];
	PF_FpShort	level_tbl[COLOR_COUNT];
	A_long		count = prm->tbl_count;
	if (count<=0) return;
	
	PF_Pixel32 c;
	for (i=0;i<count;i++){
		c = CONV8TO32(prm->color_tbl[i]);
		c.alpha	= 1;
		color_tbl[i] = c;
		level_tbl[i] = (PF_FpShort)prm->level_tbl[i]/65536;
	}
	PF_FpShort lv = (PF_FpShort)prm->color_opacity/65536;

	//まず、ターゲットの場所を得る。
	for (j=0;j<h;j++){
		for (i=0;i<w;i++){
			if ( cmpareTable32(iD[targetIn],color_tbl,level_tbl,count)==TRUE){
				PF_FpShort p0 = oD[targetOut].alpha;
				oD[targetOut].alpha = RoundFpShortDouble(p0 + lv - (p0 * lv));

			}
			targetIn++;
			targetOut++;
		}
		targetIn	+= iw;
		targetOut	+= ow;
	}
}
*/
//-------------------------------------------------------------------------------
