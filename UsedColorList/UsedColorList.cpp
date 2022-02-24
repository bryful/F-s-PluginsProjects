

#include "UsedColorList.h"


// **********************************************************

//-------------------------------------------------------------------------------------------------
static PF_Err 
NFsNoise8 (
	void* refcon,
	A_long		xL, 
	A_long		yL, 
	PF_Pixel8	*inP, 
	PF_Pixel8	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo* infoP = reinterpret_cast<ParamInfo*>(refcon);

	
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
NFsNoise16(
	void* refcon,
	A_long		xL, 
	A_long		yL, 
	PF_Pixel16	*inP, 
	PF_Pixel16	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo* infoP = reinterpret_cast<ParamInfo*>(refcon);




	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
NFsNoise32(
	void* refcon,
	A_long		xL, 
	A_long		yL, 
	PF_PixelFloat	*inP, 
	PF_PixelFloat	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo* infoP = reinterpret_cast<ParamInfo*>(refcon);

	


	return err;
}
//-------------------------------------------------------------------------------------------------
// **********************************************************
PF_Err UsedColorList::ParamsSetup(
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
	//位置の指定
	AEFX_CLR_STRUCT(def);
	PF_ADD_POINT(STR_POS0,			/*"New Center"*/
		50,	// X
		50,	// Y
		0,	// Flag
		ID_POS0
	);
	//----------------------------------------------------------------
	//位置の指定
	AEFX_CLR_STRUCT(def);
	PF_ADD_POINT(STR_POS1,			/*"New Center"*/
		75,	// X
		75,	// Y
		0,	// Flag
		ID_POS1
	);
	//----------------------------------------------------------------
	//色の指定
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(STR_ExceptColor0,
		0xFF,
		0x0,
		0x0,
		ID_ExceptColor0
	);	
	//----------------------------------------------------------------
	//色の指定
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(STR_ExceptColor1,
		0x00,
		0xFF,
		0x0,
		ID_ExceptColor1
	);
	//----------------------------------------------------------------
	//色の指定
	AEFX_CLR_STRUCT(def);
	PF_ADD_COLOR(STR_ExceptColor2,
		0x00,
		0x00,
		0xFF,
		ID_ExceptColor2
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_CHECKBOX(STR_ISDISP,
		STR_ON,
		TRUE,
		0,
		ID_ISDISP
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(STR_GRID_WIDTH,	//パラメータの名前
		10, 		//数値入力する場合の最小値
		300,			//数値入力する場合の最大値
		10,				//スライダーの最小値 
		100,			//スライダーの最大値
		50,				//デフォルトの値
		ID_GRID_WIDTH
	);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_SLIDER(STR_GRID_HEIGHT,	//パラメータの名前
		10, 		//数値入力する場合の最小値
		300,			//数値入力する場合の最大値
		10,				//スライダーの最小値 
		100,			//スライダーの最大値
		50,				//デフォルトの値
		ID_GRID_HEIGHT
	);	//----------------------------------------------------------------
	out_data->num_params = ID_NUM_PARAMS;
	return err;
};
// **********************************************************
PF_Err UsedColorList::GetParams(ParamInfo* infoP)
{
	PF_Err err = PF_Err_NONE;
	PF_FixedPoint p0;
	PF_FixedPoint p1;
	ERR(GetPOINT_FIXED(ID_POS0, &p0));
	ERR(GetPOINT_FIXED(ID_POS1, &p1));

	infoP->Area.left = (A_long)((double)p0.x / 65536 + 0.5);
	infoP->Area.top= (A_long)((double)p0.y / 65536 + 0.5);
	infoP->Area.right = (A_long)((double)p1.x / 65536 + 0.5);
	infoP->Area.bottom = (A_long)((double)p1.y / 65536 + 0.5);
	if (infoP->Area.left > infoP->Area.right)
	{
		A_long p = infoP->Area.left;
		infoP->Area.left = infoP->Area.right;
		infoP->Area.right = p;
	}
	if (infoP->Area.top > infoP->Area.bottom)
	{
		A_long p = infoP->Area.top;
		infoP->Area.top = infoP->Area.bottom;
		infoP->Area.bottom = p;
	}
	if (infoP->Area.left < 0) infoP->Area.left = 0;
	if (infoP->Area.top < 0) infoP->Area.top = 0;

	ERR(GetCHECKBOX(ID_ISDISP, &infoP->IsGridDisp));
	ERR(GetADD(ID_GRID_WIDTH, &infoP->GridWidth));
	ERR(GetADD(ID_GRID_HEIGHT, &infoP->GridHeight));

	ERR(GetCOLOR(ID_ExceptColor0, &infoP->ExceptColor0));
	ERR(GetCOLOR(ID_ExceptColor1, &infoP->ExceptColor1));
	ERR(GetCOLOR(ID_ExceptColor2, &infoP->ExceptColor2));

	return err;
};
// **********************************************************
PF_Err UsedColorList::Exec(ParamInfo* infoP)
{
	PF_Err err = PF_Err_NONE;
	NFsWorld* src = new NFsWorld(input, in_data, pixelFormat());
	UsedColorListFX* dst = new UsedColorListFX(output, in_data, pixelFormat());
	dst->Copy(src);

	if (infoP->IsGridDisp)
	{
		dst->ColorScan(infoP);
		dst->DrawList(infoP);
	}
	ERR(dst->DrawRect(infoP->Area));

	delete src;
	delete dst;

	return err;

};
// **********************************************************
