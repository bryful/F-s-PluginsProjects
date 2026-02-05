#pragma once
#pragma once
#ifndef Filter_Entry_H
#define Filter_Entry_H

#include "../FsLibrary_next/FsAEHeader.h"
#include "Filter.h"
#include "Filter_Target.h"

//=======================================================================================
static PF_Err
About(
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output)
{
	PF_Err	err = PF_Err_NONE;

	CAE ae;
	err = ae.About(in_data, out_data, params, output);



	return PF_Err_NONE;
}

//=======================================================================================
static PF_Err
GlobalSetup(
	PF_InData		*in_dataP,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output)
{
	PF_Err	err = PF_Err_NONE;
	CAE ae;
	err = ae.GlobalSetup(in_dataP, out_data, params, output);
	return err;

}
//-------------------------------------------------------------------------------------------------
static PF_Err GlobalSetdown(
	PF_InData	*in_data)
{
	PF_Err	err = PF_Err_NONE;
	CAE ae;
	err = ae.GlobalSetdown(in_data);
	return PF_Err_NONE;
}

//-------------------------------------------------------------------------------------------------
static PF_Err SequenceSetup(
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output)
{

	return PF_Err_NONE;
}
//-------------------------------------------------------------------------------------------------
static PF_Err SequenceSetdown(
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output)
{
	return PF_Err_NONE;
}


//-------------------------------------------------------------------------------------------------
static PF_Err SequenceResetup(
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output)
{
	return PF_Err_NONE;
}
/*
//=======================================================================================
static PF_Err
QueryDynamicFlags(
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	void			*extra)
{
	PF_Err 	err = PF_Err_NONE;
	return err;
}
//=======================================================================================
static PF_Err
HandleChangedParam(
	PF_InData					*in_data,
	PF_OutData					*out_data,
	PF_ParamDef					*params[],
	PF_LayerDef					*outputP,
	PF_UserChangedParamExtra	*extraP)
{
	PF_Err				err = PF_Err_NONE;

	return err;
}
*/
//-----------------------------------------------------------------------------------
static PF_Err
RespondtoAEGP(
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output,
	void*			extraP)
{
	PF_Err			err = PF_Err_NONE;

	AEGP_SuiteHandler suites(in_data->pica_basicP);

	suites.ANSICallbacksSuite1()->sprintf(out_data->return_msg,
		"%s",
		reinterpret_cast<A_char*>(extraP));

	return err;
}
//=======================================================================================
static PF_Err
Render(
	PF_InData		*in_dataP,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output)
{
	PF_Err	err = PF_Err_NONE;
	PF_Handle		pixelTable = NULL;

	CAE ae(in_dataP, out_data, params, output, ID_NUM_PARAMS);
	err = ae.resultErr();
	if (!err) {
		ParamInfo info;
		ERR(GetParams(&ae, &info));
		ERR(Exec(&ae, &info));
	}
	return err;
}

//=======================================================================================
#if defined(SUPPORT_SMARTFX)
static PF_Err
PreRender(
	PF_InData			*in_dataP,
	PF_OutData			*out_dataP,
	PF_PreRenderExtra	*extraP)
{
	PF_Err		err = PF_Err_NONE;
	CAE ae(in_dataP, out_dataP, extraP, sizeof(ParamInfo), ID_NUM_PARAMS);
	err = ae.resultErr();
	if (!err) {

		ParamInfo *infoP = reinterpret_cast<ParamInfo*>(ae.LockPreRenderData());
		if (infoP) {
			ae.SetHostPreRenderData();
			ERR(GetParams(&ae, infoP));
			ERR(ae.UnSetPreRenderData());
			ae.UnlockPreRenderData();
		}
		else {
			err = PF_Err_OUT_OF_MEMORY;
		}
	}
	return err;
}
#endif
//=======================================================================================
#if defined(SUPPORT_SMARTFX)
static PF_Err
SmartRender(
	PF_InData				*in_data,
	PF_OutData				*out_data,
	PF_SmartRenderExtra		*extraP)
{

	PF_Err			err = PF_Err_NONE,
		err2 = PF_Err_NONE;

	CAE ae(in_data, out_data, extraP, ID_NUM_PARAMS);
	err = ae.resultErr();
	if (!err) {
		ParamInfo *infoP = reinterpret_cast<ParamInfo*>(ae.LockPreRenderData());
		if (infoP) {
			ERR(Exec(&ae, infoP));
			ERR2(ae.UnsetSmartRender());
			ae.UnlockPreRenderData();
		}
		else {
			err = PF_Err_OUT_OF_MEMORY;
		}
	}
	return err;

}
#endif

