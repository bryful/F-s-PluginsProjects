#include "colorThreshold.h"

//*************************************************************************************************
//-------------------------------------------------------------------------------------------------
/*
	アルファを2値化
*/
PF_Err 
AlphaThreshold32 (
	refconType		refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_PixelFloat	*inP, 
	PF_PixelFloat	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo *	niP		= reinterpret_cast<ParamInfo*>(refcon);
	PF_PixelFloat tp = {0,1,1,1};


	A_u_char v = RoundByteLong(FS_CONVERT32TO8(inP->alpha));
	if (v >= niP->alpha_border){
		*outP = *inP;
		outP->alpha = (PF_FpShort)((PF_FpShort)CT_TARGET / CT_FLOAT);
	}else{
		*outP = tp;
	}

	return err;
}
//-------------------------------------------------------------------------------------------------
PF_Err 
AlphaFix32 (
	refconType		refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_PixelFloat	*inP, 
	PF_PixelFloat	*outP)
{
	PF_Err			err = PF_Err_NONE;
	A_long v = (A_long)(outP->alpha * CT_FLOAT) & CT_DECISION;
	if ( v == 0) { 
		outP->alpha = 0;
	}else{
		outP->alpha = 1;
	}
	return err;
}
//*************************************************************************************************
//-------------------------------------------------------------------------------------------------
/*
	指定された色ならONにする
*/
//-------------------------------------------------------------------------------------------------
PF_Err 
hslMatch32 (
	refconType		refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_PixelFloat	*inP, 
	PF_PixelFloat	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo *	niP	= reinterpret_cast<ParamInfo*>(refcon);
	
	A_long v = (A_long)(outP->alpha * CT_FLOAT)  & CT_DECISION; 
	if ( v== CT_TARGET){
		hls o = rgbToHls( CONV32TO8(*outP));
		if (niP->main_col_count>0){
			for ( A_long i = 0; i< niP->main_col_count; i++){
				if ( colorCompare(o,niP->main_col[i],niP->hs_level,niP->l_level)==TRUE)
				{
					*outP = niP->main_col32[i];
					A_long a =  (A_long)(outP->alpha * CT_FLOAT);
					a |= CT_DECISION;
					outP->alpha =  (PF_FpShort)((PF_FpShort)a/ CT_FLOAT); 
					return err;
				}
			}
		}
		if(niP->sub_col_count>0){
			for ( A_long i = 0; i< niP->sub_col_count; i++){
				if ( colorCompare(o,niP->sub_col[i],niP->hs_level,niP->l_level)==TRUE)
				{
					*outP = niP->sub_col32[i];
					A_long a =  (A_long)(outP->alpha * CT_FLOAT);
					a |= CT_DECISION;
					outP->alpha =  (PF_FpShort)((PF_FpShort)a/ CT_FLOAT); 
					return err;
				}
			}
		}
	}
	return err;
}
//-------------------------------------------------------------------------------------------------
PF_Err 
line_search32 (
	refconType		refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel32	*inP, 
	PF_Pixel32	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo *	niP		= reinterpret_cast<ParamInfo*>(refcon);
	
	A_long v = (A_long)(outP->alpha * CT_FLOAT) & CT_DECISION; 
	if ( v== CT_TARGET){
		hls o = rgbToHls( CONV32TO8(*outP));
		for (A_long i=0; i<niP->main_col_count; i++ ){
			if ( colorCompare(o,niP->main_col[i],niP->main_hs[i],niP->main_l[i])==TRUE)
			{
				*outP = niP->main_col32[i];
				A_long a =  (A_long)(outP->alpha * CT_FLOAT);
				a |= CT_DECISION;
				outP->alpha =  (PF_FpShort)((PF_FpShort)a/ CT_FLOAT); 
				break;
			}
		}

	}
	return err;
}
//-------------------------------------------------------------------------------------------------
PF_Err 
use_search32 (
	refconType		refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_PixelFloat	*inP, 
	PF_PixelFloat	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo *	niP		= reinterpret_cast<ParamInfo*>(refcon);
	
	A_long v = (A_long)(outP->alpha * CT_FLOAT) & CT_DECISION; 
	if ( v== CT_TARGET){
		PF_PixelFloat tbl[4];
		tbl[0] = (niP->out->getPixel32(xL  ,yL-1));
		tbl[1] = (niP->out->getPixel32(xL-1,yL  ));
		tbl[2] = (niP->out->getPixel32(xL+1,yL  ));
		tbl[3] = (niP->out->getPixel32(xL  ,yL+1));

		A_long idx = -1;
		PF_FpLong dis = 100.0;
		hls o = rgbToHls( CONV32TO8(*outP));
		for ( A_long i=0; i<4; i++)
		{
			A_long a = (A_long)(tbl[i].alpha * CT_FLOAT) &CT_DECISION;
			if ( a== CT_DECISION) {
				PF_FpLong dis2 = colorDiatance(o,tbl[i]);
				if (dis > dis2) {
					dis = dis2;
					idx = (A_long)(tbl[i].alpha * CT_FLOAT);
				}
			}
		}
		if (idx>=0){
			idx = (idx & 0xFC)|CT_SEARCH;
			outP->alpha = (PF_FpShort)((PF_FpShort)idx / CT_FLOAT);
		}else{
			niP->finFlag = FALSE;
		}

	}
	return err;

}
//-------------------------------------------------------------------------------------------------
PF_Err 
use_searchSub32 (
	refconType		refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_PixelFloat	*inP, 
	PF_PixelFloat	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo *	niP		= reinterpret_cast<ParamInfo*>(refcon);
	
	A_long v = (A_long)(outP->alpha * CT_FLOAT) & CT_DECISION; 
	if ( v== CT_SEARCH){
		A_long idx = (A_long)(outP->alpha * CT_FLOAT) >>2;
		PF_FpShort a = (PF_FpShort)(idx|CT_DECISION)/CT_FLOAT;
		if ( idx<COUNT_OF_MAIN) {
			*outP = niP->main_col32[idx];
			outP->alpha  =  a; 
		}else if ( idx< (COUNT_OF_MAIN +COUNT_OF_SUB) ){
			*outP = niP->sub_col32[idx-COUNT_OF_MAIN];
			outP->alpha  =  a; 
		}
	}
	return err;
}
//-------------------------------------------------------------------------------------------------
