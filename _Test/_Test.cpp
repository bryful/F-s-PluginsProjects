//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------


#include "_Test.h"

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

	NF_ParamsSetup cs(in_data, out_data);
	// ----------------------------------------------------------------
	cs.AddPoint(
		STR_POS1,
		25,
		25,
		FALSE,
		ID_POS1
	);
	cs.AddPoint(
		STR_POS2,
		75,
		25,
		FALSE,
		ID_POS2
	);
	cs.AddPoint(
		STR_POS3,
		75,
		75,
		FALSE,
		ID_POS3
	);
	cs.AddPoint(
		STR_POS3,
		25,
		75,
		FALSE,
		ID_POS3
	);
	cs.AddPoint(
		STR_POSA1,
		0,
		0,
		FALSE,
		ID_POSA1
	);
	cs.AddPoint(
		STR_POS2,
		50,
		10,
		FALSE,
		ID_POSA2
	);
	cs.AddPoint(
		STR_POS3,
		100,
		30,
		FALSE,
		ID_POSA3
	);
	cs.AddFloatSlider(
		STR_WA1,
		0,
		100,
		0,
		100,
		5,
		1,
		0,
		FALSE,
		ID_WA1
	);
	cs.AddFloatSlider(
		STR_WA2,
		0,
		100,
		0,
		100,
		20,
		1,
		0,
		FALSE,
		ID_WA2
	);
	cs.AddFloatSlider(
		STR_WA3,
		0,
		100,
		0,
		100,
		5,
		1,
		0,
		FALSE,
		ID_WA3
	);
	cs.AddPath(
		STR_PATH,
		0,
		ID_PATH
	);
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

	for (int i = 0; i < 4; i++)
		ERR(ae->GetPOINT(ID_POS1 + i, &infoP->pos[i]));
	for (int i = 0; i < 3; i++)
		ERR(ae->GetPOINT(ID_POSA1 + i, &infoP->posA[i]));
	for (int i = 0; i < 3; i++) {
		double d = 0;
		ERR(ae->GetFLOAT(ID_WA1 + i, &d));
		infoP->wA[i] = static_cast<float>(d);
	}
	return err;
}
//-------------------------------------------------------------------------------------------------
static inline PF_Point offP(PF_Point p, A_long x, A_long y)
{
	p.x += x;
	p.y += y;
	return p;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
	Exec (NF_AE*ae , ParamInfo *infoP)
{
	PF_Err	err = PF_Err_NONE;
	PF_InData* in_data = ae->in_data;
	PF_FILL(NULL, NULL, ae->output);
	
	std::vector<std::vector<float>> buf(ae->outputInfo.height,std::vector<float>(ae->outputInfo.width,0));
	A_long ox = -ae->output->origin_x;
	A_long oy = -ae->output->origin_y;

	draw_a_rect(buf,
		offP(infoP->pos[0],ox,oy), offP(infoP->pos[1], ox, oy), offP(infoP->pos[2], ox, oy), offP(infoP->pos[3], ox, oy),
		1.0f);
	draw_a_line(buf, offP(infoP->posA[0], ox, oy), infoP->wA[0], offP(infoP->posA[1], ox, oy), infoP->wA[1],1.0f);
	draw_a_line(buf, offP(infoP->posA[1], ox, oy), infoP->wA[1], offP(infoP->posA[2], ox, oy), infoP->wA[2], 1.0f);

	// ************************************************************
	
	std::vector<PF_PathVertex> pnts;
	ERR(ae->GetPathFromUI(ID_PATH, &pnts));
	if (pnts.size() > 0) {
		std::vector<a_linePrm> linePrms;
		for (const auto& vertex : pnts) {
			PF_FpLong x = vertex.x - ae->output->origin_x;
			PF_FpLong y = vertex.y - ae->output->origin_y;
			a_linePrm prm = { static_cast<float>(x), static_cast<float>(y), 20.0f };
			linePrms.push_back(prm);
		}
		draw_polyline(buf, linePrms, 1.0f);
	}
	
	// *************************************************


	ERR(DrawMask(
		ae->in_data,
		ae->output,
		ae->pixelFormat(),
		ae->suitesP,
		&buf
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

