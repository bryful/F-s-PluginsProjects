
#pragma once

#ifndef FsBUFFER_H
#define FsBUFFER_H

//******************************************************************************

/*
#include "AEConfig.h"
#include "entry.h"
//#include "PrSDKAESupport.h"
#include "AE_Effect.h"
#include "AE_EffectCB.h"
#include "AE_EffectCBSuites.h"
#include "AE_Macros.h"
#include "AEGP_SuiteHandler.h"
#include "String_Utils.h"
#include "Param_Utils.h"
#include "Smart_Utils.h"
*/

//******************************************************************************
class CFsBuffer{
private:
protected:
	PF_InData	*in_data;
	A_long		m_size;
	PF_Handle	m_bufH;
	PF_Err		m_err;
	PF_Boolean	m_isLock;
	A_long		m_width;
	A_long		m_height;
	//-----------------------------
	void Init(){
		in_data		= NULL;
		m_size		= 0;
		m_bufH		= 0;
		m_err		= PF_Err_NONE;
		m_isLock	= FALSE;
		m_width		=0;
		m_height	=0;
	}
public:
	//-----------------------------
	PF_Err Create(A_long size)
	{
		PF_Err err	= PF_Err_NONE;
		if (in_data!= NULL) {
			try{
				if (size>0) m_bufH = PF_NEW_HANDLE(size);
			}catch(PF_Err & e){
				err = e;
			}
			if (!m_bufH) {
				err = PF_Err_OUT_OF_MEMORY;
			}else{
				m_size		= size; 
				m_width		= size;
				m_height	= 1;
			}
		}else{
			err = PF_Err_OUT_OF_MEMORY;
		}
		return err;
	}
	//-----------------------------
	void Dispose(){
		if ( in_data !=NULL){
			if ( m_bufH) {
				if ( m_isLock ==TRUE){
					PF_UNLOCK_HANDLE(m_bufH);
					m_isLock = FALSE;
				}
				PF_DISPOSE_HANDLE(m_bufH);
				m_bufH = NULL;
				m_size = 0;
				m_err = PF_Err_NONE;
			}
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
	void Lock()
	{ 
		if ( in_data != NULL)
			if ( m_bufH != NULL)
				if ( m_isLock ==FALSE){
					PF_LOCK_HANDLE(m_bufH);
					m_isLock = TRUE;
				}
	}
	//-----------------------------
	void Unlock()
	{ 
		if ( in_data != NULL)
			if ( m_bufH != NULL)
				if ( m_isLock ==TRUE) {
					PF_LOCK_HANDLE(m_bufH);
					m_isLock = FALSE;
				}
	}
	//-----------------------------
	PF_Err		err()		{ return m_err;}
	PF_Handle	bufH()		{return m_bufH;}
	A_long*		bufA_long() { return *(A_long**)m_bufH;}
	A_u_char*	bufA_u_char()	{ return *(A_u_char**)m_bufH;}
	PF_FpShort*	bufFpShort()	{return *(PF_FpShort**)m_bufH;}
	A_u_short*	bufA_u_short()	{ return *(A_u_short**)m_bufH;}
	PF_PixelPtr	bufPixelPtr()	{ return *(PF_PixelPtr*)m_bufH;}
	PF_Pixel*	bufPixel() {	return *(PF_Pixel**)m_bufH;}
	PF_Pixel16*	bufPixel16()	{ return *(PF_Pixel16**)m_bufH;}
	PF_PixelFloat*	bufPixelFloat() { return *(PF_PixelFloat**)m_bufH;}
	PF_Boolean	alive()			{ return ((in_data!=NULL)&&(m_bufH!=NULL));}

	A_u_longlong size()
	{
		if ( m_bufH != NULL)
			return PF_GET_HANDLE_SIZE(m_bufH);
		else
			return 0;
	}
};


#endif