//=======================================================================================
extern "C" DllExport
PF_Err PluginDataEntryFunction(
	PF_PluginDataPtr inPtr,
	PF_PluginDataCB inPluginDataCallBackPtr,
	SPBasicSuite* inSPBasicSuitePtr,
	const char* inHostName,
	const char* inHostVersion)
{
	PF_Err result = PF_Err_INVALID_CALLBACK;

	result = PF_REGISTER_EFFECT(
		inPtr,
		inPluginDataCallBackPtr,
		FS_NAME, // Name
		FS_MATCH_NAME, // Match Name
		FS_CATEGORY, // Category
		AE_RESERVED_INFO); // Reserved Info

	return result;
}

//=======================================================================================
PF_Err
EffectMain(
	PF_Cmd			cmd,
	PF_InData		*in_dataP,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output,
	void			*extraP)
{
	PF_Err		err = PF_Err_NONE;

	try {
		switch (cmd)
		{
		case PF_Cmd_ABOUT:
			err = About(in_dataP, out_data, params, output);
			break;
		case PF_Cmd_GLOBAL_SETUP:
			err = GlobalSetup(in_dataP, out_data, params, output);
			break;
		case PF_Cmd_GLOBAL_SETDOWN:
			err = GlobalSetdown(in_dataP);
			break;
		case PF_Cmd_PARAMS_SETUP:
			err = ParamsSetup(in_dataP, out_data, params, output);
			break;
		case PF_Cmd_SEQUENCE_SETUP:
			err = SequenceSetup(in_dataP, out_data, params, output);
			break;
		case PF_Cmd_SEQUENCE_SETDOWN:
			err = SequenceSetdown(in_dataP, out_data, params, output);
			break;
		case PF_Cmd_SEQUENCE_RESETUP:
			err = SequenceResetup(in_dataP, out_data, params, output);
		break;			case PF_Cmd_RENDER:
			err = Render(in_dataP, out_data, params, output);
			break;
#if defined(SUPPORT_SMARTFX)
		case PF_Cmd_SMART_PRE_RENDER:
			err = PreRender(in_dataP, out_data, (PF_PreRenderExtra*)extraP);
			break;
		case PF_Cmd_SMART_RENDER:
			err = SmartRender(in_dataP, out_data, (PF_SmartRenderExtra*)extraP);
			break;
#endif
		case PF_Cmd_COMPLETELY_GENERAL:
			err = RespondtoAEGP(in_dataP, out_data, params, output, extraP);
			break;
		case PF_Cmd_DO_DIALOG:
			//err = PopDialog(in_dataP,out_data,params,output);
			break;
		case PF_Cmd_USER_CHANGED_PARAM:
			err = HandleChangedParam(in_dataP,
				out_data,
				params,
				output,
				reinterpret_cast<PF_UserChangedParamExtra*>(extraP));
			break;
		case PF_Cmd_QUERY_DYNAMIC_FLAGS:
			err = QueryDynamicFlags(in_dataP,
				out_data,
				params,
				reinterpret_cast<PF_UserChangedParamExtra*>(extraP));
			break;
		case PF_Cmd_FRAME_SETUP:
			err = FrameSetup(in_dataP, out_data, params, output);
			break;
		}
	}
	catch (PF_Err &thrown_err) {
		// Never EVER throw exceptions into AE.
		err = thrown_err;
	}
	return err;
}

#endif // OpenCVSkelton_Entry_H


