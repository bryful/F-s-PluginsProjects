//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------


#include "SelectiveColorBlur.h"

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
	PF_Pixel defColors[] = {
		{0xFF,0xFF,0x00,0x00}, // red
		{0xFF,0x00,0xFF,0x00}, // green
		{0xFF,0x00,0x00,0xFF}, // blue
		{0xFF,0xFF,0xFF,0x00}, // yellow
		{0xFF,0x00,0xFF,0xFF}, // cyan
		{0xFF,0xFF,0x00,0xFF}, // magenta
		{0xFF,0x80,0x80,0x80}, // black
		{0xFF,0xE0,0xE0,0xE0}  // white
	};

	NF_ParamsSetup cs(in_data, out_data);
	// ----------------------------------------------------------------
	cs.AddSlider(	// noise offset
		STR_BLUR_VALUE,		//パラメータの名前
		0, 				//数値入力する場合の最小値
		1000,			//数値入力する場合の最大値
		0,				//スライダーの最小値 
		50,				//スライダーの最大値
		0,				//デフォルトの値
		ID_BLUR_VALUE
	);
	// ----------------------------------------------------------------
	cs.AddTopic(STR_TOPIC_COLOR, ID_TOPIC_COLOR);
	// ----------------------------------------------------------------
	for (int i = 0; i < COLOR_COUNT; i++) {
		std::string name_enable = STR_TARGET_ENABLE;
		name_enable += std::to_string(i);
		std::string name_color = STR_TARGET_COL;
		name_color += std::to_string(i);
		cs.AddCheckBox(	// noise frame
			name_enable.c_str(),
			STR_TARGET_ENABLE1,
			FALSE,
			ID_TARGET_ENABLED(i)
		);
		cs.AddColor(	// color
			name_color.c_str(),
			defColors[i],
			ID_TARGET_COL(i)
		);
	}

	cs.EndTopic(ID_TOPIC_COLOR_END);
	cs.Finalize();
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err
HandleChangedParam(
	PF_InData					*in_data,
	PF_OutData					*out_data,
	PF_ParamDef					*params[],
	PF_LayerDef					*outputP,
	PF_UserChangedParamExtra	*extraP)
{
	PF_Err				err					= PF_Err_NONE,
						err2				= PF_Err_NONE;

	
	return err;
}
//-----------------------------------------------------------------------------------
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

