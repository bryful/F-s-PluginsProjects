#include "FsGraphics.h"


//******************************************************************************
/*
	CFsGraph
	描画用のクラス
*/
//******************************************************************************
	//-----------------------
CFsGraph::CFsGraph(
		PF_EffectWorld	*world, 
		PF_InData		*in_data,
		PF_PixelFormat	format)
{
	//メンバの初期化

	lineHeiht		= 1;

	m_frame = 0;
	if (in_data!=NULL){
		m_in_data = in_data;
		//カレントフレームを求める画頭は０
		if ( (in_data->current_time>=0)&&(in_data->time_step>0) ) {
			m_frame	=(in_data->current_time/in_data->time_step); 
		}
	}
	m_format = format;
	if ( world!=NULL){
		m_world		= world;
		m_data		= world->data;
		m_width		= m_world->width;
		m_height	= m_world->height;
		switch(m_format){
			case PF_PixelFormat_ARGB128:
				m_widthTrue	= m_world->rowbytes / sizeof(PF_PixelFloat);
				break;
			case PF_PixelFormat_ARGB64:
				m_widthTrue	= m_world->rowbytes / sizeof(PF_Pixel16);
				break;

			case PF_PixelFormat_ARGB32:
			default:
				m_widthTrue	= m_world->rowbytes / sizeof(PF_Pixel8);
				break;
		}
		m_offsetWidth	= m_widthTrue - m_width;
		m_vurTblH = PF_NEW_HANDLE(sizeof(A_long) * m_height * 2);

		if (m_vurTblH) {
			PF_LOCK_HANDLE(m_vurTblH);
			m_vurTbl = *(A_long**)m_vurTblH;
			for (A_long i = 0; i < m_height; i++)
			{
				m_vurTbl[i] = m_widthTrue*i;
			}

			m_Enabled = TRUE;
		}
	}


}
CFsGraph::~CFsGraph()
{
	if (m_vurTblH != NULL)
	{
		PF_InData* in_data = m_in_data;
		PF_UNLOCK_HANDLE(m_vurTblH);
		PF_DISPOSE_HANDLE(m_vurTblH);
	}
}
//******************************************************************************
//******************************************************************************
