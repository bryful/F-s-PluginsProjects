#include "LineDetection.h"

//-------------------------------------------------------------------------------
//RGBの最大値とアルファーの値を適当に8Bitに割り振る
inline A_u_short rgbMax16(PF_Pixel16 c)
{
	A_u_short m0,m1;
	if ( c.alpha ==0 ) return 0;

	m0 =  MAX( MAX(c.red,c.green),c.blue);
	m1 =  MIN( MIN(c.red,c.green),c.blue);
	
	A_long  mp = (m0+m1) >> 1;
	mp = mp * c.alpha / PF_MAX_CHAN16;
	if (mp<0) { mp = 0; }
	else if(mp>PF_MAX_CHAN16) { mp = PF_MAX_CHAN16; }
	return  (A_u_short)mp;

}
//-------------------------------------------------------------------------------
inline A_u_short pixDelta16(PointPrm *pp)
{
	PF_Pixel16 *oD;
	A_long wt = pp->widthTrue;
	//long w = pp->width;
	A_long w1 = pp->width -1;
	A_long h1 = pp->height -1;
	oD = (PF_Pixel16 *)pp->data;
	A_u_short pv = rgbMax16(oD[pp->adr]);
	A_u_short v1 = 0;

	A_long x = pp->x;
	A_long y = pp->y;
	PF_Pixel16 p;
	if ( (x>0)&&(y>0) ){
		p = oD[pp->adr -1 -wt];
		v1 = MAX(rgbMax16(p)-pv,v1);
	}
	if (y>0){
		p = oD[pp->adr    -wt];
		v1 = MAX(rgbMax16(p)-pv,v1);
	}
	if ( (x<w1)&&(y>0) ){
		p = oD[pp->adr +1 -wt];
		v1 = MAX(rgbMax16(p)-pv,v1);
	}
	if (x>0){
		p = oD[pp->adr -1    ];
		v1 = MAX(rgbMax16(p)-pv,v1);
	}
	if (x<w1){
		p = oD[pp->adr +1    ];
		v1 = MAX(rgbMax16(p)-pv,v1);
	}
	if ( (x>0)&&(y<h1) ){
		p = oD[pp->adr -1 +wt];
		v1 = MAX(rgbMax16(p)-pv,v1);
	}
	if ( (y<h1) ){
		p = oD[pp->adr    +wt];
		v1 = MAX(rgbMax16(p)-pv,v1);
	}
	if ( (x<w1)&&(y<h1) ){
		p = oD[pp->adr +1 +wt];
		v1 = MAX(rgbMax16(p)-pv,v1);
	}

	return v1;
}
//-------------------------------------------------------------------------------
inline A_u_short pixDeltaAlpha16(PointPrm *pp)
{
	PF_Pixel16 *oD;
	A_long wt = pp->widthTrue;
	//long w = pp->width;
	A_long w1 = pp->width -1;
	A_long h1 = pp->height -1;
	oD = (PF_Pixel16 *)pp->data;
	A_u_short pv = oD[pp->adr].alpha;
	A_u_short v1 = 0;

	A_long x = pp->x;
	A_long y = pp->y;
	PF_Pixel16 p;
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
void FindTargetDelta16(CFsAE *ae,ParamInfo	*prm)
{
	PF_Pixel16 *iD;
	PF_Pixel16 *oD;
	iD = (PF_Pixel16 *)ae->in->data();
	oD = (PF_Pixel16 *)ae->out->data();
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
	PF_Pixel16 c;
	c = CONV8TO16(prm->line_color);
	c.alpha	= PF_MAX_CHAN16;
	PF_Fixed lv_d = prm->delta_opacity >> 4;
	PF_Fixed lv_a = prm->alpha_opacity >> 4;
	//まず、ターゲットの場所を得る。
	//redへフラグを建てる
	pp.adr =0;
	if ( (prm->delta_flg==TRUE)&&(prm->alpha_flg==TRUE)){
		for (j=0;j<h;j++){
			pp.y =j;
			for (i=0;i<w;i++){
				pp.x =i;
				long p0 = pixDelta16(&pp) * lv_d >>12;
				long p1 = pixDeltaAlpha16(&pp) * lv_a >>12;
				c.alpha= RoundShort(p0 + p1 - (p0 * p1 >>15));
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
				c.alpha= RoundShort(pixDelta16(&pp) * lv_d >>12);
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
				c.alpha= RoundShort(pixDeltaAlpha16(&pp) * lv_a >>12);
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
inline PF_Boolean cmparePix16(PF_Pixel16 s,PF_Pixel16 d,A_long rng)
{
	if (s.alpha !=PF_MAX_CHAN16) return FALSE;
	if (rng>0){
		A_long r  = ABS(s.red - d.red);
		A_long g  = ABS(s.green - d.green);
		A_long b  = ABS(s.blue - d.blue);
		return ( (r<=rng)&&(g<=rng)&&(b<=rng) );
	}else{
		return  ( (s.red==d.red)&&(s.green==d.green)&&(s.blue==d.blue) );
	}
}
//-------------------------------------------------------------------------------
inline PF_Boolean cmpareTable16(PF_Pixel16 s,PF_Pixel16 *color_tbl, A_long *level_tbl,A_long cnt)
{
	if (cnt<=0) return FALSE;
	long i;
	for(i=0; i<cnt;i++){
		if (cmparePix16(s,color_tbl[i],level_tbl[i])==TRUE) {
			return TRUE;
		}
	}
	return FALSE;
}
/*
//-------------------------------------------------------------------------------
void FindTarget16(CFsAE *ae,ParamInfo	*prm)
{
	PF_Pixel16 *iD;
	PF_Pixel16 *oD;
	iD = (PF_Pixel16 *)ae->in->data();
	oD = (PF_Pixel16 *)ae->out->data();
	A_long w		= ae->in->width();
	A_long h		= ae->in->height();
	A_long iw		= ae->in->offsetWidth();
	A_long ow		= ae->out->offsetWidth();
	//long iwt	= aprm->inWidth;
	//long owt	= aprm->outWidth;

	A_long targetIn =0;
	A_long targetOut =0;
	A_long i,j;
	PF_Pixel16	color_tbl[COLOR_COUNT];
	A_long		level_tbl[COLOR_COUNT];
	A_long		count = prm->tbl_count;
	if (count<=0) return;
	
	PF_Pixel16 c;
	for (i=0;i<count;i++){
		c = CONV8TO16(prm->color_tbl[i]);
		c.alpha	= PF_MAX_CHAN16;
		color_tbl[i] = c;
		level_tbl[i] = PF_MAX_CHAN16 * prm->level_tbl[i] >> 16;
	}
	long lv = PF_MAX_CHAN16 * prm->color_opacity >> 16;

	//まず、ターゲットの場所を得る。
	for (j=0;j<h;j++){
		for (i=0;i<w;i++){
			if ( cmpareTable16(iD[targetIn],color_tbl,level_tbl,count)==TRUE){
				long p0 = oD[targetOut].alpha;
				oD[targetOut].alpha = RoundShort(p0 + lv - (p0 * lv >>16));

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
