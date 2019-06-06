//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------


#include "OpenCV.h"


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
	AEFX_CLR_STRUCT(def);
	//def.ui_flags = PF_PUI_DISABLED;
	PF_ADD_SLIDER(	STR_VALUE,	//パラメータの名前
					0, 		//数値入力する場合の最小値
					10,			//数値入力する場合の最大値
					0,				//スライダーの最小値 
					10,			//スライダーの最大値
					0,				//デフォルトの値
					ID_VALUE
					);
	//----------------------------------------------------------------


	//----------------------------------------------------------------
	out_data->num_params = 	ID_NUM_PARAMS; 

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
cv::Mat World2CVMat8(const PF_EffectWorldPtr world)
{
	int w = world->width;
	int wt = world->rowbytes / sizeof(PF_Pixel);
	int h = world->height;

	int cz = 4;
	int czw = cz * w;

	cv::Mat ret(cv::Size((int)world->width, (int)world->height), CV_8UC4);

	A_u_char* mData = (A_u_char*)ret.data;
	PF_Pixel* wData = (PF_Pixel *)world->data;


	A_long matPos = 0;
	A_long wldPos = 0;
	for (A_long y = 0; y < h; y++)
	{

		for (A_long x = 0; x < w; x++)
		{
			A_long matPosx = matPos + x * cz;
			A_long wldPosx = wldPos + x;
			mData[matPosx + 0] = wData[wldPosx].red;
			mData[matPosx + 1] = wData[wldPosx].green;
			mData[matPosx + 2] = wData[wldPosx].blue;
			mData[matPosx + 3] = wData[wldPosx].alpha;

		}
		matPos += czw;
		wldPos += wt;
	}
	return ret;

}
//-------------------------------------------------------------------------------------------------
void WorldToCVMat8(const PF_EffectWorldPtr world,cv::Mat mat)
{
	int w = world->width;
	int wt = world->rowbytes / sizeof(PF_Pixel);
	int h = world->height;

	int cz = 4;
	int czw = cz * w;

	mat =  cv::Mat(cv::Size((int)world->width, (int)world->height), CV_8UC4);

	A_u_char* mData = (A_u_char*)mat.data;
	PF_Pixel* wData = (PF_Pixel *)world->data;


	A_long matPos = 0;
	A_long wldPos = 0;
	for (A_long y = 0; y < h; y++)
	{

		for (A_long x = 0; x < w; x++)
		{
			A_long matPosx = matPos + x * cz;
			A_long wldPosx = wldPos + x;
			mData[matPosx + 0] = wData[wldPosx].red;
			mData[matPosx + 1] = wData[wldPosx].green;
			mData[matPosx + 2] = wData[wldPosx].blue;
			mData[matPosx + 3] = wData[wldPosx].alpha;

		}
		matPos += czw;
		wldPos += wt;
	}

}
//-------------------------------------------------------------------------------------------------
void CVMat2World8(const cv::Mat mat, PF_EffectWorldPtr world)
{
	int w = world->width;
	int wt = world->rowbytes / sizeof(PF_Pixel);
	int h = world->height;

	int w2 = mat.cols;
	int h2 = mat.rows;

	if ((w != w2) || (h != h2)) return;

	int cz = 4;
	int czw = cz * w;

	A_u_char* mData = (A_u_char*)mat.data;
	PF_Pixel* wData = (PF_Pixel *)world->data;


	A_long matPos = 0;
	A_long wldPos = 0;
	for (A_long y = 0; y < h; y++)
	{

		for (A_long x = 0; x < w; x++)
		{
			A_long matPosx = matPos + x * cz;
			A_long wldPosx = wldPos + x;
			wData[wldPosx].red = mData[matPosx + 0];
			wData[wldPosx].green = mData[matPosx + 1];
			wData[wldPosx].blue = mData[matPosx + 2];
			wData[wldPosx].alpha = mData[matPosx + 3];

		}
		matPos += czw;
		wldPos += wt;
	}

}

