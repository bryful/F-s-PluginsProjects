#include "OpticalDiffusion.h"

//-------------------------------------------------------------------------------------------------
static PF_Err 
noise8 (
	refconType		refcon, 
	A_long		xL, 
	A_long		yL, 
	PF_Pixel8	*inP, 
	PF_Pixel8	*outP)
{
	PF_Err			err = PF_Err_NONE;
	ParamInfo *	niP		= reinterpret_cast<ParamInfo*>(refcon);

	A_long v = (A_long)(niP->noiseValue * PF_MAX_CHAN8);
	A_long rnd =  v  -  (v * 2 * F_RAND() / F_RAND_MAX);
	if (rnd!=0){
		if (outP->alpha>0){
			outP->red	= RoundByteLong( outP->red + rnd);
			outP->green	= RoundByteLong( outP->green + rnd);
			outP->blue	= RoundByteLong( outP->blue + rnd);
		}
	}
	return err;
}
