//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2026
*/
//-----------------------------------------------------------------------------------

//FsAE
#pragma once
#ifndef NF_AE_H
#define NF_AE_H

//******************************************************************************

#include "AE_SDK.h"

#include "NF_Utils.h"
#include "NF_Version.h"


#include <vector>
#define PF_Cmd_NONE -1
#define FsAE_ERR	PF_Err_BAD_CALLBACK_PARAM
//-----------------------------------------------------------------------------------
/*
	プラグインID獲得のための構造体
*/
typedef struct {
	PF_Boolean		initializedB;
	AEGP_PluginID	my_id;
} ae_global_data, *ae_global_dataP, **ae_global_dataH;


//-----------------------------------------------------------------------------------


//******************************************************************************
#define FS_SCRIPT_ALERT	"alert(%s);\r\n"

static const char FS_ABOUT_DIALOG[] = { 0x28,0x66,0x75,0x6e,0x63,0x74,0x69,0x6f,0x6e,0x28,0x6d,0x65,0x29,0x7b,0x0d,0x0a,0x09,0x76,0x61,0x72,0x20,0x73,0x74,0x72,0x4e,0x61,0x6d,0x65,0x20,0x3d,0x20,0x22,0x25,0x73,0x22,0x3b,0x0d,0x0a,0x09,0x76,0x61,0x72,0x20,0x73,0x74,0x72,0x56,0x65,0x72,0x73,0x69,0x6f,0x6e,0x20,0x3d,0x20,0x22,0x76,0x65,0x72,0x20,0x25,0x64,0x2e,0x25,0x64,0x20,0x5b,0x25,0x73,0x5d,0x22,0x3b,0x0d,0x0a,0x09,0x76,0x61,0x72,0x20,0x73,0x74,0x72,0x44,0x69,0x73,0x20,0x3d,0x20,0x22,0x25,0x73,0x22,0x3b,0x0d,0x0a,0x09,0x76,0x61,0x72,0x20,0x73,0x74,0x72,0x4d,0x79,0x4e,0x61,0x6d,0x65,0x20,0x3d,0x20,0x22,0x68,0x74,0x74,0x70,0x73,0x3a,0x2f,0x2f,0x67,0x69,0x74,0x68,0x75,0x62,0x2e,0x63,0x6f,0x6d,0x2f,0x62,0x72,0x79,0x66,0x75,0x6c,0x22,0x3b,0x0d,0x0a,0x20,0x20,0x20,0x20,0x2f,0x2f,0x76,0x61,0x72,0x20,0x6e,0x61,0x6e,0x61,0x65,0x20,0x3d,0x20,0x22,0x4e,0x61,0x6e,0x61,0x65,0x20,0x46,0x75,0x72,0x75,0x68,0x61,0x73,0x68,0x69,0x20,0x2d,0x20,0x4d,0x79,0x20,0x62,0x65,0x6c,0x6f,0x76,0x65,0x64,0x20,0x64,0x61,0x75,0x67,0x68,0x74,0x65,0x72,0x2e,0x20,0x4d,0x61,0x79,0x20,0x73,0x68,0x65,0x20,0x72,0x65,0x73,0x74,0x20,0x69,0x6e,0x20,0x70,0x65,0x61,0x63,0x65,0x2e,0x22,0x3b,0x0d,0x0a,0x09,0x76,0x61,0x72,0x20,0x77,0x69,0x6e,0x4f,0x62,0x6a,0x20,0x3d,0x20,0x6e,0x65,0x77,0x20,0x57,0x69,0x6e,0x64,0x6f,0x77,0x28,0x22,0x64,0x69,0x61,0x6c,0x6f,0x67,0x22,0x2c,0x20,0x22,0x4e,0x46,0x27,0x73,0x20,0x50,0x6c,0x75,0x67,0x69,0x6e,0x73,0x22,0x2c,0x20,0x5b,0x20,0x30,0x2c,0x20,0x20,0x30,0x2c,0x20,0x20,0x34,0x38,0x30,0x2c,0x20,0x31,0x38,0x30,0x5d,0x20,0x29,0x3b,0x0d,0x0a,0x09,0x76,0x61,0x72,0x20,0x65,0x64,0x46,0x73,0x4e,0x61,0x6d,0x65,0x20,0x3d,0x20,0x77,0x69,0x6e,0x4f,0x62,0x6a,0x2e,0x61,0x64,0x64,0x28,0x22,0x65,0x64,0x69,0x74,0x74,0x65,0x78,0x74,0x22,0x2c,0x20,0x5b,0x20,0x20,0x33,0x30,0x2c,0x20,0x20,0x20,0x31,0x32,0x2c,0x20,0x20,0x20,0x33,0x30,0x2b,0x20,0x34,0x34,0x30,0x2c,0x20,0x20,0x20,0x31,0x32,0x2b,0x20,0x20,0x32,0x30,0x5d,0x2c,0x20,0x73,0x74,0x72,0x4e,0x61,0x6d,0x65,0x2c,0x20,0x7b,0x20,0x72,0x65,0x61,0x64,0x6f,0x6e,0x6c,0x79,0x3a,0x74,0x72,0x75,0x65,0x2c,0x20,0x62,0x6f,0x72,0x64,0x65,0x72,0x6c,0x65,0x73,0x73,0x3a,0x74,0x72,0x75,0x65,0x20,0x7d,0x29,0x3b,0x0d,0x0a,0x09,0x76,0x61,0x72,0x20,0x65,0x64,0x46,0x73,0x56,0x65,0x72,0x73,0x69,0x6f,0x6e,0x20,0x3d,0x20,0x77,0x69,0x6e,0x4f,0x62,0x6a,0x2e,0x61,0x64,0x64,0x28,0x22,0x65,0x64,0x69,0x74,0x74,0x65,0x78,0x74,0x22,0x2c,0x20,0x5b,0x20,0x20,0x33,0x30,0x2c,0x20,0x20,0x20,0x34,0x32,0x2c,0x20,0x20,0x20,0x33,0x30,0x2b,0x20,0x34,0x34,0x30,0x2c,0x20,0x20,0x20,0x34,0x32,0x2b,0x20,0x32,0x30,0x5d,0x2c,0x20,0x73,0x74,0x72,0x56,0x65,0x72,0x73,0x69,0x6f,0x6e,0x2c,0x20,0x7b,0x20,0x72,0x65,0x61,0x64,0x6f,0x6e,0x6c,0x79,0x3a,0x74,0x72,0x75,0x65,0x2c,0x20,0x62,0x6f,0x72,0x64,0x65,0x72,0x6c,0x65,0x73,0x73,0x3a,0x74,0x72,0x75,0x65,0x20,0x7d,0x29,0x3b,0x0d,0x0a,0x09,0x76,0x61,0x72,0x20,0x65,0x64,0x46,0x73,0x44,0x69,0x73,0x20,0x3d,0x20,0x77,0x69,0x6e,0x4f,0x62,0x6a,0x2e,0x61,0x64,0x64,0x28,0x22,0x65,0x64,0x69,0x74,0x74,0x65,0x78,0x74,0x22,0x2c,0x20,0x5b,0x20,0x20,0x33,0x30,0x2c,0x20,0x20,0x20,0x37,0x32,0x2c,0x20,0x20,0x20,0x33,0x30,0x2b,0x20,0x34,0x34,0x30,0x2c,0x20,0x20,0x20,0x37,0x32,0x2b,0x20,0x20,0x32,0x30,0x5d,0x2c,0x20,0x73,0x74,0x72,0x44,0x69,0x73,0x2c,0x20,0x7b,0x20,0x72,0x65,0x61,0x64,0x6f,0x6e,0x6c,0x79,0x3a,0x74,0x72,0x75,0x65,0x2c,0x20,0x62,0x6f,0x72,0x64,0x65,0x72,0x6c,0x65,0x73,0x73,0x3a,0x74,0x72,0x75,0x65,0x20,0x7d,0x29,0x3b,0x0d,0x0a,0x09,0x76,0x61,0x72,0x20,0x65,0x64,0x4d,0x79,0x4e,0x61,0x6d,0x65,0x20,0x3d,0x20,0x77,0x69,0x6e,0x4f,0x62,0x6a,0x2e,0x61,0x64,0x64,0x28,0x22,0x65,0x64,0x69,0x74,0x74,0x65,0x78,0x74,0x22,0x2c,0x20,0x5b,0x20,0x20,0x33,0x30,0x2c,0x20,0x20,0x31,0x30,0x32,0x2c,0x20,0x20,0x20,0x33,0x30,0x2b,0x20,0x34,0x34,0x30,0x2c,0x20,0x20,0x31,0x30,0x32,0x2b,0x20,0x20,0x32,0x30,0x5d,0x2c,0x20,0x73,0x74,0x72,0x4d,0x79,0x4e,0x61,0x6d,0x65,0x2c,0x20,0x7b,0x20,0x72,0x65,0x61,0x64,0x6f,0x6e,0x6c,0x79,0x3a,0x74,0x72,0x75,0x65,0x2c,0x20,0x62,0x6f,0x72,0x64,0x65,0x72,0x6c,0x65,0x73,0x73,0x3a,0x74,0x72,0x75,0x65,0x20,0x7d,0x29,0x3b,0x0d,0x0a,0x20,0x20,0x20,0x20,0x2f,0x2f,0x76,0x61,0x72,0x20,0x73,0x74,0x4e,0x61,0x6e,0x61,0x20,0x3d,0x20,0x77,0x69,0x6e,0x4f,0x62,0x6a,0x2e,0x61,0x64,0x64,0x28,0x22,0x73,0x74,0x61,0x74,0x69,0x63,0x74,0x65,0x78,0x74,0x22,0x2c,0x20,0x5b,0x20,0x20,0x31,0x38,0x30,0x2c,0x20,0x20,0x31,0x36,0x33,0x2c,0x20,0x20,0x20,0x31,0x38,0x30,0x2b,0x20,0x34,0x34,0x30,0x2c,0x20,0x20,0x31,0x36,0x33,0x2b,0x20,0x20,0x32,0x30,0x5d,0x2c,0x20,0x6e,0x61,0x6e,0x61,0x65,0x2c,0x20,0x7b,0x20,0x72,0x65,0x61,0x64,0x6f,0x6e,0x6c,0x79,0x3a,0x74,0x72,0x75,0x65,0x2c,0x20,0x62,0x6f,0x72,0x64,0x65,0x72,0x6c,0x65,0x73,0x73,0x3a,0x74,0x72,0x75,0x65,0x20,0x7d,0x29,0x3b,0x0d,0x0a,0x09,0x76,0x61,0x72,0x20,0x62,0x74,0x6e,0x4f,0x4b,0x20,0x3d,0x20,0x77,0x69,0x6e,0x4f,0x62,0x6a,0x2e,0x61,0x64,0x64,0x28,0x22,0x62,0x75,0x74,0x74,0x6f,0x6e,0x22,0x2c,0x20,0x5b,0x20,0x33,0x36,0x30,0x2c,0x20,0x20,0x31,0x34,0x30,0x2c,0x20,0x20,0x33,0x36,0x30,0x2b,0x20,0x31,0x30,0x30,0x2c,0x20,0x20,0x31,0x34,0x30,0x2b,0x20,0x20,0x32,0x34,0x5d,0x2c,0x20,0x22,0x4f,0x4b,0x22,0x20,0x2c,0x20,0x7b,0x20,0x6e,0x61,0x6d,0x65,0x3a,0x22,0x6f,0x6b,0x22,0x20,0x7d,0x29,0x3b,0x0d,0x0a,0x09,0x77,0x69,0x6e,0x4f,0x62,0x6a,0x2e,0x63,0x65,0x6e,0x74,0x65,0x72,0x28,0x29,0x3b,0x0d,0x0a,0x09,0x77,0x69,0x6e,0x4f,0x62,0x6a,0x2e,0x73,0x68,0x6f,0x77,0x28,0x29,0x3b,0x0d,0x0a,0x7d,0x29,0x28,0x74,0x68,0x69,0x73,0x29,0x3b,0x0d,0x0a,0x00 };