//-------------------------------------------------------------------------------------------------
PF_Err Test8(CFsAE *ae, ParamInfo *infoP)
{
	PF_Err			err = PF_Err_NONE;

	int w = ae->input->width;
	int wt = ae->input->rowbytes / sizeof(PF_Pixel);
	int h = ae->input->height;

	int cz = 4;
	int czw = cz * w;

	cv::Mat src(cv::Size((int)w, (int)h), CV_8UC4, cv::Scalar(128, 0, 0, 255));

	//WorldToCVMat8(ae->input, src);
	//cv::Mat dst;
	//cv::GaussianBlur(src, src, cv::Size(31, 31), 0, 0);

	//cv::Mat dst(cv::Size(ae->out->width(), ae->out->height()), CV_8UC4, cv::Scalar(128,0, 0, 255));

	//cv::Mat green_img = src.clone();
	//src = cv::Scalar(255,0, 255, 255);
	cv::putText(src, "Test Moon", cv::Point(50, 50), cv::FONT_HERSHEY_TRIPLEX, 1.5, cv::Scalar(255,0, 128, 200), 2, CV_AA);


	CVMat2World8(src, ae->output);


	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
FilterImage8 (
	refconType		refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel8	*inP, 
	PF_Pixel8	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo *	niP		= reinterpret_cast<ParamInfo*>(refcon);



	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
FilterImage16 (
	refconType	refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel16	*inP, 
	PF_Pixel16	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo *	niP		= reinterpret_cast<ParamInfo*>(refcon);

	

	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
FilterImage32 (
	refconType	refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_PixelFloat	*inP, 
	PF_PixelFloat	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo *	niP		= reinterpret_cast<ParamInfo*>(refcon);



	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err GetParams(CFsAE *ae, ParamInfo *infoP)
{
	PF_Err		err 		= PF_Err_NONE;

	ERR(ae->GetADD(ID_VALUE,&infoP->value));
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
	Exec (CFsAE *ae , ParamInfo *infoP)
{
	PF_Err	err = PF_Err_NONE;

	//画面をコピー
	//ERR(ae->CopyInToOut());
	

	switch(ae->pixelFormat())
	{
	case PF_PixelFormat_ARGB128:
		//ERR(ae->iterate32((refconType)infoP,FilterImage32));
		break;
	case PF_PixelFormat_ARGB64:
		//ERR(ae->iterate16((refconType)infoP,FilterImage16));
		break;
	case PF_PixelFormat_ARGB32:
		//ERR(ae->iterate8((refconType)infoP,FilterImage8));
		ERR(Test8(ae,infoP));
		break;
	}
	return err;
}

//-------------------------------------------------------------------------------------------------
//レンダリングのメイン
/*
	SmartFXに対応していないホスト(After Effects7以前のもの)はこの関数が呼び出されて描画する
	この関数を書いておけば一応v6.5対応になる
*/
static PF_Err 
Render ( 
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output )
{

	PF_Err	err = PF_Err_NONE;
	PF_Handle		pixelTable = NULL;
	
	CFsAE ae(in_data,out_data,params,output,ID_NUM_PARAMS);
	err =ae.resultErr();
	if (!err){
		ParamInfo info;
		ERR(GetParams(&ae,&info));
		ERR(Exec(&ae,&info));
	}
	return err;
}
//-----------------------------------------------------------------------------------
/*
	SmartFX対応の場合、まずこの関数が呼ばれてパラメータの獲得を行う
*/
#if defined(SUPPORT_SMARTFX)
static PF_Err
PreRender(
	PF_InData			*in_data,
	PF_OutData			*out_data,
	PF_PreRenderExtra	*extraP)
{
	PF_Err		err 		= PF_Err_NONE;
	CFsAE ae(in_data,out_data,extraP,sizeof(ParamInfo),ID_NUM_PARAMS);
	err = ae.resultErr();
	if (!err){

		ParamInfo *infoP = reinterpret_cast<ParamInfo*>(ae.LockPreRenderData());
		if (infoP){
			ae.SetHostPreRenderData();
			ERR(GetParams(&ae,infoP));
			ERR(ae.UnSetPreRenderData());
			ae.UnlockPreRenderData();
		}else{
			err = PF_Err_OUT_OF_MEMORY;
		}
	}
	return err;
}
#endif
//-----------------------------------------------------------------------------------
#if defined(SUPPORT_SMARTFX)
static PF_Err
SmartRender(
	PF_InData				*in_data,
	PF_OutData				*out_data,
	PF_SmartRenderExtra		*extraP)
{
	PF_Err			err		= PF_Err_NONE,
					err2 	= PF_Err_NONE;

	CFsAE ae(in_data,out_data,extraP,ID_NUM_PARAMS);
	err = ae.resultErr();
	if (!err){
		ParamInfo *infoP = reinterpret_cast<ParamInfo*>(ae.LockPreRenderData());
		if (infoP){
			ERR(Exec(&ae,infoP));
			ERR2(ae.UnsetSmartRender());
			ae.UnlockPreRenderData();
		}else{
			err = PF_Err_OUT_OF_MEMORY;
		}
	}
	return err;
}
#endif

#include "Fs_Entry.h"
