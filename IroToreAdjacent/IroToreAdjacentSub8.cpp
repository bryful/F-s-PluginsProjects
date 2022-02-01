#include "IroToreAdjacent.h"

/*
コピー
まず主線部分の抽出検出はinDataで　alpha Maxが主線としてコピー

主線を消す。

残ったら適当な色で塗る


色変え・線のみがOFFなら終了

まず主線部分の抽出　をもう一回やる

色変えがOFFなら終了

色変え

ブラーがONならブラー

線のみがONなら終了

アルファーを元の状態へ修正

終了

*/
//################################################################################
inline void scanlineCopyH(ParamInfo *infoP, A_long y)
{
	//スキャンラインへコピー
	A_long v = y * infoP->wt;
	for ( int i=0; i<infoP->w; i++ ){
		infoP->scanline[i		      ] = infoP->data[i + v];
	}
}
//################################################################################
inline void scanlineCopyV(ParamInfo *infoP, A_long x)
{
	//スキャンラインへコピー
	A_long v = x;
	for ( int i=0; i<infoP->h; i++ ){
		infoP->scanline[i		      ] = infoP->data[v];
		v += infoP->wt;
	}
}
//################################################################################
inline PF_Boolean isMain(ParamInfo *infoP, PF_Pixel p)
{
	PF_Boolean ret = FALSE;
	for (A_long i=0; i<infoP->mnColorMax; i++){
		if (  ( F_ABS((A_long)infoP->mnColor[i].red   - (A_long)p.red  ) <= infoP->level)
			&&( F_ABS((A_long)infoP->mnColor[i].green - (A_long)p.green) <= infoP->level)
			&&( F_ABS((A_long)infoP->mnColor[i].blue  - (A_long)p.blue ) <= infoP->level)){
			ret = TRUE;
			break;
		}
	}
	return ret;
}
//################################################################################
inline PF_Boolean isAdj(ParamInfo *infoP, PF_Pixel p)
{
	PF_Boolean ret = FALSE;
	for (A_long i=0; i<infoP->adColorMax; i++){
		if (  ( F_ABS((A_long)infoP->adColor[i].red   - (A_long)p.red  ) <= infoP->level)
			&&( F_ABS((A_long)infoP->adColor[i].green - (A_long)p.green) <= infoP->level)
			&&( F_ABS((A_long)infoP->adColor[i].blue  - (A_long)p.blue ) <= infoP->level)){
			ret = TRUE;
			break;
		}
	}
	return ret;
}
//################################################################################
static PF_Err 
targetSelect (
	refconType		refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel8	*inP, 
	PF_Pixel8	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo *	niP		= reinterpret_cast<ParamInfo*>(refcon);

	if (isMain(niP,*inP)==TRUE){
		outP->alpha = TARGET_MAIN8;

	}else if (isAdj(niP,*inP)==TRUE){
		outP->alpha = TARGET_ADJ8;
	}else{
		outP->alpha = 0;
	}


	return err;
}
//################################################################################
static PF_Err 
targetAlphaRedo (
	refconType		refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel8	*inP, 
	PF_Pixel8	*outP)
{
	PF_Err			err = PF_Err_NONE;
	//ParamInfo *	niP		= reinterpret_cast<ParamInfo*>(refcon);

	outP->alpha = inP->alpha;

	return err;
}
//################################################################################
static PF_Err 
targetLineOnly (
	refconType		refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel8	*inP, 
	PF_Pixel8	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo *	niP		= reinterpret_cast<ParamInfo*>(refcon);

	if (isMain(niP,*inP)==TRUE){
		outP->alpha = PF_MAX_CHAN8;
	}else{
		outP->alpha = 0;
	}

	return err;
}
//################################################################################
//minMax
static PF_Err 
	targetMax (CFsAE *ae,ParamInfo *infoP)
{

	PF_Err			err = PF_Err_NONE;
	if (infoP->minmax==0) return err;

	A_long value2 = F_ABS(infoP->minmax);

	//まず横方向
	A_long now = 0;
	for ( A_long y=0; y<infoP->h;y++){
		scanlineCopyH(infoP,y);		
		for ( A_long x=0; x<infoP->w;x++){
			//ターゲットだ
			if (infoP->data[now].alpha ==TARGET_MAIN8) {
				PF_Boolean ok = FALSE;
				//左
				for (A_long i =1; i<=value2;i++){
					A_long xx = x - i;
					if (xx<0) break;
					A_long p = infoP->scanline[xx].alpha;
					if (p==TARGET_NEW8) {
						ok = TRUE;
						break;
					}else if (p!=TARGET_MAIN8){
						break;
					}
					
				}
				if (ok==FALSE){
					//右
					for (A_long i =1; i<=value2;i++){
						A_long xx = x + i;
						if (xx>=infoP->w) break;
						A_long p = infoP->scanline[xx].alpha;
						if (p==TARGET_NEW8) {
							ok = TRUE;
							break;
						}else if (p!=TARGET_MAIN8){
							break;
						}
					}
				}
				if (ok) {
					infoP->data[now] = infoP->newColor;
				}


			}
			now++;
		}
		now += infoP->offset;
	}
	//まず縦方向
	now = 0;
	for ( A_long x=0; x<infoP->w;x++){
		scanlineCopyV(infoP,x);
		now = x;
		for ( A_long y=0; y<infoP->h;y++){
			//ターゲットだ
			if (infoP->data[now].alpha ==TARGET_MAIN8) {
				PF_Boolean ok = FALSE;
				//左
				for (A_long i =1; i<=value2;i++){
					A_long yy = y - i;
					if (yy<0) break;
					A_long p = infoP->scanline[yy].alpha;
					if (p==TARGET_NEW8) {
						ok = TRUE;
						break;
					}else if (p!=TARGET_MAIN8){
						break;
					}
					
				}
				if (ok==FALSE){
					//右
					for (A_long i =1; i<=value2;i++){
						A_long yy = y + i;
						if (yy>=infoP->h) break;
						A_long p = infoP->scanline[yy].alpha;
						if (p==TARGET_NEW8) {
							ok = TRUE;
							break;
						}else if (p!=TARGET_MAIN8){
							break;
						}
					}
				}
				if (ok) {
					infoP->data[now] = infoP->newColor;
				}


			}
			now+= infoP->wt;
		}
	}
	return err;

}
//################################################################################
//minMax
static PF_Err 
	targetMin (CFsAE *ae,ParamInfo *infoP)
{

	PF_Err			err = PF_Err_NONE;
	if (infoP->minmax==0) return err;

	A_long value2 = F_ABS(infoP->minmax);

	//まず横方向
	A_long now = 0;
	for ( A_long y=0; y<infoP->h;y++){
		scanlineCopyH(infoP,y);		
		for ( A_long x=0; x<infoP->w;x++){
			//ターゲットだ
			if (infoP->data[now].alpha ==TARGET_NEW8) {
				PF_Boolean ok = FALSE;
				//左
				for (A_long i =1; i<=value2;i++){
					A_long xx = x - i;
					if (xx<0) break;
					A_long p = infoP->scanline[xx].alpha;
					if (p==TARGET_MAIN8) {
						ok = TRUE;
						break;
					}else if (p!=TARGET_NEW8){
						break;
					}
					
				}
				if (ok==FALSE){
					//右
					for (A_long i =1; i<=value2;i++){
						A_long xx = x + i;
						if (xx>=infoP->w) break;
						A_long p = infoP->scanline[xx].alpha;
						if (p==TARGET_MAIN8) {
							ok = TRUE;
							break;
						}else if (p!=TARGET_NEW8){
							break;
						}
					}
				}
				if (ok) {
					infoP->data[now] = infoP->mnColor[0];
				}


			}
			now++;
		}
		now += infoP->offset;
	}
	//まず縦方向
	now = 0;
	for ( A_long x=0; x<infoP->w;x++){
		scanlineCopyV(infoP,x);
		now = x;
		for ( A_long y=0; y<infoP->h;y++){
			//ターゲットだ
			if (infoP->data[now].alpha ==TARGET_NEW8) {
				PF_Boolean ok = FALSE;
				//左
				for (A_long i =1; i<=value2;i++){
					A_long yy = y - i;
					if (yy<0) break;
					A_long p = infoP->scanline[yy].alpha;
					if (p==TARGET_MAIN8) {
						ok = TRUE;
						break;
					}else if (p!=TARGET_NEW8){
						break;
					}
					
				}
				if (ok==FALSE){
					//右
					for (A_long i =1; i<=value2;i++){
						A_long yy = y + i;
						if (yy>=infoP->h) break;
						A_long p = infoP->scanline[yy].alpha;
						if (p==TARGET_MAIN8) {
							ok = TRUE;
							break;
						}else if (p!=TARGET_NEW8){
							break;
						}
					}
				}
				if (ok) {
					infoP->data[now] = infoP->mnColor[0];
				}


			}
			now+= infoP->wt;
		}
	}
	return err;

}


