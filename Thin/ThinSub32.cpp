#include "Thin.h"


//*****************************************************************************
inline PF_Boolean CompPx(thinInfo32 *ti, PF_PixelFloat p)
{
	PF_Boolean ret = FALSE;
	PF_Pixel p2 = CONV32TO8(p);
	for (A_long i=0; i<ti->colorMax; i++){
		if (  ( F_ABS((A_long)ti->color8[i].red   - (A_long)p2.red  ) <= ti->level)
			&&( F_ABS((A_long)ti->color8[i].green - (A_long)p2.green) <= ti->level)
			&&( F_ABS((A_long)ti->color8[i].blue  - (A_long)p2.blue ) <= ti->level)){
			ret = TRUE;
			break;
		}
	}
	return ret;
}
//*****************************************************************************
inline PF_Boolean CompPxs(thinInfo32 *ti, PF_PixelFloat p1,PF_PixelFloat p2,PF_PixelFloat p3)
{
	PF_Boolean ret = FALSE;
	PF_Pixel pp1 = CONV32TO8(p1);
	PF_Pixel pp2 = CONV32TO8(p2);
	PF_Pixel pp3 = CONV32TO8(p3);
	if (  ( F_ABS((A_long)pp1.red   - (A_long)pp2.red  ) <= ti->level)
		&&( F_ABS((A_long)pp1.green - (A_long)pp2.green) <= ti->level)
		&&( F_ABS((A_long)pp1.blue  - (A_long)pp2.blue ) <= ti->level)){
		if (  ( F_ABS((A_long)pp1.red   - (A_long)pp3.red  ) <= ti->level)
			&&( F_ABS((A_long)pp1.green - (A_long)pp3.green) <= ti->level)
			&&( F_ABS((A_long)pp1.blue  - (A_long)pp3.blue ) <= ti->level)){
				ret = TRUE;
		}
	}
	return ret;
}
//*****************************************************************************
inline PF_Boolean CompPxs2(thinInfo32 *ti, PF_PixelFloat p1,PF_PixelFloat p2)
{
	PF_Boolean ret = FALSE;
	PF_Pixel pp1 = CONV32TO8(p1);
	PF_Pixel pp2 = CONV32TO8(p2);
	if (  ( F_ABS((A_long)pp1.red   - (A_long)pp2.red  ) <= ti->level)
		&&( F_ABS((A_long)pp1.green - (A_long)pp2.green) <= ti->level)
		&&( F_ABS((A_long)pp1.blue  - (A_long)pp2.blue ) <= ti->level)){
				ret = TRUE;
	}
	return ret;
}


