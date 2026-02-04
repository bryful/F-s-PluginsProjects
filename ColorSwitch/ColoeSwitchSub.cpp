#include "ColorSwitch.h"

// --- ビット深度ごとの型変換を吸収するヘルパー ---
inline PF_Pixel NF_To8(PF_Pixel c) { return c; }
inline PF_Pixel NF_To8(PF_Pixel16 c) { return NF_Pixel16To8(c); }
inline PF_Pixel NF_To8(PF_PixelFloat c) { return NF_Pixel32To8(c); }

// テンプレート用の情報を保持する構造体
template <typename T>
struct ColSWInfo {
	A_long		count;
	A_long		mode;
	PF_Pixel	old_colors[COLOR_TABLE_MAX];
	T			new_colors[COLOR_TABLE_MAX];
};

//-------------------------------------------------------------------------------------------------
// テンプレート化したピクセルサンプリング関数
//-------------------------------------------------------------------------------------------------
template <typename T>
static PF_Err
ColSW_Template(
	void* refcon,
	A_long		xL,
	A_long		yL,
	T* inP,
	T* outP)
{
	PF_Err			err = PF_Err_NONE;
	ColSWInfo<T>* infoP = reinterpret_cast<ColSWInfo<T>*>(refcon);

	// オーバーロードされた NF_To8 により、T の型に応じた関数が呼ばれます
	PF_Pixel src = NF_To8(*inP);
	int idx = -1;

	for (A_long i = 0; i < infoP->count; i++) {
		if (
			src.red == infoP->old_colors[i].red &&
			src.green == infoP->old_colors[i].green &&
			src.blue == infoP->old_colors[i].blue
			) {
			idx = i;
			break;
		}
	}

	if (infoP->mode == SwitchMode::SWITCH_MODE_REPLACE)
	{
		if (idx >= 0) {
			*outP = infoP->new_colors[idx];
		}
		else {
			*outP = *inP;
		}
	}
	else if (infoP->mode == SwitchMode::SWITCH_MODE_KEY)
	{
		if (idx >= 0) {
			outP->alpha = 0;
		}
		else {
			*outP = *inP;
		}

	}
	else if (infoP->mode == SwitchMode::SWITCH_MODE_EXTRACT)
	{
		if (idx < 0) {
			outP->alpha = 0;
		}
		else {
			*outP = *inP;
		}
	}
	return err;
}
static A_long IndexOfColor(PF_Pixel* table, A_long len, PF_Pixel c)
{
	A_long cnt = len;
	if (len <= 0) return -1;
	if (len > COLOR_TABLE_MAX) cnt = COLOR_TABLE_MAX;
	for (A_long i = 0; i < len; i++)
	{
		if (
			table[i].red == c.red &&
			table[i].green == c.green &&
			table[i].blue == c.blue)
		{
			return i;
		}
	}
	return -1;
}
//-------------------------------------------------------------------------------------------------
PF_Err ColSWitch(NF_AE* ae, ParamInfo* infoP)
{
	PF_Err err = PF_Err_NONE;
	ColSWInfo<PF_Pixel> info8;
	AEFX_CLR_STRUCT(info8);
	info8.mode = infoP->mode;
	int cnt = 0;
	for (int i = 0; i < infoP->active_param_count; i++)
	{
		if(infoP->turnon_colors[i] == FALSE) {
			// 無効な色はスキップ
			continue;
		}
		int idx = IndexOfColor(info8.old_colors, i, infoP->old_colors[i]);
		if(idx>=0){
			// すでに登録されている色はスキップ
			continue;
		}
		info8.old_colors[cnt] = infoP->old_colors[i];
		info8.new_colors[cnt] = infoP->new_colors[i];
		cnt++;
	}
	info8.count = cnt;
	if(cnt<=0){
		// 置換対象の色が無い場合は処理しない
		return err;
	}

	switch (ae->pixelFormat()) {
	case PF_PixelFormat_ARGB128:
	{
		ColSWInfo<PF_PixelFloat> info32;
		AEFX_CLR_STRUCT(info32);
		info32.count = info8.count;
		info32.mode = info8.mode;
		for (int i = 0; i < info8.count; i++) {
			info32.old_colors[i] = info8.old_colors[i];
			// 32bit用の変換関数があればそれを使用してください（例: NF_Pixel8ToFloat）
			info32.new_colors[i] = NF_Pixel8ToFloat(info8.new_colors[i]);
		}
		ae->iterate32(ae->input, &info32, ColSW_Template<PF_PixelFloat>, ae->output);
	}
	break;

	case PF_PixelFormat_ARGB64:
	{
		ColSWInfo<PF_Pixel16> info16;
		AEFX_CLR_STRUCT(info16);
		info16.count = info8.count;
		info16.mode = info8.mode;
		for (int i = 0; i < info8.count; i++) {
			info16.old_colors[i] = info8.old_colors[i];
			info16.new_colors[i] = NF_Pixel8To16(info8.new_colors[i]);
		}
		ae->iterate16(ae->input, &info16, ColSW_Template<PF_Pixel16>, ae->output);
	}
	break;

	case PF_PixelFormat_ARGB32:
	{
		ae->iterate8(ae->input, &info8, ColSW_Template<PF_Pixel>, ae->output);
	}
	break;
	}
	return err;
}