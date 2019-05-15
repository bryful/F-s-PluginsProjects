#include "FsGraphics.h"

//-------------------------------------------------------
void CFsGraph::toBlackMat8()
{

	if (m_mat == MAT::blackMat) {
		return;
	}else if (m_mat == MAT::whiteMat){
		fromWhiteMat8();
	}
	A_long target = 0;
	PF_Pixel *data;
	data =(PF_Pixel *)m_data;
	for ( A_long j=0; j<m_height;j++){
		for (A_long i=0; i<m_width;i++){
			PF_Pixel p = data[target];
			if (p.alpha<PF_MAX_CHAN8){
				if(p.alpha<=0){
					p.blue = p.green = p.red = 0;
				}else if (p.alpha < PF_MAX_CHAN8){
					p.blue	= RoundByteLong((A_long)p.blue * (A_long)p.alpha / PF_MAX_CHAN8);
					p.green	= RoundByteLong((A_long)p.green * (A_long)p.alpha / PF_MAX_CHAN8);
					p.red	= RoundByteLong((A_long)p.red * (A_long)p.alpha / PF_MAX_CHAN8);
				}
				data[target] = p;
			}
			target++;
		}
		target += m_offsetWidth;
	}
	m_mat = MAT::blackMat;
}
//-------------------------------------------------------
void CFsGraph::toBlackMat16()
{
	if (m_mat == MAT::blackMat) {
		return;
	}else if (m_mat == MAT::whiteMat){
		fromWhiteMat16();
	}
	A_long target = 0;
	PF_Pixel16 *data;
	data =(PF_Pixel16 *)m_data;
	for ( A_long j=0; j<m_height;j++){
		for (A_long i=0; i<m_width;i++){
			PF_Pixel16 p = data[target];
			if (p.alpha<PF_MAX_CHAN16){
				if(p.alpha<=0){
					p.blue = p.green = p.red = 0;
				}else if (p.alpha < PF_MAX_CHAN16){
					p.blue	= RoundShort((A_long)p.blue * (A_long)p.alpha / PF_MAX_CHAN16);
					p.green	= RoundShort((A_long)p.green * (A_long)p.alpha / PF_MAX_CHAN16);
					p.red	= RoundShort((A_long)p.red * (A_long)p.alpha / PF_MAX_CHAN16);
				}
				data[target] = p;
			}
			target++;
		}
		target += m_offsetWidth;
	}
	m_mat = MAT::blackMat;
}
//-------------------------------------------------------
void CFsGraph::toBlackMat32()
{
	if (m_mat == MAT::blackMat) {
		return;
	}else if (m_mat == MAT::whiteMat){
		fromWhiteMat32();
	}
	A_long target = 0;
	PF_PixelFloat *data;
	data =(PF_PixelFloat *)m_data;
	for ( A_long j=0; j<m_height;j++){
		for (A_long i=0; i<m_width;i++){
			PF_PixelFloat p = data[target];
			if (p.alpha<1.0){
				if(p.alpha<=0.0){
					p.blue = p.green = p.red = 0.0;
				}else if (p.alpha < 1.0){
					p.blue	= RoundFpShortDouble((double)p.blue * (double)p.alpha);
					p.green	= RoundFpShortDouble((double)p.green * (double)p.alpha);
					p.red	= RoundFpShortDouble((double)p.red * (double)p.alpha);
				}
				data[target] = p;
			}
			target++;
		}
		target += m_offsetWidth;
	}
	m_mat = MAT::blackMat;
}
//-------------------------------------------------------
void CFsGraph::fromBlackMat8()
{
	if( m_mat == MAT::blackMat){
		A_long target = 0;
		PF_Pixel *data;
		data =(PF_Pixel *)m_data;
		for ( A_long j=0; j<m_height;j++){
			for (A_long i=0; i<m_width;i++){
				PF_Pixel p = data[target];
				if (p.alpha<PF_MAX_CHAN8){
					if(p.alpha<=0){
						p.blue = p.green = p.red = 0;
					}else if (p.alpha < PF_MAX_CHAN8){
						p.blue	= RoundByteLong((A_long)p.blue * PF_MAX_CHAN8 / (A_long)p.alpha);
						p.green	= RoundByteLong((A_long)p.green * PF_MAX_CHAN8 / (A_long)p.alpha);
						p.red	= RoundByteLong((A_long)p.red * PF_MAX_CHAN8 / (A_long)p.alpha);
					}
					data[target] = p;
				}
				target++;
			}
			target += m_offsetWidth;
		}
		m_mat = MAT::none;
	}
}
//-------------------------------------------------------
void CFsGraph::fromBlackMat16()
{
	if( m_mat == MAT::blackMat){
		A_long target = 0;
		PF_Pixel16 *data;
		data =(PF_Pixel16 *)m_data;
		for ( A_long j=0; j<m_height;j++){
			for (A_long i=0; i<m_width;i++){
				PF_Pixel16 p = data[target];
				if (p.alpha<PF_MAX_CHAN16){
					if(p.alpha<=0){
						p.blue = p.green = p.red = 0;
					}else if (p.alpha < PF_MAX_CHAN16){
						p.blue	= RoundShort((A_long)p.blue * PF_MAX_CHAN16 / (A_long)p.alpha);
						p.green	= RoundShort((A_long)p.green * PF_MAX_CHAN16 / (A_long)p.alpha);
						p.red	= RoundShort((A_long)p.red * PF_MAX_CHAN16 / (A_long)p.alpha);
					}
					data[target] = p;
				}
				target++;
			}
			target += m_offsetWidth;
		}
		m_mat = MAT::none;
	}
}
//-------------------------------------------------------
void CFsGraph::fromBlackMat32()
{
	if( m_mat == MAT::blackMat){
		A_long target = 0;
		PF_PixelFloat *data;
		data =(PF_PixelFloat *)m_data;
		for ( A_long j=0; j<m_height;j++){
			for (A_long i=0; i<m_width;i++){
				PF_PixelFloat p = data[target];
				if (p.alpha<1.0){
					if(p.alpha<=0){
						p.blue = p.green = p.red = 0;
					}else if (p.alpha < 1.0){
						p.blue	= RoundFpShortDouble(p.blue  / p.alpha);
						p.green	= RoundFpShortDouble(p.green / p.alpha);
						p.red	= RoundFpShortDouble(p.red   / p.alpha);
					}
					data[target] = p;
				}
				target++;
			}
			target += m_offsetWidth;
		}
		m_mat = MAT::none;
	}
}
//-------------------------------------------------------
void CFsGraph::toWhiteMat8()
{
	if (m_mat == MAT::whiteMat) {
		return;
	}else if (m_mat == MAT::blackMat){
		fromBlackMat8();
	}
	A_long target = 0;
	PF_Pixel *data;
	data =(PF_Pixel *)m_data;
	for ( A_long j=0; j<m_height;j++){
		for (A_long i=0; i<m_width;i++){
			PF_Pixel p = data[target];
			if (p.alpha<PF_MAX_CHAN8){
				if(p.alpha<=0){
					p.blue = p.green = p.red = PF_MAX_CHAN8;
				}else{
					p.red	= RoundByteLong( ((A_long)p.red   * (A_long)p.alpha) / PF_MAX_CHAN8 +PF_MAX_CHAN8 - (A_long)p.alpha );
					p.green	= RoundByteLong( ((A_long)p.green * (A_long)p.alpha) / PF_MAX_CHAN8 +PF_MAX_CHAN8 - (A_long)p.alpha );
					p.blue	= RoundByteLong( ((A_long)p.green * (A_long)p.alpha) / PF_MAX_CHAN8 +PF_MAX_CHAN8 - (A_long)p.alpha );
				}
				data[target] = p;
			}
			target++;
		}
		target += m_offsetWidth;
	}
	m_mat = MAT::whiteMat;
}
//-------------------------------------------------------
void CFsGraph::toWhiteMat16()
{
	if (m_mat == MAT::whiteMat) {
		return;
	}else if (m_mat == MAT::blackMat){
		fromBlackMat16();
	}
	A_long target = 0;
	PF_Pixel16 *data;
	data =(PF_Pixel16 *)m_data;
	for ( A_long j=0; j<m_height;j++){
		for (A_long i=0; i<m_width;i++){
			PF_Pixel16 p = data[target];
			if (p.alpha<PF_MAX_CHAN16){
				if(p.alpha<=0){
					p.blue = p.green = p.red = PF_MAX_CHAN16;
				}else{
					p.red	= RoundShort( ((A_long)p.red   * (A_long)p.alpha) / PF_MAX_CHAN16 +PF_MAX_CHAN16 - (A_long)p.alpha );
					p.green	= RoundShort( ((A_long)p.green * (A_long)p.alpha) / PF_MAX_CHAN16 +PF_MAX_CHAN16 - (A_long)p.alpha );
					p.blue	= RoundShort( ((A_long)p.green * (A_long)p.alpha) / PF_MAX_CHAN16 +PF_MAX_CHAN16 - (A_long)p.alpha );
				}
				data[target] = p;
			}
			target++;
		}
		target += m_offsetWidth;
	}
	m_mat = MAT::whiteMat;
}
//-------------------------------------------------------
void CFsGraph::toWhiteMat32()
{
	if (m_mat == MAT::whiteMat) {
		return;
	}else if (m_mat == MAT::blackMat){
		fromBlackMat16();
	}
	A_long target = 0;
	PF_PixelFloat *data;
	data =(PF_PixelFloat *)m_data;
	for ( A_long j=0; j<m_height;j++){
		for (A_long i=0; i<m_width;i++){
			PF_PixelFloat p = data[target];
			if (p.alpha<1.0){
				if(p.alpha<=0){
					p.blue = p.green = p.red = 1.0;
				}else if (p.alpha < 1.0){
					p.red	= RoundFpShortDouble( (p.red   * p.alpha) + 1.0 - p.alpha );
					p.green	= RoundFpShortDouble( (p.green * p.alpha) + 1.0 - p.alpha );
					p.blue	= RoundFpShortDouble( (p.green * p.alpha) + 1.0 - p.alpha );
				 }
				data[target] = p;
			}
			target++;
		}
		target += m_offsetWidth;
	}
	m_mat = MAT::whiteMat;
}
//-------------------------------------------------------
void CFsGraph::fromWhiteMat8()
{
	if (m_mat == MAT::whiteMat) {
		A_long target = 0;
		PF_Pixel *data;
		data =(PF_Pixel *)m_data;
		for ( A_long j=0; j<m_height;j++){
			for (A_long i=0; i<m_width;i++){
				PF_Pixel p = data[target];
				if (p.alpha<PF_MAX_CHAN8){
					if(p.alpha<=0){
						p.blue = p.green = p.red = 0;
					}else if (p.alpha < PF_MAX_CHAN8){
						p.red	= RoundByteLong( ((A_long)p.red   - PF_MAX_CHAN8 + (A_long)p.alpha) *PF_MAX_CHAN8/(A_long)p.alpha );
						p.green	= RoundByteLong( ((A_long)p.green - PF_MAX_CHAN8 + (A_long)p.alpha) *PF_MAX_CHAN8/(A_long)p.alpha );
						p.blue	= RoundByteLong( ((A_long)p.blue  - PF_MAX_CHAN8 + (A_long)p.alpha) *PF_MAX_CHAN8/(A_long)p.alpha );
					
					}
					data[target] = p;
				}
				target++;
			}
			target += m_offsetWidth;
		}
		m_mat = MAT::none;
	}
}
//-------------------------------------------------------
void CFsGraph::fromWhiteMat16()
{
	if (m_mat == MAT::whiteMat) {
		A_long target = 0;
		PF_Pixel16 *data;
		data =(PF_Pixel16 *)m_data;
		for ( A_long j=0; j<m_height;j++){
			for (A_long i=0; i<m_width;i++){
				PF_Pixel16 p = data[target];
				if (p.alpha<PF_MAX_CHAN16){
					if(p.alpha<=0){
						p.blue = p.green = p.red = 0;
					}else if (p.alpha < PF_MAX_CHAN16){
						p.red	= RoundShort( ((A_long)p.red   - PF_MAX_CHAN16 + (A_long)p.alpha) *PF_MAX_CHAN16/(A_long)p.alpha );
						p.green	= RoundShort( ((A_long)p.green - PF_MAX_CHAN16 + (A_long)p.alpha) *PF_MAX_CHAN16/(A_long)p.alpha );
						p.blue	= RoundShort( ((A_long)p.blue  - PF_MAX_CHAN16 + (A_long)p.alpha) *PF_MAX_CHAN16/(A_long)p.alpha );
					
					}
					data[target] = p;
				}
				target++;
			}
			target += m_offsetWidth;
		}
		m_mat = MAT::none;
	}
}
//-------------------------------------------------------
void CFsGraph::fromWhiteMat32()
{
	if (m_mat == MAT::whiteMat) {
		A_long target = 0;
		PF_PixelFloat *data;
		data =(PF_PixelFloat *)m_data;
		for ( A_long j=0; j<m_height;j++){
			for (A_long i=0; i<m_width;i++){
				PF_PixelFloat p = data[target];
				if (p.alpha<1.0){
					if(p.alpha<=0){
						p.blue = p.green = p.red = 0;
					}else if (p.alpha < 1.0){
						p.red	= RoundFpShortDouble( (p.red   - 1.0 + p.alpha) /p.alpha );
						p.green	= RoundFpShortDouble( (p.green - 1.0 + p.alpha) /p.alpha );
						p.blue	= RoundFpShortDouble( (p.blue  - 1.0 + p.alpha) /p.alpha );
					
					}
					data[target] = p;
				}
				target++;
			}
			target += m_offsetWidth;
		}
		m_mat = MAT::none;
	}
}
//-------------------------------------------------------
void CFsGraph::toStraightMat8()
{
	if (m_mat == MAT::none){
	}else if (m_mat == MAT::blackMat){
		fromBlackMat8();
	}else if (m_mat == MAT::whiteMat){
		fromWhiteMat8();
	}
}
//-------------------------------------------------------
void CFsGraph::toStraightMat16()
{
	if (m_mat == MAT::none){
	}else if (m_mat == MAT::blackMat){
		fromBlackMat16();
	}else if (m_mat == MAT::whiteMat){
		fromWhiteMat16();
	}
}
//-------------------------------------------------------
void CFsGraph::toStraightMat32()
{
	if (m_mat == MAT::none){
	}else if (m_mat == MAT::blackMat){
		fromBlackMat32();
	}else if (m_mat == MAT::whiteMat){
		fromWhiteMat32();
	}
}
