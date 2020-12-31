#include "IroTore.h"

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
inline PF_FpLong pixelValue(PF_PixelFloat p)
{
	return ( ( 0.29891 * (PF_FpLong)p.red) + ( 0.58661 * (PF_FpLong)p.green) + ( 0.11448 * (PF_FpLong)p.blue));
}
//################################################################################
inline void scanlineCopy3(ParamInfo32 *infoP, A_long y)
{
	//スキャンラインへコピー
	A_long v = 0; 
	if (y ==0) {
		for ( int i=0; i<infoP->w; i++ ){
			infoP->scanline[i		      ] = infoP->data[i ];
			infoP->scanline[i + infoP->wt ] = infoP->data[i ];
			infoP->scanline[i + infoP->wt2] = infoP->data[i + infoP->wt]; 
		}
	}else{
		for ( int i=0; i<infoP->w; i++ ){
			infoP->scanline[i             ] = infoP->scanline[i + infoP->wt ];
			infoP->scanline[i + infoP->wt ] = infoP->scanline[i + infoP->wt2];
		}
		if (y<infoP->h-1 ){
			v = (y+1)*infoP->wt; 
			for ( int i=0; i<infoP->w; i++ ){
				infoP->scanline[i + infoP->wt2] = infoP->data[i + v]; 
			}
		}
	}
}
//################################################################################
inline void scanlineCopyH(ParamInfo32 *infoP, A_long y)
{
	//スキャンラインへコピー
	A_long v = y * infoP->wt;
	for ( int i=0; i<infoP->w; i++ ){
		infoP->scanline[i		      ] = infoP->data[i + v];
	}
}
//################################################################################
inline void scanlineCopyV(ParamInfo32 *infoP, A_long x)
{
	//スキャンラインへコピー
	A_long v = x;
	for ( int i=0; i<infoP->h; i++ ){
		infoP->scanline[i		      ] = infoP->data[v];
		v += infoP->wt;
	}
}
//################################################################################
inline PF_PixelFloat getScanLine(ParamInfo32 *infoP, A_long offsetX, A_long offsetY)
{
	A_long xx = infoP->nowX + offsetX;
	A_long yy = 1 + offsetY;
	if (xx<0) {
		xx =0;
	} else if (xx>=infoP->w) {
		xx = infoP->w -1;
	}
	if (yy<0) {
		yy =0;
	} else if (yy>2) {
		yy= 2;
	}
	 
	 return infoP->scanline[ xx + yy*infoP->wt];
}
//################################################################################
inline PF_Boolean isTarget(ParamInfo32 *infoP, PF_PixelFloat p)
{
	PF_Boolean ret = FALSE;
	PF_Pixel pp = CONV32TO8(p);
	for (A_long i=0; i<infoP->colorMax; i++){
		if (  ( F_ABS((A_long)infoP->color[i].red   - (A_long)pp.red  ) <= infoP->level)
			&&( F_ABS((A_long)infoP->color[i].green - (A_long)pp.green) <= infoP->level)
			&&( F_ABS((A_long)infoP->color[i].blue  - (A_long)pp.blue ) <= infoP->level)){
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
	PF_PixelFloat	*inP, 
	PF_PixelFloat	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo32 *	niP		= reinterpret_cast<ParamInfo32*>(refcon);

	if (isTarget(niP,*inP)==TRUE){
		outP->alpha = PF_MAX_CHAN32;
	}else{
		if ((inP->alpha==0) || ( (inP->blue>=PF_MAX_CHAN32)&&(inP->green>=PF_MAX_CHAN32)&&(inP->red>=PF_MAX_CHAN32))){
			outP->alpha = 0;
		}else{
			outP->alpha = PF_HALF_CHAN32;
		}
	}


	return err;
}
//################################################################################
static PF_Err 
targetAlphaRedo (
	refconType		refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_PixelFloat	*inP, 
	PF_PixelFloat	*outP)
{
	PF_Err			err = PF_Err_NONE;

	outP->alpha = inP->alpha;

	return err;
}
//################################################################################
static PF_Err 
targetLineOnly (
	refconType		refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_PixelFloat	*inP, 
	PF_PixelFloat	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo32 *	niP		= reinterpret_cast<ParamInfo32*>(refcon);

	if (isTarget(niP,*inP)==TRUE){
		outP->alpha = PF_MAX_CHAN32;
	}else{
		outP->alpha = 0;
	}

	return err;
}
//################################################################################
static PF_Err 
targetColor (
	refconType		refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_PixelFloat	*inP, 
	PF_PixelFloat	*outP)
{
	PF_Err			err = PF_Err_NONE;
	if (outP->alpha != PF_MAX_CHAN32 ) return err;

	ParamInfo32 *	niP		= reinterpret_cast<ParamInfo32*>(refcon);

	PF_PixelFloat p =*outP;
	if (p.red>1.0) p.red = 1.0;
	if (p.green>1.0) p.green = 1.0;
	if (p.blue>1.0) p.blue = 1.0;

	HLSA hls = RGBtoHLS(p);

	if (niP->hue>0) {
		hls.H = hls.H + niP->hue;
		if (hls.H>1.0) hls.H -= 1.0;
		else if (hls.H<0.0) hls.H += 1.0;
	}
	if (niP->sat>0) {
		hls.S += (1.0 - hls.S) * niP->sat; 
	}else if (niP->sat<0){
		hls.S += hls.S * niP->sat; 
	}
	if (niP->light>0) {
		hls.L += (PF_FpShort)( (1.0 - hls.L) * niP->light); 
	}else if (niP->light<0){
		hls.L += (PF_FpShort)(hls.L * niP->light); 
	}
	p = HLStoRGB(hls);

	if (niP->red>0) {
		p.red += (PF_FpShort)((1.0 - p.red) * niP->red); 
	}else if (niP->red<0) {
		p.red += (PF_FpShort)(p.red * niP->red); 
	}
	if (niP->green>0) {
		p.green += (PF_FpShort)((1.0 - p.green) * niP->green); 
	}else if (niP->green<0) {
		p.green += (PF_FpShort)(p.green * niP->green); 
	}
	if (niP->blue>0) {
		p.blue += (PF_FpShort)((1.0 - p.blue) * niP->blue); 
	}else if (niP->blue<0) {
		p.blue += (PF_FpShort)(p.blue * niP->blue); 
	}
	p.alpha = 1.0;
	*outP = p;

	return err;
}
//################################################################################
//minMax
static PF_Err 
	targetMinMax (CFsAE *ae,ParamInfo32 *infoP,A_long value)
{

	PF_Err			err = PF_Err_NONE;
	if (value==0) return err;

	PF_Boolean maxF = (value>0);
	A_long value2 = F_ABS(value);

	//まず横方向
	A_long now = 0;
	for ( A_long y=0; y<infoP->h;y++){
		scanlineCopyH(infoP,y);		
		for ( A_long x=0; x<infoP->w;x++){
			//ターゲットだ
			if (infoP->data[now].alpha == PF_MAX_CHAN32) {
				PF_PixelFloat target = infoP->data[now];
				double v = pixelValue(target);
				PF_Boolean ok = FALSE;
				//左
				for (A_long i =1; i<=value2;i++){
					A_long xx = x - i;
					if (xx<0) break;
					PF_PixelFloat p = infoP->scanline[xx];
					if (p.alpha!=PF_MAX_CHAN32)break;
					double v2 = pixelValue(p);
					if (maxF) {
						if (v<v2) {
							v = v2;
							target = p;
							ok = TRUE;
						}
					}else{
						if (v>v2) {
							v = v2;
							target = p;
							ok = TRUE;
						}
					}
				}
				//右
				for (A_long i =1; i<=value2;i++){
					A_long xx = x + i;
					if (xx>=infoP->w) break;
					PF_PixelFloat p = infoP->scanline[xx];
					if (p.alpha!=PF_MAX_CHAN32)break;
					double v2 = pixelValue(p);
					if (maxF) {
						if (v<v2) {
							v = v2;
							target = p;
							ok = TRUE;
						}
					}else{
						if (v>v2) {
							v = v2;
							target = p;
							ok = TRUE;
						}
					}
				}
				if (ok) {
					infoP->data[now] = target;
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
			if (infoP->data[now].alpha ==PF_MAX_CHAN32) {
				PF_PixelFloat target = infoP->data[now];
				double v = pixelValue(target);
				PF_Boolean ok = FALSE;
				//上
				for (A_long i =1; i<=value2;i++){
					A_long yy = y - i;
					if (yy<0) break;
					PF_PixelFloat p = infoP->scanline[yy];
					if (p.alpha!=PF_MAX_CHAN32)break;
					double v2 = pixelValue(p);
					if (maxF) {
						if (v<v2) {
							v = v2;
							target = p;
							ok = TRUE;
						}
					}else{
						if (v>v2) {
							v = v2;
							target = p;
							ok = TRUE;
						}
					}
				}
				//下
				for (A_long i =1; i<=value2;i++){
					A_long yy = y + i;
					if (yy>=infoP->h) break;
					PF_PixelFloat p = infoP->scanline[yy];
					if (p.alpha!=PF_MAX_CHAN32)break;
					double v2 = pixelValue(p);
					if (maxF) {
						if (v<v2) {
							v = v2;
							target = p;
							ok = TRUE;
						}
					}else{
						if (v>v2) {
							v = v2;
							target = p;
							ok = TRUE;
						}
					}
				}
				if (ok==TRUE) {
					infoP->data[now] = target;
				}


			}
			now+= infoP->wt;
		}
	}
	return err;

}

//################################################################################
static A_long 
targetCount (CFsAE *ae)
{
	A_long ret = 0;
	PF_PixelFloat *data;
	data = (PF_PixelFloat *)ae->out->data();
	A_long w = ae->out->width();
	A_long h = ae->out->height();
	A_long os = ae->out->offsetWidth();

	A_long now = 0;
	for ( A_long y=0; y<h;y++){
		for ( A_long x=0; x<w;x++){
			if (data[now].alpha ==PF_MAX_CHAN32) {
				ret++;
			}
			now++;
		}
		now += os;
	}
	return ret;

}
//################################################################################
#define	CHK_PX32(X,Y)	do {			\
	if (v>0) {							\
	subpx = getScanLine(infoP,X,Y);	\
	if (subpx.alpha == PF_HALF_CHAN32){	\
		double v2 = ( ( 0.29891 * (PF_FpLong)subpx.red) + ( 0.58661 * (PF_FpLong)subpx.green) + ( 0.11448 * (PF_FpLong)subpx.blue) +0.5);	\
		if (v>v2) {					\
			v = v2;					\
			target = subpx;			\
			cnt++;					\
		}							\
	}								\
}									\
} while (0)


//################################################################################
//上下左右をチェック
static PF_Err 
	targetRemove (CFsAE *ae,ParamInfo32 *infoP)
{

	PF_Err			err = PF_Err_NONE;

	A_long now = 0;
	for ( A_long y=0; y<infoP->h;y++){
		infoP->nowY = y;
		scanlineCopy3(infoP,y);		
		for ( A_long x=0; x<infoP->w;x++){
			infoP->nowX = x;
			//ターゲットだ
			if (infoP->data[now].alpha ==PF_MAX_CHAN32) {
				PF_PixelFloat subpx,target;
				double v = PF_MAX_CHAN32;
				A_long cnt=0;
				CHK_PX32( 0,-1);
				CHK_PX32( 1, 0);
				CHK_PX32( 0, 1);
				CHK_PX32(-1,0);
				if (cnt>0) {
					infoP->data[now] = target;
					infoP->data[now].alpha = PF_HALF_CHAN32;
					infoP->targetCount--;
					if (infoP->targetCount<=0){
						return err;
					}
				}
			}
			now++;
		}
		now += infoP->offset;
	}
	return err;

}
//################################################################################
//斜め方向をチェック
static PF_Err 
	targetRemove2nd (CFsAE *ae,ParamInfo32 *infoP)
{

	PF_Err			err = PF_Err_NONE;

	A_long now = 0;
	for ( A_long y=0; y<infoP->h;y++){
		infoP->nowY = y;
		scanlineCopy3(infoP,y);		
		for ( A_long x=0; x<infoP->w;x++){
			infoP->nowX = x;
			//ターゲットだ
			if (infoP->data[now].alpha ==PF_MAX_CHAN32) {
				PF_PixelFloat subpx,target;
				double v=PF_MAX_CHAN32;
				A_long cnt=0;
				CHK_PX32(-1,-1);
				CHK_PX32( 1,-1);
				CHK_PX32(-1, 1);
				CHK_PX32( 1, 1);
				if (cnt>0) {
					infoP->data[now] = target;
					infoP->data[now].alpha = PF_HALF_CHAN32;
					infoP->targetCount--;
					if (infoP->targetCount<=0){
						return err;
					}
				}
			}
			now++;
		}
		now += infoP->offset;
	}
	return err;

}
//################################################################################
//################################################################################
//問答無用で塗りつぶす
static PF_Err 
	targetRemoveMax (CFsAE *ae,ParamInfo32 *infoP)
{

	PF_Err			err = PF_Err_NONE;
	A_long now = 0;
	for ( A_long y=0; y<infoP->h;y++){
		for ( A_long x=0; x<infoP->w;x++){
			if (infoP->data[now].alpha ==PF_MAX_CHAN32) {
				infoP->data[now] = infoP->igColor;
				infoP->targetCount--;
					if (infoP->targetCount<=0){
						return err;
					}

			}
			now++;
		}
		now += infoP->offset;
	}
	return err;
	
}
//################################################################################
//################################################################################
//主線だけぼかす
static PF_Err 
	targetBlur (CFsAE *ae,ParamInfo32 *infoP)
{
	PF_Err			err = PF_Err_NONE;
	A_long v = infoP->blur;
	if (v<=0) return err;
	PF_PixelFloat p;

	A_long now = 0;
	//まず横方向
	for ( A_long y=0; y<infoP->h;y++){
		scanlineCopyH(infoP,y);
		for ( A_long x=0; x<infoP->w;x++){
			if (infoP->data[now].alpha ==PF_MAX_CHAN32) {
				double r = 0;
				double g = 0;
				double b = 0;
				A_long cnt =0;
				//中央
				r = (double)infoP->scanline[x].red;
				g = (double)infoP->scanline[x].green;
				b = (double)infoP->scanline[x].blue;
				cnt = 1;
				//まず右
				for ( A_long i=1; i<=v;i++){
					A_long xx = x-i;
					if (xx<0) break;
					p = infoP->scanline[xx];
					if (p.alpha != PF_MAX_CHAN32)break;
					r += (double)p.red;
					g += (double)p.green;
					b += (double)p.blue;
					cnt++;
				}
				//次は左
				for ( A_long i=1; i<=v;i++){
					A_long xx = x+i;
					if (xx>=infoP->w) break;
					p = infoP->scanline[xx];
					if (p.alpha != PF_MAX_CHAN32) break;
					r += (double)p.red;
					g += (double)p.green;
					b += (double)p.blue;
					cnt++;
				}
				if (cnt>1){
					p.red   = RoundFpShortDouble(((double)r / (double)cnt)); 
					p.green = RoundFpShortDouble(((double)g / (double)cnt)); 
					p.blue  = RoundFpShortDouble(((double)b / (double)cnt)); 
					p.alpha = PF_MAX_CHAN32;
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
			if (infoP->data[now].alpha ==PF_MAX_CHAN32) {
				double r = 0;
				double g = 0;
				double b = 0;
				double cnt =0;
				//中央
				r = (double)infoP->scanline[y].red;
				g = (double)infoP->scanline[y].green;
				b = (double)infoP->scanline[y].blue;
				cnt = 1;
				//まず上
				for ( A_long i=1; i<=v;i++){
					A_long yy = y-i;
					if (yy<0) break;
					p = infoP->scanline[yy];
					if (p.alpha != PF_MAX_CHAN32) break;
					r += (double)p.red;
					g += (double)p.green;
					b += (double)p.blue;
					cnt++;
				}
				//次は下
				for ( A_long i=1; i<=v;i++){
					A_long yy = y+i;
					if (yy>=infoP->h) break;
					p = infoP->scanline[yy];
					if (p.alpha != PF_MAX_CHAN32) break;
					r += (double)p.red;
					g += (double)p.green;
					b += (double)p.blue;
					cnt++;
				}
				if (cnt>1){
					p.red   = RoundFpShortDouble(((double)r / (double)cnt)); 
					p.green = RoundFpShortDouble(((double)g / (double)cnt)); 
					p.blue  = RoundFpShortDouble(((double)b / (double)cnt)); 
					p.alpha = PF_MAX_CHAN32;
					infoP->data[now] = p;
				}
			}
			now+= infoP->wt;
		}
	}
	//また横方向。数値は半分
	v = v/2;
	if (v<=0) return err;
	now = 0;
	for ( A_long y=0; y<infoP->h;y++){
		scanlineCopyH(infoP,y);
		for ( A_long x=0; x<infoP->w;x++){
			if (infoP->data[now].alpha ==PF_MAX_CHAN32) {
				double r = 0;
				double g = 0;
				double b = 0;
				A_long cnt =0;
				//中央
				r = (double)infoP->scanline[x].red;
				g = (double)infoP->scanline[x].green;
				b = (double)infoP->scanline[x].blue;
				cnt = 1;
				//まず右
				for ( A_long i=1; i<=v;i++){
					A_long xx = x-i;
					if (xx<0) break;
					p = infoP->scanline[xx];
					if (p.alpha != PF_MAX_CHAN32)break;
					r += (double)p.red;
					g += (double)p.green;
					b += (double)p.blue;
					cnt++;
				}
				//次は左
				for ( A_long i=1; i<=v;i++){
					A_long xx = x+i;
					if (xx>=infoP->w) break;
					p = infoP->scanline[xx];
					if (p.alpha != PF_MAX_CHAN32) break;
					r += (double)p.red;
					g += (double)p.green;
					b += (double)p.blue;
					cnt++;
				}
				if (cnt>1){
					p.red   = RoundFpShortDouble(((double)r / (double)cnt)); 
					p.green = RoundFpShortDouble(((double)g / (double)cnt)); 
					p.blue  = RoundFpShortDouble(((double)b / (double)cnt)); 
					p.alpha = PF_MAX_CHAN32;
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
PF_Err irotoreExec32(CFsAE *ae , ParamInfo32 *infoP)
{
	PF_Err	err = PF_Err_NONE;

	//パラメータを作成
	infoP->data = (PF_PixelFloat *)ae->out->data();
	infoP->w = ae->out->width();
	infoP->wt = ae->out->widthTrue();
	infoP->wt2 = ae->out->widthTrue() *2;
	infoP->h = ae->out->height();
	infoP->offset = ae->out->offsetWidth();

	//主線を抽出
	ERR(ae->iterate32((refconType)infoP,targetSelect));
	
	//主線のピクセル数を数える
	infoP->targetCount = targetCount(ae);
	//0なら終わる
	if (infoP->targetCount<=0) {
		if (infoP->lineOnly==TRUE){
			ae->out->clear();
		}else{
			ae->CopyInToOut();
		}
		return err;
	}
	//主線を消す
	//メモリの確保。念のため大きめにとる
	A_long mm = infoP->wt;
	if (mm < infoP->h) mm = infoP->h; 
	infoP->scanlineH = ae->NewHandle(mm * sizeof(PF_PixelFloat) * 4);// 3Line分　と余裕
	if ( !infoP->scanlineH ) return PF_Err_INTERNAL_STRUCT_DAMAGED;
	infoP->scanline = *(PF_PixelFloat**)infoP->scanlineH; ;

	//主線部分がなくなるまで繰り返す。
	while(infoP->targetCount>0)
	{
		A_long bk = infoP->targetCount;
		targetRemove(ae,infoP);
		//塗りつぶせなかったら斜め方向検出
		if (infoP->targetCount == bk) {
			targetRemove2nd(ae,infoP);
			if (infoP->targetCount == bk) {
				//諦める
				targetRemoveMax(ae,infoP);
			}
		}
	}
	//主線抜き出しやり直し
	PF_Boolean loFlag = TRUE;
	if ( (infoP->blur>0)
		||(infoP->hue!=0)||(infoP->sat!=0)||(infoP->light!=0)||(infoP->red!=0)||(infoP->green!=0)||(infoP->blue!=0)
		||(infoP->minmax1!=0)||(infoP->minmax2!=0)
		)
	{
		ERR(ae->iterate32((refconType)infoP,targetLineOnly));
		loFlag = FALSE;
	}
	if (infoP->minmax1!=0){
		targetMinMax(ae,infoP,infoP->minmax1);
	}
	if (infoP->minmax2!=0){
		targetMinMax(ae,infoP,infoP->minmax2);
	}

	//ブラーの処理
	if (infoP->blur>0)
	{
		targetBlur(ae,infoP);
	}
	//色変え
	if ( (infoP->hue!=0)||(infoP->sat!=0)||(infoP->light!=0)||(infoP->red!=0)||(infoP->green!=0)||(infoP->blue!=0))
	{
		ERR(ae->iterate32((refconType)infoP,targetColor));
	}

	//主線部分を抜き出す
	if (infoP->lineOnly){
		if (loFlag==TRUE){
			ERR(ae->iterate32((refconType)infoP,targetLineOnly));
		}
	}else{
		ERR(ae->iterate32((refconType)infoP,targetAlphaRedo));
	}

	//メモリの開放
	if (infoP->scanlineH != NULL){
		ae->DisposeHandle(infoP->scanlineH);
		infoP->scanlineH = NULL;
	}
	return err;
}
