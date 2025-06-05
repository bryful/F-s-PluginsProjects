#include "MainLineRepaint.h"
PF_Err Exec32(CFsAE* ae, ParamInfo* infoP)
{
	PF_Err 	err = PF_Err_NONE;
	A_long w = ae->out->width();
	A_long wt = ae->out->widthTrue();
	A_long h = ae->out->height();
	PF_PixelFloat* scanline;
	A_long* scanlineV;
	A_long* scanlineV2;
	scanline = (PF_PixelFloat*)infoP->scanline;
	scanlineV = (A_long*)(scanline + w * 2);
	scanlineV2 = (A_long*)(scanlineV + w * 2);

	PF_PixelFloat* data;
	data = (PF_PixelFloat*)ae->out->data();

	A_long targetP = PPTRANS;

	//水平方向
	A_long cnt = 0;
	for (A_long j = 0; j < h; j++) {
		A_long adrY = j * wt;
		//元画像をスキャンラインごとに退避
		for (A_long i = 0; i < w; i++) {
			scanline[i] = data[adrY + i];
			//主線・透明と明るさのテーブル
			//　明るさ 0-255
			// 透明 256 PPTRANS
			// 主線 257 PPMAIN
			scanlineV[i] = pV32(scanline[i], infoP->Main_Color, infoP->lv);
			scanlineV2[i] = PxStatus(scanlineV[i]);
		}

		for (A_long i = 1; i < w - 1; i++) {
			if (scanlineV[i] == PPMAIN) {
				PF_PixelFloat dst = scanline[i];
				PF_Boolean dv = TRUE;

				A_long lc = i;//現在のポジションを初期値に
				A_long rc = i;

				for (int k = 1; k < infoP->scanLength; k++)
				{
					int k2 = i - k;
					if (k2 < 0) break;
					if (scanlineV[k2] <= targetP)
					{
						lc = k2;
						break;
					}
				}
				for (int k = 1; k < infoP->scanLength; k++)
				{
					int k2 = i + k;
					if (k2 >= w) break;
					if (scanlineV[k2] <= targetP)
					{
						rc = k2;
						break;
					}
				}
				A_long pat = scanlineV2[lc] << 8 | scanlineV2[rc];
				if (pat == (PS_NORMAL << 8 | PS_NORMAL))
				{
					if (scanlineV[lc] <= scanlineV[rc])
					{
						dst = scanline[lc];
					}
					else {
						dst = scanline[rc];
					}
				}
				else if (pat == (PS_MAIN << 8 | PS_NORMAL))
				{
					dst = scanline[rc];
				}
				else if (pat == (PS_NORMAL << 8 | PS_MAIN))
				{
					dst = scanline[lc];
				}
				else if (pat == (PS_TRANS << 8 | PS_NORMAL))
				{
					dst = scanline[rc];
				}
				else if (pat == (PS_NORMAL << 8 | PS_TRANS))
				{
					dst = scanline[lc];
				}
				else if (pat == (PS_TRANS << 8 | PS_TRANS))
				{
					dst = { 0,0,0,0 };
				}
				else {
					dv = FALSE;
				}

				if (dv == TRUE) {
					data[adrY + i] = dst;
				}
				else {
					cnt++;
				}
			}
		}
		if (scanlineV[0] >= PPMAIN) {
			if (scanlineV[1] < PPMAIN) {
				data[adrY] = scanline[1];
			}
			else {
				cnt++;
			}
		}
		if (scanlineV[w - 1] >= PPMAIN) {
			if (scanlineV[w - 2] < PPMAIN) {
				data[adrY + w - 1] = scanline[w - 1];
			}
			else {
				cnt++;
			}
		}
	}
	if (cnt == 0) return err;
	// 垂直方向
	cnt = 0;
	for (A_long i = 0; i < w; i++) {
		for (A_long j = 0; j < h; j++) {
			scanline[j] = data[j * wt + i];
			scanlineV[j] = pV32(scanline[j], infoP->Main_Color, infoP->lv);
			scanlineV2[j] = PxStatus(scanlineV[j]);
		}



		for (A_long j = 1; j < h - 1; j++) {
			if (scanlineV[j] == PPMAIN) {
				PF_PixelFloat dst = scanline[j];
				PF_Boolean dv = TRUE;
				//PF_Pixel lp = infoP->Main_Color;
				//PF_Pixel rp = infoP->Main_Color;
				//A_long lv = PPMAIN;
				//A_long rv = PPMAIN;
				A_long lc = j;//現在のポジションを初期値に
				A_long rc = j;


				for (int k = 1; k < infoP->scanLength; k++)
				{
					int k2 = j - k;
					if (k2 < 0) break;
					if (scanlineV[k2] <= targetP)
					{
						lc = k2;
						break;
					}
				}

				for (int k = 1; k < infoP->scanLength; k++)
				{
					int k2 = j + k;
					if (k2 >= h) break;
					if (scanlineV[k2] <= targetP)
					{
						rc = k2;
						break;
					}
				}
				A_long pat = scanlineV2[lc] << 8 | scanlineV2[rc];

				if (pat == (PS_NORMAL << 8 | PS_NORMAL))
				{
					//if ((i - lc) <= (rc - i))
					if (scanlineV[lc] <= scanlineV[rc])
					{
						dst = scanline[lc];
					}
					else {
						dst = scanline[rc];
					}
				}
				else if (pat == (PS_MAIN << 8 | PS_NORMAL))
				{
					dst = scanline[rc];
				}
				else if (pat == (PS_NORMAL << 8 | PS_MAIN))
				{
					dst = scanline[lc];
				}
				else if (pat == (PS_TRANS << 8 | PS_NORMAL))
				{
					dst = scanline[rc];
				}
				else if (pat == (PS_NORMAL << 8 | PS_TRANS))
				{
					dst = scanline[lc];
				}
				else if (pat == (PS_TRANS << 8 | PS_TRANS))
				{
					dst = { 0,0,0,0 };
				}
				else {
					dv = FALSE;
				}

				if (dv == TRUE) {
					data[j * wt + i] = dst;
				}
				else {
					cnt++;
				}
			}
		}
		if (scanlineV[0] >= PPMAIN) {
			if (scanlineV[1] < PPMAIN) {
				data[i] = scanline[1];
			}
			else {
				cnt++;
			}
		}
		if (scanlineV[w - 1] >= PPMAIN) {
			if (scanlineV[w - 2] < PPMAIN) {
				data[wt * (h - 1) + i] = scanline[w - 1];
			}
			else {
				cnt++;
			}
		}
	}
	infoP->count = cnt;
	return err;
}
