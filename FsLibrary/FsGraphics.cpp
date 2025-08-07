#include "FsGraphics.h"


CFsGraph::CFsGraph(
		PF_EffectWorld	*world, 
		PF_InData		*in_data,
		PF_PixelFormat	format)
{
	PF_Err err = PF_Err_NONE;
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

		PF_NewWorldFlags f = PF_NewWorldFlag_CLEAR_PIXELS | PF_NewWorldFlag_NONE;

		// NULLチェックを追加
		m_vurWorld.data = NULL;
		m_scanlineWorld.data = NULL;
		if (m_in_data != NULL && m_in_data->utils != NULL && m_in_data->effect_ref != NULL) {
			ERR((*m_in_data->utils->new_world)(m_in_data->effect_ref, m_height/4, 8, PF_PixelFormat_ARGB32, &m_vurWorld));
			if (m_vurWorld.data != NULL) {
				m_vurTbl = (A_long*)m_vurWorld.data;
				for (A_long i = 0; i < m_height; i++)
				{
					m_vurTbl[i] = m_widthTrue * i;
				}
				m_Enabled = TRUE;
			}
		}


	}
}
CFsGraph::~CFsGraph()
{
	if (m_in_data != NULL && m_vurWorld.data != NULL) {
		(*m_in_data->utils->dispose_world)(m_in_data->effect_ref, &m_vurWorld);
		m_vurWorld.data = NULL;
	}
	if (m_in_data != NULL && m_scanlineWorld.data != NULL) {
		(*m_in_data->utils->dispose_world)(m_in_data->effect_ref, &m_scanlineWorld);
		m_scanlineWorld.data = NULL;
	}
}
//******************************************************************************
//******************************************************************************
PF_Err CFsGraph::MakeScanlineworld(A_long size)
{
	PF_Err err = PF_Err_NONE;
	if (m_in_data != NULL && m_in_data->utils != NULL && m_in_data->effect_ref != NULL) {
		if (m_scanlineWorld.data == NULL) {
			ERR((*m_in_data->utils->new_world)(m_in_data->effect_ref, size / 4, 8, PF_PixelFormat_ARGB32, &m_scanlineWorld));
			if (m_scanlineWorld.data != NULL) {
				m_scanlineWorld.rowbytes = m_widthTrue * sizeof(PF_Pixel);
			} else {
				err = PF_Err_OUT_OF_MEMORY;
			}
		}
	}
	return err;
}