//*****************************************************************************
inline void scanlineCopy(thinInfo32 *ti, A_long y)
{
	//スキャンラインへコピー
	A_long v = 0; 
	if (y ==0) {
		for ( int i=0; i<ti->w; i++ ){
			ti->scanline[i          ] = ti->data[i ];
			ti->scanline[i + ti->wt ] = ti->data[i ];
			ti->scanline[i + ti->wt2] = ti->data[i + ti->wt]; 
		}
	}else{
		for ( int i=0; i<ti->w; i++ ){
			ti->scanline[i          ] = ti->scanline[i + ti->wt ];
			ti->scanline[i + ti->wt ] = ti->scanline[i + ti->wt2];
		}
		if (y<ti->h-1 ){
			v = (y+1)*ti->wt; 
			for ( int i=0; i<ti->w; i++ ){
				ti->scanline[i + ti->wt2] = ti->data[i + v]; 
			}
		}
	}
}
//*****************************************************************************
inline void scanlineCopyB(thinInfo32 *ti, A_long y)
{
	//スキャンラインへコピー
	A_long v0 = 0; 
	A_long v1 = 0; 
	A_long v2 = 0; 
	if (y ==0) {
		v0 = 0; 
		v1 = 0; 
		v2 = ti->wt; 
		
	}else if (y==ti->h-1 ){
		v0 = (y-1) * ti->wt; 
		v1 = y * ti->wt; 
		v2 = v1; 
	}else{
		v0 = (y-1) * ti->wt; 
		v1 = (y  ) * ti->wt; 
		v2 = (y+1) * ti->wt; 
	}
	for ( int i=0; i<ti->w; i++ ){
		ti->scanline[i          ] = ti->data[i + v0];
		ti->scanline[i + ti->wt ] = ti->data[i + v1];
		ti->scanline[i + ti->wt2] = ti->data[i + v2]; 
	}
}
//*****************************************************************************
inline PF_PixelFloat getScanLine(thinInfo32 *ti, A_long offsetX, A_long offsetY)
{
	A_long xx = ti->nowX + offsetX;
	 A_long yy = 1 + offsetY;
	 if (xx<0) {
		 xx =0;
	 } else if (xx>=ti->w) {
		 xx = ti->w -1;
	 }
	 if (yy<0) {
		 yy =0;
	 } else if (yy>2) {
		 yy= 2;
	 }
	 
	 return ti->scanline[ xx + yy*ti->wt];
}
//*****************************************************************************
inline PF_Boolean getData(thinInfo32 *ti, A_long offsetX, A_long offsetY)
{
	A_long xx = ti->nowX + offsetX;
	A_long yy = ti->nowY + offsetY;
	if (xx<0) {
		xx =0;
	} else if (xx>=ti->w) {
		xx = ti->w -1;
	}
	if (yy<0) {
		yy =0;
	} else if (yy>=ti->h) {
		yy= ti->h-1;
	}
	PF_PixelFloat p = ti->data[ xx + yy*ti->wt]; 
	 
	return CompPx(ti,p);
}
//*****************************************************************************
inline PF_PixelFloat getPx(thinInfo32 *ti, A_long offsetX, A_long offsetY)
{
	A_long xx = ti->nowX + offsetX;
	A_long yy = ti->nowY + offsetY;
	if (xx<0) {
		xx =0;
	} else if (xx>=ti->w) {
		xx = ti->w -1;
	}
	if (yy<0) {
		yy =0;
	} else if (yy>=ti->h) {
		yy= ti->h-1;
	}
	PF_PixelFloat p = ti->data[ xx + yy*ti->wt]; 
	 
	return p;
}
//*****************************************************************************
inline PF_PixelFloat minColor(PF_PixelFloat s,PF_PixelFloat d)
{
	double ss = (double)s.red + (double)s.green + (double)s.blue;
	double dd = (double)d.red + (double)d.green + (double)d.blue;
	if (ss<dd) {
		return s;
	}else{
		return d;
	}
}
//*****************************************************************************
inline thinBak32 getPatEdge(thinInfo32 *ti)
{
	thinBak32 ret;
	ret.drawFlag = FALSE;


	// 00,01,02
	// 03,xx,04
	// 05,06,07

	PF_PixelFloat table[8];
	table[0] = getScanLine(ti,-1,-1);
	table[1] = getScanLine(ti, 0,-1);
	table[2] = getScanLine(ti, 1,-1);
	table[3] = getScanLine(ti,-1, 0);
	table[4] = getScanLine(ti, 1, 0);
	table[5] = getScanLine(ti,-1, 1);
	table[6] = getScanLine(ti, 0, 1);
	table[7] = getScanLine(ti, 1, 1);

	/*
	注目するピクセルの周囲8点を調べ同じ色かどうかを8bitのビットフラグで管理
	256通りのパターンが考えられるのでそれに合わせて条件分岐
	*/
	//パターン化
	A_long pat = 0;
	A_long patCount = 0;
	for (A_long i = 0; i<8; i++){
		pat <<= 1;
		if (CompPx(ti,table[i])) {
			pat = pat | 1;
			patCount++;
		}
	}
	////
	//●○○
	//○●●
	//●●●
	//0X8F(143)
	//10001111
	//●○○
	//○●●
	//●●○
	//0X8E(142)
	//10001110
	if ((pat==0x8F)||(pat==0x8E)) {
		if (CompPxs(ti,table[1],table[2],table[3])==TRUE) {
			ret.drawFlag = TRUE;
			ret.color = table[1];
		}
		return ret;
	//○●●
	//○●●
	//●○●
	//0X6D(109)
	//01101101
	//○●○
	//○●●
	//●○●
	//0X4D(77)
	//01001101
	}else if ((pat==0x6D)||(pat==0x4D)) {
		if (CompPxs(ti,table[0],table[3],table[6])==TRUE) {
			ret.drawFlag = TRUE;
			ret.color = table[0];
		}
		return ret;
	//●●●
	//●●○
	//○○●
	//0XF1(241)
	//11110001
	//○●●
	//●●○
	//○○●
	//0X71(113)
	//01110001
	}else if ((pat==0xF1)||(pat==0x71)) {
		if (CompPxs(ti,table[4],table[5],table[6])==TRUE) {
			ret.drawFlag = TRUE;
			ret.color = table[4];
		}
		return ret;
	//●○●
	//●●○
	//●●○
	//0XB6(182)
	//10110110
	//●○●
	//●●○
	//○●○
	//0XB2(178)
	//10110010
	}else if ((pat==0x86)||(pat==0xB2)) {
		if (CompPxs(ti,table[1],table[4],table[7])==TRUE) {
			ret.drawFlag = TRUE;
			ret.color = table[1];
		}
		return ret;
	//○○●
	//●●○
	//●●●
	//0X37(55)
	//00110111
	//○○●
	//●●○
	//○●●
	//0X33(51)
	//00110011
	}else if ((pat==0x35)||(pat==0x33)) {
		if (CompPxs(ti,table[0],table[1],table[4])==TRUE) {
			ret.drawFlag = TRUE;
			ret.color = table[0];
		}
		return ret;
	//●●○
	//●●○
	//●○●
	//0XD5(213)
	//11010101
	//○●○
	//●●○
	//●○●
	//0X55(85)
	//01010101
	}else if ((pat==0xD5)||(pat==0x55)) {
		if (CompPxs(ti,table[2],table[4],table[6])==TRUE) {
			ret.drawFlag = TRUE;
			ret.color = table[2];
		}
		return ret;
	//●●●
	//○●●
	//●○○
	//0XEC(236)
	//11101100
	//●●○
	//○●●
	//●○○
	//0XCC(204)
	//11001100
	}else if ((pat==0xEC)||(pat==0xCC)) {
		if (CompPxs(ti,table[3],table[6],table[7])==TRUE) {
			ret.drawFlag = TRUE;
			ret.color = table[3];
		}
		return ret;
	//●○●
	//○●●
	//○●●
	//0XAB(171)
	//10101011
	//●○●
	//○●●
	//○●○
	//0XAA(170)
	//10101010
	}else if ((pat==0xAB)||(pat==0xAA)) {
		if (CompPxs(ti,table[1],table[3],table[5])==TRUE) {
			ret.drawFlag = TRUE;
			ret.color = table[1];
		}
		return ret;
	//●●●
	//○●●
	//●○●
	//0XED(237)
	//11101101
	//●●○
	//○●●
	//●○●
	//0XCD(205)
	//11001101
	}else if ((pat==0xED)||(pat==0xCD)) {
		if (CompPxs2(ti,table[3],table[6])==TRUE) {
			ret.drawFlag = TRUE;
			ret.color = table[3];
		}
		return ret;
//●○●
//○●●
//●●●
//0XAF(175)
//10101111
//●○●
//○●●
//●●○
//0XAE(174)
//10101110
	}else if ((pat==0xAF)||(pat==0xAE)) {
		if (CompPxs2(ti,table[1],table[3])==TRUE) {
			ret.drawFlag = TRUE;
			ret.color = table[1];
		}
		return ret;
	//●○●
	//●●○
	//●●●
	//0XB7(183)
	//10110111
	//●○●
	//●●○
	//○●●
	//0XB3(179)
	//10110011
	}else if ((pat==0xB7)||(pat==0xB3)) {
		if (CompPxs2(ti,table[1],table[4])==TRUE) {
			ret.drawFlag = TRUE;
			ret.color = table[1];
		}
		return ret;
	//●●●
	//●●○
	//●○●
	//0XF5(245)
	//11110101
	//○●●
	//●●○
	//●○●
	//0X75(117)
	//01110101
	}else if ((pat==0xF5)||(pat==0x75)) {
		if (CompPxs2(ti,table[4],table[6])==TRUE) {
			ret.drawFlag = TRUE;
			ret.color = table[4];
		}
	}
	return ret;
}