#define FsAE_ITEM_COUNT	256

//******************************************************************************
//-----------------------------------------------------------------------------

typedef struct {
	A_long			width;
	A_long			height;
	A_long			widthTrue;
	A_long			offsetWidth;
}WORLDInfo;
//******************************************************************************
class NF_AE {
private:
	
protected:
	PF_PixelFormat		m_format;
	A_long				m_frame;
	A_long				m_frameCount;
	PF_Cmd				m_cmd;
	A_long				m_paramsCount;
	PF_Err				m_resultErr;
	PF_Boolean			m_isGetEffectStream;

public:
	//--------------------------------------------------------------------
	PF_InData			*in_data;
	PF_OutData			*out_data;
	PF_ParamDef			*params[FsAE_ITEM_COUNT];

	PF_SmartRenderExtra	*SRextraP;
	PF_PreRenderExtra	*PRextraP;
	PF_WorldSuite2		*ws2P;

	PF_EffectWorld		*input;
	PF_EffectWorld		*output;
	
	WORLDInfo			inputInfo;
	WORLDInfo			outputInfo;

	PF_Handle			PreRenderH;
	AEGP_SuiteHandler	*suitesP;
	PF_Handle			ae_plugin_idH;
	ae_global_dataP		ae_plugin_idP;
	AEGP_StreamRefH		ae_item_streamH[FsAE_ITEM_COUNT];
	AEGP_EffectRefH		ae_effect_refH;


