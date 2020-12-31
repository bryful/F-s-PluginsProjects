//FsAE
#ifndef FsAE_H
#define FsAE_H

#pragma once

#include "AEConfig.h"

#include "entry.h"
#include "AE_Effect.h"
#include "AE_EffectCB.h"
#include "AE_Macros.h"
#include "Param_Utils.h"
#include "FsGraphics.h"


#ifdef AE_OS_WIN
	#include <Windows.h>
#endif

//******************************************************************************
//
//******************************************************************************
class CFsAE{
//private:
protected:
	PF_Boolean			m_is16Bit;
	PF_Boolean			m_Enabled;
	long						m_Frame;
public:
	CFsAE(
		PF_InData			*in_data,
		PF_OutData		*out_data,
		PF_ParamDef		*params[],
		PF_LayerDef		*output)
		{
		//とりあえず初期化
			m_Enabled				= FALSE;
			m_is16Bit				= FALSE;
			if ( (params==NULL)||(output==NULL) ) return;
			
			CFsAE::in_data		= in_data;
			CFsAE::out_data		= out_data;
			CFsAE::input			= &params[0]->u.ld;	//入力画像のアドレス
			CFsAE::output			= output;
			m_is16Bit					= PF_WORLD_IS_DEEP( CFsAE::input );

			if ( input==NULL ) return;
			
			//カレントフレームを求める画頭は０
			if ( (in_data->current_time>=0)&&(in_data->time_step>0) ) {
				m_Frame	=(in_data->current_time/in_data->time_step); 
			}else{
				m_Frame = 0;
			}
			in		= new CFsGraph(CFsAE::input,in_data);
			out		= new CFsGraph(CFsAE::output,in_data);

			m_Enabled = TRUE;
		}
	
	
	~CFsAE(){
		delete in;
		delete out;
	}   // ←デストラクタ

	PF_InData				*in_data;
	PF_OutData			*out_data;
	PF_EffectWorld	*input;
	PF_EffectWorld	*output;
	CFsGraph*				in;
	CFsGraph*				out;

	PF_Boolean			Enabled(){ return m_Enabled;}
	PF_Boolean			is16Bit(){ return	m_is16Bit;}
	long						frame(){ return m_Frame; }

	PF_Err					Copy(){ return PF_COPY(input, output, NULL, NULL);}
	PF_Err					Blend(PF_Fixed per){
		if (m_Enabled==FALSE) return 0xFF;
		if (m_is16Bit==TRUE){
			FsBlend16(input,output,per);
		}else{
			FsBlend8(input,output,per);
		}
	}
};
//******************************************************************************
//
//******************************************************************************
//******************************************************************************

#endif