//################################################################################
//上下左右をチェック
static PF_Err 
	targetRemove (CFsAE *ae,ParamInfo *infoP)
{

	PF_Err			err = PF_Err_NONE;
	A_long now = 0;
	//まず左右
	for ( A_long y=0; y<infoP->h;y++){
		scanlineCopyH(infoP,y);		
		for ( A_long x=0; x<infoP->w;x++){
			if (infoP->data[now].alpha == TARGET_MAIN8)
			{
				PF_Boolean on = FALSE;
				A_long a = 0;
				if (x>0){
					a = infoP->scanline[x-1].alpha;
					if ( ( a == TARGET_ADJ8)||(a==TARGET_NEW8)){
						on =TRUE;
					}
				}
				if (on==FALSE) {
					if (x<infoP->w-1){
					a = infoP->scanline[x+1].alpha;
						if ( ( a == TARGET_ADJ8)||(a==TARGET_NEW8)){
							on =TRUE;
						}
					}
				}
				if (on==TRUE){
					infoP->data[now] = infoP->newColor;
				}
			}
			now++;
		}
		now += infoP->offset;
	}
	//次は上下
	for ( A_long x=0; x<infoP->w;x++){
		now = x;
		scanlineCopyV(infoP,x);		
		for ( A_long y=0; y<infoP->h;y++){
			if (infoP->data[now].alpha == TARGET_MAIN8)
			{
				PF_Boolean on = FALSE;
				A_long a = 0;
				if (y>0){
					a = infoP->scanline[y-1].alpha;
					if ( ( a == TARGET_ADJ8)||(a==TARGET_NEW8)){
						on =TRUE;
					}
				}
				if (on==FALSE) {
					if (y<infoP->h-1){
					a = infoP->scanline[y+1].alpha;
						if ( ( a == TARGET_ADJ8)||(a==TARGET_NEW8)){
							on =TRUE;
						}
					}
				}
				if (on==TRUE){
					infoP->data[now] = infoP->newColor;
				}
			}
			now += infoP->wt;
		}
	}
	return err;

}
//################################################################################
//################################################################################
//主線だけぼかす
static PF_Err 
	targetBlur (CFsAE *ae,ParamInfo *infoP)
{
	PF_Err			err = PF_Err_NONE;
	A_long v = infoP->blur;
	if (v<=0) return err;
	PF_Pixel p;

	A_long now = 0;
	//まず横方向
	for ( A_long y=0; y<infoP->h;y++){
		scanlineCopyH(infoP,y);
		for ( A_long x=0; x<infoP->w;x++){
			if (infoP->data[now].alpha ==TARGET_MAIN8) {
				A_long r = 0;
				A_long g = 0;
				A_long b = 0;
				A_long cnt =0;
				//中央
				r = (A_long)infoP->scanline[x].red;
				g = (A_long)infoP->scanline[x].green;
				b = (A_long)infoP->scanline[x].blue;
				cnt = 1;
				//まず右
				for ( A_long i=1; i<=v;i++){
					A_long xx = x-i;
					if (xx<0) break;
					p = infoP->scanline[xx];
					if (p.alpha != TARGET_MAIN8)break;
					r += (A_long)p.red;
					g += (A_long)p.green;
					b += (A_long)p.blue;
					cnt++;
				}
				//次は左
				for ( A_long i=1; i<=v;i++){
					A_long xx = x+i;
					if (xx>=infoP->w) break;
					p = infoP->scanline[xx];
					if (p.alpha != TARGET_MAIN8) break;
					r += (A_long)p.red;
					g += (A_long)p.green;
					b += (A_long)p.blue;
					cnt++;
				}
				if (cnt>1){
					p.red   = RoundByteFpLong(((double)r / (double)cnt)+0.5); 
					p.green = RoundByteFpLong(((double)g / (double)cnt)+0.5); 
					p.blue  = RoundByteFpLong(((double)b / (double)cnt)+0.5); 
					p.alpha = TARGET_MAIN8;
					infoP->data[now] = p;
				}
			}
			now++;
		}
		now += infoP->offset;
	}
	//次は縦方向
	for ( A_long x=0; x<infoP->w;x++){
		scanlineCopyV(infoP,x);
		now = x;
		for ( A_long y=0; y<infoP->h;y++){
			if (infoP->data[now].alpha ==TARGET_MAIN8) {
				A_long r = 0;
				A_long g = 0;
				A_long b = 0;
				A_long cnt =0;
				//中央
				r = (A_long)infoP->scanline[y].red;
				g = (A_long)infoP->scanline[y].green;
				b = (A_long)infoP->scanline[y].blue;
				cnt = 1;
				//まず上
				for ( A_long i=1; i<=v;i++){
					A_long yy = y-i;
					if (yy<0) break;
					p = infoP->scanline[yy];
					if (p.alpha != TARGET_MAIN8) break;
					r += (A_long)p.red;
					g += (A_long)p.green;
					b += (A_long)p.blue;
					cnt++;
				}
				//次は下
				for ( A_long i=1; i<=v;i++){
					A_long yy = y+i;
					if (yy>=infoP->h) break;
					p = infoP->scanline[yy];
					if (p.alpha != TARGET_MAIN8) break;
					r += (A_long)p.red;
					g += (A_long)p.green;
					b += (A_long)p.blue;
					cnt++;
				}
				if (cnt>1){
					p.red   = RoundByteFpLong(((double)r / (double)cnt)+0.5); 
					p.green = RoundByteFpLong(((double)g / (double)cnt)+0.5); 
					p.blue  = RoundByteFpLong(((double)b / (double)cnt)+0.5); 
					p.alpha = TARGET_MAIN8;
					infoP->data[now] = p;
				}
			}
			now+= infoP->wt;
		}
	}
	//股横方向
	v = v/2;
	if (v<=0) return err;

	now = 0;
	for ( A_long y=0; y<infoP->h;y++){
		scanlineCopyH(infoP,y);
		for ( A_long x=0; x<infoP->w;x++){
			if (infoP->data[now].alpha ==TARGET_MAIN8) {
				A_long r = 0;
				A_long g = 0;
				A_long b = 0;
				A_long cnt =0;
				//中央
				r = (A_long)infoP->scanline[x].red;
				g = (A_long)infoP->scanline[x].green;
				b = (A_long)infoP->scanline[x].blue;
				cnt = 1;
				//まず右
				for ( A_long i=1; i<=v;i++){
					A_long xx = x-i;
					if (xx<0) break;
					p = infoP->scanline[xx];
					if (p.alpha != TARGET_MAIN8)break;
					r += (A_long)p.red;
					g += (A_long)p.green;
					b += (A_long)p.blue;
					cnt++;
				}
				//次は左
				for ( A_long i=1; i<=v;i++){
					A_long xx = x+i;
					if (xx>=infoP->w) break;
					p = infoP->scanline[xx];
					if (p.alpha != TARGET_MAIN8) break;
					r += (A_long)p.red;
					g += (A_long)p.green;
					b += (A_long)p.blue;
					cnt++;
				}
				if (cnt>1){
					p.red   = RoundByteFpLong(((double)r / (double)cnt)+0.5); 
					p.green = RoundByteFpLong(((double)g / (double)cnt)+0.5); 
					p.blue  = RoundByteFpLong(((double)b / (double)cnt)+0.5); 
					p.alpha = TARGET_MAIN8;
					infoP->data[now] = p;
				}
			}
			now++;
		}
		now += infoP->offset;
	}
	return err;
}	
//*****************************************************************************
PF_Err irotoreExec8(CFsAE *ae , ParamInfo *infoP)
{
	PF_Err	err = PF_Err_NONE;
	if (infoP->value<=0) return err;

	//パラメータを作成
	infoP->data = (PF_Pixel *)ae->out->data();
	infoP->w = ae->out->width();
	infoP->wt = ae->out->widthTrue();
	infoP->wt2 = ae->out->widthTrue() *2;
	infoP->h = ae->out->height();
	infoP->offset = ae->out->offsetWidth();

	//主線を抽出
	ERR(ae->iterate8((refconType)infoP,targetSelect));

	//主線を消す
	//メモリの確保。念のため大きめにとる
	A_long mm = infoP->wt;
	if (mm < infoP->h) mm = infoP->h; 
	infoP->scanlineH = ae->NewHandle(mm * sizeof(PF_Pixel) * 2);// 1Line分　と余裕
	if ( !infoP->scanlineH ) return PF_Err_INTERNAL_STRUCT_DAMAGED;
	infoP->scanline = *(PF_Pixel8**)infoP->scanlineH; ;

	for ( A_long i=0; i<infoP->value; i++){
		targetRemove(ae,infoP);
	}

	if (infoP->minmax>0){
		targetMax(ae,infoP);
		
	}else if (infoP->minmax<0){
		targetMin(ae,infoP);
	}
	if (infoP->blur>0)
	{
		ERR(ae->iterate8((refconType)infoP,targetLineOnly));
		targetBlur(ae,infoP);
	}

	//主線部分を抜き出す
	if (infoP->lineOnly){
		//if (loFlag==TRUE){
			ERR(ae->iterate8((refconType)infoP,targetLineOnly));
		//}
	}else{
		ERR(ae->iterate8((refconType)infoP,targetAlphaRedo));
	}
	/*
	//主線抜き出しやり直し
	PF_Boolean loFlag = TRUE;
	if ( (infoP->blur>0)
		||(infoP->minmax!=0)
		)
	{
		loFlag = FALSE;
	}
	if (infoP->minmax!=0){
		targetMinMax(ae,infoP);
	}
	
	//ブラーの処理
	*/
	//メモリの開放
	if (infoP->scanlineH != NULL){
		ae->DisposeHandle(infoP->scanlineH);
		infoP->scanlineH = NULL;
	}
	return err;
}
