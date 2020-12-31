//-----------------------------------------------------------------------------------
/*
	F's Plugins for VS2010/VS2012
*/
//-----------------------------------------------------------------------------------


#include "NoiseHiLo_RGB.h"


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
	PF_ADD_FLOAT_SLIDER(STR_HI_POS,	//Name
						5,						//VALID_MIN
						30,						//VALID_MAX
						5,						//SLIDER_MIN
						30,						//SLIDER_MAX
						1,						//CURVE_TOLERANCE
						20,						//DFLT
						1,						//PREC
						0,						//DISP
						0,						//WANT_PHASE
						ID_HI_POS
						);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_LO_POS,	//Name
						5,						//VALID_MIN
						30,						//VALID_MAX
						5,						//SLIDER_MIN
						30,						//SLIDER_MAX
						1,						//CURVE_TOLERANCE
						20,						//DFLT
						1,						//PREC
						0,						//DISP
						0,						//WANT_PHASE
						ID_LO_POS
						);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_HI_LV,	//Name
						0,						//VALID_MIN
						100,					//VALID_MAX
						0,						//SLIDER_MIN
						5,						//SLIDER_MAX
						1,						//CURVE_TOLERANCE
						0,						//DFLT
						1,						//PREC
						0,						//DISP
						0,						//WANT_PHASE
						ID_HI_LV
						);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_MD_LV,	//Name
						0,						//VALID_MIN
						100,					//VALID_MAX
						0,						//SLIDER_MIN
						5,						//SLIDER_MAX
						1,						//CURVE_TOLERANCE
						0,						//DFLT
						1,						//PREC
						0,						//DISP
						0,						//WANT_PHASE
						ID_MD_LV
						);
	//----------------------------------------------------------------
	AEFX_CLR_STRUCT(def);
	PF_ADD_FLOAT_SLIDER(STR_LO_LV,	//Name
						0,						//VALID_MIN
						100,					//VALID_MAX
						0,						//SLIDER_MIN
						5,						//SLIDER_MAX
						1,						//CURVE_TOLERANCE
						0,						//DFLT
						1,						//PREC
						0,						//DISP
						0,						//WANT_PHASE
						ID_LO_LV
						);

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
	PF_Err				err					= PF_Err_NONE;
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
	PF_Err 	err 	= PF_Err_NONE;
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
	PF_Pixel p = *outP;
	

	PF_FpLong a =  (PF_FpLong)F_RAND_D1();
	PF_FpLong b =  (PF_FpLong)F_RAND_D1();
	PF_FpLong m =  (PF_FpLong)F_RAND_D1();
	
	if (p.alpha<=0) return err;

	ParamInfo *	niP		= reinterpret_cast<ParamInfo*>(refcon);
		
	PF_FpLong v = pxV8(p);
	if((v<=0) || (v>=1.0) ) return err;

	a *= niP->hiLv;
	if (a != 0){
		PF_FpLong st = ( 1 - 2 * niP->hiPos);  
		if(v>st){
			PF_FpLong md = ( 1 - niP->hiPos); 
			if (v == md){
				//そのまま
			}else if (v<md){
				a *= (v-st)/(md-st);
			}else{
				a *= (1-v)/(1-md);
			}
			A_long aa = (A_long)(PF_MAX_CHAN8 * a);
			p.red	= RoundByteLong((A_long)p.red+aa);
			p.green	= RoundByteLong((A_long)p.green+aa);
			p.blue	= RoundByteLong((A_long)p.blue+aa);
		}
	}
	m *= niP->mdLv;
	if (m != 0){
		PF_FpLong st = (niP->loPos);  
		PF_FpLong md1 = (niP->loPos*2);  
		PF_FpLong md2 = (1 - niP->hiPos*2);
		if ( md1>md2){
			md1 = (md1-md2)/2;
			md2 = md1;
		}
		PF_FpLong lt = (1 - niP->hiPos);  
		if((v>st)&&(v<lt)){
			if ((v >= md1)&&(v <= md2)){
				//そのまま
			}else if (v<md1){
				m *= (v-st)/(md1-st);
			}else{
				m *= 1 - (v-md2)/(lt-md2);
			}
			A_long mm = (A_long)(PF_MAX_CHAN8 * m);
			p.red	= RoundByteLong((A_long)p.red+mm);
			p.green	= RoundByteLong((A_long)p.green+mm);
			p.blue	= RoundByteLong((A_long)p.blue+mm);
		}
	}

	b *= niP->loLv;
	if (b != 0){
		PF_FpLong lt = ( niP->loPos *2); 
		if(v<lt){
			PF_FpLong md = ( niP->loPos); 
			if (v == md){
				//そのまま
			}else if (v<md){
				b *= v/md;
			}else{
				b *= 1 - (v-md)/(lt-md);
			}
			A_long bb = (A_long)(PF_MAX_CHAN8 * b);
			p.red	= RoundByteLong((A_long)p.red+bb);
			p.green	= RoundByteLong((A_long)p.green+bb);
			p.blue	= RoundByteLong((A_long)p.blue+bb);
		}
	}
	*outP = p;
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
FilterImage16 (
	refconType		refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel16	*inP, 
	PF_Pixel16	*outP)
{
	PF_Err			err = PF_Err_NONE;
	PF_Pixel16 p = *outP;
	

	PF_FpLong a =  (PF_FpLong)F_RAND_D1();
	PF_FpLong b =  (PF_FpLong)F_RAND_D1();
	PF_FpLong m =  (PF_FpLong)F_RAND_D1();
	
	if (p.alpha<=0) return err;

	ParamInfo *	niP		= reinterpret_cast<ParamInfo*>(refcon);
		
	PF_FpLong v = pxV16(p);
	if((v<=0) || (v>=1.0) ) return err;

	a *= niP->hiLv;
	if (a != 0){
		PF_FpLong st = ( 1 - 2 * niP->hiPos);  
		if(v>st){
			PF_FpLong md = ( 1 - niP->hiPos); 
			if (v == md){
				//そのまま
			}else if (v<md){
				a *= (v-st)/(md-st);
			}else{
				a *= (1-v)/(1-md);
			}
			A_long aa = (A_long)(PF_MAX_CHAN16 * a);
			p.red	= RoundShort((A_long)p.red+aa);
			p.green	= RoundShort((A_long)p.green+aa);
			p.blue	= RoundShort((A_long)p.blue+aa);
		}
	}
	m *= niP->mdLv;
	if (m != 0){
		PF_FpLong st = (niP->loPos);  
		PF_FpLong md1 = (niP->loPos*2);  
		PF_FpLong md2 = (1 - niP->hiPos*2);
		if ( md1>md2){
			md1 = (md1-md2)/2;
			md2 = md1;
		}
		PF_FpLong lt = (1 - niP->hiPos);  
		if((v>st)&&(v<lt)){
			if ((v >= md1)&&(v <= md2)){
				//そのまま
			}else if (v<md1){
				m *= (v-st)/(md1-st);
			}else{
				m *= 1 - (v-md2)/(lt-md2);
			}
			A_long mm = (A_long)(PF_MAX_CHAN16 * m);
			p.red	= RoundShort((A_long)p.red+mm);
			p.green	= RoundShort((A_long)p.green+mm);
			p.blue	= RoundShort((A_long)p.blue+mm);
		}
	}

	b *= niP->loLv;
	if (b != 0){
		PF_FpLong lt = ( niP->loPos *2); 
		if(v<lt){
			PF_FpLong md = ( niP->loPos); 
			if (v == md){
				//そのまま
			}else if (v<md){
				b *= v/md;
			}else{
				b *= 1 - (v-md)/(lt-md);
			}
			A_long bb = (A_long)(PF_MAX_CHAN16 * b);
			p.red	= RoundShort((A_long)p.red+bb);
			p.green	= RoundShort((A_long)p.green+bb);
			p.blue	= RoundShort((A_long)p.blue+bb);
		}
	}
	*outP = p;
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
FilterImage32 (
	refconType		refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_PixelFloat	*inP, 
	PF_PixelFloat	*outP)
{
	PF_Err			err = PF_Err_NONE;
	PF_PixelFloat p = *outP;
	

	PF_FpLong a =  (PF_FpLong)F_RAND_D1();
	PF_FpLong b =  (PF_FpLong)F_RAND_D1();
	PF_FpLong m =  (PF_FpLong)F_RAND_D1();
	
	if (p.alpha<=0) return err;

	ParamInfo *	niP		= reinterpret_cast<ParamInfo*>(refcon);
		
	PF_FpLong v = pxV32(p);
	if((v<=0) || (v>=1.0) ) return err;

	a *= niP->hiLv;
	if (a != 0){
		PF_FpLong st = ( 1 - 2 * niP->hiPos);  
		if(v>st){
			PF_FpLong md = ( 1 - niP->hiPos); 
			if (v == md){
				//そのまま
			}else if (v<md){
				a *= (v-st)/(md-st);
			}else{
				a *= (1-v)/(1-md);
			}
			PF_FpLong aa = a;
			p.red	= RoundFpShortDouble(p.red+aa);
			p.green	= RoundFpShortDouble(p.green+aa);
			p.blue	= RoundFpShortDouble(p.blue+aa);
		}
	}
	m *= niP->mdLv;
	if (m != 0){
		PF_FpLong st = (niP->loPos);  
		PF_FpLong md1 = (niP->loPos*2);  
		PF_FpLong md2 = (1 - niP->hiPos*2);
		if ( md1>md2){
			md1 = (md1-md2)/2;
			md2 = md1;
		}
		PF_FpLong lt = (1 - niP->hiPos);  
		if((v>st)&&(v<lt)){
			if ((v >= md1)&&(v <= md2)){
				//そのまま
			}else if (v<md1){
				m *= (v-st)/(md1-st);
			}else{
				m *= 1 - (v-md2)/(lt-md2);
			}
			PF_FpLong mm = m;
			p.red	= RoundFpShortDouble(p.red+mm);
			p.green	= RoundFpShortDouble(p.green+mm);
			p.blue	= RoundFpShortDouble(p.blue+mm);
		}
	}

	b *= niP->loLv;
	if (b != 0){
		PF_FpLong lt = ( niP->loPos *2); 
		if(v<lt){
			PF_FpLong md = ( niP->loPos); 
			if (v == md){
				//そのまま
			}else if (v<md){
				b *= v/md;
			}else{
				b *= 1 - (v-md)/(lt-md);
			}
			PF_FpLong bb = b;
			p.red	= RoundFpShortDouble(p.red+bb);
			p.green	= RoundFpShortDouble(p.green+bb);
			p.blue	= RoundFpShortDouble(p.blue+bb);
		}
	}
	*outP = p;
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err GetParams(CFsAE *ae, ParamInfo *infoP)
{
	PF_Err		err 		= PF_Err_NONE;

	ERR(ae->GetFLOAT(ID_HI_POS,&infoP->hiPos));
	infoP->hiPos /= 100;
	ERR(ae->GetFLOAT(ID_LO_POS,&infoP->loPos));
	infoP->loPos /= 100;

	ERR(ae->GetFLOAT(ID_HI_LV,&infoP->hiLv));
	infoP->hiLv /= 100;
	ERR(ae->GetFLOAT(ID_MD_LV,&infoP->mdLv));
	infoP->mdLv /= 100;
	ERR(ae->GetFLOAT(ID_LO_LV,&infoP->loLv));
	infoP->loLv /= 100;
	return err;
}
//-------------------------------------------------------------------------------------------------
static PF_Err 
	Exec (CFsAE *ae , ParamInfo *infoP)
{
	PF_Err	err = PF_Err_NONE;

	//画面をコピー
	ERR(ae->CopyInToOut());
	
	F_SRAND(ae->frame());
	switch(ae->pixelFormat())
	{
	case PF_PixelFormat_ARGB128:
		ERR(ae->iterate32((refconType)infoP,FilterImage32));
		break;
	case PF_PixelFormat_ARGB64:
		ERR(ae->iterate16((refconType)infoP,FilterImage16));
		break;
	case PF_PixelFormat_ARGB32:
		ERR(ae->iterate8((refconType)infoP,FilterImage8));
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
