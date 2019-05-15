#include "FsGraphics.h"


static bool levelMin(A_long s,A_long d){ return (s>=d);}
static bool levelMax(A_long s,A_long d){ return (s<=d);}
static bool (*levelComp)(A_long s,A_long d);
//*********************************************************************************************
void CFsGraph::Minimax_rgb32(MiniMaxPrm *prm)
{
	PF_PixelFloat *data = (PF_PixelFloat *)m_data;
	PF_PixelFloat *scanline = (PF_PixelFloat *)prm->scanline;
	A_long *lv = prm->level;

	if(prm->minusFlag){
		levelComp = levelMin;
	}else{
		levelComp = levelMax;
	}

	A_long adr = 0;
	if ((prm->mode == SCANLINE::HorAndVer)||(prm->mode == SCANLINE::Hor)){
		
		for ( A_long y=0; y<m_height; y++)
		{
			for(A_long i=0; i<m_width; i++){
				PF_PixelFloat p = data[adr+i];
				scanline[i] = p;
				lv[i] =  (A_long)( ( 0.29891 * (double)p.red * 65536) + ( 0.58661 * (double)p.green * 65536) + ( 0.11448 * (double)p.blue * 65536) +0.5);
			}

			A_long maxIndex = -1;
			for (A_long x=0; x<m_width; x++)
			{
				A_long k0 = x - prm->value;
				if (k0<0) k0=0;
				A_long k1  = x + prm->value-1;
				if (k1>=m_width) k1 = m_width -1;

				if (maxIndex >= k0)
				{
					if (levelComp(lv[maxIndex],lv[k1]))maxIndex = k1;
				}else{
					maxIndex = k0;
					for ( A_long k= k0; k<=k1;k++)
					{
						if (levelComp(lv[maxIndex],lv[k]))maxIndex = k;	
					}
				}
				data[adr+x].red		= scanline[maxIndex].red;
				data[adr+x].green	= scanline[maxIndex].green;
				data[adr+x].blue	= scanline[maxIndex].blue;
			}
			adr += m_widthTrue;
		}


	}
	if ((prm->mode == SCANLINE::HorAndVer)||(prm->mode == SCANLINE::Ver)){
		for ( A_long x=0; x<m_width; x++)
		{
			adr = x;
			for(A_long i=0; i<m_height; i++){
				PF_PixelFloat p = data[adr];
				scanline[i] = p;
				lv[i] =  (A_long)( ( 0.29891 * (double)p.red * 65536) + ( 0.58661 * (double)p.green * 65536) + ( 0.11448 * (double)p.blue * 65536) +0.5);
				adr+=m_widthTrue;
			}
			adr = x;
			A_long maxIndex = -1;
			for (A_long y=0; y<m_height; y++)
			{
				A_long k0 = y - prm->value;
				if (k0<0) k0=0;
				A_long k1  = y + prm->value-1;
				if (k1>=m_height) k1 = m_height -1;

				if (maxIndex >= k0)
				{
					if (levelComp(lv[maxIndex],lv[k1]))maxIndex = k1;						
				}else{
					maxIndex = k0;
					for ( A_long k= k0; k<=k1;k++)
					{
						if (levelComp(lv[maxIndex],lv[k]))maxIndex = k;	
					}
				}
				data[adr].red	= scanline[maxIndex].red;
				data[adr].green	= scanline[maxIndex].green;
				data[adr].blue	= scanline[maxIndex].blue;
				adr += m_widthTrue;
			}
		}
	}
}
//*********************************************************************************************
void CFsGraph::Minimax_alpha32(MiniMaxPrm *prm)
{
	PF_PixelFloat *data = (PF_PixelFloat *)m_data;
	PF_PixelFloat *scanline = (PF_PixelFloat *)prm->scanline;
	A_long *lv = prm->level;

	if(prm->minusFlag){
		levelComp = levelMin;
	}else{
		levelComp = levelMax;
	}

	A_long adr = 0;
	if ((prm->mode == SCANLINE::HorAndVer)||(prm->mode == SCANLINE::Hor)){
		
		for ( A_long y=0; y<m_height; y++)
		{
			for(A_long i=0; i<m_width; i++){
				PF_PixelFloat p = data[adr+i];
				scanline[i] = p;
				lv[i] =  (A_long)(p.alpha*65536+0.5);
			}
			A_long maxIndex = -1;
			for (A_long x=0; x<m_width; x++)
			{
				A_long k0 = x - prm->value;
				if (k0<0) k0=0;
				A_long k1  = x + prm->value-1;
				if (k1>=m_width) k1 = m_width -1;

				if (maxIndex >= k0)
				{
					if (levelComp(lv[maxIndex],lv[k1]))maxIndex = k1;						
				}else{
					maxIndex = k0;
					for ( A_long k= k0; k<=k1;k++)
					{
						if (levelComp(lv[maxIndex],lv[k]))maxIndex = k;	
					}
				}
				data[adr+x].alpha = scanline[maxIndex].alpha;
			}
			adr += m_widthTrue;
		}


	}
	if ((prm->mode == SCANLINE::HorAndVer)||(prm->mode == SCANLINE::Ver)){
		for ( A_long x=0; x<m_width; x++)
		{
			adr = x;
			for(A_long i=0; i<m_height; i++){
				PF_PixelFloat p = data[adr];
				scanline[i] = p;
				lv[i] =  (A_long)(p.alpha*65536+0.5);
				adr+=m_widthTrue;
			}
			adr = x;
			A_long maxIndex = -1;
			for (A_long y=0; y<m_height; y++)
			{
				A_long k0 = y - prm->value;
				if (k0<0) k0=0;
				A_long k1  = y + prm->value-1;
				if (k1>=m_height) k1 = m_height -1;

				if (maxIndex >= k0)
				{
					if (levelComp(lv[maxIndex],lv[k1]))maxIndex = k1;						
				}else{
					maxIndex = k0;
					for ( A_long k= k0; k<=k1;k++)
					{
						if (levelComp(lv[maxIndex],lv[k]))maxIndex = k;	
					}
				}
				data[adr].alpha	= scanline[maxIndex].alpha;
				adr += m_widthTrue;
			}
		}
	}

}
//*********************************************************************************************
PF_Err CFsGraph::Minimax32(A_long value,ScanLineMode mode,TargetChannelMode target)
{
	PF_Err err = PF_Err_NONE;
	if (value == 0) return err;
	
	PF_InData *in_data;
	in_data = m_in_data;

	MiniMaxPrm mp;
	mp.value = F_ABS(value);
	mp.minusFlag = (value<0);
	mp.mode = mode;

	if (mp.minusFlag){
		toWhiteMat32();
	}else{
		toBlackMat32();
	}

	A_long w = MAX(m_width,m_height);

	PF_Handle sH = PF_NEW_HANDLE( w * sizeof(PF_PixelFloat)*2);
	if (!sH) return PF_Err_OUT_OF_MEMORY;
	PF_Handle lH = PF_NEW_HANDLE(w * sizeof(A_long)*2);
	if (!lH) {
		PF_DISPOSE_HANDLE(sH);
		return PF_Err_OUT_OF_MEMORY;
	}

	mp.scanline	= *(PF_PixelPtr*)sH;
	mp.level	= *(A_long **)lH;

	

	//rgb
	if ( (target == TARGET_CHANNEL::rgbAndAlpha)||(target == TARGET_CHANNEL::rgb)){
		Minimax_rgb32(&mp);	
	}

	//alpha
	if ( (target == TARGET_CHANNEL::rgbAndAlpha)||(target == TARGET_CHANNEL::alpha)){
		Minimax_alpha32(&mp);	
	}


	PF_DISPOSE_HANDLE(sH);
	PF_DISPOSE_HANDLE(lH);
	
	if (mp.minusFlag){
		fromWhiteMat32();
	}else{
		fromBlackMat32();
	}
	


	return err;
}
//*********************************************************************************************
//*********************************************************************************************
PF_Err CFsGraph::MinimaxA32(A_long value)
{
	PF_Err err = PF_Err_NONE;
	if (value == 0) return err;
	
	PF_InData *in_data;
	in_data = m_in_data;

	MiniMaxPrm mp;
	mp.value = F_ABS(value);
	mp.minusFlag = (value<0);
	mp.mode = SCANLINE::HorAndVer;


	A_long w = MAX(m_width,m_height);

	PF_Handle sH = PF_NEW_HANDLE( w * sizeof(PF_PixelFloat)*2);
	if (!sH) return PF_Err_OUT_OF_MEMORY;
	PF_Handle lH = PF_NEW_HANDLE(w * sizeof(A_long)*2);
	if (!lH) {
		PF_DISPOSE_HANDLE(sH);
		return PF_Err_OUT_OF_MEMORY;
	}

	mp.scanline	= *(PF_PixelPtr*)sH;
	mp.level	= *(A_long **)lH;

	


	//alpha
	Minimax_alpha32(&mp);	


	PF_DISPOSE_HANDLE(sH);
	PF_DISPOSE_HANDLE(lH);
	
	return err;
}
//*********************************************************************************************