//*****************************************************************************
//*****************************************************************************
//*****************************************************************************
inline thinBak32 getPatDot(thinInfo32 *ti)
{
	thinBak32 ret;
	ret.drawFlag = FALSE;

	// 00,01,02
	// 03,xx,04
	// 05,06,07

	PF_PixelFloat table[8];
	table[0] = getScanLine(ti,-1,-1);
	table[1] = getScanLine(ti, 0,-1);
	table[2] = getScanLine(ti, 1,-1);
	table[3] = getScanLine(ti,-1, 0);
	table[4] = getScanLine(ti, 1, 0);
	table[5] = getScanLine(ti,-1, 1);
	table[6] = getScanLine(ti, 0, 1);
	table[7] = getScanLine(ti, 1, 1);

	/*
	注目するピクセルの周囲8点を調べ同じ色かどうかを8bitのビットフラグで管理
	256通りのパターンが考えられるのでそれに合わせて条件分岐
	*/
	//パターン化
	A_long pat = 0;
	A_long patCount = 0;
	for (A_long i = 0; i<8; i++){
		pat <<= 1;
		if (CompPx(ti,table[i])) {
			pat = pat | 1;
			patCount++;
		}
	}
	//if (patCount == 3) {
		//●●●
		//○●○
		//○○○
		//0XE0(224)
		//11100000
		if ( pat == 0XE0){
			ret.drawFlag = TRUE;
			ret.color = minColor(table[3],minColor(table[4],table[6]));
			return ret;
		//○○●
		//○●●
		//○○●
		//0X29(41)
		//00101001
		}else if ( pat == 0X29) {
			ret.drawFlag = TRUE;
			ret.color = minColor(table[1],minColor(table[3],table[6]));
		//○○○
		//○●○
		//●●●
		//0X7(7)
		//00000111
		}else if ( pat == 0X07) {
			ret.drawFlag = TRUE;
			ret.color = minColor(table[1],minColor(table[3],table[4]));
		//●○○
		//●●○
		//●○○
		//0X94(148)
		//10010100
		}else if ( pat == 0X94) {
			ret.drawFlag = TRUE;
			ret.color = minColor(table[1],minColor(table[4],table[6]));
		}
	//}
	return ret;
}
//*****************************************************************************
//*****************************************************************************
//*****************************************************************************
inline thinBak32 getPatDot2nd(thinInfo32 *ti)
{
	thinBak32 ret;
	ret.drawFlag = FALSE;
	ret.dir = -1;

	// 00,01,02
	// 03,xx,04
	// 05,06,07

	PF_PixelFloat table[8];
	table[0] = getScanLine(ti,-1,-1);
	table[1] = getScanLine(ti, 0,-1);
	table[2] = getScanLine(ti, 1,-1);
	table[3] = getScanLine(ti,-1, 0);
	table[4] = getScanLine(ti, 1, 0);
	table[5] = getScanLine(ti,-1, 1);
	table[6] = getScanLine(ti, 0, 1);
	table[7] = getScanLine(ti, 1, 1);

	/*
	注目するピクセルの周囲8点を調べ同じ色かどうかを8bitのビットフラグで管理
	256通りのパターンが考えられるのでそれに合わせて条件分岐
	*/
	//パターン化
	A_long pat = 0;
	A_long patCount = 0;
	for (A_long i = 0; i<8; i++){
		pat <<= 1;
		if (CompPx(ti,table[i])) {
			pat = pat | 1;
			patCount++;
		}
	}
	//if (patCount == 3) {
		//○○○
		//●●●
		//●○●
		//0X1D(29)
		//00011101
		if ( (pat & 0x1F) == 0X1D){
			ret.drawFlag = TRUE;
			ret.color = table[3];
			ret.dir = 2;
			return ret;
		//●●○
		//○●○
		//●●○
		//0XC6(198)
		//11000110
		}else if ( (pat & 0xD6) == 0XC6) {
			ret.drawFlag = TRUE;
			ret.color = table[0];
			ret.dir = 3;
			return ret;
		//●○●
		//●●●
		//○○○
		//0XB8(184)
		//10111000
		}else if ( (pat & 0xF8) == 0XB8) {
			ret.drawFlag = TRUE;
			ret.color = table[0];
			ret.dir = 0;
			return ret;
		//○●●
		//○●○
		//○●●
		//0X63(99)
		//01100011
		}else if ((pat & 0x6B) == 0X63) {
			ret.drawFlag = TRUE;
			ret.color = table[1];
			ret.dir = 1;
			return ret;
		}
	//}
	return ret;
}

