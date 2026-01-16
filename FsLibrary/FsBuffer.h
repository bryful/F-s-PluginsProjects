
#pragma once

#ifndef FsBUFFER_H
#define FsBUFFER_H

//******************************************************************************

#include "Fs.h"

//******************************************************************************
class CFsBuffer{
private:
protected:
	PF_InData	*in_data;
	A_long		m_size;
	PF_Err		m_err;
	A_long		m_width;
	A_long		m_height;
	PF_EffectWorld	m_world;
	//-----------------------------
	void Init(){
		in_data		= NULL;
		m_size		= 0;
		m_err		= PF_Err_NONE;
		m_width		=0;
		m_height	=0;
		m_world.data = NULL;
	}
public:
	//-----------------------------
	PF_Err Create(A_long size)
	{
		PF_Err err	= PF_Err_NONE;
		if (in_data!= NULL) {

			try{
				PF_NewWorldFlags f = PF_NewWorldFlag_CLEAR_PIXELS | PF_NewWorldFlag_NONE;

				ERR((*in_data->utils->new_world)(in_data->effect_ref, size / 4, 8, PF_PixelFormat_ARGB32, &m_world));
				m_size = size;
				m_width = size;
				m_height = 1;
			}catch(PF_Err & e){
				err = e;
			}
		}else{
			err = PF_Err_OUT_OF_MEMORY;
		}
		return err;
	}
	//-----------------------------
	void Dispose(){
		if (( in_data !=NULL)&&(m_world.data!=NULL)){
			(*in_data->utils->dispose_world)(in_data->effect_ref, &m_world);
			m_world.data = NULL;
		}
	}
	//-----------------------------
	CFsBuffer(PF_InData	*in_dataP)
	{
		Init();
		in_data = in_dataP;
	}
	//-----------------------------
	CFsBuffer(PF_InData	*in_dataP,A_long w,A_long h,A_long size)
	{
		A_long sz = w * h * size;
		Init();
		if ((sz>0)&&(in_dataP !=NULL)){
			in_data = in_dataP;

			m_err = Create(sz);
			if (m_err==PF_Err_NONE){
				m_size		= sz; 
				m_width		= w;
				m_height	= h;
			}
		}else{
			m_err = PF_Err_OUT_OF_MEMORY;
		}
	}
	//-----------------------------
	CFsBuffer(PF_InData	*in_dataP,A_long size)
	{
		Init();
		if (in_dataP !=NULL){
			in_data = in_dataP;
			m_err = Create(size);
			if (!m_err){
				m_size		= size; 
				m_width		= size;
				m_height	= 1;
			}
		}else{
			m_err = PF_Err_OUT_OF_MEMORY;
		}
	}
	//-----------------------------
	~CFsBuffer(){
		Dispose();
	}
	//-----------------------------
	//-----------------------------
	PF_Err		err()		{ return m_err;}
	A_long*		bufA_long() { return (A_long*)m_world.data;}
	A_u_char*	bufA_u_char()	{ return (A_u_char*)m_world.data;}
	PF_FpShort*	bufFpShort()	{return (PF_FpShort*)m_world.data;}
	A_u_short*	bufA_u_short()	{ return (A_u_short*)m_world.data;}
	PF_PixelPtr	bufPixelPtr()	{ return (PF_PixelPtr)m_world.data;}
	PF_Pixel*	bufPixel() {	return (PF_Pixel*)m_world.data;}
	PF_Pixel16*	bufPixel16()	{ return (PF_Pixel16*)m_world.data;}
	PF_PixelFloat*	bufPixelFloat() { return (PF_PixelFloat*)m_world.data;}
	PF_Boolean	alive()			{ return (in_data!=NULL);}

	A_u_longlong size()
	{
		if (in_data != NULL)
			return m_world.width*m_world.height;
		else
			return 0;
	}
};


#endif
