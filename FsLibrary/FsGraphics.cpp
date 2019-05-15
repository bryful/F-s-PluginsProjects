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
	m_Enabled = FALSE;
	m_in_data	= NULL;
	m_world		= NULL;
	m_data		= NULL;
	m_format	= PF_PixelFormat_INVALID;

	m_width			= 0;
	m_widthTrue		= 0;
	m_height		= 0;
	m_offsetWidth	= 0;
	
	m_mat			= MAT::none;

	lineHeiht		= 1;

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
		m_Enabled	= TRUE;
	}


}
//******************************************************************************
/*
CFsGraph::CFsGraph(
		PF_EffectWorld *world, 
		PF_InData *in_data)
{
	PF_PixelFormat fromat	=	PF_PixelFormat_INVALID;
	if( PF_WORLD_IS_DEEP( world )){
		fromat = PF_PixelFormat_ARGB64;
	}else{
		fromat = PF_PixelFormat_ARGB32;
	}
	CFsGraph(world,in_data,fromat);
}
 */
//******************************************************************************
//******************************************************************************
