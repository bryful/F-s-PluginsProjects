#include "FsGraphics.h"

//------------------------------------------------------------------------------
void CFsGraph::paint_scan_line8(paintPrm *prm)
{
	paintPrm *p = prm;
	long xl = prm->now.Xleft;
	long xr = prm->now.Xright;
	long y = prm->now.Y;
	long oy = prm->now.Yparent;
	PF_Pixel col = prm->targetCol;

	while(xl <= xr){
		while(xl < xr){
			if (  paint_cmp8( paint_getPix8(xl,y),col)==TRUE ) break;
			xl++;
		}
		if (paint_cmp8( paint_getPix8(xl,y),col)==FALSE) break;
		p->buf[p->last].Xleft = xl;

		while (xl <= xr) {
			if (paint_cmp8( paint_getPix8(xl,y),col)==FALSE) break;
			xl++;
		}
		p->buf[p->last].Xright  = xl -1;
		p->buf[p->last].Y       = y;
		p->buf[p->last].Yparent = oy;
		p->last+=1;
		if (p->last==p->max) p->last = 0;
	}
}
//------------------------------------------------------------------------------
void CFsGraph::paint_scan_line16(paintPrm *prm)
{
	paintPrm *p = prm;
	long xl = prm->now.Xleft;
	long xr = prm->now.Xright;
	long y = prm->now.Y;
	long oy = prm->now.Yparent;
	PF_Pixel16 col = prm->targetCol16;

	while(xl <= xr){
		while(xl < xr){
			if (  paint_cmp16( paint_getPix16(xl,y),col)==TRUE ) break;
			xl++;
		}
		if (paint_cmp16( paint_getPix16(xl,y),col)==FALSE) break;
		p->buf[p->last].Xleft = xl;

		while (xl <= xr) {
			if (paint_cmp16( paint_getPix16(xl,y),col)==FALSE) break;
			xl++;
		}
		p->buf[p->last].Xright  = xl -1;
		p->buf[p->last].Y       = y;
		p->buf[p->last].Yparent = oy;
		p->last+=1;
		if (p->last==p->max) p->last = 0;
	}
}
//------------------------------------------------------------------------------
PF_Err CFsGraph::paint8(long x, long y, PF_Pixel col)
{
	PF_Err		err 				= PF_Err_NONE;
	paintPrm	prm;
	PF_Handle	queTable		= NULL;
	PF_InData *in_data		= m_in_data;
	PF_Pixel	*data				= (PF_Pixel *)m_data;
	long wt = m_widthTrue;
	long i,target;
	long lx,rx,uy,dy,oy;
  long lxsav,rxsav;
	PF_Pixel	paintCol = col;

	if ( (x<0)||(y<0)||(x>=m_width)||(y>=m_height) ) {
		return err;
	}
	
	//バッファーの作成。初期値を代入
	queTable = PF_NEW_HANDLE(m_width * sizeof(paintQue));
	if (!queTable) {
		err = PF_Err_INTERNAL_STRUCT_DAMAGED;
		return err;
	}
	prm.buf = *(paintQue**)queTable;

  prm.max   = m_width;
  prm.start = 0;
  prm.last  = 1;
	prm.buf[prm.start].Xleft		= x;
  prm.buf[prm.start].Xright		= x;
  prm.buf[prm.start].Y				= y;
  prm.buf[prm.start].Yparent	= y;
  prm.targetCol = paint_getPix8(x,y);
	do {
    lx	= prm.buf[prm.start].Xleft;
    rx	= prm.buf[prm.start].Xright;
    uy	= prm.buf[prm.start].Y;
    dy	= prm.buf[prm.start].Y;
    oy	= prm.buf[prm.start].Yparent;

    lxsav = lx - 1;
    rxsav = rx + 1;

    prm.start+=1;
    if (prm.start == prm.max) prm.start =0;

		// 処理済のシードなら無視
		if (paint_cmp8(paint_getPix8(lx,uy),paintCol)==TRUE) continue;

		//右方向の境界を探す
		while (rx < (m_width -1) ) {
			if (paint_cmp8(paint_getPix8(rx+1,uy),prm.targetCol)==FALSE) break;
      rx++;
		}

		//左方向の境界を探す
		while(lx>0) {
			if (paint_cmp8(paint_getPix8(lx-1,uy),prm.targetCol)==FALSE) break;
      lx--;
		}

	//lx-rxの線分を描画
		target = lx + uy * wt;
		for (i=lx;i<=rx;i++) {
			data[target] = col;
			target++;
		}

  //真上のスキャンラインを走査する
		uy--;
		if (uy>=0) {
			if (uy==oy) {
				prm.now.Xleft   = lx;
				prm.now.Xright  = lxsav;
				prm.now.Y       = uy;
				prm.now.Yparent = uy+1;
				paint_scan_line8(&prm);
				prm.now.Xleft   = rxsav;
				prm.now.Xright  = rx;
				prm.now.Y       = uy;
				prm.now.Yparent = uy+1;
				paint_scan_line8(&prm);
			}else{
				prm.now.Xleft   = lx;
				prm.now.Xright  = rx;
				prm.now.Y       = uy;
				prm.now.Yparent = uy+1;
				paint_scan_line8(&prm);
			}
		}
		//真下のスキャンラインを走査する
		dy++;
		if (dy<m_height) {
			if (dy==oy) {
				prm.now.Xleft   = lx;
				prm.now.Xright  = lxsav;
				prm.now.Y       = dy;
				prm.now.Yparent = dy-1;
				paint_scan_line8(&prm);
				prm.now.Xleft   = rxsav;
				prm.now.Xright  = rx;
				prm.now.Y       = dy;
				prm.now.Yparent = dy-1;
				paint_scan_line8(&prm);
			}else{
				prm.now.Xleft   = lx;
				prm.now.Xright  = rx;
				prm.now.Y       = dy;
				prm.now.Yparent = dy-1;
				paint_scan_line8(&prm);
			}
		}
	}while(prm.start != prm.last);

	//メモリの破棄
	if (queTable){
		PF_DISPOSE_HANDLE(queTable);
		queTable =NULL;
	}
	return err;
}
//------------------------------------------------------------------------------
PF_Err CFsGraph::paint16(long x, long y, PF_Pixel16 col)
{
	PF_Err		err 				= PF_Err_NONE;
	paintPrm	prm;
	PF_Handle	queTable		= NULL;
	PF_InData *in_data		= m_in_data;
	PF_Pixel16	*data				= (PF_Pixel16 *)m_data;
	long wt = m_widthTrue;
	long i,target;
	long lx,rx,uy,dy,oy;
  long lxsav,rxsav;
	PF_Pixel16	paintCol;

	if ( (x<0)||(y<0)||(x>=m_width)||(y>=m_height) ) {
		return err;
	}
	
	paintCol = col;
	/*
	paintCol.alpha	= (unsigned short)CONVERT8TO16(col.alpha);
	paintCol.red		= (unsigned short)CONVERT8TO16(col.red);
	paintCol.green	= (unsigned short)CONVERT8TO16(col.green);
	paintCol.blue		= (unsigned short)CONVERT8TO16(col.blue);
	*/
	//バッファーの作成。初期値を代入
	queTable = PF_NEW_HANDLE(m_width * sizeof(paintQue));
	if (!queTable) {
		err = PF_Err_INTERNAL_STRUCT_DAMAGED;
		return err;
	}
	prm.buf = *(paintQue**)queTable;

	prm.max   = m_width;
	prm.start = 0;
	prm.last  = 1;
	prm.buf[prm.start].Xleft		= x;
	prm.buf[prm.start].Xright		= x;
	prm.buf[prm.start].Y				= y;
	prm.buf[prm.start].Yparent	= y;
	prm.targetCol16 = paint_getPix16(x,y);
	do {
    lx	= prm.buf[prm.start].Xleft;
    rx	= prm.buf[prm.start].Xright;
    uy	= prm.buf[prm.start].Y;
    dy	= prm.buf[prm.start].Y;
    oy	= prm.buf[prm.start].Yparent;

    lxsav = lx - 1;
    rxsav = rx + 1;

    prm.start+=1;
    if (prm.start == prm.max) prm.start =0;

		// 処理済のシードなら無視
		if (paint_cmp16(paint_getPix16(lx,uy),paintCol)==TRUE) continue;

		//右方向の境界を探す
		while (rx < (m_width -1) ) {
			if (paint_cmp16(paint_getPix16(rx+1,uy),prm.targetCol16)==FALSE) break;
      rx++;
		}

		//左方向の境界を探す
		while(lx>0) {
			if (paint_cmp16(paint_getPix16(lx-1,uy),prm.targetCol16)==FALSE) break;
      lx--;
		}

	//lx-rxの線分を描画
		target = lx + uy * wt;
		for (i=lx;i<=rx;i++) {
			data[target] = paintCol;
			target++;
		}

  //真上のスキャンラインを走査する
		uy--;
		if (uy>=0) {
			if (uy==oy) {
				prm.now.Xleft   = lx;
				prm.now.Xright  = lxsav;
				prm.now.Y       = uy;
				prm.now.Yparent = uy+1;
				paint_scan_line16(&prm);
				prm.now.Xleft   = rxsav;
				prm.now.Xright  = rx;
				prm.now.Y       = uy;
				prm.now.Yparent = uy+1;
				paint_scan_line16(&prm);
			}else{
				prm.now.Xleft   = lx;
				prm.now.Xright  = rx;
				prm.now.Y       = uy;
				prm.now.Yparent = uy+1;
				paint_scan_line16(&prm);
			}
		}
		//真下のスキャンラインを走査する
		dy++;
		if (dy<m_height) {
			if (dy==oy) {
				prm.now.Xleft   = lx;
				prm.now.Xright  = lxsav;
				prm.now.Y       = dy;
				prm.now.Yparent = dy-1;
				paint_scan_line16(&prm);
				prm.now.Xleft   = rxsav;
				prm.now.Xright  = rx;
				prm.now.Y       = dy;
				prm.now.Yparent = dy-1;
				paint_scan_line16(&prm);
			}else{
				prm.now.Xleft   = lx;
				prm.now.Xright  = rx;
				prm.now.Y       = dy;
				prm.now.Yparent = dy-1;
				paint_scan_line16(&prm);
			}
		}
	}while(prm.start != prm.last);

	//メモリの破棄
	if (queTable){
		PF_DISPOSE_HANDLE(queTable);
		queTable =NULL;
	}
	return err;
}
//******************************************************************************
//******************************************************************************
/*
PF_Err CFsGraph::paint(long x, long y,PF_Pixel col)
{
	PF_Err		err 				= PF_Err_NONE;

	if (m_Enabled==FALSE){
		err = 0xFF;
		return err;
	}
	if (m_is16Bit==TRUE){
		return paint16(x,y,col);
	}else{
		return paint8(x,y,col);
	}
}
*/
//******************************************************************************
