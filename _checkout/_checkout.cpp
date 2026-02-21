//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------


#include "_checkout.h"

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
	cs.AddLayer(	// layer
		STR_LAYER,
		PF_LayerDefault_NONE,
		ID_LAYER
	);
	
	cs.AddPoint(
		STR_POS,
		50,
		50,
		FALSE,
		ID_POS
	);
	cs.AddSlider(
		STR_COUNT,
		0,
		30000,
		0,
		1000,
		60,
		ID_COUNT
	);
	cs.AddSlider(
		STR_RADIUS,
		0,
		1000,
		0,
		100,
		50,
		ID_RADIUS
	);
	cs.AddFloatSlider(	// R
		STR_SCALE,			//Name
		10,				//VALID_MIN
		200,			//VALID_MAX
		50,				//SLIDER_MIN
		100,			//SLIDER_MAX
		100,			//DFLT
		1,				//PREC 小数点以下の桁数
		0,				//DISP
		FALSE,			//WANT_PHASE
		ID_SCALE
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

	ERR(ae->GetPOINT(ID_POS, &infoP->pos));
	ERR(ae->GetADD(ID_COUNT, &infoP->count));
	ERR(ae->GetADD(ID_RADIUS, &infoP->radius));
	PF_FpLong scale;
	ERR(ae->GetFLOAT(ID_SCALE, &scale));
	infoP->scale =  (float)(scale/100.0f);
	if (infoP->scale < 0.1f) infoP->scale = 0.1f;

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
	Exec (NF_AE*ae , ParamInfo *infoP)
{
	PF_Err	err = PF_Err_NONE;
	PF_InData* in_data = ae->in_data;
	//画面をコピー
	ERR(ae->CopyInToOut());
	std::vector<std::vector<A_u_char>> spat = GetDefSpatData(infoP->scale);

	PF_ParamDef checkout_param;
	err = PF_CHECKOUT_PARAM(in_data,
		ID_LAYER,
		in_data->current_time,
		in_data->time_step,
		in_data->time_scale,
		&checkout_param);
	if (!err) {
		PF_EffectWorld* other_layer_world = &checkout_param.u.ld;
		/*
		if (other_layer_world->data) {
			PF_Rect src_rect = { 0,0,other_layer_world->width, other_layer_world->height };
			PF_Rect dst_rect = { 
				infoP->pos.x, infoP->pos.y, 
				infoP->pos.x + other_layer_world->width, 
				infoP->pos.y + other_layer_world->height };
			err = ae->suitesP->WorldTransformSuite1()->copy(
				in_data->effect_ref,
				other_layer_world, // 元
				ae->output,     // 先
				&src_rect, &dst_rect);
		}
		*/
		if (other_layer_world->data) {
			int sz = 48;
				int cw = other_layer_world->width /sz;
				int ch = other_layer_world->height / sz;
				int nsz = (int)((float)sz * infoP->scale + 0.5);
				int nw = cw * nsz;
				int nh = ch * nsz;
				if (nw > sz && nh > sz) {
					PF_EffectWorld temp_world;
					ae->NewWorld(nw, nh, ae->pixelFormat(), &temp_world);
					PF_InData* in_data = ae->in_data;
					PF_Rect src_rect = { 0,0,other_layer_world->width, other_layer_world->height };
					PF_Rect dst_rect = { 0,0,nw,nh };
					PF_COPY(other_layer_world, &temp_world, &src_rect, &dst_rect);
					spat = GetSpatDataFromWorld(&temp_world, ae->pixelFormat(), nsz, nsz);
					ae->DisposeWorld(&temp_world);
				}
		}
		// 4. 「他のレイヤ」をチェックイン（必須！）
		err = PF_CHECKIN_PARAM(in_data, &checkout_param);

	}
	
	if (infoP->count > 0)
	{
		if (spat.size() > 0) {

			for (int i = 0; i < infoP->count; i++) {
				A_long index = (A_long)(hash_float(i, 500, ae->frame()) * (float)spat.size());
				float angle = (float)(hash_float(i, 10, ae->frame()) * 360.0f * M_PI / 180.0f);
				//float radius = (float)(hash_float(i, 20, ae->frame()) * infoP->radius);
				float radius = (float)(sqrt(hash_float(i, 20, ae->frame())) * infoP->radius);

				A_long xx = (A_long)(radius * cos(angle));
				A_long yy = (A_long)(radius * sin(angle));

				A_u_char r = (A_u_char)(hash_float(i, 100, ae->frame()) * 255);
				A_u_char g = (A_u_char)(hash_float(i, 300, ae->frame()) * 255);
				A_u_char b = (A_u_char)(hash_float(i, 400, ae->frame()) * 255);

				float rscale = (float)(hash_float(i, 500, ae->frame()) * 1.2f + 0.3f);
				BlendSpatToWorld(
					ae->output,
					ae->pixelFormat(),
					infoP->pos.x + xx,
					infoP->pos.y + yy,
					ScaleSpatBilinear(spat[index],rscale),
					PF_Pixel{ 255,r,g,b },
					1.0f);

			}
		}

	}



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
		{
			PF_PreRenderExtra* prExtra = reinterpret_cast<PF_PreRenderExtra*>(extraP);
			err = ae.PreRender(in_data, out_data, prExtra, sizeof(ParamInfo), ID_NUM_PARAMS);
			if (!err) {
				ParamInfo* infoP = reinterpret_cast<ParamInfo*>(ae.LockPreRenderData());
				if (infoP) {
					ae.SetHostPreRenderData();
					ERR(GetParams(&ae, infoP));
					ERR(ae.UnSetPreRenderData());
					ae.UnlockPreRenderData();
					PF_RenderRequest req = prExtra->input->output_request;
					PF_CheckoutResult checkout_res;
					err = prExtra->cb->checkout_layer(in_data->effect_ref,
						ID_LAYER,
						ID_LAYER,
						&req,
						in_data->current_time,
						in_data->time_step,
						in_data->time_scale,
						&checkout_res);
					if (!err) {
						UnionLRect(&checkout_res.result_rect, &prExtra->output->result_rect);
						UnionLRect(&checkout_res.max_result_rect, &prExtra->output->max_result_rect);

						//prExtra->output->solid = FALSE;
						//prExtra->output->result_rect = prExtra->input->output_request.rect;
						//prExtra->output->max_result_rect = prExtra->input->output_request.rect;
        
						prExtra->output->solid = FALSE;
					}
				}
				else {
					err = PF_Err_OUT_OF_MEMORY;
				}
			}
			break;
		}
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

