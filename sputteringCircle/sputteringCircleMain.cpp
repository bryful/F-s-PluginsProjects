//-----------------------------------------------------------------------------
/*
	AEP Project用のプラグイン
	sputteringCircle
	描画のみ
*/
#include "sputteringCircle.h"

//-----------------------------------------------------------------------------
PF_Err MainRender8 (CFsAE *ae, ParamInfo *prm)
{
	PF_Err			err = PF_Err_NONE;

	A_long i,j;
	A_long value = prm->value;
	A_long pValue = prm->point_value;
	PF_Fixed scale = prm->length_scale;
	PF_Fixed pLen = prm->point_length; //値は整数
	PF_Fixed opa = prm->opa;
	A_long colRnd = prm->color_max -1;
	A_long colR;

	F_SRAND(prm->seed);	
	sputDrawPrm sdp;
	sdp.data	= ae->out->data();
	sdp.width	= ae->out->width();
	sdp.height	= ae->out->height();
	sdp.widthTrue	= ae->out->widthTrue();
	
	CRotCalc rp(ae->in_data);
	CRotCalc rp0(ae->in_data);

	A_long x,y;

	if (prm->point_len_sync == TRUE){
		pLen = pLen * scale >>16;
	}
	A_long xy0= -(prm->radius +prm->point_length);
	A_long xx1= ae->out->width() + (prm->radius +prm->point_length);
	A_long yy1= ae->out->height() + (prm->radius +prm->point_length);

	for ( i=0;i<value;i++){
		rp0.SetRotLength((FM_RAND((360L<<4) -1)) <<12,FM_RAND(prm->radius));
		PF_Fixed shiftX = rp0.pos().x;
		PF_Fixed shiftY = rp0.pos().y * prm->aspect >> 16;//アスペクトの調整

		if (prm->anchor_enabled == TRUE){
			x = prm->anchor.x + ((prm->pos.x +shiftX - prm->anchor.x) * scale >>16);
			y = prm->anchor.y + ((prm->pos.y +shiftY - prm->anchor.y) * scale >>16);
		}else{
			x = prm->pos.x + (shiftX * scale >>16);
			y = prm->pos.y + (shiftY * scale >>16);
		}

		for (j=0; j<pValue;j++){
			//ばらつきを作る
			if (FM_RAND(5)==0){
				rp.SetRotLength((FM_RAND((360L<<4) -1)) <<12, FM_RAND(pLen * 2));
			}else{
				rp.SetRotLength((FM_RAND((360L<<4) -1)) <<12, FM_RAND(pLen));
			}
			sdp.x = x + (rp.pos().x);
			sdp.y = y + (rp.pos().y);
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
	PF_Fixed scale = prm->length_scale;
	PF_Fixed pLen = prm->point_length; //値は整数
	PF_Fixed opa = prm->opa;
	A_long colRnd = prm->color_max -1;
	A_long colR;
	PF_Pixel16 colors[4];
	for (i=0; i<UI_COLOR_MAX_V;i++) 
		colors[i] = CONV8TO16(prm->colors[i]);

	F_SRAND(prm->seed);	
	sputDrawPrm sdp;
	sdp.data	= ae->out->data();
	sdp.width	= ae->out->width();
	sdp.height	= ae->out->height();
	sdp.widthTrue	= ae->out->widthTrue();
	
	CRotCalc rp(ae->in_data);
	CRotCalc rp0(ae->in_data);


	A_long x,y;

	if (prm->point_len_sync == TRUE){
		pLen = pLen * scale >>16;
	}
	A_long xy0= -(prm->radius +prm->point_length);
	A_long xx1= ae->out->width() + (prm->radius +prm->point_length);
	A_long yy1= ae->out->height() + (prm->radius +prm->point_length);

	for ( i=0;i<value;i++){
		rp0.SetRotLength((FM_RAND((360L<<4) -1)) <<12,FM_RAND(prm->radius));
		PF_Fixed shiftX = rp0.pos().x;
		PF_Fixed shiftY = rp0.pos().y * prm->aspect >> 16;//アスペクトの調整

		if (prm->anchor_enabled == TRUE){
			x = prm->anchor.x + ((prm->pos.x +shiftX - prm->anchor.x) * scale >>16);
			y = prm->anchor.y + ((prm->pos.y +shiftY - prm->anchor.y) * scale >>16);
		}else{
			x = prm->pos.x + (shiftX * scale >>16);
			y = prm->pos.y + (shiftY * scale >>16);
		}


		for (j=0; j<pValue;j++){
			//ばらつきを作る
			if (FM_RAND(5)==0){
				rp.SetRotLength((FM_RAND((360L<<4) -1)) <<12, FM_RAND(pLen * 2));
			}else{
				rp.SetRotLength((FM_RAND((360L<<4) -1)) <<12, FM_RAND(pLen));
			}
			sdp.x = x + (rp.pos().x);
			sdp.y = y + (rp.pos().y);
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
PF_Err MainRender32 (CFsAE *ae, ParamInfo *prm)
{
	PF_Err			err = PF_Err_NONE;

	A_long i,j;
	A_long value = prm->value;
	A_long pValue = prm->point_value;
	PF_Fixed scale = prm->length_scale;
	PF_Fixed pLen = prm->point_length; //値は整数
	PF_Fixed opa = prm->opa;
	A_long colRnd = prm->color_max -1;
	A_long colR;
	PF_PixelFloat colors[4];
	for (i=0; i<UI_COLOR_MAX_V;i++) 
		colors[i] = CONV8TO32(prm->colors[i]);

	F_SRAND(prm->seed);	
	sputDrawPrm sdp;
	sdp.data	= ae->out->data();
	sdp.width	= ae->out->width();
	sdp.height	= ae->out->height();
	sdp.widthTrue	= ae->out->widthTrue();
	
	CRotCalc rp(ae->in_data);
	CRotCalc rp0(ae->in_data);


	A_long x,y;

	if (prm->point_len_sync == TRUE){
		pLen = pLen * scale >>16;
	}
	A_long xy0= -(prm->radius +prm->point_length);
	A_long xx1= ae->out->width() + (prm->radius +prm->point_length);
	A_long yy1= ae->out->height() + (prm->radius +prm->point_length);

	for ( i=0;i<value;i++){
		rp0.SetRotLength((FM_RAND((360L<<4) -1)) <<12,FM_RAND(prm->radius));
		PF_Fixed shiftX = rp0.pos().x;
		PF_Fixed shiftY = rp0.pos().y * prm->aspect >> 16;//アスペクトの調整

		if (prm->anchor_enabled == TRUE){
			x = prm->anchor.x + ((prm->pos.x +shiftX - prm->anchor.x) * scale >>16);
			y = prm->anchor.y + ((prm->pos.y +shiftY - prm->anchor.y) * scale >>16);
		}else{
			x = prm->pos.x + (shiftX * scale >>16);
			y = prm->pos.y + (shiftY * scale >>16);
		}


		for (j=0; j<pValue;j++){
			//ばらつきを作る
			if (FM_RAND(5)==0){
				rp.SetRotLength((FM_RAND((360L<<4) -1)) <<12, FM_RAND(pLen * 2));
			}else{
				rp.SetRotLength((FM_RAND((360L<<4) -1)) <<12, FM_RAND(pLen));
			}
			sdp.x = x + (rp.pos().x);
			sdp.y = y + (rp.pos().y);
			sdp.opa = (PF_FpShort)FM_RAND(opa) /65536;
			sdp.sptIdx = prm->sputRandTable[FM_RAND(200-1)];
			colR = FM_RAND(colRnd);
			if ( (x>=xy0)&&(x<xx1)&&(y>=xy0)&&(y<yy1) )
				SputDataDraw32(&sdp,colors[colR]);
		}
	}
	
	return err;
}