//-------------------------------------------------------------------------------------------------
static PF_Err GetParams(NF_AE *ae, ParamInfo *infoP)
{
	PF_Err		err 		= PF_Err_NONE;

	ERR(ae->GetADD(ID_BLUR_VALUE, &infoP->blur_value));

	int cnt = 0;
	for(int i=0; i<COLOR_COUNT;i++) {
		PF_Boolean enabled;
		ERR(ae->GetCHECKBOX(ID_TARGET_ENABLED(i), &enabled));
		if (!err && enabled == TRUE) {
			PF_Pixel col;
			ERR(ae->GetCOLOR(ID_TARGET_COL(i), &col));
			if (!err) {
				infoP->target_col[cnt] = col;
				cnt++;
			}
		}
	}
	infoP->target_count = cnt;


	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
	Exec (NF_AE*ae , ParamInfo *infoP)
{
	PF_Err	err = PF_Err_NONE;

	if(infoP->blur_value<=0 ||infoP->target_count<=0){
		ERR(ae->CopyInToOut());
		return err;
	}
	ERR(SelectColorCopy(ae, infoP));
	blurPrm bp;
	bp.suitesP = ae->suitesP;
	bp.data = ae->output->data;
	bp.width = ae->output->width;
	bp.height = ae->output->height;

	// NF_AEクラスがGetWorldSize()で計算済みの値をそのまま代入
	bp.rowbytes = ae->outputInfo.widthTrue;

	bp.blur_value = infoP->blur_value;
	bp.pixelFormat = ae->pixelFormat();

	// 実行
	ERR(BlurSub(&bp));
	ERR(CombineInput(ae));
	return err;
}


//-----------------------------------------------------------------------------------
DllExport
PF_Err PluginDataEntryFunction2(
	PF_PluginDataPtr inPtr,
	PF_PluginDataCB2 inPluginDataCallBackPtr,
	SPBasicSuite* inSPBasicSuitePtr,
	const char* inHostName,
	const char* inHostVersion)
{
	PF_Err result = PF_Err_INVALID_CALLBACK;

	result = PF_REGISTER_EFFECT_EXT2(
		inPtr,
		inPluginDataCallBackPtr,
		NF_NAME, // Name
		NF_MATCHNAME, // Match Name
		NF_CATEGORY, // Category
		AE_RESERVED_INFO, // Reserved Info
		"EffectMain",	// Entry point
		"https://github.com/bryful");	// support URL

	return result;
}
DllExport	PF_Err
EffectMain(
	PF_Cmd			cmd,
	PF_InData* in_data,
	PF_OutData* out_data,
	PF_ParamDef* params[],
	PF_LayerDef* output,
	void* extraP)
{
	PF_Err			err = PF_Err_NONE,
					err2 = PF_Err_NONE;

	try
	{
		NF_AE ae;
		switch (cmd) {
		case PF_Cmd_ABOUT:
			err = ae.About(in_data, out_data, params, output);
			break;
		case PF_Cmd_GLOBAL_SETUP:
			err = ae.GlobalSetup(in_data, out_data, params, output);
			break;
		case PF_Cmd_GLOBAL_SETDOWN:
			err = ae.GlobalSetdown(in_data);
			break;
		case PF_Cmd_PARAMS_SETUP:
			err = ParamsSetup(in_data, out_data, params, output);
			break;
		case PF_Cmd_SEQUENCE_SETUP:
			break;
		case PF_Cmd_SEQUENCE_SETDOWN:
			break;
		case PF_Cmd_SEQUENCE_RESETUP:
			break;
		case PF_Cmd_RENDER:
			err = ae.Render(in_data, out_data, params, output, ID_NUM_PARAMS);
			if (!err) {
				ParamInfo info;
				ERR(GetParams(&ae, &info));
				ERR(Exec(&ae, &info));
			}
			break;
		case PF_Cmd_SMART_PRE_RENDER:
			err = ae.PreRender(in_data, out_data, reinterpret_cast<PF_PreRenderExtra*>(extraP),sizeof(ParamInfo), ID_NUM_PARAMS);
			if (!err) {
				ParamInfo* infoP = reinterpret_cast<ParamInfo*>(ae.LockPreRenderData());
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
			break;
		case PF_Cmd_SMART_RENDER:
			err = ae.SmartRender(in_data, out_data, reinterpret_cast<PF_SmartRenderExtra*>(extraP), ID_NUM_PARAMS);
			if (!err) {
				ParamInfo* infoP = reinterpret_cast<ParamInfo*>(ae.LockPreRenderData());
				if (infoP) {
					ERR(Exec(&ae, infoP));
					ERR2(ae.UnsetSmartRender());
					ae.UnlockPreRenderData();
				}
				else {
					err = PF_Err_OUT_OF_MEMORY;
				}
			}
			break;
		case PF_Cmd_COMPLETELY_GENERAL:
		{
			AEGP_SuiteHandler suites(in_data->pica_basicP);

			suites.ANSICallbacksSuite1()->sprintf(out_data->return_msg,
				"%s",
				reinterpret_cast<A_char*>(extraP));
			break;
		}
		case PF_Cmd_DO_DIALOG:
		{
			//err = PopDialog(in_data,out_data,params,output);
			break;
		}
		case PF_Cmd_USER_CHANGED_PARAM:
		{
			err = HandleChangedParam(in_data,
				out_data,
				params,
				output,
				reinterpret_cast<PF_UserChangedParamExtra*>(extraP));

			break;
		}
		case PF_Cmd_QUERY_DYNAMIC_FLAGS:
			err = QueryDynamicFlags(in_data,
				out_data,
				params,
				reinterpret_cast<PF_UserChangedParamExtra*>(extraP));
			break;
		}
	}
	catch (PF_Err& thrown_err) {
		err = thrown_err;
	}
	if (err != PF_Err_NONE) {
		AEGP_SuiteHandler suites(in_data->pica_basicP);

		suites.ANSICallbacksSuite1()->sprintf(out_data->return_msg,
			"%d", cmd
			);
	}
	return err;
}

