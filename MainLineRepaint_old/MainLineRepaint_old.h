/*
	F's Plugins AfterEffects 7.0 SDK
	MainLineRepaint
*/

#ifndef MainLineRepaint_H
#define MainLineRepaint_H

#pragma once

#include "AEConfig.h"

#include "entry.h"
#include "AE_Effect.h"
#include "AE_EffectCB.h"
#include "AE_Macros.h"
#include "Param_Utils.h"
#include "String_Utils.h"

#include "MainLineRepaint_old_Strings.h"
#include "FsAE.h"
#include "FsGraphics.h"


#ifdef AE_OS_WIN
	#include <Windows.h>
#endif

//バージョンを買えたMainLineRepaintPiPL.rのAE_Effect_Versionも変えること
#define	MAJOR_VERSION		1
#define	MINOR_VERSION		0
#define	BUG_VERSION			0
//#define	STAGE_VERSION		PF_Stage_DEVELOP
//#define	STAGE_VERSION		PF_Stage_ALPHA
//#define	STAGE_VERSION		PF_Stage_BETA
#define	STAGE_VERSION		PF_Stage_RELEASE
#define	BUILD_VERSION		0



//ユーザーインターフェースのID
//ParamsSetup関数とRender関数のparamsパラメータのIDになる
enum {
	MY_INPUT = 0,	// default input layer 
	
	MY_Main_Color,

	MY_NUM_PARAMS
	};
	


class CMainLineRepaint : public CFsAE{
public:
	CMainLineRepaint(
		PF_InData			*in_data,
		PF_OutData		*out_data,
		PF_ParamDef		*params[],
		PF_LayerDef		*output ):CFsAE(in_data,out_data,params,output){
			getParams(in_data,out_data,params,output);
		}

		typedef struct  {
			long	X;
			long	Y;
			long	Adr;
			long	len;
			PF_Pixel mc8;
			PF_Pixel16 mc16;
			PF_Pixel fc8;
			PF_Pixel16 fc16;
		}findColorPrm;

	//パラメータを獲得する関数
	PF_Err getParams(
		PF_InData			*in_data,
		PF_OutData		*out_data,
		PF_ParamDef		*params[],
		PF_LayerDef		*output );
	//以下固有のパラメータ
	PF_Pixel  		Main_Color;

