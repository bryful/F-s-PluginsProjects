#include "Mult.h"


// **********************************************************
PF_Err Mult::ParamsSetup(
	PF_InData* in_dataP,
	PF_OutData* out_dataP,
	PF_ParamDef* paramsP[],
	PF_LayerDef* outputP)
{
	PF_Err err = PF_Err_NONE;
	Init();
	m_cmd = PF_Cmd_PARAMS_SETUP;
	in_data = in_dataP;
	out_data = out_dataP;
	PF_ParamDef		def;
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(NF_CATEGORY, NF_NAME, FALSE, 0, ID_VERSION);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_POPUP(
		"Mode",
		4,	//メニューの数
		1,	//デフォルト
		"Unmult|Mult|ToStraight|ToPremultiplited",
		ID_MODE
	);
	//----------------------------------------------------------------
//----------------------------------------------------------------
	out_data->num_params = ID_NUM_PARAMS;
	return err;
};
// **********************************************************
PF_Err Mult::GetParams(ParamInfo* infoP)
{
	PF_Err err = PF_Err_NONE;
	ERR(GetPOPUP(ID_MODE, &infoP->Mode));
	return err;
};
// **********************************************************
PF_Err Mult::Exec(ParamInfo* infoP)
{
	PF_Err err = PF_Err_NONE;
	NFWorld* src = new NFWorld(input, in_data, pixelFormat());
	NFWorld* dst = new NFWorld(output, in_data, pixelFormat());


	switch (infoP->Mode)
	{
	case 2:
		MultExec(infoP, src, dst);
		break;
	case 1:
		UnmultExec(infoP, src, dst);
		break;
	case 3:
		ToStraightExec(infoP, src, dst);
		break;
	case 4:
		ToPremultExec(infoP, src, dst);
		break;
	default:
		dst->Copy(src);
		break;
	}
	delete src;
	delete dst;

	return err;

};
// **********************************************************
