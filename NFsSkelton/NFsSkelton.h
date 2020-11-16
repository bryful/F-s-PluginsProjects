//-----------------------------------------------------------------------------------
/*
	NFsSkelton for VS2010
*/
//-----------------------------------------------------------------------------------

#pragma once
#ifndef NFsSkelton_H
#define NFsSkelton_H

#include "../NFsLibrary/AE_SDK.h"
#include "../NFsLibrary/AEInfo.h"


#include "NFs_Target.h"



//ユーザーインターフェースのID
//ParamsSetup関数とRender関数のparamsパラメータのIDになる
enum {
	ID_INPUT = 0,	// default input layer

	ID_VALUE,
	ID_CHECK,
	ID_NUM_PARAMS
};

//UIの表示文字列
#define	STR_VALUE			"value"
#define	STR_CHECK			"check"
#define	STR_ON				"on"




//UIのパラメータ
typedef struct ParamInfo {
	PF_FpLong	value;
	PF_Boolean	check;
} ParamInfo, *ParamInfoP, **ParamInfoH;

//-------------------------------------------------------
class NFsSkelton : public AEInfo
{
public:
	NFsSkelton() 
	{
	};

	NFsSkelton(
		PF_Cmd			cmd,
		PF_InData*		in_dataP,
		PF_OutData*		out_data,
		PF_ParamDef*	params[],
		PF_LayerDef*	output,
		void*			extraP
	) :AEInfo(
		cmd,
		in_dataP,
		out_data,
		params,
		output,
		extraP,
		ID_NUM_PARAMS,
		sizeof(ParamInfo)
	)
	{

	};
	
	// ******************************************************
	PF_Err	About(
		PF_InData* in_dataP,
		PF_OutData* out_dataP,
		PF_ParamDef* paramsP[],
		PF_LayerDef* outputP) override
	{
		PF_Err err = PF_Err_NONE;
		AEInfo::About(in_dataP, out_dataP, paramsP, outputP);

		ERR(AboutBox(
			NFS_DISPNAME,
			MAJOR_VERSION,
			MINOR_VERSION,
			NFS_DESCRIPTION));

		return err;

	};
	// ******************************************************
	PF_Err	GlobalSetup(
		PF_InData* in_dataP,
		PF_OutData* out_dataP,
		PF_ParamDef* paramsP[],
		PF_LayerDef* outputP)
	{
		PF_Err	err = PF_Err_NONE;
		Init();
		in_data = in_dataP;
		suitesP = new AEGP_SuiteHandler(in_dataP->pica_basicP);


		return err;
	}
	// ******************************************************
	PF_Err ParamsSetup(
			PF_InData * in_data,
			PF_OutData * out_data,
			PF_ParamDef * params[],
			PF_LayerDef * output) override
	{
		PF_Err err = PF_Err_NONE;

		return err;
	};
};

//-----------------------------------------------------------------------------------
extern "C" {

	DllExport
	PF_Err
	EffectMain(
			PF_Cmd			cmd,
			PF_InData*		in_dataP,
			PF_OutData*		out_dataP,
			PF_ParamDef*	paramsP[],
			PF_LayerDef*	outputP,
			void*			extraP)
	{
		PF_Err err = PF_Err_NONE;

		try
		{
			NFsSkelton ae;

			switch (cmd) {
			case PF_Cmd_ABOUT:
				err = ae.About(in_dataP, out_dataP, paramsP, outputP);
				break;
			case PF_Cmd_GLOBAL_SETUP:
				err = ae.GlobalSetup(in_dataP, out_dataP, paramsP, outputP);
				break;
			case PF_Cmd_GLOBAL_SETDOWN:
				err = ae.GlobalSetdown(in_dataP);
				break;
			case PF_Cmd_PARAMS_SETUP:
				err = ae.ParamsSetup(in_dataP, out_dataP, paramsP, outputP);
				break;
			case PF_Cmd_SEQUENCE_SETUP:
				err = ae.SequenceSetup(in_dataP, out_dataP, paramsP, outputP);
				break;
			case PF_Cmd_SEQUENCE_SETDOWN:
				err = ae.SequenceSetdown(in_dataP, out_dataP, paramsP, outputP);
				break;
			case PF_Cmd_SEQUENCE_RESETUP:
				err = ae.SequenceResetup(in_dataP, out_dataP, paramsP, outputP);
				break;
			case PF_Cmd_RENDER:
				err = ae.Render(in_dataP, out_dataP, paramsP, outputP);
				break;
			case PF_Cmd_SMART_PRE_RENDER:
				err = ae.PreRender(in_dataP, out_dataP, reinterpret_cast<PF_PreRenderExtra*>(extraP));
				break;
			case PF_Cmd_SMART_RENDER:
				err = ae.SmartRender(in_dataP, out_dataP, reinterpret_cast<PF_SmartRenderExtra*>(extraP));
				break;
			case PF_Cmd_COMPLETELY_GENERAL:
				err = ae.RespondtoAEGP(in_dataP, out_dataP, paramsP, outputP, extraP);
				break;
			case PF_Cmd_DO_DIALOG:
				//err = PopDialog(in_data,out_data,params,output);
				break;
			case PF_Cmd_USER_CHANGED_PARAM:
				err = ae.HandleChangedParam(in_dataP,
					out_dataP,
					paramsP,
					outputP,
					reinterpret_cast<PF_UserChangedParamExtra*>(extraP));
				break;
			case PF_Cmd_QUERY_DYNAMIC_FLAGS:
				err = ae.QueryDynamicFlags(in_dataP,
					out_dataP,
					paramsP,
					reinterpret_cast<PF_UserChangedParamExtra*>(extraP));
				break;
			}
		}
		catch (PF_Err& thrown_err) {
			err = thrown_err;
		}
		return err;
	}
}
#endif // NFsSkelton_H
