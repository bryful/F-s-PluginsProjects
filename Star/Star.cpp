//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS20126
*/
//-----------------------------------------------------------------------------------


#include "Star.h"

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

	NF_ParamsSetup setup(in_data, out_data);

	setup.AddTopic(STR_MODE_TOPIC, ID_MODE_TOPIC);
	// 全体設定
	setup.AddAngle(	// angle
		STR_ANGLE,
		0,
		ID_ANGLE
	);
	setup.AddFloatSlider(
		STR_LENGTH,	// R
		0,				//VALID_MIN
		1000,			//VALID_MAX
		0,				//SLIDER_MIN
		200,			//SLIDER_MAX
		100,			//DFLT
		1,				//PREC 小数点以下の桁数
		0,				//DISP
		FALSE,			//WANT_PHASE
		ID_LENGTH
	);
	setup.AddFloatSlider(
		STR_INTENSITY,	// R
		0,//VALID_MIN
		100,				//VALID_MAX
		0,				//SLIDER_MIN
		100,				//SLIDER_MAX
		100,				//DFLT
		1,				//PREC 小数点以下の桁数
		0,				//DISP
		FALSE,			//WANT_PHASE
		ID_INTENSITY
	);
	setup.AddFloatSlider(
		STR_HYPERBOLIC,	// R
		0,			//VALID_MIN
		50,				//VALID_MAX
		0,				//SLIDER_MIN
		20,				//SLIDER_MAX
		10,				//DFLT
		1,				//PREC 小数点以下の桁数
		0,				//DISP
		FALSE,			//WANT_PHASE
		ID_HYPERBOLIC
	);
	setup.AddPopup(STR_COLOR_MODE,
		2,
		1,
		STR_COLOR_MODE_ITEMS,
		ID_COLOR_MODE
	);
	setup.AddColor(	// color
		STR_COLOR,
		{ 0xFF, 0x80, 0x80, 0xFF },
		ID_COLOR
	);
	setup.EndTopic(ID_MODE_TOPIC_END);
	// 各ラインの設定 (トピックを使って整理)
	const char* line_names[] = { STR_LINE1_TOPIC, STR_LINE2_TOPIC, STR_LINE3_TOPIC, STR_LINE4_TOPIC};
	float default_angles[] = { 0.0, 45.0, 90.0, 135.0 };
	float default_length[] = { 100.0, 60.0, 100.0, 60.0 };
	float default_int[] = { 100.0, 100.0, 100.0, 100.0 };

	A_long ofs = 5;
	for (int i = 0; i < 4; i++) {
		setup.AddTopic(line_names[i], ID_LINE1_TOPIC + i* ofs);
		setup.AddFloatSlider(
			std::string(std::string(STR_LINE_LEN) + std::to_string(i + 1) + " (%)").c_str(),	// R
			0,				//VALID_MIN
			100,			//VALID_MAX
			0,				//SLIDER_MIN
			100,			//SLIDER_MAX
			default_length[i],//DFLT
			1,				//PREC 小数点以下の桁数
			0,				//DISP
			FALSE,			//WANT_PHASE
			ID_LINE1_LEN + i * ofs
		);
		setup.AddFloatSlider(
			std::string(std::string(STR_LINE_INT) + std::to_string(i + 1) +" (%)").c_str(),	// R
			0,				//VALID_MIN
			100,			//VALID_MAX
			0,				//SLIDER_MIN
			100,			//SLIDER_MAX
			default_int[i],			//DFLT
			1,				//PREC 小数点以下の桁数
			0,				//DISP
			FALSE,			//WANT_PHASE
			ID_LINE1_INT + i * ofs
		);
		setup.AddAngle(	// angle
			std::string(std::string(STR_LINE_ANGLE) + std::to_string(i + 1)).c_str(),
			default_angles[i],
			ID_LINE1_ANGLE + i * ofs
		);
		setup.EndTopic(ID_LINE1_TOPIC_END + i * ofs);
	}
	out_data->num_params = ID_NUM_PARAMS;

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
	PF_Err				err = PF_Err_NONE;

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

	PF_Fixed	fixed_val;
	ERR(ae->GetANGLE(ID_ANGLE, &fixed_val));
	infoP->overall_angle = F_FIX2FLT(fixed_val);
	ERR(ae->GetFLOAT(ID_INTENSITY, &infoP->overall_intensity));
	infoP->overall_intensity /= 100;
	ERR(ae->GetFLOAT(ID_LENGTH, &infoP->overall_length));
	ERR(ae->GetFLOAT(ID_HYPERBOLIC, &infoP->hyperbolic));

	ERR(ae->GetPOPUP(ID_COLOR_MODE, &infoP->color_mode));
	PF_Pixel p = { 0,0,0,0 };
	ERR(ae->GetCOLOR(ID_COLOR, &p));
	infoP->cross_color = NF_Pixel8To32(p);
	
	for(int i=0;i<4;i++)
	{
		A_long ofs = 5;
		PF_Fixed angle_fixed=0;
		ERR(ae->GetFLOAT(ID_LINE1_LEN + i * ofs, &infoP->lines[i].len_per));
		infoP->lines[i].len_per /= 100;
		ERR(ae->GetFLOAT(ID_LINE1_INT + i * ofs, &infoP->lines[i].int_per));
		infoP->lines[i].int_per /= 100;
		ERR(ae->GetANGLE(ID_LINE1_ANGLE + i * ofs, &angle_fixed));
		infoP->lines[i].angle_offset = F_FIX2FLT(angle_fixed);
	}



	return err;
}

//-------------------------------------------------------------------------------------------------
static PF_Err 
	Exec (NF_AE*ae , ParamInfo *infoP)
{
	PF_Err	err = PF_Err_NONE;

	PF_InData* in_data = ae->in_data;
	ERR(PF_FILL(NULL, NULL, ae->output));

	infoP->overall_length = ae->downScale( infoP->overall_length);

	if (infoP->overall_length <= 0 || infoP->overall_intensity <= 0) {
		return err;
	}
	std::vector<std::vector<float>> mask(ae->output->height, std::vector<float>(ae->output->width));

	ERR(ExtractMask(
		ae->in_data,
		ae->input,
		ae->pixelFormat(),
		ae->suitesP,
		&mask,
		infoP->color_mode
	));
	std::vector<std::vector<float>> mask2 = CalcMask(ae->suitesP,&mask);

	A_long count = 0;
	std::vector <StarSource> sources = ExtractStarSources(mask2,&count);


	ERR(StarMain(ae, sources,infoP ));

	ERR(StarBlend(
		ae,
		infoP,
		&mask
	));
	ERR(Mult(
		ae->in_data,
		ae->output,
		ae->pixelFormat(),
		ae->suitesP,
		TRUE
	));
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