	//------------------------------------------------------
	inline PF_Boolean cmpPx8(PF_Pixel c0,PF_Pixel c1){
		return ( (c0.blue == c1.blue)&&(c0.green == c1.green)&&(c0.red == c1.red) );
	}
	//------------------------------------------------------
	inline PF_Boolean cmpPx16(PF_Pixel16 c0,PF_Pixel16 c1){
		return ( (c0.blue == c1.blue)&&(c0.green == c1.green)&&(c0.red == c1.red) );
	}
	//------------------------------------------------------
	void alphaOn8()
	{
		int w = out->width();
		int h = out->height();
		int ow = out->offsetWidth();
		int i,j,target =0;
		PF_Pixel c ;
		PF_Pixel tc = {PF_MAX_CHAN8,PF_MAX_CHAN8,PF_MAX_CHAN8,PF_MAX_CHAN8};
		PF_Pixel* data = (PF_Pixel *)out->data();

		for (j=0;j<h;j++){
			for (i=0;i<w;i++){
				c = data[target];
				if (c.alpha!=PF_MAX_CHAN8)
				{
					data[target] = tc;
				}
				target++;
			}
			target+= ow;
		}
	}
	//------------------------------------------------------
	void alphaOn16()
	{
		int w = out->width();
		int h = out->height();
		int ow = out->offsetWidth();
		int i,j,target =0;
		PF_Pixel16 c ;
		PF_Pixel16 tc = {PF_MAX_CHAN16,PF_MAX_CHAN16,PF_MAX_CHAN16,PF_MAX_CHAN16};
		PF_Pixel16* data = (PF_Pixel16 *)out->data();

		for (j=0;j<h;j++){
			for (i=0;i<w;i++){
				c = data[target];
				if (c.alpha!=PF_MAX_CHAN16)
				{
					data[target] = tc;
				}
				target++;
			}
			target+= ow;
		}
	}
	//------------------------------------------------------
	void onePointChk8()
	{
		int w = out->width();
		int h = out->height();
		int ow = out->offsetWidth();
		int i,j,target =0;
		PF_Pixel p = Main_Color;
		PF_Pixel c ;
		PF_Pixel tc = {PF_MAX_CHAN8,PF_MAX_CHAN8,PF_MAX_CHAN8,PF_MAX_CHAN8};
		PF_Pixel* data = (PF_Pixel *)out->data();

		for (j=0;j<h;j++){
			for (i=0;i<w;i++){
				c = data[target];
				if (cmpPx8(c,p)){
					if (
						(cmpPx8(out->getPixel8_W(i-1,j-1),tc))&&
						(cmpPx8(out->getPixel8_W(i  ,j-1),tc))&&
						(cmpPx8(out->getPixel8_W(i+1,j-1),tc))&&
						(cmpPx8(out->getPixel8_W(i-1,j  ),tc))&&
						(cmpPx8(out->getPixel8_W(i+1,j  ),tc))&&
						(cmpPx8(out->getPixel8_W(i-1,j+1),tc))&&
						(cmpPx8(out->getPixel8_W(i  ,j+1),tc))&&
						(cmpPx8(out->getPixel8_W(i+1,j+1),tc))
						)
					{
						data[target] = tc;
					}
				}
				target++;
			}
			target+= ow;
		}
	}
	//------------------------------------------------------
	inline PF_Pixel16 change16(PF_Pixel c)
	{
		PF_Pixel16 p;
		p.alpha = (unsigned short)CONVERT8TO16(c.alpha);
		p.blue = (unsigned short)CONVERT8TO16(c.blue);
		p.green = (unsigned short)CONVERT8TO16(c.green);
		p.red = (unsigned short)CONVERT8TO16(c.red);
		return p;
	}
	//------------------------------------------------------
	void onePointChk16()
	{
		int w = out->width();
		int h = out->height();
		int ow = out->offsetWidth();
		int i,j,target =0;
		PF_Pixel16 p = change16(Main_Color);
		PF_Pixel16 c ;
		PF_Pixel16 tc = {PF_MAX_CHAN16,PF_MAX_CHAN16,PF_MAX_CHAN16,PF_MAX_CHAN16};
		PF_Pixel16* data = (PF_Pixel16 *)out->data();

		for (j=0;j<h;j++){
			for (i=0;i<w;i++){
				c = data[target];
				if (cmpPx16(c,p)){
					if (
						(cmpPx16(out->getPixel16_W(i-1,j-1),tc))&&
						(cmpPx16(out->getPixel16_W(i  ,j-1),tc))&&
						(cmpPx16(out->getPixel16_W(i+1,j-1),tc))&&
						(cmpPx16(out->getPixel16_W(i-1,j  ),tc))&&
						(cmpPx16(out->getPixel16_W(i+1,j  ),tc))&&
						(cmpPx16(out->getPixel16_W(i-1,j+1),tc))&&
						(cmpPx16(out->getPixel16_W(i  ,j+1),tc))&&
						(cmpPx16(out->getPixel16_W(i+1,j+1),tc))
						)
					{
						data[target] = tc;
					}
				}
				target++;
			}
			target+= ow;
		}
	}
	//------------------------------------------------------
	PF_Boolean findColor8(findColorPrm *prm)
	{
		long x = prm->X;
		long y = prm->Y;
		long adr = prm->Adr;
		PF_Pixel mc = prm->mc8;
		PF_Pixel p = prm->mc8;
		PF_Pixel W = {PF_MAX_CHAN8,PF_MAX_CHAN8,PF_MAX_CHAN8,PF_MAX_CHAN8};
		long l = prm->len;
		
		if (l<=0) {
			return FALSE;
		}else if (l==1){
			p = out->getPixel8_W(x-1,y-1); if ( ( cmpPx8(p,W)==FALSE)&&( cmpPx8(p,mc)==FALSE) ) {prm->fc8 = p; return TRUE; } 
			p = out->getPixel8_W(x  ,y-1); if ( ( cmpPx8(p,W)==FALSE)&&( cmpPx8(p,mc)==FALSE) ) {prm->fc8 = p; return TRUE; } 
			p = out->getPixel8_W(x+1,y-1); if ( ( cmpPx8(p,W)==FALSE)&&( cmpPx8(p,mc)==FALSE) ) {prm->fc8 = p; return TRUE; } 
			p = out->getPixel8_W(x-1,y  ); if ( ( cmpPx8(p,W)==FALSE)&&( cmpPx8(p,mc)==FALSE) ) {prm->fc8 = p; return TRUE; } 
			//p = out->getPixel8_W(x  ,y  ); if ( ( cmpPx8(p,W)==FALSE)&&( cmpPx8(p,mc)==FALSE) ) {prm->fc8 = p; return TRUE; } 
			p = out->getPixel8_W(x+1,y  ); if ( ( cmpPx8(p,W)==FALSE)&&( cmpPx8(p,mc)==FALSE) ) {prm->fc8 = p; return TRUE; } 
			p = out->getPixel8_W(x-1,y+1); if ( ( cmpPx8(p,W)==FALSE)&&( cmpPx8(p,mc)==FALSE) ) {prm->fc8 = p; return TRUE; } 
			p = out->getPixel8_W(x  ,y+1); if ( ( cmpPx8(p,W)==FALSE)&&( cmpPx8(p,mc)==FALSE) ) {prm->fc8 = p; return TRUE; } 
			p = out->getPixel8_W(x+1,y+1); if ( ( cmpPx8(p,W)==FALSE)&&( cmpPx8(p,mc)==FALSE) ) {prm->fc8 = p; return TRUE; } 
		}
	}
	//------------------------------------------------------
	void mainColor8()
	{
		int w = out->width();
		int h = out->height();
		int ow = out->offsetWidth();
		PF_Pixel* data = (PF_Pixel *)out->data();
		PF_Pixel W = {PF_MAX_CHAN8,PF_MAX_CHAN8,PF_MAX_CHAN8,PF_MAX_CHAN8};

		long i,j,ii,target =0;
		//long jj;
		long lp = 100;
		PF_Pixel mc = Main_Color;
		PF_Pixel tc,p;
		long nv = 1000;
		long pv = 1000;

		PF_Pixel nc = {0,0,0,PF_MAX_CHAN8};
		PF_Pixel BL = {0,0,0,PF_MAX_CHAN8};

		//----------------------
		target =0;
		for (j=0;j<h;j++){
			for (i=0;i<w;i++){
				tc = data[target];
				if (cmpPx8(tc,mc)){
					pv = 1000;
					lp = 6;
					//00
					nv = -1;
					for (ii=1;ii<=lp;ii++){
						p = in->getPixel8_W(i,j-ii);
						if (cmpPx8(p,W)) { nv=-1;break; }
						if (cmpPx8(p,mc)==FALSE) { nv=ii;break;}
					}
					if ( (nv>0)&&(nv<pv) ){pv = nv; nc = p;lp = nv;}
					//45
					nv = -1;
					for (ii=1;ii<=lp;ii++){
						p = in->getPixel8_W(i+ii,j-ii);
						if (cmpPx8(p,W)) { nv=-1;break; }
						if (cmpPx8(p,mc)==FALSE) { nv=ii;break;}
					}
					if ( (nv>0)&&(nv<pv) ){pv = nv; nc = p;lp = nv;}
					//90
					nv = -1;
					for (ii=1;ii<=lp;ii++){
						p = in->getPixel8_W(i+ii,j);
						if (cmpPx8(p,W)) { nv=-1;break; }
						if (cmpPx8(p,mc)==FALSE) { nv=ii;break;}
					}
					if ( (nv>0)&&(nv<pv) ){pv = nv; nc = p;lp = nv;}
					//135
					nv = -1;
					for (ii=1;ii<=lp;ii++){
						p = in->getPixel8_W(i+ii,j+ii);
						if (cmpPx8(p,W)) { nv=-1;break; }
						if (cmpPx8(p,mc)==FALSE) { nv=ii;break;}
					}
					if ( (nv>0)&&(nv<pv) ){pv = nv; nc = p;lp = nv;}
					//180
					nv = -1;
					for (ii=1;ii<=lp;ii++){
						p = in->getPixel8_W(i,j+ii);
						if (cmpPx8(p,W)) { nv=-1;break; }
						if (cmpPx8(p,mc)==FALSE) { nv=ii;break;}
					}
					if ( (nv>0)&&(nv<pv) ){pv = nv; nc = p;lp = nv;}
					//225
					nv = -1;
					for (ii=1;ii<=lp;ii++){
						p = in->getPixel8_W(i-ii,j+ii);
						if (cmpPx8(p,W)) { nv=-1;break; }
						if (cmpPx8(p,mc)==FALSE) { nv=ii;break;}
					}
					if ( (nv>0)&&(nv<pv) ){pv = nv; nc = p;lp = nv;}
					//270
					nv = -1;
					for (ii=1;ii<=lp;ii++){
						p = in->getPixel8_W(i-ii,j);
						if (cmpPx8(p,W)) { nv=-1;break; }
						if (cmpPx8(p,mc)==FALSE) { nv=ii;break;}
					}
					if ( (nv>0)&&(nv<pv) ){pv = nv; nc = p;lp = nv;}
					//315
					nv = -1;
					for (ii=1;ii<=lp;ii++){
						p = in->getPixel8_W(i-ii,j-ii);
						if (cmpPx8(p,W)) { nv=-1;break; }
						if (cmpPx8(p,mc)==FALSE) { nv=ii;break;}
					}
					if ( (nv>0)&&(nv<pv) ){pv = nv; nc = p;lp = nv;}
					
					if (pv<1000){
						data[target] = nc;
					}

				}
				target++;
			}
			target+= ow;
		}
		//-------------------------------------------------------------
		target =0;
		PF_Boolean op = FALSE;
		for (j=0;j<h;j++){
			for (i=0;i<w;i++){
				tc = data[target];
				if (cmpPx8(tc,mc)){
					op = FALSE;
					p = mc;
					if (op==FALSE) { p = out->getPixel8_W(i-1,j-1);if ( (cmpPx8(p,mc)==FALSE)&&(cmpPx8(p,W)==FALSE) ) op = TRUE;}
					if (op==FALSE) { p = out->getPixel8_W(i  ,j-1);if ( (cmpPx8(p,mc)==FALSE)&&(cmpPx8(p,W)==FALSE) ) op = TRUE;}
					if (op==FALSE) { p = out->getPixel8_W(i+1,j-1);if ( (cmpPx8(p,mc)==FALSE)&&(cmpPx8(p,W)==FALSE) ) op = TRUE;}
					if (op==FALSE) { p = out->getPixel8_W(i-1,j  );if ( (cmpPx8(p,mc)==FALSE)&&(cmpPx8(p,W)==FALSE) ) op = TRUE;}
					//if (op==FALSE) { p = out->getPixel8_W(i  ,j  );if ( (cmpPx8(p,mc)==FALSE)&&(cmpPx8(p,W)==FALSE) ) op = TRUE;}
					if (op==FALSE) { p = out->getPixel8_W(i+1,j  );if ( (cmpPx8(p,mc)==FALSE)&&(cmpPx8(p,W)==FALSE) ) op = TRUE;}
					if (op==FALSE) { p = out->getPixel8_W(i-1,j+1);if ( (cmpPx8(p,mc)==FALSE)&&(cmpPx8(p,W)==FALSE) ) op = TRUE;}
					if (op==FALSE) { p = out->getPixel8_W(i  ,j+1);if ( (cmpPx8(p,mc)==FALSE)&&(cmpPx8(p,W)==FALSE) ) op = TRUE;}
					if (op==FALSE) { p = out->getPixel8_W(i+1,j+1);if ( (cmpPx8(p,mc)==FALSE)&&(cmpPx8(p,W)==FALSE) ) op = TRUE;}

					if (op==TRUE) {out->paint8(i,j,p);}
				}
				target++;

			}
			target += ow;
		}
		//-------------------------------------------------------------
		target =0;
		for (j=0;j<h;j++){
			for (i=0;i<w;i++){
				tc = data[target];
				if (cmpPx8(tc,mc)){
					data[target] = W;
				}
				target++;

			}
			target += ow;
		}
	}
	//------------------------------------------------------
	void mainColor16()
	{
		int w = out->width();
		int h = out->height();
		int ow = out->offsetWidth();
		PF_Pixel16* data = (PF_Pixel16 *)out->data();

		long i,j,ii,target =0;
		//long jj;
		long lp = 100;
		PF_Pixel16 mc = change16( Main_Color);
		PF_Pixel16 tc,p;
		long nv = 1000;
		long pv = 1000;

		PF_Pixel16 nc = {0,0,0,PF_MAX_CHAN16};
		PF_Pixel16 BL = {0,0,0,PF_MAX_CHAN16};
		PF_Pixel16 W = {PF_MAX_CHAN16,PF_MAX_CHAN16,PF_MAX_CHAN16,PF_MAX_CHAN16};


		target =0;
		for (j=0;j<h;j++){
			for (i=0;i<w;i++){
				tc = data[target];
				if (cmpPx16(tc,mc)){
					pv = 1000;
					lp = 6;
					//00
					nv = -1;
					for (ii=1;ii<=lp;ii++){
						p = in->getPixel16_W(i,j-ii);
						if (cmpPx16(p,W)) { nv=-1;break; }
						if (cmpPx16(p,mc)==FALSE) { nv=ii;break;}
					}
					if ( (nv>0)&&(nv<pv) ){pv = nv; nc = p;lp = nv;}
					//45
					nv = -1;
					for (ii=1;ii<=lp;ii++){
						p = in->getPixel16_W(i+ii,j-ii);
						if (cmpPx16(p,W)) { nv=-1;break; }
						if (cmpPx16(p,mc)==FALSE) { nv=ii;break;}
					}
					if ( (nv>0)&&(nv<pv) ){pv = nv; nc = p;lp = nv;}
					//90
					nv = -1;
					for (ii=1;ii<=lp;ii++){
						p = in->getPixel16_W(i+ii,j);
						if (cmpPx16(p,W)) { nv=-1;break; }
						if (cmpPx16(p,mc)==FALSE) { nv=ii;break;}
					}
					if ( (nv>0)&&(nv<pv) ){pv = nv; nc = p;lp = nv;}
					//135
					nv = -1;
					for (ii=1;ii<=lp;ii++){
						p = in->getPixel16_W(i+ii,j+ii);
						if (cmpPx16(p,W)) { nv=-1;break; }
						if (cmpPx16(p,mc)==FALSE) { nv=ii;break;}
					}
					if ( (nv>0)&&(nv<pv) ){pv = nv; nc = p;lp = nv;}
					//180
					nv = -1;
					for (ii=1;ii<=lp;ii++){
						p = in->getPixel16_W(i,j+ii);
						if (cmpPx16(p,W)) { nv=-1;break; }
						if (cmpPx16(p,mc)==FALSE) { nv=ii;break;}
					}
					if ( (nv>0)&&(nv<pv) ){pv = nv; nc = p;lp = nv;}
					//225
					nv = -1;
					for (ii=1;ii<=lp;ii++){
						p = in->getPixel16_W(i-ii,j+ii);
						if (cmpPx16(p,W)) { nv=-1;break; }
						if (cmpPx16(p,mc)==FALSE) { nv=ii;break;}
					}
					if ( (nv>0)&&(nv<pv) ){pv = nv; nc = p;lp = nv;}
					//270
					nv = -1;
					for (ii=1;ii<=lp;ii++){
						p = in->getPixel16_W(i-ii,j);
						if (cmpPx16(p,W)) { nv=-1;break; }
						if (cmpPx16(p,mc)==FALSE) { nv=ii;break;}
					}
					if ( (nv>0)&&(nv<pv) ){pv = nv; nc = p;lp = nv;}
					//315
					nv = -1;
					for (ii=1;ii<=lp;ii++){
						p = in->getPixel16_W(i-ii,j-ii);
						if (cmpPx16(p,W)) { nv=-1;break; }
						if (cmpPx16(p,mc)==FALSE) { nv=ii;break;}
					}
					if ( (nv>0)&&(nv<pv) ){pv = nv; nc = p;lp = nv;}
					
					if (pv<1000){
						data[target] = nc;
					}

				}
				target++;
			}
			target+= ow;
		}
		//-------------------------------------------------------------
		target =0;
		PF_Boolean op = FALSE;
		for (j=0;j<h;j++){
			for (i=0;i<w;i++){
				tc = data[target];
				if (cmpPx16(tc,mc)){
					op = FALSE;
					p = mc;
					if (op==FALSE) { p = out->getPixel16_W(i-1,j-1);if ( (cmpPx16(p,mc)==FALSE)&&(cmpPx16(p,W)==FALSE) ) op = TRUE;}
					if (op==FALSE) { p = out->getPixel16_W(i  ,j-1);if ( (cmpPx16(p,mc)==FALSE)&&(cmpPx16(p,W)==FALSE) ) op = TRUE;}
					if (op==FALSE) { p = out->getPixel16_W(i+1,j-1);if ( (cmpPx16(p,mc)==FALSE)&&(cmpPx16(p,W)==FALSE) ) op = TRUE;}
					if (op==FALSE) { p = out->getPixel16_W(i-1,j  );if ( (cmpPx16(p,mc)==FALSE)&&(cmpPx16(p,W)==FALSE) ) op = TRUE;}
					//if (op==FALSE) { p = out->getPixel8_W(i  ,j  );if ( (cmpPx8(p,mc)==FALSE)&&(cmpPx8(p,W)==FALSE) ) op = TRUE;}
					if (op==FALSE) { p = out->getPixel16_W(i+1,j  );if ( (cmpPx16(p,mc)==FALSE)&&(cmpPx16(p,W)==FALSE) ) op = TRUE;}
					if (op==FALSE) { p = out->getPixel16_W(i-1,j+1);if ( (cmpPx16(p,mc)==FALSE)&&(cmpPx16(p,W)==FALSE) ) op = TRUE;}
					if (op==FALSE) { p = out->getPixel16_W(i  ,j+1);if ( (cmpPx16(p,mc)==FALSE)&&(cmpPx16(p,W)==FALSE) ) op = TRUE;}
					if (op==FALSE) { p = out->getPixel16_W(i+1,j+1);if ( (cmpPx16(p,mc)==FALSE)&&(cmpPx16(p,W)==FALSE) ) op = TRUE;}

					if (op==TRUE) {out->paint16(i,j,p);}
				}
				target++;
			}
			target += ow;
		}
		//-------------------------------------------------------------
		target =0;
		for (j=0;j<h;j++){
			for (i=0;i<w;i++){
				tc = data[target];
				if (cmpPx16(tc,mc)){
					data[target] = W;
				}
				target++;

			}
			target += ow;
		}
	}
	//------------------------------------------------------
	void exec8()
	{
		alphaOn8();
		onePointChk8();
		mainColor8();
	}
	//------------------------------------------------------
	void exec16()
	{
		alphaOn16();
		onePointChk16();
		mainColor16();
	}
};


//-------------------------------------------------------
extern "C" {
DllExport	
PF_Err 
EntryPointFunc (
	PF_Cmd			cmd,
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output,
	void			*extraP);
}
//-------------------------------------------------------------------------------------------------
inline unsigned char RoundByte(long x)
{
	long temp=x;
	if (temp<0)   temp=0;
	if (temp>PF_MAX_CHAN8) temp=PF_MAX_CHAN8;
	return (unsigned char)temp;
}
//-----------------------------------------------------------------------------------
inline unsigned short RoundShort(long x)
{
	long temp=x;
	if (temp<0)   temp=0;
	if (temp>PF_MAX_CHAN16) temp=PF_MAX_CHAN16;
	return (unsigned short)temp;
}
//-----------------------------------------------------------------------------------
inline	PF_Pixel16 toPixel16(PF_Pixel s){
		PF_Pixel16 r;
		r.alpha	= (unsigned short)CONVERT8TO16(s.alpha);
		r.red		= (unsigned short)CONVERT8TO16(s.red);
		r.green	= (unsigned short)CONVERT8TO16(s.green);
		r.blue	= (unsigned short)CONVERT8TO16(s.blue);
		return r;
	}

#endif // MainLineRepaint_H
