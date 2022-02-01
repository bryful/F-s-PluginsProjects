#include "FsGraphics.h"

PF_Err CFsGraph::gblur8(PF_FpLong value,ScanLineMode mode)
{
	PF_Err err = PF_Err_NONE;
	if (value <=0 ) return err;

	PF_InData *in_data;
	in_data = m_in_data;

	//スキャンラインの用意
	A_long w = m_width;
	if (w<m_height) w = m_height;
	PF_Handle sH = PF_NEW_HANDLE( w * sizeof(PF_Pixel) * 2);
	if (!sH) return PF_Err_OUT_OF_MEMORY;
	PF_Pixel *scanline= *(PF_Pixel**)sH;

	A_long range	= (A_long)PF_CEIL(value);
	if (range<1) range = 1;
	double zone	= value/3;
	A_long *tbl;
	PF_Handle tH = PF_NEW_HANDLE( range * sizeof(A_long) * 2);
	if (!tH) {
		PF_DISPOSE_HANDLE(sH);
		return PF_Err_OUT_OF_MEMORY;


	}
	tbl= *(A_long**)tH;

	for (A_long i = 0; i <= range; i++) {
		tbl[i] = (A_long)(PF_EXP(-i * i / (2 * zone * zone)) * 0x100);
	}

	if( (mode ==SCANLINE::HorAndVer)||(mode ==SCANLINE::Hor)){
		A_long adr = 0;
		for (A_long y=0; y<m_height; y++)
		{
			//スキャンラインへ転送
			for ( A_long x=0; x<m_width; x++) {
				PF_Pixel p = m_data[adr + x];
				if (p.alpha ==0){
					p.red = p.green = p.blue = 0;
				}else if (p.alpha <PF_MAX_CHAN8) {
					p.red	= RoundByteLong(p.red   * p.alpha / PF_MAX_CHAN8);
					p.green	= RoundByteLong(p.green * p.alpha / PF_MAX_CHAN8);
					p.blue	= RoundByteLong(p.blue  * p.alpha / PF_MAX_CHAN8);
				}
				scanline[x] = p;
			}
			for (A_long x=0; x<m_width; x++){
				A_long count = 0; 
				A_long gauss = 0;
				A_long r = 0;
				A_long g = 0;
				A_long b = 0;
				A_long a = 0;
				//真ん中
				gauss = tbl[0];
				if (scanline[x].alpha >0){
					r += scanline[x].red * gauss;
					g += scanline[x].green * gauss;
					b += scanline[x].blue * gauss;
					a += scanline[x].alpha * gauss;
				}
				count += gauss;
				for (A_long i=1; i<=range;i++)
				{
					A_long il = x-i;
					gauss = tbl[i];
					if ( il>=0) {
						if (scanline[il].alpha>0){
							r += scanline[il].red * gauss;
							g += scanline[il].green * gauss;
							b += scanline[il].blue * gauss;
							a += scanline[il].alpha * gauss;
						}
						count += gauss;
					}
					A_long ir = x + i;
					if ( ir<m_width) {
						if (scanline[ir].alpha>0){
							r += scanline[ir].red * gauss;
							g += scanline[ir].green * gauss;
							b += scanline[ir].blue * gauss;
							a += scanline[ir].alpha * gauss;
						}
						count += gauss;
					}
				}
				if (count>0){
					PF_Pixel c;
					c.alpha	= RoundByteLong(a/count);
					if (c.alpha<=0){
						c.red = c.green = c.blue = 0;
					}else if(c.alpha<PF_MAX_CHAN8){
						double div = ((double)1/(double)count)*PF_MAX_CHAN8/(double)c.alpha;
						c.red	= RoundByteFpLong((double)r * div);
						c.green	= RoundByteFpLong((double)g * div);
						c.blue	= RoundByteFpLong((double)b * div);
					}else{
						c.red	= RoundByteLong(r/count);
						c.green	= RoundByteLong(g/count);
						c.blue	= RoundByteLong(b/count);
					}
					
					m_data[adr + x] = c;
				}
			}
			adr += m_widthTrue;
		}
	}
	//****
	if( (mode ==SCANLINE::HorAndVer)||(mode ==SCANLINE::Ver)){
		A_long adr = 0;
		for (A_long x=0; x<m_width; x++)
		{
			adr = x;
			//スキャンラインへ転送
			for ( A_long y=0; y<m_height; y++) {
				PF_Pixel p = m_data[adr];
				if (p.alpha ==0){
					p.red = p.green = p.blue = 0;
				}else if (p.alpha <PF_MAX_CHAN8) {
					p.red	= RoundByteLong(p.red   * p.alpha / PF_MAX_CHAN8);
					p.green	= RoundByteLong(p.green * p.alpha / PF_MAX_CHAN8);
					p.blue	= RoundByteLong(p.blue  * p.alpha / PF_MAX_CHAN8);
				}
				scanline[y] = p;
				adr += m_widthTrue;
			}
			adr = x;
			for (A_long y=0; y<m_height; y++){
				A_long count = 0; 
				A_long gauss = 0;
				A_long r = 0;
				A_long g = 0;
				A_long b = 0;
				A_long a = 0;
				//真ん中
				gauss = tbl[0];
				if (scanline[y].alpha >0){
					r += scanline[y].red * gauss;
					g += scanline[y].green * gauss;
					b += scanline[y].blue * gauss;
					a += scanline[y].alpha * gauss;
				}
				count += gauss;
				for (A_long i=1; i<=range;i++)
				{
					A_long il = y-i;
					gauss = tbl[i];
					if ( il>=0) {
						if (scanline[il].alpha>0){
							r += scanline[il].red * gauss;
							g += scanline[il].green * gauss;
							b += scanline[il].blue * gauss;
							a += scanline[il].alpha * gauss;
						}
						count += gauss;
					}
					A_long ir = y + i;
					if ( ir<m_height) {
						if (scanline[ir].alpha>0){
							r += scanline[ir].red * gauss;
							g += scanline[ir].green * gauss;
							b += scanline[ir].blue * gauss;
							a += scanline[ir].alpha * gauss;
						}
						count += gauss;
					}
				}
				if (count>0){
					PF_Pixel c;
					c.alpha	= RoundByteLong(a/count);
					if (c.alpha<=0){
						c.red = c.green = c.blue = 0;
					}else if(c.alpha<PF_MAX_CHAN8){
						double div = ((double)1/(double)count)*PF_MAX_CHAN8/(double)c.alpha;
						c.red	= RoundByteFpLong((double)r * div);
						c.green	= RoundByteFpLong((double)g * div);
						c.blue	= RoundByteFpLong((double)b * div);
					}else{
						c.red	= RoundByteLong(r/count);
						c.green	= RoundByteLong(g/count);
						c.blue	= RoundByteLong(b/count);
					}
					
					m_data[adr] = c;
				}
				adr += m_widthTrue;
			}
		}
	}


	PF_DISPOSE_HANDLE(sH);
	PF_DISPOSE_HANDLE(tH);
	return err;
}
//*****************************************************************************
PF_Err CFsGraph::gblur16(PF_FpLong value,ScanLineMode mode)
{
	PF_Err err = PF_Err_NONE;
	if (value <=0 ) return err;

	PF_InData *in_data;
	in_data = m_in_data;

	PF_Pixel16 *data;
	data =(PF_Pixel16 *)m_data; 

	//スキャンラインの用意
	A_long w = m_width;
	if (w<m_height) w = m_height;
	PF_Handle sH = PF_NEW_HANDLE( w * sizeof(PF_Pixel16) * 2);
	if (!sH) return PF_Err_OUT_OF_MEMORY;
	PF_Pixel16 *scanline= *(PF_Pixel16**)sH;

	A_long range	= (A_long)PF_CEIL(value);
	if (range<1) range = 1;
	double zone	= value/3;
	A_long *tbl;
	PF_Handle tH = PF_NEW_HANDLE( range * sizeof(A_long) * 2);
	if (!tH) {
		PF_DISPOSE_HANDLE(sH);
		return PF_Err_OUT_OF_MEMORY;


	}
	tbl= *(A_long**)tH;

	for (A_long i = 0; i <= range; i++) {
		tbl[i] = (A_long)(PF_EXP(-i * i / (2 * zone * zone)) * 0x100);
	}

	if( (mode ==SCANLINE::HorAndVer)||(mode ==SCANLINE::Hor)){
		A_long adr = 0;
		for (A_long y=0; y<m_height; y++)
		{
			//スキャンラインへ転送
			for ( A_long x=0; x<m_width; x++) {
				PF_Pixel16 p = data[adr + x];
				if (p.alpha ==0){
					p.red = p.green = p.blue = 0;
				}else if (p.alpha <PF_MAX_CHAN16) {
					p.red	= RoundShort(p.red   * p.alpha / PF_MAX_CHAN16);
					p.green	= RoundShort(p.green * p.alpha / PF_MAX_CHAN16);
					p.blue	= RoundShort(p.blue  * p.alpha / PF_MAX_CHAN16);
				}
				scanline[x] = p;
			}
			for (A_long x=0; x<m_width; x++){
				A_u_long count = 0; 
				A_u_long gauss = 0;
				A_u_long r = 0;
				A_u_long g = 0;
				A_u_long b = 0;
				A_u_long a = 0;
				//真ん中
				gauss = tbl[0];
				if (scanline[x].alpha >0){
					r += (A_u_long)scanline[x].red * gauss;
					g += (A_u_long)scanline[x].green * gauss;
					b += (A_u_long)scanline[x].blue * gauss;
					a += (A_u_long)scanline[x].alpha * gauss;
				}
				count += gauss;
				for (A_long i=1; i<=range;i++)
				{
					A_long il = x-i;
					gauss = tbl[i];
					if ( il>=0) {
						if (scanline[il].alpha>0){
							r += (A_u_long)scanline[il].red * gauss;
							g += (A_u_long)scanline[il].green * gauss;
							b += (A_u_long)scanline[il].blue * gauss;
							a += (A_u_long)scanline[il].alpha * gauss;
						}
						count += gauss;
					}
					A_long ir = x + i;
					if ( ir<m_width) {
						if (scanline[ir].alpha>0){
							r += (A_u_long)scanline[ir].red * gauss;
							g += (A_u_long)scanline[ir].green * gauss;
							b += (A_u_long)scanline[ir].blue * gauss;
							a += (A_u_long)scanline[ir].alpha * gauss;
						}
						count += gauss;
					}
				}
				if (count>0){
					PF_Pixel16 c;
					c.alpha	= RoundShort(a/count);
					if (c.alpha<=0){
						c.red = c.green = c.blue = 0;
					}else if(c.alpha<PF_MAX_CHAN16){
						double div = ((double)1/(double)count)*PF_MAX_CHAN16/(double)c.alpha;
						c.red	= RoundShortFpLong((double)r * div);
						c.green	= RoundShortFpLong((double)g * div);
						c.blue	= RoundShortFpLong((double)b * div);
					}else{
						c.red	= RoundShort(r/count);
						c.green	= RoundShort(g/count);
						c.blue	= RoundShort(b/count);
					}
					
					data[adr + x] = c;
				}
			}
			adr += m_widthTrue;
		}
	}
	//****
	if( (mode ==SCANLINE::HorAndVer)||(mode ==SCANLINE::Ver)){
		A_long adr = 0;
		for (A_long x=0; x<m_width; x++)
		{
			adr = x;
			//スキャンラインへ転送
			for ( A_long y=0; y<m_height; y++) {
				PF_Pixel16 p = data[adr];
				if (p.alpha ==0){
					p.red = p.green = p.blue = 0;
				}else if (p.alpha <PF_MAX_CHAN16) {
					p.red	= RoundShort(p.red   * p.alpha / PF_MAX_CHAN16);
					p.green	= RoundShort(p.green * p.alpha / PF_MAX_CHAN16);
					p.blue	= RoundShort(p.blue  * p.alpha / PF_MAX_CHAN16);
				}
				scanline[y] = p;
				adr += m_widthTrue;
			}
			adr = x;
			for (A_long y=0; y<m_height; y++){
				A_u_long count = 0; 
				A_u_long gauss = 0;
				A_u_long r = 0;
				A_u_long g = 0;
				A_u_long b = 0;
				A_u_long a = 0;
				//真ん中
				gauss = tbl[0];
				if (scanline[y].alpha >0){
					r += (A_u_long)scanline[y].red * gauss;
					g += (A_u_long)scanline[y].green * gauss;
					b += (A_u_long)scanline[y].blue * gauss;
					a += (A_u_long)scanline[y].alpha * gauss;
				}
				count += gauss;
				for (A_long i=1; i<=range;i++)
				{
					A_long il = y-i;
					gauss = tbl[i];
					if ( il>=0) {
						if (scanline[il].alpha>0){
							r += (A_u_long)scanline[il].red * gauss;
							g += (A_u_long)scanline[il].green * gauss;
							b += (A_u_long)scanline[il].blue * gauss;
							a += (A_u_long)scanline[il].alpha * gauss;
						}
						count += gauss;
					}
					A_long ir = y + i;
					if ( ir<m_height) {
						if (scanline[ir].alpha>0){
							r += (A_u_long)scanline[ir].red * gauss;
							g += (A_u_long)scanline[ir].green * gauss;
							b += (A_u_long)scanline[ir].blue * gauss;
							a += (A_u_long)scanline[ir].alpha * gauss;
						}
						count += gauss;
					}
				}
				if (count>0){
					PF_Pixel16 c;
					c.alpha	= RoundShortFpLong((double)a/count);
					if (c.alpha<=0){
						c.red = c.green = c.blue = 0;
					}else if(c.alpha<PF_MAX_CHAN16){
						double div = ((double)1/(double)count)*PF_MAX_CHAN16/(double)c.alpha;
						c.red	= RoundShortFpLong((double)r * div);
						c.green	= RoundShortFpLong((double)g * div);
						c.blue	= RoundShortFpLong((double)b * div);
					}else{
						c.red	= RoundShort(r/count);
						c.green	= RoundShort(g/count);
						c.blue	= RoundShort(b/count);
					}
					
					data[adr] = c;
				}
				adr += m_widthTrue;
			}
		}
	}


	PF_DISPOSE_HANDLE(sH);
	PF_DISPOSE_HANDLE(tH);
	return err;
}
//**********************************************************************************
PF_Err CFsGraph::gblur32(PF_FpLong value,ScanLineMode mode)
{
	PF_Err err = PF_Err_NONE;
	if (value <=0 ) return err;

	PF_InData *in_data;
	in_data = m_in_data;

	PF_PixelFloat *data;
	data =(PF_PixelFloat *)m_data; 

	//スキャンラインの用意
	A_long w = m_width;
	if (w<m_height) w = m_height;
	PF_Handle sH = PF_NEW_HANDLE( w * sizeof(PF_PixelFloat) * 2);
	if (!sH) return PF_Err_OUT_OF_MEMORY;
	PF_PixelFloat *scanline= *(PF_PixelFloat**)sH;

	A_long range	= (A_long)PF_CEIL(value);
	if (range<1) range = 1;
	double zone	= value/3;
	A_long *tbl;
	PF_Handle tH = PF_NEW_HANDLE( range * sizeof(A_long) * 2);
	if (!tH) {
		PF_DISPOSE_HANDLE(sH);
		return PF_Err_OUT_OF_MEMORY;


	}
	tbl= *(A_long**)tH;

	for (A_long i = 0; i <= range; i++) {
		tbl[i] = (A_long)(PF_EXP(-i * i / (2 * zone * zone)) * 0x100);
	}

	if( (mode ==SCANLINE::HorAndVer)||(mode ==SCANLINE::Hor)){
		A_long adr = 0;
		for (A_long y=0; y<m_height; y++)
		{
			//スキャンラインへ転送
			for ( A_long x=0; x<m_width; x++) {
				PF_PixelFloat p = data[adr + x];
				if (p.alpha ==0){
					p.red = p.green = p.blue = 0;
				}else if (p.alpha <1.0) {
					p.red	= p.red   * p.alpha;
					p.green	= p.green * p.alpha;
					p.blue	= p.blue  * p.alpha;
				}
				scanline[x] = p;
			}
			for (A_long x=0; x<m_width; x++){
				A_u_long count = 0; 
				A_u_long gauss = 0;
				PF_FpLong r = 0;
				PF_FpLong g = 0;
				PF_FpLong b = 0;
				PF_FpLong a = 0;
				//真ん中
				gauss = tbl[0];
				if (scanline[x].alpha >0){
					r += (PF_FpLong)scanline[x].red * gauss;
					g += (PF_FpLong)scanline[x].green * gauss;
					b += (PF_FpLong)scanline[x].blue * gauss;
					a += (PF_FpLong)scanline[x].alpha * gauss;
				}
				count += gauss;
				for (A_long i=1; i<=range;i++)
				{
					A_long il = x-i;
					gauss = tbl[i];
					if ( il>=0) {
						if (scanline[il].alpha>0){
							r += (PF_FpLong)scanline[il].red * gauss;
							g += (PF_FpLong)scanline[il].green * gauss;
							b += (PF_FpLong)scanline[il].blue * gauss;
							a += (PF_FpLong)scanline[il].alpha * gauss;
						}
						count += gauss;
					}
					A_long ir = x + i;
					if ( ir<m_width) {
						if (scanline[ir].alpha>0){
							r += (PF_FpLong)scanline[ir].red * gauss;
							g += (PF_FpLong)scanline[ir].green * gauss;
							b += (PF_FpLong)scanline[ir].blue * gauss;
							a += (PF_FpLong)scanline[ir].alpha * gauss;
						}
						count += gauss;
					}
				}
				if (count>0){
					PF_PixelFloat c;
					c.alpha	= RoundFpShortDouble(a/count);
					if (c.alpha<=0){
						c.red = c.green = c.blue = 0;
					}else if(c.alpha<1.0){
						double div = ((double)1/(double)count)/(double)c.alpha;
						c.red	= RoundFpShortDouble((double)r * div);
						c.green	= RoundFpShortDouble((double)g * div);
						c.blue	= RoundFpShortDouble((double)b * div);
					}else{
						c.red	= RoundFpShortDouble(r/count);
						c.green	= RoundFpShortDouble(g/count);
						c.blue	= RoundFpShortDouble(b/count);
					}
					
					data[adr + x] = c;
				}
			}
			adr += m_widthTrue;
		}
	}
	//****
	if( (mode ==SCANLINE::HorAndVer)||(mode ==SCANLINE::Ver)){
		A_long adr = 0;
		for (A_long x=0; x<m_width; x++)
		{
			adr = x;
			//スキャンラインへ転送
			for ( A_long y=0; y<m_height; y++) {
				PF_PixelFloat p = data[adr];
				if (p.alpha ==0){
					p.red = p.green = p.blue = 0;
				}else if (p.alpha <1.0) {
					p.red	= p.red   * p.alpha;
					p.green	= p.green * p.alpha;
					p.blue	= p.blue  * p.alpha;
				}
				scanline[y] = p;
				adr += m_widthTrue;
			}
			adr = x;
			for (A_long y=0; y<m_height; y++){
				A_u_long count = 0; 
				A_u_long gauss = 0;
				PF_FpLong r = 0;
				PF_FpLong g = 0;
				PF_FpLong b = 0;
				PF_FpLong a = 0;
				//真ん中
				gauss = tbl[0];
				if (scanline[y].alpha >0){
					r += (PF_FpLong)scanline[y].red * gauss;
					g += (PF_FpLong)scanline[y].green * gauss;
					b += (PF_FpLong)scanline[y].blue * gauss;
					a += (PF_FpLong)scanline[y].alpha * gauss;
				}
				count += gauss;
				for (A_long i=1; i<=range;i++)
				{
					A_long il = y-i;
					gauss = tbl[i];
					if ( il>=0) {
						if (scanline[il].alpha>0){
							r += (PF_FpLong)scanline[il].red * gauss;
							g += (PF_FpLong)scanline[il].green * gauss;
							b += (PF_FpLong)scanline[il].blue * gauss;
							a += (PF_FpLong)scanline[il].alpha * gauss;
						}
						count += gauss;
					}
					A_long ir = y + i;
					if ( ir<m_height) {
						if (scanline[ir].alpha>0){
							r += (PF_FpLong)scanline[ir].red * gauss;
							g += (PF_FpLong)scanline[ir].green * gauss;
							b += (PF_FpLong)scanline[ir].blue * gauss;
							a += (PF_FpLong)scanline[ir].alpha * gauss;
						}
						count += gauss;
					}
				}
				if (count>0){
					PF_PixelFloat c;
					c.alpha	= RoundFpShortDouble(a/count);
					if (c.alpha<=0){
						c.red = c.green = c.blue = 0;
					}else if(c.alpha<1.0){
						double div = ((double)1/(double)count)/(double)c.alpha;
						c.red	= RoundFpShortDouble((double)r * div);
						c.green	= RoundFpShortDouble((double)g * div);
						c.blue	= RoundFpShortDouble((double)b * div);
					}else{
						c.red	= RoundFpShortDouble(r/count);
						c.green	= RoundFpShortDouble(g/count);
						c.blue	= RoundFpShortDouble(b/count);
					}
					
					data[adr] = c;
				}
				adr += m_widthTrue;
			}
		}
	}


	PF_DISPOSE_HANDLE(sH);
	PF_DISPOSE_HANDLE(tH);
	return err;
}
