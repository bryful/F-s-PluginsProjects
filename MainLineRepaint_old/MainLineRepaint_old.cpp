/*
	F's Plugins AfterEffects 7.0 SDK
	FsGraphics
*/

#include "MainLineRepaint_old.h"


//-------------------------------------------------------------------------------------------------
// Aboutダイアログ
static PF_Err About (
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{
	PF_SPRINTF(	out_data->return_msg, 
				"%s, v%d.%d%d\r%s",
				STR(StrID_Name), 
				MAJOR_VERSION, 
				MINOR_VERSION,
				BUG_VERSION,
				STR(StrID_Description)
				);
	return PF_Err_NONE;
}

//-------------------------------------------------------------------------------------------------
static PF_Err GlobalSetup (
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{
	PF_Err	err = PF_Err_NONE;

	//AE_Effect.hで定義されるPF_VERSIONマクロで求められる値をリソースファイルのAE_Effect_Versionと一致させる事
	out_data->my_version = PF_VERSION(	MAJOR_VERSION, 
										MINOR_VERSION,
										BUG_VERSION, 
										STAGE_VERSION, 
										BUILD_VERSION);

	//Output Flags
	//AE_Effect.hのOutput Flagsを参照
	//16Bit対応させる為wにはPF_OutFlag_DEEP_COLOR_AWAREを追加する
	//
	//-------------------------------
	//必要なフレームはレンダリングしない。通常はこれ。
	//リソースファイルのAE_Effect_Global_OutFlagsの値は33555520
	/*
out_data->out_flags	|=
		PF_OutFlag_USE_OUTPUT_EXTENT	|
		PF_OutFlag_PIX_INDEPENDENT	|
		PF_OutFlag_DEEP_COLOR_AWARE;
	*/
	//-------------------------------
	//ノイズ等で全フレーム描画する場合はこれ
	//リソースファイルのAE_Effect_Global_OutFlagsの値は33559620
	/*
out_data->out_flags	|=
		PF_OutFlag_NON_PARAM_VARY	|
		PF_OutFlag_USE_OUTPUT_EXTENT	|
		PF_OutFlag_PIX_INDEPENDENT	|
		PF_OutFlag_I_SHRINK_BUFFER	|
		PF_OutFlag_DEEP_COLOR_AWARE;
	*/
out_data->out_flags	|=
		PF_OutFlag_USE_OUTPUT_EXTENT	|
		PF_OutFlag_PIX_INDEPENDENT	|
		PF_OutFlag_DEEP_COLOR_AWARE;

	return err;
}

//-------------------------------------------------------------------------------------------------
static PF_Err SequenceSetup (
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{
	return PF_Err_NONE;
}

//-------------------------------------------------------------------------------------------------
static PF_Err SequenceSetdown (
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{
	return PF_Err_NONE;
}

//-------------------------------------------------------------------------------------------------
static PF_Err SequenceResetup (
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{
	return PF_Err_NONE;
}

//-------------------------------------------------------------------------------------------------
//AfterEffextsにパラメータを通達する
//Param_Utils.hを参照のこと
static PF_Err ParamsSetup (
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output)
{
	PF_Err			err = PF_Err_NONE;
	PF_ParamDef		def;

	//----------------------------------------------------------------
	//色の指定
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(	STR(StrID_MY_Main_Color), 
					0x00,
					0x00,
					0x00,
					MY_Main_Color
					);

	//----------------------------------------------------------------
	out_data->num_params = 	MY_NUM_PARAMS; 

	return err;
}
//-------------------------------------------------------------------------------------------------

PF_Err CMainLineRepaint::getParams(
	PF_InData			*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{
	PF_Err		err 		= PF_Err_NONE;
	if ( (params==NULL)||(output==NULL) ) {
		return 0xFF;
	}

	Main_Color			= params[MY_Main_Color]->u.cd.value;



	return err;
}

//-------------------------------------------------------------------------------------------------
//レンダリングのメイン
static PF_Err Render (
	PF_InData			*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{
	PF_Err		err 		= PF_Err_NONE;

	CMainLineRepaint exe(in_data,out_data,params,output);
	
	if  ( exe.Enabled() == FALSE ) {
		PF_SPRINTF(	out_data->return_msg, 
					"[%s] %s",
					STR(StrID_Name),
					STR(StrID_ERR_getParams)
					);
		return err;
	}
	
	//画像を複写する
	exe.Copy();
	
	if (exe.is16Bit()==FALSE){
		exe.exec8();
	}else{
		exe.exec16();
	}
	return err;
}
//-------------------------------------------------------------------------------------------------
DllExport PF_Err EntryPointFunc(
	PF_Cmd			cmd,
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output,
	void			*extra)
{
	PF_Err		err = PF_Err_NONE;
	
	switch (cmd) {
		case PF_Cmd_ABOUT:
			err = About(in_data,out_data,params,output);
			break;
		case PF_Cmd_GLOBAL_SETUP:
			err = GlobalSetup(in_data,out_data,params,output);
			break;
		case PF_Cmd_PARAMS_SETUP:
			err = ParamsSetup(in_data,out_data,params,output);
			break;
		case PF_Cmd_SEQUENCE_SETUP:
			err = SequenceSetup(in_data,out_data,params,output);
			break;
		case PF_Cmd_SEQUENCE_SETDOWN:
			err = SequenceSetdown(in_data,out_data,params,output);
			break;
		case PF_Cmd_SEQUENCE_RESETUP:
			err = SequenceResetup(in_data,out_data,params,output);
			break;
		case PF_Cmd_RENDER:
			err = Render(in_data,out_data,params,output);
			break;
	}
	return err;
}
//-------------------------------------------------------------------------------------------------