	//*********************************************************************************
	//*********************************************************************************
	void Init()
	{
		m_format			= PF_PixelFormat_INVALID;
		m_frame				= 0;
		m_frameCount		= 0;
		m_cmd				= PF_Cmd_NONE;
		m_paramsCount		= 0;
		m_isGetEffectStream	= FALSE;
		m_resultErr			= PF_Err_NONE;

		AEFX_CLR_STRUCT(NF_AE::inputInfo);
		AEFX_CLR_STRUCT(NF_AE::outputInfo);


		NF_AE::in_data		= NULL;
		NF_AE::out_data		= NULL;
		NF_AE::input		= NULL;
		NF_AE::output		= NULL;

		NF_AE::PreRenderH	= NULL;
		NF_AE::PRextraP		= NULL;
		NF_AE::SRextraP		= NULL;
		NF_AE::ws2P			= NULL;
		NF_AE::suitesP		= NULL;
		NF_AE::ae_plugin_idH	= NULL;
		NF_AE::ae_plugin_idP	= NULL;

		NF_AE::ae_effect_refH	= NULL;
		for (A_long i=0; i<FsAE_ITEM_COUNT; i++) NF_AE::ae_item_streamH[i]=NULL;

	}
	//*********************************************************************************
	NF_AE()
	{
		Init();
	}
	//*********************************************************************************
	PF_Err GetFrame(PF_InData *in_dataP)
	{
		PF_Err err = PF_Err_NONE;
		if (in_dataP == NULL) {
			err = PF_Err_BAD_CALLBACK_PARAM;
			return err;
		}
		m_frame = 0;
		m_frameCount = 0;
		in_data = in_dataP;
		//カレントフレームを求める画頭は０
		if ((m_cmd == PF_Cmd_RENDER) || (m_cmd == PF_Cmd_SMART_RENDER))
		{
			if ((in_dataP->current_time >= 0) && (in_dataP->time_step > 0)) {
				m_frame = (in_dataP->current_time / in_dataP->time_step);
			}
			if (in_dataP->local_time_step > 0)
			{
				m_frameCount = in_dataP->total_time / in_dataP->local_time_step;
			}
		}
		return err;
	}
	//*********************************************************************************
	PF_Err GetSuites(PF_InData* in_dataP, PF_OutData* out_dataP)
	{
		PF_Err err = PF_Err_NONE;
		if (in_dataP == NULL) {
			err = PF_Err_BAD_CALLBACK_PARAM;
			return err;
		};
		NF_AE::in_data = in_dataP;
		NF_AE::suitesP = new AEGP_SuiteHandler(in_dataP->pica_basicP);
		
		if (NF_AE::in_data->global_data) {
			NF_AE::ae_plugin_idH = NF_AE::in_data->global_data;
			NF_AE::ae_plugin_idP = reinterpret_cast<ae_global_dataP>(DH(NF_AE::in_data->global_data));
		}
		if (out_dataP != NULL) {
			NF_AE::out_data = out_dataP;
			ERR(AEFX_AcquireSuite(in_dataP,
				out_dataP,
				kPFWorldSuite,
				kPFWorldSuiteVersion2,
				"Couldn't load suite.",
				(void**)&(NF_AE::ws2P)));

		}

		return err;
	}
	PF_Err GetPixelFormat()
	{
		PF_Err err = PF_Err_NONE;
		if (NF_AE::input == NULL) {
			err = PF_Err_BAD_CALLBACK_PARAM;
			return err;
		}
		if (NF_AE::ws2P != NULL) {
			err = NF_AE::ws2P->PF_GetPixelFormat(NF_AE::input, &m_format);
		}
		else {
			if(PF_WORLD_IS_DEEP(NF_AE::input) == TRUE) {
				m_format = PF_PixelFormat_ARGB64;
			}
			else {
				m_format = PF_PixelFormat_ARGB32;
			}
		}
		return err;
	}
	PF_Err GetWorldSize()
	{
		PF_Err err = PF_Err_NONE;

		if (NF_AE::input != NULL) {
			inputInfo.width = NF_AE::input->width;
			inputInfo.height = NF_AE::input->height;
			switch (m_format)
			{
			case PF_PixelFormat_ARGB128:
				inputInfo.widthTrue = NF_AE::input->rowbytes / sizeof(PF_PixelFloat);

				break;
			case PF_PixelFormat_ARGB64:
				inputInfo.widthTrue = NF_AE::input->rowbytes / sizeof(PF_Pixel16);
				break;
			case PF_PixelFormat_ARGB32:
				inputInfo.widthTrue = NF_AE::input->rowbytes / sizeof(PF_Pixel);
				break;
			default:
				err = PF_Err_BAD_CALLBACK_PARAM;
				break;
			}
			inputInfo.offsetWidth = inputInfo.widthTrue - inputInfo.width;
		}
		if (NF_AE::output != NULL) {
			outputInfo.width = NF_AE::output->width;
			outputInfo.height = NF_AE::output->height;
			switch (m_format)
			{
			case PF_PixelFormat_ARGB128:
				outputInfo.widthTrue = NF_AE::output->rowbytes / sizeof(PF_PixelFloat);

				break;
			case PF_PixelFormat_ARGB64:
				outputInfo.widthTrue = NF_AE::output->rowbytes / sizeof(PF_Pixel16);
				break;
			case PF_PixelFormat_ARGB32:
				outputInfo.widthTrue = NF_AE::output->rowbytes / sizeof(PF_Pixel);
				break;
			default:
				err = PF_Err_BAD_CALLBACK_PARAM;
				break;
			}
			outputInfo.offsetWidth = outputInfo.widthTrue - outputInfo.width;
		}
		return err;
	}
	//*********************************************************************************
	//Render
	//*********************************************************************************
	//--------------------------------
	NF_AE(
		PF_InData		*in_dataP,
		PF_OutData		*out_dataP,
		PF_ParamDef		*paramsP[],
		PF_LayerDef		*outputP,
		A_long			paramsCount
		)
	{
		m_resultErr = Render(in_dataP,out_dataP,paramsP,outputP,paramsCount);
	}
	//--------------------------------
	PF_Err Render(
		PF_InData		*in_dataP,
		PF_OutData		*out_dataP,
		PF_ParamDef		*paramsP[],
		PF_LayerDef		*outputP,
		A_long			paramsCount
		)
	{
		Init();
		PF_Err err = PF_Err_NONE;
		m_cmd = PF_Cmd_RENDER;
		if ( (in_dataP ==NULL) || ( out_dataP ==NULL)|| (paramsP ==NULL) || (outputP ==NULL)||(paramsCount<=0)){
			m_resultErr = FsAE_ERR;
			return m_resultErr;
		}

		NF_AE::in_data = in_dataP;
		NF_AE::out_data = out_dataP;
		NF_AE::output = outputP;
		m_paramsCount = paramsCount;
		if (paramsP != NULL) {
			NF_AE::input = &paramsP[0]->u.ld;
			for (A_long i = 0; i < paramsCount; i++) NF_AE::params[i] = paramsP[i];
		}

		ERR(GetFrame(in_dataP));
		ERR(GetSuites(in_dataP,out_dataP));
		ERR(GetPixelFormat());
		ERR(GetWorldSize());	

		m_resultErr		= err;

		return err;
	}
	//*********************************************************************************
	//PreRender
	//*********************************************************************************
	//--------------------------------
	NF_AE(
		PF_InData			*in_dataP,
		PF_OutData			*out_dataP,
		PF_PreRenderExtra	*extraP,
		A_long				infoSize,
		A_long				paramsCount
		)
	{
		m_resultErr = PreRender(in_dataP,out_dataP,extraP,infoSize,paramsCount);
	}
	//--------------------------------
	PF_Err PreRender(
		PF_InData			*in_dataP,
		PF_OutData			*out_dataP,
		PF_PreRenderExtra	*extraP,
		A_long				infoSize,
		A_long				paramsCount
		)
	{
		Init();
		PF_Err err = PF_Err_NONE;
		m_cmd = PF_Cmd_SMART_PRE_RENDER;
		if ((in_dataP ==NULL) || ( out_dataP ==NULL)|| (extraP ==NULL)){
			m_resultErr = FsAE_ERR;
			return m_resultErr;
		}
		NF_AE::in_data		= in_dataP;
		NF_AE::out_data		= out_dataP;
		NF_AE::PRextraP		= extraP;
		m_paramsCount		= paramsCount;
		
		err = GetFrame(in_dataP);
		err = GetSuites(in_dataP,out_dataP);

		if (infoSize > 0) {
			NewPreRenderData(infoSize);
			if (PreRenderH == NULL) {
				m_resultErr = PF_Err_OUT_OF_MEMORY;
				m_cmd = PF_Cmd_NONE;
				return m_resultErr;
			}
		}
		m_resultErr		= err;
		return err;
	}
	//*********************************************************************************
	//SmartRender
	//*********************************************************************************
	//--------------------------------
	NF_AE(
		PF_InData				*in_dataP,
		PF_OutData				*out_dataP,
		PF_SmartRenderExtra		*extraP,
		A_long					paramsCount
		)
	{
		m_resultErr = SmartRender(in_dataP, out_dataP, extraP, paramsCount);
	}
	//--------------------------------
	PF_Err SmartRender(
		PF_InData				*in_dataP,
		PF_OutData				*out_dataP,
		PF_SmartRenderExtra		*extraP,
		A_long					paramsCount
		)
	{
		Init();
		PF_Err	err		= PF_Err_NONE;
		m_cmd = PF_Cmd_SMART_RENDER;
		if ((in_dataP ==NULL) || ( out_dataP ==NULL)|| (extraP ==NULL)){
			m_resultErr = FsAE_ERR;
			return m_resultErr;
		}
		NF_AE::in_data = in_dataP;
		NF_AE::out_data = out_dataP;
		NF_AE::SRextraP = extraP;
		m_paramsCount = paramsCount;
		ERR((extraP->cb->checkout_layer_pixels(in_dataP->effect_ref, 0, &(NF_AE::input))));
		ERR(extraP->cb->checkout_output(in_dataP->effect_ref, &(NF_AE::output)));

		ERR(GetFrame(in_dataP));
		ERR(GetSuites(in_dataP,out_dataP));
		ERR(GetPixelFormat());
		ERR(GetWorldSize());

		m_resultErr = err;
		return err;
	}
	//*********************************************************************************
	//*********************************************************************************
	//その他の処理
	//*********************************************************************************
	PF_Err About
	(
		PF_InData		*in_dataP,
		PF_OutData		*out_dataP,
		PF_ParamDef		*paramsP[],
		PF_LayerDef		*outputP)
	{
		(void)paramsP;  // 未使用パラメータの警告を抑制
		(void)outputP;  // 未使用パラメータの警告を抑制
		PF_Err	err				= PF_Err_NONE;

		m_cmd = PF_Cmd_ABOUT;
		in_data = in_dataP;
		out_data = out_dataP;
		output = outputP;
		GetFrame(in_dataP);
		GetSuites(in_dataP,out_dataP);

		if (ae_plugin_idP!=NULL){
			
			A_char scriptCode[2048] = {'\0'}; 
			PF_SPRINTF(	scriptCode,FS_ABOUT_DIALOG,
				NF_NAME, 
				MAJOR_VERSION, 
				MINOR_VERSION, 
				__DATE__,
				NF_DESCRIPTION);
			
			ERR(suitesP->UtilitySuite5()->AEGP_ExecuteScript(ae_plugin_idP->my_id, scriptCode, TRUE, NULL, NULL));

		}else{
			PF_SPRINTF(	out_dataP->return_msg, 
				"%s, v%d.%d (%s)\r%s",
				NF_NAME, 
				MAJOR_VERSION, 
				MINOR_VERSION, 
				__DATE__,
				NF_DESCRIPTION);
		}
		m_resultErr = err;
		return err;
	}
	//*********************************************************************************
	PF_Err GlobalSetup
	(
		PF_InData		*in_dataP,
		PF_OutData		*out_dataP,
		PF_ParamDef		*paramsP[],
		PF_LayerDef		*outputP)
	{
		PF_Err	err				= PF_Err_NONE;
		Init();
		m_cmd = PF_Cmd_GLOBAL_SETUP;
		in_data = in_dataP;
		out_data = out_dataP;
		output = outputP;
		GetSuites(in_dataP, out_dataP);	

		//NF_Target.hで定義
		out_data->my_version	=	NF_VERSION;
		out_data->out_flags		=	NF_OUT_FLAGS;
		out_data->out_flags2	=	NF_OUT_FLAGS2;

		/**********************************************************
		プラグインIDを獲得して、グローバルにアクセスできるように保存
		*/
		ae_plugin_idH	=	suitesP->HandleSuite1()->host_new_handle(sizeof(ae_global_data));
	
		if (ae_plugin_idH) {
			ae_plugin_idP = reinterpret_cast<ae_global_dataP>(suitesP->HandleSuite1()->host_lock_handle(ae_plugin_idH));
			if (ae_plugin_idP) {
				ae_plugin_idP->initializedB 	= TRUE;

				if (in_data->appl_id != 'PrMr') {
					ERR(suitesP->UtilitySuite3()->AEGP_RegisterWithAEGP(NULL, NF_NAME, &ae_plugin_idP->my_id));
				}
				if (!err){
					out_data->global_data 	= ae_plugin_idH;
				}
			}
			suitesP->HandleSuite1()->host_unlock_handle(ae_plugin_idH);
		} else	{
			err = PF_Err_INTERNAL_STRUCT_DAMAGED;
		}
		//**********************************************************

		return err;

	}
	//*********************************************************************************
	PF_Err GlobalSetdown(PF_InData	*in_dataP)
	{
		PF_Err	err				= PF_Err_NONE;
		Init();
		m_cmd = PF_Cmd_GLOBAL_SETDOWN;
		in_data = in_dataP;
		suitesP = new AEGP_SuiteHandler(in_dataP->pica_basicP);

		//ae_plugin_idH
		if (in_data->global_data) {
			suitesP->HandleSuite1()->host_dispose_handle(in_data->global_data);
		}

		return err;
	}
	//*********************************************************************************
	PF_Err HandleChangedParam(
		PF_InData					*in_dataP,
		PF_OutData					*out_dataP,
		PF_ParamDef					*paramsP[],
		PF_LayerDef					*outputP,
		PF_UserChangedParamExtra	*extraP,
		A_long						paramsCount
		)
	{
		PF_Err	err				= PF_Err_NONE;
		Init();
		m_cmd = PF_Cmd_USER_CHANGED_PARAM;

		if ( (in_dataP ==NULL) || ( out_dataP ==NULL)||(paramsCount<=0)){
			m_resultErr = FsAE_ERR;
			return m_resultErr;
		}
		NF_AE::in_data		= in_dataP;
		NF_AE::out_data		= out_dataP;
		m_paramsCount = paramsCount;
		if (paramsP != NULL) {
			NF_AE::input = &paramsP[0]->u.ld;
			for (A_long i = 0; i < paramsCount; i++) NF_AE::params[i] = paramsP[i];
		}

		ERR(GetFrame(in_dataP));
		ERR(GetSuites(in_dataP,out_dataP));
		ERR(GetPixelFormat());
		

		if ( ae_plugin_idP){
			err = suitesP->PFInterfaceSuite1()->AEGP_GetNewEffectForEffect(ae_plugin_idP->my_id, in_dataP->effect_ref, &ae_effect_refH);
		}

		return err;
	}
	//*********************************************************************************
	PF_Err
	QueryDynamicFlags(	
		PF_InData		*in_dataP,	
		PF_OutData		*out_dataP,	
		PF_ParamDef		*paramsP[],	
		void			*extraP,
		A_long			paramsCount
		)	
	{
		PF_Err	err				= PF_Err_NONE;
		Init();
		m_cmd = PF_Cmd_QUERY_DYNAMIC_FLAGS;
		m_paramsCount = paramsCount;
		if ( (in_dataP ==NULL) || ( out_dataP ==NULL)|| (paramsCount<=0)){
			m_resultErr = FsAE_ERR;
			return m_resultErr;
		}
		NF_AE::in_data = in_dataP;
		NF_AE::out_data = out_dataP;
		if (paramsP != NULL) {
			NF_AE::input = &paramsP[0]->u.ld;
			for (A_long i = 0; i < paramsCount; i++) NF_AE::params[i] = paramsP[i];
		}

		ERR(GetFrame(in_dataP));
		ERR(GetSuites(in_dataP,out_dataP));


		return err;

	}
	//*********************************************************************************
	//デストラクタ
	//*********************************************************************************
	~NF_AE(){
		PF_Err 	err 	= PF_Err_NONE,
				err2 	= PF_Err_NONE;
		if (ae_effect_refH!=NULL){
			suitesP->EffectSuite2()->AEGP_DisposeEffect(ae_effect_refH);
			ae_effect_refH=NULL;
		}
		if (m_paramsCount>0){
			if ( m_isGetEffectStream==TRUE){
				for ( A_long i=0; i<m_paramsCount; i++){
					if (ae_item_streamH[i]!=NULL){
						ERR2(suitesP->StreamSuite2()->AEGP_DisposeStream(ae_item_streamH[i]));
						ae_item_streamH[i]=NULL;
					}
				}
			}
		}


		if ( suitesP != NULL) {
			delete suitesP;
			suitesP = NULL;
		}

		if (NF_AE::ws2P != NULL){
			ERR2(AEFX_ReleaseSuite(	in_data,
									out_data,
									kPFWorldSuite, 
									kPFWorldSuiteVersion2, 
									"Couldn't release suite."));
		}
	}
	//*********************************************************************************
	//--------------------------------------------------------------------
	PF_Handle NewPreRenderData(A_long handleSize)
	{
		if (m_cmd == PF_Cmd_SMART_PRE_RENDER){
			PreRenderH = suitesP->HandleSuite1()->host_new_handle(handleSize);
			return PreRenderH;
		}else{
			return NULL;
		}
	}
	//--------------------------------------------------------------------
	void * LockPreRenderData()
	{
		if (m_cmd == PF_Cmd_SMART_PRE_RENDER){
			if ( PreRenderH != NULL){
				return suitesP->HandleSuite1()->host_lock_handle(PreRenderH);
			}
		}else if (m_cmd == PF_Cmd_SMART_RENDER){
			if ( SRextraP != NULL){
				return suitesP->HandleSuite1()->host_lock_handle(reinterpret_cast<PF_Handle>(SRextraP->input->pre_render_data));
			}
		}
		return NULL;
	}
	//--------------------------------------------------------------------
	void  UnlockPreRenderData()
	{
		if (m_cmd == PF_Cmd_SMART_PRE_RENDER){
			if ( PreRenderH != NULL){
				suitesP->HandleSuite1()->host_unlock_handle(PreRenderH);
			}
		}else if (m_cmd == PF_Cmd_SMART_RENDER){
			if ( SRextraP != NULL){
				suitesP->HandleSuite1()->host_lock_handle(reinterpret_cast<PF_Handle>(SRextraP->input->pre_render_data));
			}
		}
	}
	//--------------------------------------------------------------------
	PF_Boolean SetHostPreRenderData()
	{
		PF_Boolean ret = FALSE;
		if ( m_cmd == PF_Cmd_SMART_PRE_RENDER){
			if ( PreRenderH != NULL){
				PRextraP->output->pre_render_data = PreRenderH;
				ret = TRUE;
			}
		}
		return ret;
	}
	//--------------------------------------------------------------------
	PF_Err UnSetPreRenderData(PF_RenderRequest *req, PF_CheckoutResult *in_result)
	{
		PF_Err err = PF_Err_NONE;
		if ( m_cmd == PF_Cmd_SMART_PRE_RENDER){
			ERR(PRextraP->cb->checkout_layer(	in_data->effect_ref,
											0,
											0,
											req,
											in_data->current_time,
											in_data->time_step,
											in_data->time_scale,
											in_result));
			
			UnionLRect(&in_result->result_rect, 		&PRextraP->output->result_rect);
			UnionLRect(&in_result->max_result_rect, 	&PRextraP->output->max_result_rect);		
		}
		return err;
	}
	//--------------------------------------------------------------------
	PF_Err UnSetPreRenderData()
	{
		PF_RenderRequest req = PRextraP->input->output_request;
		PF_CheckoutResult in_result;
		return UnSetPreRenderData(&req,&in_result);
	}
	//--------------------------------------------------------------------
	PF_Err UnsetSmartRender()
	{
		return SRextraP->cb->checkin_layer_pixels(in_data->effect_ref, 0);
	}
	//--------------------------------------------------------------------
	PF_Err UI_DISABLE(A_long idx,PF_Boolean b)
	{
		PF_Err err = PF_Err_NONE;
		if ((idx<0)||(idx>=m_paramsCount)||(params == NULL))
		{
			err = PF_Err_BAD_CALLBACK_PARAM;
		}else{
			if (b == TRUE){
				params[idx]->ui_flags	|= PF_PUI_DISABLED;
			}else{
				params[idx]->ui_flags	&= ~PF_PUI_DISABLED;
			}
#if defined(PF_AE110_PLUG_IN_VERSION)
			err =suitesP->ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref, idx,params[idx]);
#else
			err =suitesP->ParamUtilsSuite1()->PF_UpdateParamUI(in_data->effect_ref, idx,params[idx]);
#endif
		}
		return err;
	}
	//--------------------------------------------------------------------
	PF_Err UpdateParamUI(A_long idx)
	{
		PF_Err err = PF_Err_NONE;
		if ((idx<0)||(idx>=m_paramsCount)||(params == NULL)||(in_data==NULL))
		{
			err = PF_Err_BAD_CALLBACK_PARAM;
		}else{
#if defined(PF_AE110_PLUG_IN_VERSION)
			err =suitesP->ParamUtilsSuite3()->PF_UpdateParamUI(in_data->effect_ref, idx,params[idx]);
#else
			err =suitesP->ParamUtilsSuite1()->PF_UpdateParamUI(in_data->effect_ref, idx,params[idx]);
#endif
		}
		return err;
	}
	//--------------------------------------------------------------------
	PF_Err GetNewEffectStreamAll()
	{
		PF_Err err = PF_Err_NONE;
		for ( A_long i=0; i<FsAE_ITEM_COUNT; i++) ae_item_streamH[i] = NULL;
		if ( m_paramsCount>0){
			if ( (ae_plugin_idP)&&(ae_effect_refH)){
				for ( A_long i=1; i<m_paramsCount; i++){
					ERR(suitesP->StreamSuite2()->AEGP_GetNewEffectStreamByIndex(ae_plugin_idP->my_id, ae_effect_refH, i, &ae_item_streamH[i]));
				}
			}else{
				err = FsAE_ERR;
			}
		}
		if (!err) m_isGetEffectStream =TRUE;
		return err;
	}
	//--------------------------------------------------------------------
	PF_Err SetDynamicStreamFlag(A_long idx,AEGP_DynStreamFlags flg,A_Boolean setB)
	{
		PF_Err err = PF_Err_NONE;
		if ( (m_paramsCount>0)&&(m_isGetEffectStream ==TRUE)){
			if ( (idx>=1)&&(idx<m_paramsCount)){
				if ( (ae_plugin_idP)&&(ae_effect_refH)){
					err = suitesP->DynamicStreamSuite2()->AEGP_SetDynamicStreamFlag(ae_item_streamH[idx],flg,FALSE,setB);
				}else{
					err = FsAE_ERR;
				}
			}else{
				err = PF_Err_INVALID_INDEX;
			}
		}
		return err;
	}
	//--------------------------------------------------------------------
	PF_Err DisposeEffectStream()
	{
		PF_Err 	err 	= PF_Err_NONE,
				err2 	= PF_Err_NONE;

		for ( A_long i=0; i<FsAE_ITEM_COUNT; i++){
			if (ae_item_streamH[i]!=NULL){
				ERR2(suitesP->StreamSuite2()->AEGP_DisposeStream(ae_item_streamH[i]));
				ae_item_streamH[i]=NULL;
			}
		}
		if (ae_effect_refH!=NULL){
			ERR2(suitesP->EffectSuite2()->AEGP_DisposeEffect(ae_effect_refH));
			ae_effect_refH=NULL;
		}

		return err;
	}

	//--------------------------------------------------------------------
	//*********************************************************************************
	PF_Err checkout_param(A_long idx,PF_ParamDef *paramP)
	{
		PF_Err err = PF_Err_NONE;
		if (( idx >=1)&&(idx < m_paramsCount)) {
			switch(m_cmd)
			{
			case PF_Cmd_SMART_PRE_RENDER:
			case PF_Cmd_QUERY_DYNAMIC_FLAGS:
				//PF_ParamDef param;
				//AEFX_CLR_STRUCT(param);
				ERR(PF_CHECKOUT_PARAM( 
					in_data, 
					idx, 
					in_data->current_time, in_data->time_step,in_data->time_scale, 
					paramP));
				if (!err) {
					//*paramP = param;
				}else{
					err = PF_Err_BAD_CALLBACK_PARAM;
				}
				break;
			default:
				err = PF_Err_BAD_CALLBACK_PARAM;
				break;
			}
		}else{
			err = PF_Err_INVALID_INDEX;
		}
		return err;
	}
	//*********************************************************************************
	PF_Err GetADD(A_long idx,A_long *a)
	{
		PF_Err err = PF_Err_NONE;
		A_long ret =0;
		if (( idx >=1)&&(idx < m_paramsCount)) {
			switch(m_cmd)
			{
			case PF_Cmd_SMART_PRE_RENDER:
			//case FsAE_QUERY_DYNAMIC_FLAGS:
				PF_ParamDef param;
				AEFX_CLR_STRUCT(param);
				ERR(PF_CHECKOUT_PARAM( 
					in_data, 
					idx, 
					in_data->current_time, in_data->time_step,in_data->time_scale, 
					&param));
				if (!err) {
					ret = param.u.sd.value;
				}else{
					err = PF_Err_BAD_CALLBACK_PARAM;
				}
				break;
			case PF_Cmd_RENDER:
			case PF_Cmd_USER_CHANGED_PARAM:
				ret = params[idx]->u.sd.value;
				break;
			default:
				err = PF_Err_BAD_CALLBACK_PARAM;
				break;
			}
		}else{
				err = PF_Err_INVALID_INDEX;
		}
		*a = ret;
		return err;
	}
	//*********************************************************************************
	PF_Err SetADD(A_long idx, A_long a)
	{
		PF_Err err = PF_Err_NONE;
		A_long ret = 0;
		if ((idx >= 1) && (idx < m_paramsCount)) {
			switch (m_cmd)
			{
			case PF_Cmd_RENDER:
			case PF_Cmd_USER_CHANGED_PARAM:
				params[idx]->u.sd.value = a;
				params[idx]->uu.change_flags = PF_ChangeFlag_CHANGED_VALUE;
				break;
			default:
				err = PF_Err_BAD_CALLBACK_PARAM;
				break;
			}
		}
		else {
			err = PF_Err_INVALID_INDEX;
		}
		return err;
	}//--------------------------------------------------------------------
	PF_Err GetFIXED(A_long idx,PF_Fixed *f)
	{
		PF_Err err = PF_Err_NONE;
		PF_Fixed ret =0;
		if (( idx >=1)&&(idx < m_paramsCount)) {
			switch(m_cmd)
			{
			case PF_Cmd_SMART_PRE_RENDER:
			//case FsAE_QUERY_DYNAMIC_FLAGS:
				PF_ParamDef param;
				AEFX_CLR_STRUCT(param);
				ERR(PF_CHECKOUT_PARAM( 
					in_data, 
					idx, 
					in_data->current_time, in_data->time_step,in_data->time_scale, 
					&param));
				if (!err) {
					ret = param.u.sd.value;
				}else{
					err = PF_Err_BAD_CALLBACK_PARAM;
				}

				break;
			case PF_Cmd_RENDER:
			case PF_Cmd_USER_CHANGED_PARAM:
				ret = params[idx]->u.sd.value;
				break;
			default:
				err = PF_Err_BAD_CALLBACK_PARAM;
				break;
			}
		}else{
				err = PF_Err_INVALID_INDEX;
		}
		*f = ret;
		return err;
	}
	//--------------------------------------------------------------------
	PF_Err GetFIXED2FpShort(A_long idx,PF_FpShort *d)
	{
		PF_Err err = PF_Err_NONE;
		PF_Fixed ret =0;
		err = GetFIXED(idx,&ret);
		if (!err){
			*d = (PF_FpShort)ret/65536; 
		}else{
			err =FsAE_ERR;
		}
		return err;
	}
	//--------------------------------------------------------------------
	PF_Err GetFIXED2FpShort_P1(A_long idx,PF_FpShort *d)
	{
		PF_Err err = PF_Err_NONE;
		PF_Fixed ret =0;
		err = GetFIXED(idx,&ret);
		if (!err){
			*d = (PF_FpShort)ret/6553600; 
		}else{
			err =FsAE_ERR;
		}
		return err;
	}
	//--------------------------------------------------------------------
	PF_Err GetFIXED_PER2ONE(A_long idx,PF_Fixed *d)
	{
		PF_Err err = PF_Err_NONE;
		PF_Fixed ret =0;
		err = GetFIXED(idx,&ret);
		if (!err){
			*d = (PF_Fixed)ret/100; 
		}else{
			err =FsAE_ERR;
		}
		return err;
	}
	//--------------------------------------------------------------------
	PF_Err GetANGLE(A_long idx,PF_Fixed *r)
	{
		PF_Err err = PF_Err_NONE;
		PF_Fixed ret =0;
		if (( idx >=1)&&(idx < m_paramsCount)) {
			switch(m_cmd)
			{
			case PF_Cmd_SMART_PRE_RENDER:
			//case FsAE_QUERY_DYNAMIC_FLAGS:
				PF_ParamDef param;
				AEFX_CLR_STRUCT(param);
				ERR(PF_CHECKOUT_PARAM( 
					in_data, 
					idx, 
					in_data->current_time, in_data->time_step,in_data->time_scale, 
					&param));
				if (!err) {
					ret = param.u.ad.value;
				}else{
					err = PF_Err_BAD_CALLBACK_PARAM;
				}
				break;
			case PF_Cmd_RENDER:
			case PF_Cmd_USER_CHANGED_PARAM:
				ret = params[idx]->u.ad.value;
				break;
			default:
				err = PF_Err_BAD_CALLBACK_PARAM;
				break;
			}
		}else{
				err = PF_Err_INVALID_INDEX;
		}
		*r = ret;
		return err;
	}
	
	//--------------------------------------------------------------------
	PF_Err GetCHECKBOX(A_long idx,PF_Boolean *b)
	{
		PF_Err err = PF_Err_NONE;
		PF_Boolean ret =FALSE;
		if (( idx >=1)&&(idx < m_paramsCount)) {
			switch(m_cmd)
			{
			case PF_Cmd_SMART_PRE_RENDER:
			//case FsAE_QUERY_DYNAMIC_FLAGS:
				PF_ParamDef param;
				AEFX_CLR_STRUCT(param);
				ERR(PF_CHECKOUT_PARAM( 
					in_data, 
					idx, 
					in_data->current_time, in_data->time_step,in_data->time_scale, 
					&param));
				if (!err) {
					ret = (PF_Boolean)param.u.bd.value;
				}else{
					err = PF_Err_BAD_CALLBACK_PARAM;
				}
				break;
			case PF_Cmd_RENDER:
			case PF_Cmd_USER_CHANGED_PARAM:
				ret = (PF_Boolean)params[idx]->u.bd.value;
				break;
			default:
				err = PF_Err_BAD_CALLBACK_PARAM;
				break;
			}
		}else{
				err = PF_Err_INVALID_INDEX;
		}
		*b = ret;
		return err;
	}
	//--------------------------------------------------------------------
	PF_Err SetCHECKBOX(A_long idx, PF_Boolean b)
	{
		PF_Err err = PF_Err_NONE;
		if ((idx >= 1) && (idx < m_paramsCount) && (m_cmd == PF_Cmd_USER_CHANGED_PARAM)) {
			params[idx]->u.bd.value = b;
			params[idx]->uu.change_flags = PF_ChangeFlag_CHANGED_VALUE;
		}
		else {
			err = PF_Err_INVALID_INDEX;
		}
		return err;
	}
	//--------------------------------------------------------------------
	PF_Err GetCOLOR(A_long idx,PF_Pixel* c)
	{
		PF_Err err = PF_Err_NONE;
		PF_Pixel ret = {0,0,0,0};
		if (( idx >=1)&&(idx < m_paramsCount)) {
			switch(m_cmd)
			{
			case PF_Cmd_SMART_PRE_RENDER:
			//case FsAE_QUERY_DYNAMIC_FLAGS:
				PF_ParamDef param;
				AEFX_CLR_STRUCT(param);
				ERR(PF_CHECKOUT_PARAM( 
					in_data, 
					idx, 
					in_data->current_time, in_data->time_step,in_data->time_scale, 
					&param));
				if (!err) {
					ret = param.u.cd.value;
				}else{
					err = PF_Err_BAD_CALLBACK_PARAM;
				}

				break;
			case PF_Cmd_RENDER:
			case PF_Cmd_USER_CHANGED_PARAM:
				ret = params[idx]->u.cd.value;
				break;
			default:
				err = PF_Err_BAD_CALLBACK_PARAM;
				break;
			}
		}else{
				err = PF_Err_INVALID_INDEX;
		}
		*c = ret;
		return err;


	}
	PF_Err SetCOLOR(A_long idx, PF_Pixel col)
	{
		PF_Err err = PF_Err_NONE;
		if ((idx >= 1) && (idx < m_paramsCount) && (m_cmd == PF_Cmd_USER_CHANGED_PARAM))
		{
			params[idx]->u.cd.value.alpha = col.alpha;
			params[idx]->u.cd.value.red = col.red;
			params[idx]->u.cd.value.green = col.green;
			params[idx]->u.cd.value.blue = col.blue;
			params[idx]->uu.change_flags = PF_ChangeFlag_CHANGED_VALUE;
		}
		return err;
	}
	//--------------------------------------------------------------------
	PF_Err GetFLOAT(A_long idx,PF_FpLong *f)
	{
		PF_Err err = PF_Err_NONE;
		PF_FpLong ret = 0;
		if (( idx >=1)&&(idx < m_paramsCount)) {
			switch(m_cmd)
			{
			case PF_Cmd_SMART_PRE_RENDER:
			//case FsAE_QUERY_DYNAMIC_FLAGS:
				PF_ParamDef param;
				AEFX_CLR_STRUCT(param);
				ERR(PF_CHECKOUT_PARAM( 
					in_data, 
					idx, 
					in_data->current_time, in_data->time_step,in_data->time_scale, 
					&param));
				if (!err) {
					ret = param.u.fs_d.value;
				}else{
					err = PF_Err_BAD_CALLBACK_PARAM;
				}

				break;
			case PF_Cmd_RENDER:
			case PF_Cmd_USER_CHANGED_PARAM:
				ret = params[idx]->u.fs_d.value;
				break;
			default:
				err = PF_Err_BAD_CALLBACK_PARAM;
				break;
			}
		}else{
				err = PF_Err_INVALID_INDEX;
		}
		*f = ret;
		return err;
	}
	PF_Err SetFLOAT(A_long idx, PF_FpLong f)
	{
		PF_Err err = PF_Err_NONE;
		if ((idx >= 1) && (idx < m_paramsCount) && (m_cmd == PF_Cmd_USER_CHANGED_PARAM))
		{
			params[idx]->u.fs_d.value = f;
			params[idx]->uu.change_flags = PF_ChangeFlag_CHANGED_VALUE;
		}
		return err;
	}
	//--------------------------------------------------------------------
	 PF_Err GetFIXEDPOINT(A_long idx,PF_FixedPoint *pos)
	{
		PF_Err err = PF_Err_NONE;
		PF_FixedPoint ret;
		ret.x =-1;
		ret.y =-1;
		if (( idx >=1)&&(idx < m_paramsCount)) {
			switch(m_cmd)
			{
			case PF_Cmd_SMART_PRE_RENDER:
			//case FsAE_QUERY_DYNAMIC_FLAGS:
				PF_ParamDef param;
				AEFX_CLR_STRUCT(param);
				ERR(PF_CHECKOUT_PARAM( 
					in_data, 
					idx, 
					in_data->current_time, in_data->time_step,in_data->time_scale, 
					&param));
				if (!err) {
					ret.x = param.u.td.x_value;
					ret.y = param.u.td.y_value;
				}else{
					err = PF_Err_BAD_CALLBACK_PARAM;
				}

				break;
			case PF_Cmd_RENDER:
			case PF_Cmd_USER_CHANGED_PARAM:
				ret.x = NF_AE::params[idx]->u.td.x_value;
				ret.y = NF_AE::params[idx]->u.td.y_value;
				break;
			default:
				err = PF_Err_BAD_CALLBACK_PARAM;
				break;
			}
		}else{
				err = PF_Err_INVALID_INDEX;
		}
		*pos = ret;
		return err;
	}
	 //--------------------------------------------------------------------
	 PF_Err GetPOINT(A_long idx, PF_Point* pos)
	 {
		 PF_Err err = PF_Err_NONE;
		 PF_FixedPoint fp;
		 ERR(GetFIXEDPOINT(idx, &fp));
		 if (!err)
		 {
			 pos->x = (A_long)((double)fp.x /65536+0.5);
			 pos->y = (A_long)((double)fp.y /65536+0.5);
		 }
		 return err;
	 }
	 //--------------------------------------------------------------------
	 PF_Err GetFLOATPOINT(A_long idx, A_FloatPoint* pos)
	 {
		 PF_Err err = PF_Err_NONE;
		 PF_FixedPoint fp;
		 ERR(GetFIXEDPOINT(idx, &fp));
		 if (!err)
		 {
			 pos->x = (float)F_FIX2FLT(fp.x);
			 pos->y = (float)F_FIX2FLT(fp.y);
		 }
		 return err;
	 }
	//--------------------------------------------------------------------
	PF_Err GetPOPUP(A_long idx,A_long *pop)
	{
		PF_Err err = PF_Err_NONE;
		A_long ret = 1;
		if (( idx >=1)&&(idx < m_paramsCount)) {
			switch(m_cmd)
			{
			case PF_Cmd_SMART_PRE_RENDER:
			//case FsAE_QUERY_DYNAMIC_FLAGS:
				PF_ParamDef param;
				AEFX_CLR_STRUCT(param);
				ERR(PF_CHECKOUT_PARAM( 
					in_data, 
					idx, 
					in_data->current_time, in_data->time_step,in_data->time_scale, 
					&param));
				if (!err) {
					ret = param.u.pd.value;
				}else{
					err = PF_Err_BAD_CALLBACK_PARAM;
				}
				break;
			case PF_Cmd_RENDER:
			case PF_Cmd_USER_CHANGED_PARAM:
				ret = params[idx]->u.pd.value;
				break;
			default:
				err = PF_Err_BAD_CALLBACK_PARAM;
				break;
			}
		}else{
				err = PF_Err_INVALID_INDEX;
		}
		*pop = ret;
		return err;
	}
	PF_Err SetPOPUP(A_long idx, A_long b)
	{
		PF_Err err = PF_Err_NONE;
		if ((idx >= 1) && (idx < m_paramsCount) && (m_cmd == PF_Cmd_USER_CHANGED_PARAM))
		{
			params[idx]->u.pd.value = b;
			params[idx]->uu.change_flags = PF_ChangeFlag_CHANGED_VALUE;
		}
		return err;
	}
	//*********************************************************************************
	PF_Err iterate8(
		PF_EffectWorld *src,
		refconType refcon,
		PF_Err		(*pix_fn)(refconType refcon, A_long x, A_long y, PF_Pixel *in, PF_Pixel *out),
		PF_EffectWorld* dst
		)
	{
		return suitesP->Iterate8Suite1()->iterate(	in_data,
												0,														// progress base
												dst->height,	// progress final
												src,		// src 
												NULL,		// area - null for all pixels
												refcon,		// refcon - your custom data pointer
												pix_fn,		// pixel function pointer
												dst);	// dest
		
	}
	//*********************************************************************************
	PF_Err iterate16(
		PF_EffectWorld* src,
		refconType refcon,
		PF_Err(*pix_fn)(refconType refcon, A_long x, A_long y, PF_Pixel16* in, PF_Pixel16* out),
		PF_EffectWorld* dst
	)
	{
		return suitesP->Iterate16Suite1()->iterate(in_data,
			0,														// progress base
			dst->height,	// progress final
			src,		// src 
			NULL,		// area - null for all pixels
			refcon,		// refcon - your custom data pointer
			pix_fn,		// pixel function pointer
			dst);	// dest

	}
	//*********************************************************************************
	PF_Err iterate32(
		PF_EffectWorld* src,
		refconType refcon,
		PF_Err(*pix_fn)(refconType refcon, A_long x, A_long y, PF_PixelFloat* in, PF_PixelFloat* out),
		PF_EffectWorld* dst
	)
	{
		return suitesP->IterateFloatSuite1()->iterate(
			in_data,
			0,														// progress base
			dst->height,	// progress final
			src,		// src 
			NULL,		// area - null for all pixels
			refcon,		// refcon - your custom data pointer
			pix_fn,		// pixel function pointer
			dst);	// dest

	}
	
	//*********************************************************************************
	PF_Err iterate8_origin(
		PF_EffectWorld* src,
		refconType refcon,
		PF_Err		(*pix_fn)(refconType refcon, A_long x, A_long y, PF_Pixel *in, PF_Pixel *out),
		PF_EffectWorld* dst
		)
	{
		PF_Point origin;
		origin.h = (A_short)(in_data->output_origin_x);
		origin.v = (A_short)(in_data->output_origin_y);

		return suitesP->Iterate8Suite1()->iterate_origin(in_data,
															0,
															dst->height,
															src,
															&dst->extent_hint,
															&origin,
															(refconType)refcon,
															pix_fn,
															dst);
		
	}
	//*********************************************************************************
	PF_Err iterate16_origin(
		PF_EffectWorld* src,
		refconType refcon,
		PF_Err		(*pix_fn)(refconType refcon, A_long x, A_long y, PF_Pixel16 *in, PF_Pixel16 *out),
		PF_EffectWorld* dst
		)
	{
		PF_Point origin;
		origin.h = (A_short)(in_data->output_origin_x);
		origin.v = (A_short)(in_data->output_origin_y);

		return suitesP->Iterate16Suite1()->iterate_origin(in_data,
															0,
															dst->height,
															src,
															&dst->extent_hint,
															&origin,
															(refconType)refcon,
															pix_fn,
															dst);
		
	}
	//*********************************************************************************
	PF_Err iterate32_origin(
		PF_EffectWorld* src,
		refconType refcon,
		PF_Err		(*pix_fn)(refconType refcon, A_long x, A_long y, PF_PixelFloat *in, PF_PixelFloat *out),
		PF_EffectWorld* dst
		)
	{
		PF_Point origin;
		origin.h = (A_short)(in_data->output_origin_x);
		origin.v = (A_short)(in_data->output_origin_y);

		return suitesP->IterateFloatSuite1()->iterate_origin(in_data,
															0,
															dst->height,
															src,
															&dst->extent_hint,
															&origin,
															(refconType)refcon,
															pix_fn,
															dst);
		
	}
	PF_Err iterate_generic(
		A_long iterationsL,
		void* refconPV,
		PF_Err(*fn_func)(
			void* refconPV,
			A_long  thread_idxL,
			A_long  i,
			A_long  itrtL)
	)
	{
		return suitesP->Iterate8Suite1()->iterate_generic(
			iterationsL,
			refconPV,
			fn_func);

	}
	//*********************************************************************************
	//*********************************************************************************
	PF_Err SetOutFlag_NON_PARAM_VARY(PF_Boolean b)
	{
		PF_Err err = PF_Err_NONE;
		if( out_data !=NULL){
			if (b==TRUE){
				out_data->out_flags |= PF_OutFlag_NON_PARAM_VARY;
			}else{
				out_data->out_flags &= ~PF_OutFlag_NON_PARAM_VARY;
			}
		}
		return err;
	}
	//*********************************************************************************
	PF_Err checkin_param(PF_ParamDef *defP)
	{
		return PF_CHECKIN_PARAM(in_data, defP);
	}
	//*********************************************************************************
	//--------------------------------------------------------------------
	PF_Boolean			is8Bit(){ return (m_format == PF_PixelFormat_ARGB32);}
	PF_Boolean			is16Bit(){ return (m_format == PF_PixelFormat_ARGB64);}
	PF_Boolean			is32Bit(){ return (m_format == PF_PixelFormat_ARGB128);}
	PF_PixelFormat		pixelFormat() { return m_format;}
	A_long				frame(){ return m_frame; }
	A_long				mode(){ return m_cmd; }
	PF_Err				resultErr() { return m_resultErr;}
	//*********************************************************************************
	
	
	//*********************************************************************************
	
	//*********************************************************************************
	PF_Err NewWorld(A_long w, A_long h,PF_PixelFormat pf, PF_EffectWorld *wld)
	{
		PF_Err err = PF_Err_NONE;
		if (m_cmd == PF_Cmd_SMART_RENDER) {
			ERR(NF_AE::ws2P->PF_NewWorld(in_data->effect_ref, w, h, TRUE, pf, wld));

		}
		else {
			PF_NewWorldFlags f = PF_NewWorldFlag_CLEAR_PIXELS | PF_NewWorldFlag_NONE;
			if (pf == PF_PixelFormat_ARGB64)
				f = PF_NewWorldFlag_CLEAR_PIXELS | PF_NewWorldFlag_DEEP_PIXELS;
			ERR((*in_data->utils->new_world)(in_data->effect_ref, w, h, f, wld));
		}
		return err;
	}
	PF_Err DisposeWorld(PF_EffectWorld* wld)
	{
		PF_Err err = PF_Err_NONE;
		if (wld != NULL) {
			if (m_cmd == PF_Cmd_SMART_RENDER) {
				ERR(NF_AE::ws2P->PF_DisposeWorld(in_data->effect_ref, wld));
			}
			else {
				ERR((*in_data->utils->dispose_world)(in_data->effect_ref, wld));
			}
		}
		return err;
	}
	//*********************************************************************************
	PF_Handle NewHandle(A_long size)
	{
		//return suitesP->HandleSuite1()->host_new_handle(size);
		return PF_NEW_HANDLE(size);
	}
	//*********************************************************************************
	void LockHandle(PF_Handle h)
	{
		//suitesP->HandleSuite1()->host_lock_handle(h);
		PF_LOCK_HANDLE(h);
	}
	//*********************************************************************************
	void UnlockHandle(PF_Handle h)
	{
		//suitesP->HandleSuite1()->host_unlock_handle(h);
		PF_UNLOCK_HANDLE(h);
	}
	//*********************************************************************************
	void DisposeHandle(PF_Handle h)
	{
		//suitesP->HandleSuite1()->host_dispose_handle(h);
		PF_DISPOSE_HANDLE(h);
	}
	
	//*********************************************************************************
	int ShowDialog(const A_char * p)
	{
		return PF_SPRINTF(	out_data->return_msg, p );
	}
	//*********************************************************************************
	//--------------------------------------------------------------------
	PF_FpLong downScale(PF_FpLong v)
	{
		v = v * (PF_FpLong)in_data->downsample_x.num / (PF_FpLong)in_data->downsample_x.den;
		return v;
	}
	//--------------------------------------------------------------------
	PF_FpLong downSaleValue()
	{
		return (PF_FpLong)in_data->downsample_x.den / (PF_FpLong)in_data->downsample_x.num;
	}
	
	//--------------------------------------------------------------------
	PF_Err CopyInToOut()
	{
		PF_Err err = PF_Err_NONE;
		err = suitesP->WorldTransformSuite1()->copy_hq(in_data->effect_ref,	// This effect ref (unique id)
												input,						// Source
												output,						// Dest
												NULL,						// Source rect - null for all pixels
												NULL);						// Dest rect - null for all pixels
		return err;
	}
	//--------------------------------------------------------------------
	PF_Err Copy(PF_EffectWorld *i,PF_EffectWorld *o)
	{
		PF_Err err = PF_Err_NONE;

		err = suitesP->WorldTransformSuite1()->copy_hq(in_data->effect_ref,	// This effect ref (unique id)
												i,						// Source
												o,						// Dest
												NULL,						// Source rect - null for all pixels
												NULL);						// Dest rect - null for all pixels
		return err;
	}
	
	//--------------------------------------------------------------------
	PF_Err premultiply_color(A_long mode, PF_Pixel *color)
	{
#if defined(PF_AE100_PLUG_IN_VERSION)
	return suitesP->FillMatteSuite2()->premultiply_color(
			in_data->effect_ref,
			input,
			color,
			mode,
			output);

#else
	return suitesP->FillMatteSuite1()->premultiply_color(
			in_data->effect_ref,
			input,
			color,
			mode,
			output);
#endif
	}
	//--------------------------------------------------------------------
	PF_Err premultiply(A_long mode)
	{
#if defined(PF_AE100_PLUG_IN_VERSION)
	return suitesP->FillMatteSuite2()->premultiply(
			in_data->effect_ref,
			mode,
			output);
#else
	return suitesP->FillMatteSuite1()->premultiply(
			in_data->effect_ref,
			mode,
			output);
#endif
	}
	//--------------------------------------------------------------------

};



#endif
