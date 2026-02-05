#include "FsGraphics.h"


static bool levelMin(A_long s,A_long d){ return (s>=d);}
static bool levelMax(A_long s,A_long d){ return (s<=d);}
static bool (*levelComp)(A_long s,A_long d);
//*********************************************************************************************
void CFsGraph::Minimax_rgb8(MiniMaxPrm *prm)
{
	PF_Pixel *data = m_data;
	PF_Pixel *scanline = (PF_Pixel *)prm->scanline;
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
				PF_Pixel p = data[adr+i];
				scanline[i] = p;
				lv[i] =  (A_long)(( 0.29891 * (double)p.red) + ( 0.58661 * (double)p.green) + ( 0.11448 * (double)p.blue) +0.5);
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
				PF_Pixel p = data[adr];
				scanline[i] = p;
				lv[i] =  (A_long)(( 0.29891 * (double)p.red) + ( 0.58661 * (double)p.green) + ( 0.11448 * (double)p.blue) +0.5);
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
void CFsGraph::Minimax_alpha8(MiniMaxPrm *prm)
{
	PF_Pixel *data = m_data;
	PF_Pixel *scanline = (PF_Pixel *)prm->scanline;
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
				PF_Pixel p = data[adr+i];
				scanline[i] = p;
				lv[i] =  (A_long)(p.alpha);
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
				PF_Pixel p = data[adr];
				scanline[i] = p;
				lv[i] =  (A_long)(p.alpha);
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
PF_Err CFsGraph::Minimax8(A_long value,ScanLineMode mode,TargetChannelMode target)
{
	PF_Err err = PF_Err_NONE;
	if (value == 0) return err;
	
	PF_InData *in_data;
	in_data = m_in_data;

	MiniMaxPrm mp;
	mp.value = F_ABS(value);
	mp.minusFlag = (value<0);
	mp.mode = mode;
	mp.scanline = NULL;  // 追加
	mp.level = NULL;     // 追加

	if (mp.minusFlag){
		toWhiteMat8();
	}else{
		toBlackMat8();
	}


	A_long w = MAX(m_width,m_height);

	PF_EffectWorld sw;
	PF_EffectWorld lw;
	if (m_in_data != NULL && m_in_data->utils != NULL && m_in_data->effect_ref != NULL) {
		(*m_in_data->utils->new_world)(m_in_data->effect_ref, w / 2, 4, PF_PixelFormat_ARGB32, &sw);
		(*m_in_data->utils->new_world)(m_in_data->effect_ref, w / 2, 4, PF_PixelFormat_ARGB32, &lw);
		mp.scanline = sw.data;
		mp.level = (A_long*)(lw.data);
	}
	if (mp.scanline == NULL)
	{
		return PF_Err_OUT_OF_MEMORY;
	}
	//rgb
	if ( (target == TARGET_CHANNEL::rgbAndAlpha)||(target == TARGET_CHANNEL::rgb)){
		Minimax_rgb8(&mp);	
	}

	//alpha
	if ( (target == TARGET_CHANNEL::rgbAndAlpha)||(target == TARGET_CHANNEL::alpha)){
		Minimax_alpha8(&mp);	
	}

	if (m_in_data != NULL && m_scanlineWorld.data != NULL) {
		(*m_in_data->utils->dispose_world)(m_in_data->effect_ref, &sw);
		sw.data = NULL;
		(*m_in_data->utils->dispose_world)(m_in_data->effect_ref, &lw);
		lw.data = NULL;
	}
	if (mp.minusFlag){
		fromWhiteMat8();
	}else{
		fromBlackMat8();
	}
	


	return err;
}
//*********************************************************************************************
PF_Err CFsGraph::MinimaxA8(A_long value)
{
	PF_Err err = PF_Err_NONE;
	if (value == 0) return err;
	
	PF_InData *in_data;
	in_data = m_in_data;

	MiniMaxPrm mp;
	mp.value = F_ABS(value);
	mp.minusFlag = (value<0);
	mp.mode = SCANLINE::HorAndVer;
	mp.scanline = NULL;  // この行を追加
	mp.level = NULL;     // この行を追加

	A_long w = MAX(m_width,m_height);



	PF_EffectWorld sw;
	PF_EffectWorld lw;
	if (m_in_data != NULL && m_in_data->utils != NULL && m_in_data->effect_ref != NULL) {
		(*m_in_data->utils->new_world)(m_in_data->effect_ref, w / 4, 8, PF_PixelFormat_ARGB32, &sw);
		(*m_in_data->utils->new_world)(m_in_data->effect_ref, w / 4, 8, PF_PixelFormat_ARGB32, &lw);
		mp.scanline = sw.data;
		mp.level = (A_long*)(lw.data);
	}
	if (mp.scanline == NULL)
	{
		return PF_Err_OUT_OF_MEMORY;
	}
	

	//alpha
	Minimax_alpha8(&mp);	

	 
	if (m_in_data != NULL && m_scanlineWorld.data != NULL) {
		(*m_in_data->utils->dispose_world)(m_in_data->effect_ref, &sw);
		sw.data = NULL;
		(*m_in_data->utils->dispose_world)(m_in_data->effect_ref, &lw);
		lw.data = NULL;
	}
	


	return err;
}
//*********************************************************************************************
//----------------------------------------------------------------------------
PF_Err CFsGraph::Minimax(A_long value,ScanLineMode mode,TargetChannelMode target)
{
	PF_Err err = PF_Err_NONE; 
	if ( value !=0){
		switch(m_format)
		{
		case PF_PixelFormat_ARGB128:
			err = Minimax32(value,mode,target);
			break;
		case PF_PixelFormat_ARGB64:
			err = Minimax16(value,mode,target);
			break;
		case PF_PixelFormat_ARGB32:
			err = Minimax8(value,mode,target);
			break;
		}
	}
	return err;
}

