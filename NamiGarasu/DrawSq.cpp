#include "NamiGarasu.h"
#include "DrawSqData.h"
/*
	0:5		
	1:20	10000
	2:40	2500
	3:60	1111
	4:80	625
	5:100	400
	6:120	278
	7:140	200	
	8:160	156	
	9:180	120
	10:200	100

	0: { 0,0,0,0,0,0,0
	1:
	2:
	3:
	4:
	5:
	6:
	7:
	8:
	9:


*/
//---------------------------------------------------------------
void drawSq8(DrawInfo *infoP)
{
	int si = infoP->sizeIdx / infoP->scale;
	int len = dataSize[si];
	int lenH= len/2;
	int cnt = 0;
	PF_Pixel *data;
	data = (PF_Pixel *)infoP->scr.data;
	for (int ay=0; ay<len; ay++){
		int y = (infoP->y + ay - lenH) % infoP->scr.height;
		if (y<0) {
			y += infoP->scr.height;
		}else if (y>=infoP->scr.height) {
			y -= infoP->scr.height;
		}
		int yP = y * infoP->scr.widthTrue;
		for (int ax=0; ax<len; ax++){
			int x = (infoP->x + ax - lenH) % infoP->scr.width;
			if (x<0) {
				x += infoP->scr.width;
			}else if (x>=infoP->scr.width) {
				x -= infoP->scr.width;
			}
			double par;
			switch(si){
			case 0:par = (double)data10[cnt]/ PF_MAX_CHAN8;break;
			case 1:par = (double)data20[cnt]/ PF_MAX_CHAN8;break;
			case 2:par = (double)data40[cnt]/ PF_MAX_CHAN8;break;
			case 3:par = (double)data60[cnt]/ PF_MAX_CHAN8;break;
			case 4:par = (double)data80[cnt]/ PF_MAX_CHAN8;break;
			case 5:par = (double)data100[cnt]/ PF_MAX_CHAN8;break;
			case 6:par = (double)data120[cnt]/ PF_MAX_CHAN8;break;
			case 7:par = (double)data140[cnt]/ PF_MAX_CHAN8;break;
			case 8:par = (double)data160[cnt]/ PF_MAX_CHAN8;break;
			case 9:par = (double)data180[cnt]/ PF_MAX_CHAN8;break;
			case 10:par = (double)data200[cnt]/ PF_MAX_CHAN8;break;
			}
			par *= infoP->strong;
			A_long s,d;
			if (par>0) { 
				A_long pos = x +yP;
				s = (A_long)data[pos].red;
				if (infoP->isW==0){
					d = PF_MAX_CHAN8;	
				}else{
					d = 0;	
				}
				d =  (A_long)( (double)s + ((double)d -(double)s)*par +0.5);
				if (d<0) d = 0;
				else if (d>PF_MAX_CHAN8) d = PF_MAX_CHAN8;
				
				data[pos].red = (A_u_char)d;
			}
			cnt++;

		}
	}
}
//---------------------------------------------------------------
void drawSq16(DrawInfo *infoP)
{
	int si = infoP->sizeIdx / infoP->scale;
	int len = dataSize[si];
	int lenH= len/2;
	int cnt = 0;
	PF_Pixel16 *data;
	data = (PF_Pixel16 *)infoP->scr.data;
	for (int ay=0; ay<len; ay++){
		int y = (infoP->y + ay - lenH) % infoP->scr.height;
		if (y<0) {
			y += infoP->scr.height;
		}else if (y>=infoP->scr.height) {
			y -= infoP->scr.height;
		}
		int yP = y * infoP->scr.widthTrue;
		for (int ax=0; ax<len; ax++){
			int x = (infoP->x + ax - lenH) % infoP->scr.width;
			if (x<0) {
				x += infoP->scr.width;
			}else if (x>=infoP->scr.width) {
				x -= infoP->scr.width;
			}
			double par;
			switch(si){
			case 0:par = (double)data10[cnt]/ PF_MAX_CHAN8;break;
			case 1:par = (double)data20[cnt]/ PF_MAX_CHAN8;break;
			case 2:par = (double)data40[cnt]/ PF_MAX_CHAN8;break;
			case 3:par = (double)data60[cnt]/ PF_MAX_CHAN8;break;
			case 4:par = (double)data80[cnt]/ PF_MAX_CHAN8;break;
			case 5:par = (double)data100[cnt]/ PF_MAX_CHAN8;break;
			case 6:par = (double)data120[cnt]/ PF_MAX_CHAN8;break;
			case 7:par = (double)data140[cnt]/ PF_MAX_CHAN8;break;
			case 8:par = (double)data160[cnt]/ PF_MAX_CHAN8;break;
			case 9:par = (double)data180[cnt]/ PF_MAX_CHAN8;break;
			case 10:par = (double)data200[cnt]/ PF_MAX_CHAN8;break;
			}
			par *= infoP->strong;
			A_long s,d;
			if (par>0) { 
				A_long pos = x +yP;
				s = (A_long)data[pos].red;
				if (infoP->isW==0){
					d = PF_MAX_CHAN16;	
				}else{
					d = 0;	
				}
				d =  (A_long)( (double)s + ((double)d -(double)s)*par +0.5);
				if (d<0) d = 0;
				else if (d>PF_MAX_CHAN16) d = PF_MAX_CHAN16;
				data[pos].red = (A_u_short)d;
			}
			cnt++;

		}
	}
}
//---------------------------------------------------------------
void drawSq32(DrawInfo *infoP)
{
	int si = infoP->sizeIdx / infoP->scale;
	int len = dataSize[si];
	int lenH= len/2;
	int cnt = 0;
	PF_PixelFloat *data;
	data = (PF_PixelFloat *)infoP->scr.data;
	for (int ay=0; ay<len; ay++){
		int y = (infoP->y + ay - lenH) % infoP->scr.height;
		if (y<0) {
			y += infoP->scr.height;
		}else if (y>=infoP->scr.height) {
			y -= infoP->scr.height;
		}
		int yP = y * infoP->scr.widthTrue;
		for (int ax=0; ax<len; ax++){
			int x = (infoP->x + ax - lenH) % infoP->scr.width;
			if (x<0) {
				x += infoP->scr.width;
			}else if (x>=infoP->scr.width) {
				x -= infoP->scr.width;
			}
			double par;
			switch(si){
			case 0:par = (double)data10[cnt]/ PF_MAX_CHAN8;break;
			case 1:par = (double)data20[cnt]/ PF_MAX_CHAN8;break;
			case 2:par = (double)data40[cnt]/ PF_MAX_CHAN8;break;
			case 3:par = (double)data60[cnt]/ PF_MAX_CHAN8;break;
			case 4:par = (double)data80[cnt]/ PF_MAX_CHAN8;break;
			case 5:par = (double)data100[cnt]/ PF_MAX_CHAN8;break;
			case 6:par = (double)data120[cnt]/ PF_MAX_CHAN8;break;
			case 7:par = (double)data140[cnt]/ PF_MAX_CHAN8;break;
			case 8:par = (double)data160[cnt]/ PF_MAX_CHAN8;break;
			case 9:par = (double)data180[cnt]/ PF_MAX_CHAN8;break;
			case 10:par = (double)data200[cnt]/ PF_MAX_CHAN8;break;
			}
			par *= infoP->strong;
			double s,d;
			if (par>0) { 
				A_long pos = x +yP;
				s = (double)data[pos].red;
				if (infoP->isW==0){
					d = 1.0;	
				}else{
					d = 0;	
				}
				//d =  (A_long)( (double)s + ((double)d -(double)s)*par +0.5);
				d =  ( s + (d - s)*par);
				if (d<0) d = 0;
				else if (d>1.0) d = 1.0;
				data[pos].red = (PF_FpShort)d;
			}
			cnt++;

		}
	}
}
