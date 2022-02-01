//-----------------------------------------------------------------------------
/*
	AEP Project用のプラグイン
	sputteringRect
	描画のみ
*/
#include "sputteringRect.h"
//-----------------------------------------------------------------------------
PF_Err MainRender8 (CFsAE *ae, ParamInfo *prm)
{
	PF_Err			err = PF_Err_NONE;

	A_long i,j;
	A_long value = prm->value;
	A_long pValue = prm->point_value;
	A_long pLen = prm->point_length;
	A_long colRnd = prm->color_max -1;
	A_long colR;
	A_long	opa = prm->opa;
	
	//範囲
	A_long	x0 = prm->rect.left;
	A_long	y0 = prm->rect.top;
	A_long	dx = prm->rect.right - prm->rect.left -1;
	A_long	dy = prm->rect.bottom - prm->rect.top -1;
	F_SRAND(prm->seed);	
	sputDrawPrm sdp;
	sdp.data	= ae->out->data();
	sdp.width	= ae->out->width();
	sdp.height	= ae->out->height();
	sdp.widthTrue	= ae->out->widthTrue();
	
	CRotCalc rp(ae->in_data);
	A_long xy0= -pLen;
	A_long xx1= ae->out->width() + pLen;
	A_long yy1= ae->out->height() + pLen;



	for ( i=0;i<value;i++){
		A_long x = x0 + FM_RAND(dx);
		A_long y = y0 + FM_RAND(dy);
		//繰り返し
		for (j=0; j<pValue;j++){
			
			//ばらつきを作る
			if (FM_RAND(5)==0){
				rp.SetRotLength((FM_RAND((360L<<4) -1)) <<12,FM_RAND(pLen * 3));
			}else{
				rp.SetRotLength((FM_RAND((360L<<4) -1)) <<12,FM_RAND(pLen));
			}
			sdp.x = x + (rp.pos().x );
			sdp.y = y + (rp.pos().y );
			sdp.opa = (PF_FpShort)FM_RAND(opa)/65536;
			sdp.sptIdx = prm->sputRandTable[FM_RAND(200-1)];
			colR = FM_RAND(colRnd);
			if ( (x>=xy0)&&(x<xx1)&&(y>=xy0)&&(y<yy1) )
				SputDataDraw8(&sdp,prm->colors[colR]);
		}
	}
	
	return err;
}
//-----------------------------------------------------------------------------
PF_Err MainRender16 (CFsAE *ae, ParamInfo *prm)
{
	PF_Err			err = PF_Err_NONE;

	A_long i,j;
	A_long value = prm->value;
	A_long pValue = prm->point_value;
	PF_Fixed pLen = prm->point_length;
	A_long colRnd = prm->color_max -1;
	A_long colR;
	A_long	opa = prm->opa;
	PF_Pixel16 colors[4];
	for (i=0; i<UI_COLOR_MAX_V;i++) 
		colors[i] = CONV8TO16(prm->colors[i]);
	
	//範囲
	A_long	x0 = prm->rect.left;
	A_long	y0 = prm->rect.top;
	A_long	dx = prm->rect.right - prm->rect.left -1;
	A_long	dy = prm->rect.bottom - prm->rect.top -1;
	F_SRAND(prm->seed);	
	sputDrawPrm sdp;
	sdp.data	= ae->out->data();
	sdp.width	= ae->out->width();
	sdp.height	= ae->out->height();
	sdp.widthTrue	= ae->out->widthTrue();
	
	CRotCalc rp(ae->in_data);

	A_long xy0= -pLen;
	A_long xx1= ae->out->width() + pLen;
	A_long yy1= ae->out->height() + pLen;


	for ( i=0;i<value;i++){
		A_long x = x0 + FM_RAND(dx);
		A_long y = y0 + FM_RAND(dy);
		//繰り返し
		for (j=0; j<pValue;j++){
			
			//ばらつきを作る
			if (FM_RAND(5)==0){
				rp.SetRotLength((FM_RAND((360L<<4) -1)) <<12,FM_RAND(pLen * 3));
			}else{
				rp.SetRotLength((FM_RAND((360L<<4) -1)) <<12,FM_RAND(pLen));
			}
			sdp.x = x + (rp.pos().x );
			sdp.y = y + (rp.pos().y );
			sdp.opa = (PF_FpShort)FM_RAND(opa)/65536;
			sdp.sptIdx = prm->sputRandTable[FM_RAND(200-1)];
			colR = FM_RAND(colRnd);
			if ( (x>=xy0)&&(x<xx1)&&(y>=xy0)&&(y<yy1) )
				SputDataDraw16(&sdp,colors[colR]);
		}
	}
	
	return err;
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
PF_Err MainRender32 (CFsAE *ae, ParamInfo *prm)
{
	PF_Err			err = PF_Err_NONE;

	A_long i,j;
	A_long value = prm->value;
	A_long pValue = prm->point_value;
	PF_Fixed pLen = prm->point_length;
	A_long colRnd = prm->color_max -1;
	A_long colR;
	A_long	opa = prm->opa;
	PF_PixelFloat colors[4];
	for (i=0; i<UI_COLOR_MAX_V;i++) 
		colors[i] = CONV8TO32(prm->colors[i]);
	
	//範囲
	A_long	x0 = prm->rect.left;
	A_long	y0 = prm->rect.top;
	A_long	dx = prm->rect.right - prm->rect.left -1;
	A_long	dy = prm->rect.bottom - prm->rect.top -1;
	F_SRAND(prm->seed);	
	sputDrawPrm sdp;
	sdp.data	= ae->out->data();
	sdp.width	= ae->out->width();
	sdp.height	= ae->out->height();
	sdp.widthTrue	= ae->out->widthTrue();
	
	CRotCalc rp(ae->in_data);

	A_long xy0= -pLen;
	A_long xx1= ae->out->width() + pLen;
	A_long yy1= ae->out->height() + pLen;


	for ( i=0;i<value;i++){
		A_long x = x0 + FM_RAND(dx);
		A_long y = y0 + FM_RAND(dy);
		//繰り返し
		for (j=0; j<pValue;j++){
			
			//ばらつきを作る
			if (FM_RAND(5)==0){
				rp.SetRotLength((FM_RAND((360L<<4) -1)) <<12,FM_RAND(pLen * 3));
			}else{
				rp.SetRotLength((FM_RAND((360L<<4) -1)) <<12,FM_RAND(pLen));
			}
			sdp.x = x + (rp.pos().x );
			sdp.y = y + (rp.pos().y );
			sdp.opa = (PF_FpShort)FM_RAND(opa) / 65536;
			sdp.sptIdx = prm->sputRandTable[FM_RAND(200-1)];
			colR = FM_RAND(colRnd);
			if ( (x>=xy0)&&(x<xx1)&&(y>=xy0)&&(y<yy1) )
				SputDataDraw32(&sdp,colors[colR]);
		}
	}
	
	return err;
}
//-----------------------------------------------------------------------------
