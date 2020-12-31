#include "FsGraphics.h"

//**********************************************************************************************
PF_Err FsBlend8(PF_EffectWorld 	*input,PF_EffectWorld 	*output,PF_Fixed per)
{
	PF_Err		err 		= PF_Err_NONE;
	PF_Fixed p = per;
	PF_Fixed p2;
	
	PF_Pixel *inData;
	PF_Pixel *outData;

	inData							= input->data;
	outData							= output->data;
	long width					= input->width;
	long height					= input->height;
	long inWidth				= input->rowbytes/sizeof(PF_Pixel);
	long outWidth				= output->rowbytes/sizeof(PF_Pixel);
	long offsetInWidth  = inWidth  - width;
	long offsetOutWidth = outWidth - width;
	
	long i,j,targetIn,targetOut,temp;
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
				pout.alpha = (unsigned char)temp;
				
				temp = ( pin.red * p + pout.red * p2) >> 16;
				if (temp<0) { temp=0; } else if (temp>PF_MAX_CHAN8) { temp=PF_MAX_CHAN8;}
				pout.red = (unsigned char)temp;

				temp = ( pin.green * p + pout.green * p2) >> 16;
				if (temp<0) { temp=0; } else if (temp>PF_MAX_CHAN8) { temp=PF_MAX_CHAN8;}
				pout.green = (unsigned char)temp;

				temp = ( pin.blue * p + pout.blue * p2) >> 16;
				if (temp<0) { temp=0; } else if (temp>PF_MAX_CHAN8) { temp=PF_MAX_CHAN8;}
				pout.blue = (unsigned char)temp;

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

	inData							= (PF_Pixel16 *)input->data;
	outData							= (PF_Pixel16 *)output->data;
	long width					= input->width;
	long height					= input->height;
	long inWidth				= input->rowbytes/sizeof(PF_Pixel16);
	long outWidth				= output->rowbytes/sizeof(PF_Pixel16);
	long offsetInWidth  = inWidth  - width;
	long offsetOutWidth = outWidth - width;
	
	long i,j,targetIn,targetOut,temp;
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
				pout.alpha = (unsigned short)temp;
				
				temp = ( pin.red * p + pout.red * p2) >> 14;
				if (temp<0) { temp=0; } else if (temp>PF_MAX_CHAN16) { temp=PF_MAX_CHAN16;}
				pout.red = (unsigned short)temp;

				temp = ( pin.green * p + pout.green * p2) >> 14;
				if (temp<0) { temp=0; } else if (temp>PF_MAX_CHAN16) { temp=PF_MAX_CHAN16;}
				pout.green = (unsigned short)temp;

				temp = ( pin.blue * p + pout.blue * p2) >> 14;
				if (temp<0) { temp=0; } else if (temp>PF_MAX_CHAN16) { temp=PF_MAX_CHAN16;}
				pout.blue = (unsigned short)temp;
				
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

