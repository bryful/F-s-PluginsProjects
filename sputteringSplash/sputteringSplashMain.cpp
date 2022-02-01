//-----------------------------------------------------------------------------
/*
	AEP Project用のプラグイン
	sputteringSplash
	描画のみ
*/
#include "sputteringSplash.h"

//-----------------------------------------------------------------------------
PF_Err MainRender8 (CFsAE *ae, ParamInfo *prm)
{
	PF_Err			err = PF_Err_NONE;

	PF_Fixed value = prm->value /10;
	A_long pValue = prm->point_value;
	PF_Fixed pLen = prm->point_length; //値は整数
	A_long colRnd = prm->color_max -1;
	A_long colR;

	A_long i;
	A_long x,y;
	A_long w = ae->out->width();
	A_long h = ae->out->height();

	//エッジの場所を探してoutputのalphaへ保存
	A_u_long targetIn =0;
	A_u_long targetOut =0;
	PF_Pixel *iD;
	iD = ae->in->data();
	PF_Pixel *oD;
	oD = ae->out->data();
	A_u_char *buf;
	buf = prm->buf;
	PF_Pixel c;
	A_long we = w-1;
	A_long he = h-1;
	A_long offset[3];
	switch(prm->dir){
		case 2: 
			offset[0] =  0 - ae->in->widthTrue();
			offset[1] = +1 - ae->in->widthTrue();
			offset[2] = +1;
			break;
		case 3:
			offset[0] = +1 ;
			offset[1] = 0;
			offset[2] = 0;
			break;
		case 4: 
			offset[0] = +1;
			offset[1] = +1 + ae->in->widthTrue();
			offset[2] =  0 + ae->in->widthTrue();
			break;
		case 5: 
			offset[0] = 0 + ae->in->widthTrue();
			offset[1] = 0;
			offset[2] = 0;
			break;
		case 6: 
			offset[0] = -1;
			offset[1] = -1 + ae->in->widthTrue();
			offset[2] =  0 + ae->in->widthTrue();
			break;
		case 7: 
			offset[0] = -1;
			offset[1] = 0;
			offset[2] = 0;
			break;
		case 8:
			offset[0] = -1;
			offset[1] = -1 - ae->in->widthTrue();
			offset[2] =  0 - ae->in->widthTrue();
			break;
		case 1:
		default:
			offset[0] = 0 - ae->in->widthTrue();
			offset[1] = 0;
			offset[2] = 0;
			break;
	}
	A_u_char eg = 0;
	for( y=0; y<h; y++){
		for( x=0; x<w; x++){
			c = iD[targetIn];
			eg = 0;
			if (c.alpha>=EDGE_BORDER8){
				if ( (x>=1)&&(x<we)&&(y>=1)&&(y<he) ){ //端っこは無視
					for (i=0; i<3;i++){
						if (offset[i]!=0) {
							if (iD[targetIn + offset[i]].alpha<EDGE_BORDER8){
								eg = TARGET_EDGE;
								break;
							}
						}
					}
					
				}
			}
			buf[targetOut] = eg;
			targetIn++;
			targetOut++;
		}
		targetIn += ae->in->offsetWidth();
		targetOut += ae->out->offsetWidth();
	}

	//さぁ、描画だ
	F_SRAND(prm->seed);	

	sputDrawPrm sdp;
	sdp.data	= ae->out->data();
	sdp.width	= ae->out->width();
	sdp.height	= ae->out->height();
	sdp.widthTrue	= ae->out->widthTrue();

	CRotCalc rp(ae->in_data);

	targetOut =0;
	for( y=0; y<h; y++){
		for( x=0; x<w; x++){
			if (buf[targetOut]>0){
				A_long rnd = FM_RAND((1L<<16));
				if (value > rnd){
					for (i=0; i<pValue;i++){
						//ばらつきを作る
						if (FM_RAND(5)==0){
							rp.SetRotLength((FM_RAND((360L<<4) -1)) <<12,FM_RAND(pLen * 3));
						}else{
							rp.SetRotLength((FM_RAND((360L<<4) -1)) <<12,FM_RAND(pLen));
						}
						sdp.x = x + (rp.x() );
						sdp.y = y + (rp.y() );
						
						PF_Fixed opa = prm->opa;
						sdp.opa = (PF_FpShort)FM_RAND(opa)/65536;
						sdp.sptIdx = prm->sputRandTable[FM_RAND(200-1)];
						colR = FM_RAND(colRnd);
						SputDataDraw8(&sdp,prm->colors[colR]);
					}
				}
			}
			targetOut++;

		}
		targetOut += ae->out->offsetWidth();
	}
	return err;
}
//-----------------------------------------------------------------------------
PF_Err MainRender16 (CFsAE *ae, ParamInfo *prm)
{
	PF_Err			err = PF_Err_NONE;

	PF_Fixed value = prm->value / 10;
	A_long pValue = prm->point_value;
	PF_Fixed pLen = prm->point_length; //値は整数
	A_long colRnd = prm->color_max -1;
	A_long colR;
	A_long i;
	A_long x,y;

	A_long w = ae->out->width();
	A_long h = ae->out->height();

	PF_Pixel16 colors[4];

	for (i=0; i<UI_COLOR_MAX_V;i++) 
		colors[i] = CONV8TO16(prm->colors[i]);


	//エッジの場所を探してoutputのalphaへ保存
	A_u_long targetIn =0;
	A_u_long targetOut =0;
	PF_Pixel16 *iD;
	iD = (PF_Pixel16 *)ae->in->data();
	PF_Pixel16 *oD;
	oD = (PF_Pixel16 *)ae->out->data();
	A_u_char *buf;
	buf = prm->buf;
	PF_Pixel16 c;
	A_long we = w-1;
	A_long he = h-1;
	A_long offset[3];
	A_long inwt = ae->in->widthTrue();
	switch(prm->dir){
		case 2: 
			offset[0] =  0 - inwt;
			offset[1] = +1 - inwt;
			offset[2] = +1;
			break;
		case 3:
			offset[0] = +1 ;
			offset[1] = 0;
			offset[2] = 0;
			break;
		case 4: 
			offset[0] = +1;
			offset[1] = +1 + inwt;
			offset[2] =  0 + inwt;
			break;
		case 5: 
			offset[0] = 0 + inwt;
			offset[1] = 0;
			offset[2] = 0;
			break;
		case 6: 
			offset[0] = -1;
			offset[1] = -1 + inwt;
			offset[2] =  0 + inwt;
			break;
		case 7: 
			offset[0] = -1;
			offset[1] = 0;
			offset[2] = 0;
			break;
		case 8:
			offset[0] = -1;
			offset[1] = -1 - inwt;
			offset[2] =  0 - inwt;
			break;
		case 1:
		default:
			offset[0] = 0 - inwt;
			offset[1] = 0;
			offset[2] = 0;
			break;
	}
	A_u_char eg = 0;
	for( y=0; y<h; y++){
		for( x=0; x<w; x++){
			c = iD[targetIn];
			eg = 0;
			if (c.alpha>=EDGE_BORDER16){
				if ( (x>=1)&&(x<we)&&(y>=1)&&(y<he) ){ //端っこは無視
					for (i=0; i<3;i++){
						if (offset[i]!=0) {
							if (iD[targetIn + offset[i]].alpha<EDGE_BORDER16){
								eg = TARGET_EDGE;
								break;
							}
						}
					}
					
				}
			}
			buf[targetOut] = eg;
			targetIn++;
			targetOut++;
		}
		targetIn += ae->in->offsetWidth();
		targetOut += ae->out->offsetWidth();
	}

	//さぁ、描画だ
	F_SRAND(prm->seed);	

	sputDrawPrm sdp;
	sdp.data	= ae->out->data();
	sdp.width	= ae->out->width();
	sdp.height	= ae->out->height();
	sdp.widthTrue	= ae->out->widthTrue();

	CRotCalc rp(ae->in_data);

	targetOut =0;
	for( y=0; y<h; y++){
		for( x=0; x<w; x++){
			if (buf[targetOut]>0){
				A_long rnd = FM_RAND((1L<<16));
				if (value > rnd){
					for (i=0; i<pValue;i++){
						//ばらつきを作る
						if (FM_RAND(5)==0){
							rp.SetRotLength((FM_RAND((360L<<4) -1)) <<12,FM_RAND(pLen * 3));
						}else{
							rp.SetRotLength((FM_RAND((360L<<4) -1)) <<12,FM_RAND(pLen));
						}
						sdp.x = x + (rp.x() );
						sdp.y = y + (rp.y() );
						
						PF_Fixed opa = prm->opa;
						sdp.opa = (PF_FpShort)FM_RAND(opa)/65536;
						sdp.sptIdx = prm->sputRandTable[FM_RAND(200-1)];
						colR = FM_RAND(colRnd);
						SputDataDraw16(&sdp,colors[colR]);
					}
				}
			}			
			targetOut++;
		}
		targetOut += ae->out->offsetWidth();
	}
	return err;
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
PF_Err MainRender32 (CFsAE *ae, ParamInfo *prm)
{
	PF_Err			err = PF_Err_NONE;

	PF_Fixed value = prm->value / 10;
	A_long pValue = prm->point_value;
	PF_Fixed pLen = prm->point_length; //値は整数
	A_long colRnd = prm->color_max -1;
	A_long colR;
	A_long i;
	A_long x,y;

	A_long w = ae->out->width();
	A_long h = ae->out->height();

	PF_PixelFloat colors[4];

	for (i=0; i<UI_COLOR_MAX_V;i++) 
		colors[i] = CONV8TO32(prm->colors[i]);


	//エッジの場所を探してoutputのalphaへ保存
	A_u_long targetIn =0;
	A_u_long targetOut =0;
	PF_PixelFloat *iD;
	iD = (PF_PixelFloat *)ae->in->data();
	PF_PixelFloat *oD;
	oD = (PF_PixelFloat *)ae->out->data();
	A_u_char *buf;
	buf = prm->buf;
	PF_PixelFloat c;
	A_long we = w-1;
	A_long he = h-1;
	A_long offset[3];
	A_long inwt = ae->in->widthTrue();
	switch(prm->dir){
		case 2: 
			offset[0] =  0 - inwt;
			offset[1] = +1 - inwt;
			offset[2] = +1;
			break;
		case 3:
			offset[0] = +1 ;
			offset[1] = 0;
			offset[2] = 0;
			break;
		case 4: 
			offset[0] = +1;
			offset[1] = +1 + inwt;
			offset[2] =  0 + inwt;
			break;
		case 5: 
			offset[0] = 0 + inwt;
			offset[1] = 0;
			offset[2] = 0;
			break;
		case 6: 
			offset[0] = -1;
			offset[1] = -1 + inwt;
			offset[2] =  0 + inwt;
			break;
		case 7: 
			offset[0] = -1;
			offset[1] = 0;
			offset[2] = 0;
			break;
		case 8:
			offset[0] = -1;
			offset[1] = -1 - inwt;
			offset[2] =  0 - inwt;
			break;
		case 1:
		default:
			offset[0] = 0 - inwt;
			offset[1] = 0;
			offset[2] = 0;
			break;
	}
	A_u_char eg = 0;
	for( y=0; y<h; y++){
		for( x=0; x<w; x++){
			c = iD[targetIn];
			eg = 0;
			if (c.alpha>=EDGE_BORDER32){
				if ( (x>=1)&&(x<we)&&(y>=1)&&(y<he) ){ //端っこは無視
					for (i=0; i<3;i++){
						if (offset[i]!=0) {
							if (iD[targetIn + offset[i]].alpha<EDGE_BORDER32){
								eg = TARGET_EDGE;
								break;
							}
						}
					}
					
				}
			}
			buf[targetOut] = eg;
			targetIn++;
			targetOut++;
		}
		targetIn += ae->in->offsetWidth();
		targetOut += ae->out->offsetWidth();
	}

	//さぁ、描画だ
	F_SRAND(prm->seed);	

	sputDrawPrm sdp;
	sdp.data	= ae->out->data();
	sdp.width	= ae->out->width();
	sdp.height	= ae->out->height();
	sdp.widthTrue	= ae->out->widthTrue();

	CRotCalc rp(ae->in_data);

	targetOut =0;
	for( y=0; y<h; y++){
		for( x=0; x<w; x++){
			if (buf[targetOut]>0){
				A_long rnd = FM_RAND((1L<<16));
				if (value > rnd){
					for (i=0; i<pValue;i++){
						//ばらつきを作る
						if (FM_RAND(5)==0){
							rp.SetRotLength((FM_RAND((360L<<4) -1)) <<12,FM_RAND(pLen * 3));
						}else{
							rp.SetRotLength((FM_RAND((360L<<4) -1)) <<12,FM_RAND(pLen));
						}
						sdp.x = x + (rp.x() );
						sdp.y = y + (rp.y() );
						
						PF_Fixed opa = prm->opa;
						sdp.opa = (PF_FpShort)FM_RAND(opa)/65536;
						sdp.sptIdx = prm->sputRandTable[FM_RAND(200-1)];
						colR = FM_RAND(colRnd);
						SputDataDraw32(&sdp,colors[colR]);
					}
				}
			}			
			targetOut++;
		}
		targetOut += ae->out->offsetWidth();
	}
	return err;
}
//-----------------------------------------------------------------------------
