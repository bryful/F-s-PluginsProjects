//-----------------------------------------------------------------------------
/*
	AEP Project用のプラグイン
	sputteringAlpha
	描画のみ
*/
//-----------------------------------------------------------------------------
#include "sputteringAlpha.h"
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
PF_Err MainRender32 (CFsAE *ae, ParamInfo *prm)
{
	PF_Err			err = PF_Err_NONE;

	PF_Fixed value = prm->value /100;
	PF_Fixed eValue = prm->edge_value /10;
	A_long pValue = prm->point_value;
	PF_Fixed pLen = prm->point_length; //値は整数
	A_long colRnd = prm->color_max -1;
	A_long colR;
	A_long i;
	A_long x,y;
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
	A_long we = ae->in->width()-1;
	A_long he = ae->in->height()-1;
	A_long offset[8];
	//テーブル作成
	offset[0] = -1 - ae->in->widthTrue(); 
	offset[1] =  0 - ae->in->widthTrue(); 
	offset[2] =  1 - ae->in->widthTrue(); 
	offset[3] = -1 ; 
	offset[4] =  1 ; 
	offset[5] = -1 + ae->in->widthTrue(); 
	offset[6] =  0 + ae->in->widthTrue(); 
	offset[7] =  1 + ae->in->widthTrue(); 
	/*
	透明なところは０
	エッジはTARGET_EDGE8
	普通なところはTARGET_SOLID8
	をbufに書き込む
	*/
	A_u_char eg = 0;
	for( y=0; y<ae->out->height(); y++){
		for( x=0; x<ae->out->width(); x++){
			c = iD[targetIn];
			eg = 0;
			if (c.alpha>=EDGE_BORDER32){
				eg = TARGET_SOLID;
				if ( (x>=1)&&(x<we)&&(y>=1)&&(y<he) ){ //端っこは無視
					for (i=0; i<=7;i++){
						if (iD[targetIn + offset[i]].alpha<EDGE_BORDER32){
							eg = TARGET_EDGE;
							break;
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

	CRotCalc rc(ae->in_data);

	targetOut =0;
	A_long xx,yy;
	for( y=0; y<ae->out->height(); y++){
		yy = y;
		for( x=0; x<ae->out->width(); x++){
			xx = x;
			A_u_char flg = buf[targetOut];
			A_long rnd = FM_RAND((1L<<16));
			A_long rndScale =1;
			if (flg == TARGET_SOLID){
				if (value < rnd) { flg = 0;}
				rndScale = 5;
			}else if (flg == TARGET_EDGE) {
				if (eValue < rnd) { flg = 0;}
				rndScale = 3;
			}else{
				flg = 0;
			}
			if (flg>0){
				for (i=0; i<pValue;i++){
					//ばらつきを作る
					if (FM_RAND(5)==0){
						rc.SetRotLength((FM_RAND((360L<<4) -1)) <<12,FM_RAND(pLen * rndScale));
					}else{
						rc.SetRotLength((FM_RAND((360L<<4) -1)) <<12,FM_RAND(pLen));
					}
					sdp.x = x + (rc.pos().x );
					sdp.y = y + (rc.pos().y );
					
					PF_Fixed opa = prm->opa;
					sdp.opa = (PF_FpShort)FM_RAND(opa)/65536;
					sdp.sptIdx = prm->sputRandTable[FM_RAND(200-1)];
					colR = FM_RAND(colRnd);
					SputDataDraw32(&sdp,colors[colR]);
				}
			}			
			targetOut++;
		}
		targetOut += ae->out->offsetWidth();
	}
	return err;
}
//-----------------------------------------------------------------------------
