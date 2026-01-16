#include "FsG.h"
// ****************************************************************************************
PF_Err To16WorldSub08(refconType refcon, A_long x, A_long y, PF_Pixel* inP, PF_Pixel16* outP)
{
	PF_Err err = PF_Err_NONE;
	*outP = CONV8TO16(*inP);
	return err;
}
// ****************************************************************************************
PF_Err To16WorldSub16(refconType refcon, A_long x, A_long y, PF_Pixel16* inP, PF_Pixel16* outP)
{
	PF_Err err = PF_Err_NONE;
	*outP = *inP;
	return err;
}
// ****************************************************************************************
PF_Err To16WorldSub32(refconType refcon, A_long x, A_long y, PF_Pixel32* inP, PF_Pixel16* outP)
{
	PF_Err err = PF_Err_NONE;
	*outP = CONV32TO16( *inP);
	return err;
}
// ****************************************************************************************
PF_Err World08To16(PF_EffectWorld *src, PF_EffectWorld *dst)
{
	PF_Err err = PF_Err_NONE;
	ERR(iterateWorld08to16(NULL, src, dst, To16WorldSub08));
	return err;
}

// ****************************************************************************************
PF_Err World32To16(PF_EffectWorld *src, PF_EffectWorld *dst)
{
	PF_Err err = PF_Err_NONE;
	ERR(iterateWorld32to16(NULL, src, dst, To16WorldSub32));
	return err;
}
// ****************************************************************************************
// ****************************************************************************************
PF_Err From16WorldSub08(refconType refcon, A_long x, A_long y, PF_Pixel16* inP, PF_Pixel8* outP)
{
	PF_Err err = PF_Err_NONE;
	*outP = CONV16TO8(*inP);
	return err;
}
// ****************************************************************************************
PF_Err From16WorldSub16(refconType refcon, A_long x, A_long y, PF_Pixel16* inP, PF_Pixel16* outP)
{
	PF_Err err = PF_Err_NONE;
	*outP = *inP;
	return err;
}
// ****************************************************************************************
PF_Err From16WorldSub32(refconType refcon, A_long x, A_long y, PF_Pixel16* inP, PF_Pixel32* outP)
{
	PF_Err err = PF_Err_NONE;
	*outP = CONV16TO32(*inP);
	return err;
}
// ****************************************************************************************
// ****************************************************************************************
PF_Err World16To08(PF_EffectWorld *src, PF_EffectWorld *dst)
{
	PF_Err err = PF_Err_NONE;
	ERR(iterateWorld16to08(NULL, src, dst, From16WorldSub08));
	return err;
}
// ****************************************************************************************
PF_Err World16To16(PF_EffectWorld *src, PF_EffectWorld *dst)
{
	PF_Err err = PF_Err_NONE;
	ERR(iterateWorld16to16(NULL, src, dst, From16WorldSub16));
	return err;
}
// ****************************************************************************************
PF_Err World16To32(PF_EffectWorld *src, PF_EffectWorld *dst)
{
	PF_Err err = PF_Err_NONE;
	ERR(iterateWorld16to32(NULL, src, dst, From16WorldSub32));
	return err;
}