//*****************************************************************************
//*****************************************************************************
//*****************************************************************************
inline thinBak32 getPat(thinInfo32 *ti)
{
	thinBak32 ret;
	ret.drawFlag = FALSE;


	// 00,01,02
	// 03,xx,04
	// 05,06,07

	PF_PixelFloat table[8];
	table[0] = getScanLine(ti,-1,-1);
	table[1] = getScanLine(ti, 0,-1);
	table[2] = getScanLine(ti, 1,-1);
	table[3] = getScanLine(ti,-1, 0);
	table[4] = getScanLine(ti, 1, 0);
	table[5] = getScanLine(ti,-1, 1);
	table[6] = getScanLine(ti, 0, 1);
	table[7] = getScanLine(ti, 1, 1);

	/*
	注目するピクセルの周囲8点を調べ同じ色かどうかを8bitのビットフラグで管理
	256通りのパターンが考えられるのでそれに合わせて条件分岐
	*/
	//パターン化
	A_long pat = 0;
	A_long patCount = 0;
	for (A_long i = 0; i<8; i++){
		pat <<= 1;
		if (CompPx(ti,table[i])) {
			pat = pat | 1;
			patCount++;
		}
	}

	/*
	同じ色の数(patCount)を記憶しておいて条件分岐の数をなるべく減らす。
	*/
//
//●●●
//●●●
//●●●

//○○○
//○●○
//○○○
	/*
	全部同じ場合は特に何もしない
	*/
	if (patCount>7) {
		return ret;
	}else if (patCount<=0){
			ret.drawFlag = TRUE;
			PF_PixelFloat c = minColor(table[1],table[3]);
			c = minColor(c,table[4]);
			c = minColor(c,table[6]);
			ret.color = c;
			return ret;
	}else if (patCount==7){
		//******************************************
//●○●
//●●●
//●●●
//0XBF(191)
//10111111
		if (pat==0xBF) {
			if (getData(ti,0,1)==TRUE){
				ret.drawFlag = TRUE;
				ret.color = table[1];
			}
			return ret;
		}
//●●●
//○●●
//●●●
//0XEF(239)
//11101111
		if (pat==0xEF) {
			if (getData(ti,1,0)==TRUE){
				ret.drawFlag = TRUE;
				ret.color = table[3];
			}
			return ret;
		}
//●●●
//●●●
//●○●
//0XFD(253)
//11111101
		if (pat==0xFD) {
			if (getData(ti,0,-1)==TRUE){
				ret.drawFlag = TRUE;
				ret.color = table[6];
			}
			return ret;
		}
//●●●
//●●○
//●●●
//0XF7(247)
//11110111
		if (pat==0x7F) {
			if (getData(ti,-1,0)==TRUE){
				ret.drawFlag = TRUE;
				ret.color = table[4];
			}
			return ret;
		}
	}else if (patCount==6){
		//******************************************
//●◎○
//●●●
//●●●
//0X9F(159)
//10011111
//○◎●
//●●●
//●●●
//0X3F(63)
//00111111
		if ((pat==0x9F)||(pat==0x3F)) {
			if (getData(ti,0,-1)==TRUE){
				ret.drawFlag = TRUE;
				ret.color = table[1];
			}
			return ret;
		}

//○●●
//◎●●
//●●●
//0X6F(111)
//01101111
//●●●
//◎●●
//○●●
//0XEB(235)
//11101011		
		else if ((pat==0x6F)||(pat==0xEB)) {
			if (getData(ti,1,0)==TRUE){
				ret.drawFlag = TRUE;
				ret.color = table[3];
			}
			return ret;
		}
//●●●
//●●●
//○◎●
//0XF9(249)
//11111001
//●●●
//●●●
//●◎○
//0XFC(252)
//11111100
		else if ((pat==0xF9)||(pat==0xFC)) {
			if (getData(ti,0,-1)==TRUE){
				ret.drawFlag = TRUE;
				ret.color = table[6];
			}
			return ret;
		}
//●●●
//●●◎
//●●○
//0XF6(246)
//11110110
//●●○
//●●◎
//●●●
//0XD7(215)
//11010111
		else if ((pat==0xF6)||(pat==0xD7)) {
			if (getData(ti,-1,0)==TRUE){
				ret.drawFlag = TRUE;
				ret.color = table[4];
			}
			return ret;
		}
		//******************************************
	}else if (patCount == 5){
//●●●
//●●●
//○◎○
//0XF8(248)
//11111000
		if (pat==0xF8) {
			if (getData(ti,0,-1)==TRUE){
				ret.drawFlag = TRUE;
				ret.color = table[6];
			}
			return ret;
		}
//○◎○
//●●●
//●●●
//0X1F(31)
//00011111
		if (pat==0x1F) {
			if (getData(ti,0,1)==TRUE){
				ret.drawFlag = TRUE;
				ret.color = table[1];
			}
			return ret;
		}
//○●●
//◎●●
//○●●
//0X6B(107)
//01101011
		else if (pat==0x6B) {
			if (getData(ti,1,0)==TRUE){
				ret.drawFlag = TRUE;
				ret.color = table[3];
			}
			return ret;
		}
//●●○
//●●◎
//●●○
//0XD6(214)
//11010110
		else if (pat==0xF8) {
			if (getData(ti,-1,0)==TRUE){
				ret.drawFlag = TRUE;
				ret.color = table[4];
			}
			return ret;
		}
//●○◎
//●●○
//●●●
//0X97(151)
//10010111
		else if (pat==0x97) {
			ret.drawFlag = TRUE;
			ret.color = minColor(table[1],table[4]);
			return ret;
		}
//◎○●
//○●●
//●●●
//0X2F(47)
//00101111
		else if (pat==0x2F) {
			ret.drawFlag = TRUE;
			ret.color = minColor(table[1],table[3]);
			return ret;
		}
//●●●
//○●●
//◎○●
//0XE9(233)
//11101001
		else if (pat==0xE9) {
			ret.drawFlag = TRUE;
			ret.color = minColor(table[3],table[6]);
			return ret;
		}
//●●●
//●●○
//●○◎
//0XF4(244)
//11110100
		else if (pat==0xF4) {
			ret.drawFlag = TRUE;
			ret.color = minColor(table[4],table[6]);
			return ret;
		}
//○●○
//●●●
//●○●
//0X5D(93)
//01011101
		else if (pat==0x5D) {
			if (getData(ti,0,-1)==TRUE){
				ret.drawFlag = TRUE;
				ret.color = table[6];
			}
			return ret;
		}
//○●●
//●●○
//○●●
//0X73(115)
//01110011
		else if (pat==0x73) {
			if (getData(ti,-1,0)==TRUE){
				ret.drawFlag = TRUE;
				ret.color = table[4];
			}
			return ret;
		}
//●○●
//●●●
//○●○
//0XBA(186)
//10111010
		else if (pat==0xBA) {
			if (getData(ti,0,-1)==TRUE){
				ret.drawFlag = TRUE;
				ret.color = table[1];
			}
			return ret;
		}
//●●○
//○●●
//●●○
//0XCE(206)
//11001110
		else if (pat==0xCE) {
			if (getData(ti,1,0)==TRUE){
				ret.drawFlag = TRUE;
				ret.color = table[3];
			}
			return ret;
		}
		//******************************************
	}else if (patCount == 4){
//◎○○
//○●●
//●●●
//0XF(15)
//00001111
//◎○●
//○●●
//○●●
//0X2B(43)
//00101011
		if ((pat==0x0F)||(pat==0x2B)) {
			ret.drawFlag = TRUE;
			ret.color = minColor(table[1],table[3]);
			return ret;
		}
//○●●
//○●●
//◎○●
//0X69(105)
//01101001
//●●●
//○●●
//◎○○
//0XE8(232)
//11101000
		else if ((pat==0x69)||(pat==0xE8)) {
			ret.drawFlag = TRUE;
			ret.color = minColor(table[3],table[6]);
			return ret;
		}

//●●●
//●●○
//○○◎
//0XF0(240)
//11110000
//●●○
//●●○
//●○◎
//0XD4(212)
//11010100
		else if ((pat==0xF0)||(pat==0xD4)) {
			ret.drawFlag = TRUE;
			ret.color = minColor(table[4],table[6]);
			return ret;
		}
//●○◎
//●●○
//●●○
//0X96(150)
//10010110
//○○◎
//●●○
//●●●
//0X17(23)
//00010111
		else if ((pat==0x96)||(pat==0x17)) {
			ret.drawFlag = TRUE;
			ret.color = minColor(table[1],table[4]);
			return ret;
		}
//●●○
//○●●
//○○●
//0XC9(201)
//11001001	
		else if (pat==0xC9) {
			if ((getData(ti,0,-1)==TRUE)&&(getData(ti,1,0)==TRUE)){
				ret.drawFlag = TRUE;
				ret.color = minColor(table[3],table[6]);
			}
			return ret;
		}
//○●●
//●●○
//●○○
//0X74(116)
//01110100
		else if (pat==0x74) {
			if ((getData(ti,0,-1)==TRUE)&&(getData(ti,-1,0)==TRUE)){
				ret.drawFlag = TRUE;
				ret.color = minColor(table[4],table[6]);
			}
			return ret;
		}
//●○○
//●●○
//○●●
//0X93(147)
//10010011
		else if (pat==0x93) {
			if ((getData(ti,-1,0)==TRUE)&&(getData(ti,0,1)==TRUE)){
				ret.drawFlag = TRUE;
				ret.color = minColor(table[1],table[4]);
			}
			return ret;
		}
//○○●
//○●●
//●●○
//0X2E(46)
//00101110
		else if (pat==0x2E) {
			if ((getData(ti,1,0)==TRUE)&&(getData(ti,0,1)==TRUE)){
				ret.drawFlag = TRUE;
				ret.color = minColor(table[1],table[3]);
			}
			return ret;
		}
		//******************************************
	}else if (patCount == 3){
//●●●
//○●○
//○◎○
//0XE0(224)
//11100000
		if (pat==0xE0) {
			if (getData(ti,0,-1)==TRUE){
				ret.drawFlag = TRUE;
				ret.color = minColor(minColor(table[3],table[4]),table[6]);
			}
			return ret;
		}
//●○○
//●●◎
//●○○
		else if (pat==0x94) {
			if (getData(ti,-1,0)==TRUE){
				ret.drawFlag = TRUE;
				ret.color = minColor(minColor(table[1],table[4]),table[6]);
			}
			return ret;
		}
//○◎○
//○●○
//●●●
		else if (pat==0x07) {
			if (getData(ti,0,1)==TRUE){
				ret.drawFlag = TRUE;
				ret.color = minColor(minColor(table[1],table[3]),table[4]);
			}
			return ret;
		}
//○○●
//◎●●
//○○●
		else if (pat==0x29) {
			if (getData(ti,1,0)==TRUE){
				ret.drawFlag = TRUE;
				ret.color = minColor(minColor(table[1],table[3]),table[6]);
			}
			return ret;
		}
//○●●
//●●◎
//○○○
//0X70(112)
//01110000
		else if (pat==0x70) {
			if (getData(ti,0,-1)==TRUE){
				ret.drawFlag = TRUE;
				ret.color = minColor(table[4],table[6]);
			}
			return ret;
		}
//○○○
//●●◎
//○●●
//0X13(19)
//00010011
		else if (pat==0x13) {
			if (getData(ti,-1,0)==TRUE){
				ret.drawFlag = TRUE;
				ret.color = minColor(table[4],table[1]);
			}
			return ret;
		}
//○◎●
//○●●
//○●○
//0X2A(42)
//00101010
		else if (pat==0x2A) {
			if (getData(ti,1,0)==TRUE){
				ret.drawFlag = TRUE;
				ret.color = minColor(table[1],table[3]);
			}
			return ret;
		}
//●◎○
//●●○
//○●○
//0X92(146)
//10010010
		else if (pat==0x92) {
			if (getData(ti,-1,0)==TRUE){
				ret.drawFlag = TRUE;
				ret.color = minColor(table[1],table[4]);
			}
			return ret;
		}
//●●○
//◎●●
//○○○
//0XC8(200)
//11001000
		else if (pat==0x2A) {
			if (getData(ti,0,-1)==TRUE){
				ret.drawFlag = TRUE;
				ret.color = minColor(table[3],table[6]);
			}
			return ret;
		}
//○○○
//◎●●
//●●○
//0XE(14)
//00001110
		else if (pat==0x0E) {
			if (getData(ti,1,0)==TRUE){
				ret.drawFlag = TRUE;
				ret.color = minColor(table[1],table[3]);
			}
			return ret;
		}

//●●○
//●●○
//○○○
//0XD0(208)
//11010000
		if (pat==0xD0) {
			if ((getData(ti,0,-1)==TRUE)&&(getData(ti,-1,0)==TRUE)){
				ret.drawFlag = TRUE;
				ret.color = minColor(table[4],table[6]);
			}
			return ret;
		}
//○○○
//●●○
//●●○
//0X16(22)
//00010110
		if (pat==0x16) {
			if ((getData(ti,-1,0)==TRUE)&&(getData(ti,0,1)==TRUE)){
				ret.drawFlag = TRUE;
				ret.color = minColor(table[1],table[4]);
			}
			return ret;
		}
//○○○
//○●●
//○●●
//0XB(11)
//00001011
		if (pat==0x0B) {
			if ((getData(ti,1,0)==TRUE)&&(getData(ti,0,1)==TRUE)){
				ret.drawFlag = TRUE;
				ret.color = minColor(table[1],table[3]);
			}
			return ret;
		}
		//******************************************
	}else if (patCount == 2){
//○●○
//●●○
//○○○
//0X50(80)
//01010000
		if (pat==0x50) {
			if ((getData(ti,0,-1)==TRUE)&&(getData(ti,-1,0)==TRUE)){
				ret.drawFlag = TRUE;
				ret.color = minColor(table[4],table[6]);
			}
			return ret;
		}
//○○○
//●●○
//○●○
//0X12(18)
//00010010
		else if (pat==0x12) {
			if ((getData(ti,-1,0)==TRUE)&&(getData(ti,0,1)==TRUE)){
				ret.drawFlag = TRUE;
				ret.color = minColor(table[1],table[4]);
			}
			return ret;
		}
//○○○
//○●●
//○●○
//0XA(10)
//00001010
		else if (pat==0x0A) {
			if ((getData(ti,1,0)==TRUE)&&(getData(ti,0,1)==TRUE)){
				ret.drawFlag = TRUE;
				ret.color = minColor(table[1],table[3]);
			}
			return ret;
		}
//○●○
//○●●
//○○○
//0X48(72)
//01001000
		else if (pat==0x48) {
			if ((getData(ti,0,-1)==TRUE)&&(getData(ti,1,0)==TRUE)){
				ret.drawFlag = TRUE;
				ret.color = minColor(table[3],table[6]);
			}
			return ret;
		}
	}
	return ret;
}
//*****************************************************************************
PF_Err thinExec32(CFsAE *ae , ParamInfo *infoP)
{
	PF_Err	err = PF_Err_NONE;
	thinInfo32 ti;



	ti.colorMax = infoP->colorMax;
	if (ti.colorMax<=0) return err;

	for (A_long i=0; i<infoP->colorMax; i++) {
		ti.color[i] = CONV8TO32( infoP->color[i]);
		ti.color8[i] = infoP->color[i];
	}
	ti.level = (A_long)((double)PF_MAX_CHAN8 * infoP->level + 0.5);

	ti.w  = ae->out->width();
	ti.wt = ae->out->widthTrue();
	ti.wt2 = ti.wt*2;
	ti.h  = ae->out->height();
	ti.offset = ti.wt - ti.w; 

	//メモリの確保
	ti.scanlineH = ae->NewHandle(ti.wt * sizeof(PF_PixelFloat) * 4);// 3Line分　と余裕
	if ( !ti.scanlineH ) return PF_Err_INTERNAL_STRUCT_DAMAGED;

	ti.scanline = *(PF_PixelFloat**)ti.scanlineH; ;
	ti.data = (PF_PixelFloat *)ae->out->data();
	ti.white = infoP->white;
	ti.alphaZero = infoP->alphaZero;
	ti.edge = infoP->edge;

	A_long now = 0;
	//exec
	for (A_long vv=0; vv<infoP->value; vv++){
		now = 0;
		if (ti.edge==TRUE){
			for (A_long j = 0; j<ti.h; j++)
			{
				ti.nowY = j; 
				scanlineCopy(&ti,j);		
				for ( A_long i = 0; i<ti.w; i++) {
					ti.nowX = i; 
					PF_PixelFloat p = getScanLine(&ti,0,0);
					if ( CompPx(&ti,p) ==TRUE ) {
						thinBak32 tb = getPatEdge(&ti);
						if (tb.drawFlag ==TRUE) {
							if (ti.white == TRUE){
								if ((tb.color.blue >= 1.0)
									&&(tb.color.green >= 1.0)
									&&(tb.color.red >= 1.0)
									&&(tb.color.alpha >= 1.0))
								{
									tb.drawFlag = FALSE;
								}
							}
							if (ti.alphaZero == TRUE){
								if (tb.color.alpha == 0) tb.drawFlag = FALSE; 
							}
							if  (tb.drawFlag ==TRUE) {
								ti.data[now] = tb.color;
							}
						}
					}
					now++;

				}
				now += ti.offset;

			}
		}
		now = 0;
		for (A_long j = 0; j<ti.h; j++)
		{
			ti.nowY = j; 
			scanlineCopy(&ti,j);		
			for ( A_long i = 0; i<ti.w; i++) {
				ti.nowX = i; 
				PF_PixelFloat p = getScanLine(&ti,0,0);
				if ( CompPx(&ti,p) ==TRUE ) {
					thinBak32 tb = getPat(&ti);
					if (tb.drawFlag ==TRUE) {
						if (ti.white == TRUE){
							if ((tb.color.blue >= 1.0)
								&&(tb.color.green >= 1.0)
								&&(tb.color.red >= 1.0)
								&&(tb.color.alpha >= 1.0))
							{
								tb.drawFlag = FALSE;
							}
						}
						if (ti.alphaZero == TRUE){
							if (tb.color.alpha == 0) tb.drawFlag = FALSE; 
						}
						if  (tb.drawFlag ==TRUE) {
							ti.data[now] = tb.color;
						}
					}
				}
				now++;

			}
			now += ti.offset;

		}
		//後処理
		now = 0;
		for (A_long j = 0; j<ti.h; j++)
		{
			ti.nowY = j; 
			scanlineCopy(&ti,j);		
			for ( A_long i = 0; i<ti.w; i++) {
				ti.nowX = i; 
				PF_PixelFloat p = getScanLine(&ti,0,0);
				if ( CompPx(&ti,p) ==TRUE ) {
					thinBak32 tb = getPatDot(&ti);
					if (tb.drawFlag ==TRUE) {
						if (ti.white == TRUE){
							if ((tb.color.blue >= 1.0)
								&&(tb.color.green >= 1.0)
								&&(tb.color.red >= 1.0)
								&&(tb.color.alpha >= 1.0))
							{
								tb.drawFlag = FALSE;
							}
						}
						if (ti.alphaZero == TRUE){
							if (tb.color.alpha == 0) tb.drawFlag = FALSE; 
						}
						if  (tb.drawFlag ==TRUE) {
							ti.data[now] = tb.color;
						}
					}
				}
				now++;

			}
			now += ti.offset;

		}
		now = 0;
		for (A_long j = 0; j<ti.h; j++)
		{
			ti.nowY = j; 
			scanlineCopy(&ti,j);		
			for ( A_long i = 0; i<ti.w; i++) {
				ti.nowX = i; 
				PF_PixelFloat p = getScanLine(&ti,0,0);
				A_long now2 = now;
				if ( CompPx(&ti,p) ==TRUE ) {
					thinBak32 tb = getPatDot2nd(&ti);

					if (tb.drawFlag ==TRUE) {
						switch(tb.dir)
						{
						case 0:
							if (ti.nowY>0) {
								now2 = now - ti.wt;
							}else{
								tb.drawFlag = FALSE;
							}
							break;
						case 1:
							if (ti.nowX<(ti.w-1)) {
								now2 = now + 1;
							}else{
								tb.drawFlag = FALSE;
							}
							break;
						case 2:
							if (ti.nowY<(ti.h-1)) {
								now2 = now + ti.wt;
							}else{
								tb.drawFlag = FALSE;
							}
							break;
						case 3:
							if (ti.nowX>0) {
								now2 = now - 1;
							}else{
								tb.drawFlag = FALSE;
							}
							break;
						}
					}
					if (tb.drawFlag ==TRUE) {
						PF_PixelFloat p = ti.data[now2];
						if (ti.white == TRUE) {
							if ((p.blue >= 1.0)
								&&(p.green >= 1.0)
								&&(p.red >= 1.0)
								&&(p.alpha >= 1.0))
							{
								tb.drawFlag = FALSE;
							}
						}
						if (ti.alphaZero == TRUE) {
							if (p.alpha ==0)
							{
								tb.drawFlag = FALSE;
							}
						}
						if (tb.drawFlag ==TRUE) {
							ti.data[now2] = tb.color;
						}
					}
				}
				now++;

			}
			now += ti.offset;

		}
	}

	if (ti.scanlineH != NULL){
		ae->DisposeHandle(ti.scanlineH);
		ti.scanlineH = NULL;
	}


	return err;
}
