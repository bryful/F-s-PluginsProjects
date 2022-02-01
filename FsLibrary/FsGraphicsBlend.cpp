#include "FsGraphics.h"

//**********************************************************************************************
PF_Err FsBlend8(PF_EffectWorld 	*input,PF_EffectWorld 	*output,PF_Fixed per)
{
	PF_Err		err 		= PF_Err_NONE;
	PF_Fixed p = per;
	PF_Fixed p2;
	
	PF_Pixel *inData;
	PF_Pixel *outData;

	inData					= input->data;
	outData					= output->data;
	A_long width			= input->width;
	A_long height			= input->height;
	A_long inWidth			= input->rowbytes/sizeof(PF_Pixel);
	A_long outWidth			= output->rowbytes/sizeof(PF_Pixel);
	A_long offsetInWidth	= inWidth  - width;
	A_long offsetOutWidth	= outWidth - width;
	
	A_long i,j,targetIn,targetOut,temp;
	PF_Pixel pin,pout;
	if ((input==NULL)||(output==NULL)){
		return PF_Err_INTERNAL_STRUCT_DAMAGED;
	}
	//p = p / 100;
	if (p<(0L<<16)) p = 0L<<16;
	if (p>(1L<<16)) p = 1L<<16;
	p2 = (1L<<16) - p;

	if (p==0){
		//何もしない。
		return err;
	}else if (p == (1L<<16)) {
		targetIn = targetOut =0;
		for (j=0;j<height;j++){
			for (i=0;i<width;i++){
				outData[targetOut] = inData[targetIn];		
				targetIn++;
				targetOut++;
			}
			targetIn += offsetInWidth;
			targetOut+= offsetOutWidth;
		}
	}else{
		targetIn = targetOut =0;
		for (j=0;j<height;j++){
			for (i=0;i<width;i++){
				pin		= inData[targetIn];
				pout	= outData[targetOut];

				temp = ( pin.alpha * p + pout.alpha * p2) >> 16;
				if (temp<0) { temp=0; } else if (temp>PF_MAX_CHAN8) { temp=PF_MAX_CHAN8;}
				pout.alpha = (A_u_char)temp;
				
				temp = ( pin.red * p + pout.red * p2) >> 16;
				if (temp<0) { temp=0; } else if (temp>PF_MAX_CHAN8) { temp=PF_MAX_CHAN8;}
				pout.red = (A_u_char)temp;

				temp = ( pin.green * p + pout.green * p2) >> 16;
				if (temp<0) { temp=0; } else if (temp>PF_MAX_CHAN8) { temp=PF_MAX_CHAN8;}
				pout.green = (A_u_char)temp;

				temp = ( pin.blue * p + pout.blue * p2) >> 16;
				if (temp<0) { temp=0; } else if (temp>PF_MAX_CHAN8) { temp=PF_MAX_CHAN8;}
				pout.blue = (A_u_char)temp;

				outData[targetOut] = pout;

				targetIn++;
				targetOut++;
			}
			targetIn += offsetInWidth;
			targetOut+= offsetOutWidth;
		}
	}

	return err;
}
//**********************************************************************************************
PF_Err FsBlend16(PF_EffectWorld 	*input,PF_EffectWorld 	*output,PF_Fixed per)
{
	PF_Err		err 		= PF_Err_NONE;
	PF_Fixed p = per;
	PF_Fixed p2;
	
	PF_Pixel16 *inData;
	PF_Pixel16 *outData;

	inData					= (PF_Pixel16 *)input->data;
	outData					= (PF_Pixel16 *)output->data;
	A_long width			= input->width;
	A_long height			= input->height;
	A_long inWidth			= input->rowbytes/sizeof(PF_Pixel16);
	A_long outWidth			= output->rowbytes/sizeof(PF_Pixel16);
	A_long offsetInWidth	= inWidth  - width;
	A_long offsetOutWidth	= outWidth - width;
	
	A_long i,j,targetIn,targetOut,temp;
	PF_Pixel16 pin,pout;

	if ((input==NULL)||(output==NULL)){
		return PF_Err_INTERNAL_STRUCT_DAMAGED;
	}
	p = p >>2;
	if (p<(0L<<14)) p = 0L<<14;
	if (p>(1L<<14)) p = 1L<<14;
	p2 = (1L<<14) - p;

	if (p==0){
		//何もしない。
		return err;
	}else if (p == (1L<<14)) {
		targetIn = targetOut =0;
		for (j=0;j<height;j++){
			for (i=0;i<width;i++){
				outData[targetOut] = inData[targetIn];		
				targetIn++;
				targetOut++;
			}
			targetIn += offsetInWidth;
			targetOut+= offsetOutWidth;
		}
	}else{
		targetIn = targetOut =0;
		for (j=0;j<height;j++){
			for (i=0;i<width;i++){
				pin		= inData[targetIn];
				pout	= outData[targetOut];

				temp = ( pin.alpha * p + pout.alpha * p2) >> 14;
				if (temp<0) { temp=0; } else if (temp>PF_MAX_CHAN16) { temp=PF_MAX_CHAN16;}
				pout.alpha = (A_u_short)temp;
				
				temp = ( pin.red * p + pout.red * p2) >> 14;
				if (temp<0) { temp=0; } else if (temp>PF_MAX_CHAN16) { temp=PF_MAX_CHAN16;}
				pout.red = (A_u_short)temp;

				temp = ( pin.green * p + pout.green * p2) >> 14;
				if (temp<0) { temp=0; } else if (temp>PF_MAX_CHAN16) { temp=PF_MAX_CHAN16;}
				pout.green = (A_u_short)temp;

				temp = ( pin.blue * p + pout.blue * p2) >> 14;
				if (temp<0) { temp=0; } else if (temp>PF_MAX_CHAN16) { temp=PF_MAX_CHAN16;}
				pout.blue = (A_u_short)temp;
				
				outData[targetOut] = pout;

				targetIn++;
				targetOut++;
			}
			targetIn += offsetInWidth;
			targetOut+= offsetOutWidth;
		}
	}

	return err;
}
//**********************************************************************************************
PF_Err FsBlend32(PF_EffectWorld 	*input,PF_EffectWorld 	*output,PF_Fixed per)
{
	PF_Err	err 		= PF_Err_NONE;
	
	PF_PixelFloat *inData;
	PF_PixelFloat *outData;

	inData					= (PF_PixelFloat *)input->data;
	outData					= (PF_PixelFloat *)output->data;
	A_long width			= input->width;
	A_long height			= input->height;
	A_long inWidth			= input->rowbytes/sizeof(PF_PixelFloat);
	A_long outWidth			= output->rowbytes/sizeof(PF_PixelFloat);
	A_long offsetInWidth	= inWidth  - width;
	A_long offsetOutWidth	= outWidth - width;
	
	A_long i,j,targetIn,targetOut;
	PF_FpShort temp;
	PF_PixelFloat pin,pout;

	if ((input==NULL)||(output==NULL)){
		return PF_Err_INTERNAL_STRUCT_DAMAGED;
	}
	PF_FpShort p = (PF_FpShort)per / (1L<<16);
	if ( p<0.0) p=0.0;
	else if ( p>1.0) p= 1.0;

	PF_FpShort p2 = (PF_FpShort)1.0 - p;

	if (p==0){
		//何もしない。
		return err;
	}else if (p == 1.0) {
		targetIn = targetOut =0;
		for (j=0;j<height;j++){
			for (i=0;i<width;i++){
				outData[targetOut] = inData[targetIn];		
				targetIn++;
				targetOut++;
			}
			targetIn += offsetInWidth;
			targetOut+= offsetOutWidth;
		}
	}else{
		targetIn = targetOut =0;
		for (j=0;j<height;j++){
			for (i=0;i<width;i++){
				pin		= inData[targetIn];
				pout	= outData[targetOut];

				temp = pin.alpha * p + pout.alpha * p2;
				if (temp<0) { temp=0; } else if (temp>1.0) { temp=1.0;}
				pout.alpha = temp;
				
				temp = pin.red * p + pout.red * p2;
				if (temp<0) { temp=0; } else if (temp>1.0) { temp=1.0;}
				pout.red = temp;

				temp = pin.green * p + pout.green * p2;
				if (temp<0) { temp=0; } else if (temp>1.0) { temp=1.0;}
				pout.green = temp;

				temp = pin.blue * p + pout.blue * p2;
				if (temp<0) { temp=0; } else if (temp>1.0) { temp=1.0;}
				pout.blue = temp;
				
				outData[targetOut] = pout;

				targetIn++;
				targetOut++;
			}
			targetIn += offsetInWidth;
			targetOut+= offsetOutWidth;
		}
	}

	return err;
}
//**********************************************************************************************

