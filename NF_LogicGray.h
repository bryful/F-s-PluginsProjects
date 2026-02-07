#pragma once

#ifndef NF_LOGICGRAY_H
#define NF_LOGICGRAY_H

#include "../AE_SDK.h"


/**
 * LogicalGrayMain - グレースケール変換処理
 * 
 * @param in_dataP After Effects入力データ
 * @param worldP 処理対象のワールド（入出力兼用）
 * @param pixelFormat ピクセルフォーマット
 * @param suitesP AEGPスイートハンドラ
 * @param gray_mode グレースケール変換モード
 *        1: YUV
 *        2: HLS
 *        3: RGB平均
 *        4: Rチャンネル
 *        5: Gチャンネル
 *        6: Bチャンネル
 *        7: RGB最大値
 *        8: LAB
 * @param org_blend 元画像とのブレンド率 (0.0～1.0)
 *        0.0: 完全にグレースケール
 *        1.0: 元画像のまま
 * @return PF_Err エラーコード
 */
PF_Err LogicalGrayMain(
	PF_InData* in_dataP,
	PF_EffectWorld* worldP,
	PF_PixelFormat pixelFormat,
	AEGP_SuiteHandler* suitesP,
	A_long gray_mode,
	PF_FpShort org_blend
);

#endif // NF_LOGICGRAY_H
