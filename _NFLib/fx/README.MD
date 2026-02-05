# fx
PF_EffectWorldへ画像処理を加えるライブラリ（関数）です。<br>
汎用を念頭に置いているのでNF_AE関係の依存はないです。ヘッダーをインクルードするだけで他のプラグインでも使えます。<br>
<br>

```
PF_Err UnMult(
    PF_InData* in_dataP,
    PF_EffectWorld* worldP,
    PF_PixelFormat pixelFormat,
    AEGP_SuiteHandler* suitesP,
	//以下は関数固有の引数
    PF_Boolean isUnMult
```
こんな感じに引数は統一しています。基本的に8/16/32Bitに対応しています。

## debug_font.h / debug_font.cpp / debug_font_1424.h / debug_font_1424.png
デバッグ用に文字出力させる物。<br>
フォントは[https://hicchicc.github.io/00ff/](https://hicchicc.github.io/00ff/)のx14y24pxHeadUpDaisyを使わせてもらってます。<br>
[https://github.com/bryful/AE_Helper](https://github.com/bryful/AE_Helper)にあるTTF2Cppで変換して使ってます。<br>

## NF_AlphaHyperbolic.h/NF_AlphaHyperbolic.cpp
アルファーに双曲線補完処理を行うもの

## NF_Blur.h/NF_Blur.cpp
ぼかし

## NF_MinMax.h/NF_MinMax.cpp
最大・最小

## NF_NF_Mult.h/Mult.cpp
これはストレート合成（マット無し）・マット合成を変換するもの

## NF_UnMult.h/NF_UnMult.cpp
こっちは俗にいうUnMult

## NF_Paint.h/ NF_Paint.cpp
閉領域フィル。俗にいうバケツツール

## NF_Noise.h/NF_Noise.cpp
ノイズ

## NF_Draw.h / NF_Draw.cpp
* DrawLine
* DrawBox
* FillBox
* DrawCircle
* FillCircle

