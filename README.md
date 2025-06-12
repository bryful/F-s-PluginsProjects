# F's Plugins New and Next
Adone After EffectsのEffectsPlugin集のソース一式とWindowsバイナリです。　<b>no supporot MacOS!</b>
　

昔から趣味でコツコツと作っていたものです。<br>
趣味といっても僕自身日本のアニメ制作者なので業務に使えます。<br>
<br>

## F's MainLineRepaint_beta.aex 高速版ベータテスト版

[FsMainLineRepaint_20250612.zip](https://github.com/bryful/F-s-PluginsProjects/raw/refs/heads/master/_DL_windowsbinary/FsMainLineRepaint_20250612.zip)<br>

FsMainLineRepaint_20250611.zip<br>


FsMainLineRepaint_20250605.zip<br>
<br>
FsMainLineRepaint_beta_20250604.zip<br>
<br>
F's MainLineRepaintの高速版を作りました。ベータテスト版ですので本番には使わないように。<br>
matchNameを変えてあるので前のバージョンと同時素養可能ですが、正式版では上書き予定です。<br>
<br>
肝心の高速化は手持ちのセル画像では平均15%アップですが、線が太いキャラとか主線の色の面積が多いとかなりの確実で取りこぼします。計算上主線を削るのは24pxでそれ以上の線は消せません。その代わりかなり高速になってます。線の細いセルなら前のバージョンの半分以下の処理時間になってます。<br>

変えた点<br>

* あきらめるのを早くした。そのおかげで取り残す可能性が出ました。
* ピクセル比較の計算をかなり減らしました。
* 色の識別で接したピクセルの色を2点として暗い方を採用するようにしました。
* 上記の変更で前のバージョンと描画結果が変わった（まぁトレス線内なので不都合無いかと、主線を半透明で載せると不自然にならなくなった）


追記１<br>
いきなり透明部分の処理入れ忘れていたので修正。<br>
<br>

追記２<br>
目立ったバグはとったのでF's MainLineRepaintを正式ベータ版としてmatchNameを全バージョンと同じにしました。
その為全バージョンと同時使用はできなくなりました。<br>
<br>
おまけで複数色対応のF's CellLineEraser.aexを作りました。<br>
<br>
* 10色まで主線を指定できます。TargetColorCountで数を指定します。
* 透明色に囲まれたピクセルを消さないスイッチを付けました。
* 判別できなかったピクセルを指定した色で塗りつぶす使用にしました。
* 白を透明色にする機能追加


変えた点

* あきらめるのを早くした。そのおかげで取り残す可能性が出ました。
* ピクセル比較の計算をかなり減らしました。
* 色の識別で接したピクセルの色を2点として暗い方を採用するようにしました。
* 上記の変更で前のバージョンと描画結果が変わった（まぁトレス線内なので不都合無いかと、主線を半透明で載せると不自然にならなくなった）


できればこれの感想が欲しいです。<br>
あと、これをF's MainLineRepaintの続きにするか、別のプラグインにするかい意見も欲しいです。<br>





## <b>**残念なお知らせ**</b><br>
2025年度からAdobeCCの料金がかなり高価に値上がりしたので、プライベートではとっても払えないので次回の更新はしないことにしました。その為F's Pluginsの更新はi今以上にかなりロースペースになります。<br>
一応僕も仕事でたまには使うのでAfter Effects自体のバージョンに合わせて再ビルドは行うつもりです(SDKの更新に合わせます)<br>

***

最近モチベーションが全然無くてメンテナンスさぼり気味です。自分でほとんど使わなくなってしまったせいですかな？<br>
少しでもやる気が出るようにAmazonの欲しいものリストを試しに公開してみます。<br>
* [bry-fulの欲しいものリスト(https://www.amazon.co.jp/hz/wishlist/ls/2ME5VSS8WJOX8?ref_=wl_sha)<br>

<br>
まぁ、バグ報告のほうがありがたいです。 <br>
<br>

## 進捗

F's sputteringAlpha.aex/F's sputteringSplash.aexが内部エラーで落ちるバグに対処。<br>
まだ原因が特定できていないので、直っていないかも。<br>
MainLineReplaceも同様なバグがあったので修正。<br>
[Fssputtering_MainLineReplace20250109.zip](https://github.com/bryful/F-s-PluginsProjects/raw/refs/heads/master/_DL_windowsbinary/Fssputtering_MainLineReplace20250109.zip)<br>
[FssputteringAS20241229.zip](https://github.com/bryful/F-s-PluginsProjects/raw/refs/heads/master/_DL_windowsbinary/FssputteringAS20241229.zip)<br>

AE2022のマルチフレームレンダーに対応させました。<br>
***

Fs_Target.hの
```
#if defined(SUPPORT_SMARTFX)
#define FS_OUT_FLAGS2 134222921
#else
#define FS_OUT_FLAGS2 134217801
#endif
```
に変えただけなので中国語バージョン作る時はそこだけの変更で良いはずです。

CC2019用からgithubでバイナリーの配布も行います。
**_DL_windowsbinary**フォルダの中に入っています。

* **chinese translated version**<br>[https://www.lookae.com/fsplugins/](https://www.lookae.com/fsplugins/)<br>


<br>

# 変更点
202４/04/14<br>
SDK2023でブルドし直しました。<br>
<br>
2022/03/15<br>
AE2022のマルチフレームレンダリングに対応しました。 <br>
<br>
2020/11/11<br>
NFsライブラリのひな型を作成しました。<br>
<br>
2020/08/15<br>
コンパイラを VS2017からVS2019へ変更。<br>
それに伴い、構造体メンバーのアライメントを16byteに変更。/Zp16<br>

* F'sgrayToCountourLine.aexを追加
ポスタリゼーションの変形バージョンです、諧調を均等に割らずにHi/Mid/Loで諧調を変えられます。
* F's grayToWaveLine.aexを追加
グレー画像を疑似３Dプロッタ風に描画します。昔のSF映画のモニタぽいものができます。

2020/07/26
CC2020 SDKに変更。

2020/03/20
CC2019 SDKに変更。
数が多くなって使いにくくなったので、カテゴリーを整理しました。
* F's Plugins-Cell
 アニメのスムージングなしのセルをターゲットにしたものです。
* F's Plugins-Channel
 チャンネル操作系です。
* F's Plugins-Colorize
 色を付けるものです。
* F's Plugins-Draw
 描画系のものです。
* F's Plugins-Filter
 フィルターエフェクト系です。
* F's Plugins-Noise
 ノイズフィルタ系です。
* F's Plugins-{Legacy}
 もう使って欲しくない。或いは使い道のないものです。
 デバッグ前のものや、紙飛行機作成補助プラグインとかになります。
### 追加プラグイン
* F's EdgeLine-Hi.aex  指定した2色の境界に線を描きます。その時描く向きを指定できます。
* F's Flare.aex 白黒マスクにグローを付けます。透過光です。
* F's graytoneToColorize.aex 簡易コロラマです。ゴールド処理やサーモグラフ処理に使います。
* F's PixelExtend.aex 指定した色を指定した方向に膨張させます。
* F's Posterization8bit.aex ポスタリゼーションです。標準と違って内部は8bit処理です。
* F's Scanline.aex スキャンライン。昔の古いパソコンの偶数列が黒いラインの状態ができます。
* F's YuvControl.aex YUV版のRGBAコントロールです。

# 開発環境
Visual studio 2017 Community 2017 C++ or Visual studio 2019 Community 2017 C++

AfterEffectsSDK CC2020
SDKはCC2020を使用していますが、CS4/CS6のSDKでもbuild可能です。

# Setup
プロジェクト等はSDKフォルダ内のExampleフォルダ内へ配置してください。

こんな感じです。

        /AfterEffectsSDK CC2019
        └─Examples
            ├─AEGP
            ├─Effect
            ├─F's PluginsProjects
            │  ├─AlphaFix
            │  ├─AlphaThreshold
            │  ├─AnimatedNoise
            <省略>
            │  ├─PluginSkeleton
            <省略>
            │  ├─whiteInOut
            ├─GP
            ├─Headers
            ├─Resources
            ├─Template
            ├─UI
            └─Util

# 使い方

SDKはCC2020を想定していますが、CS6及びCC2020までのSDKでビルド可能です。

**NFsLibrary**ではCC2020以降のサポートとなります。

F's PluginsProjectsフォルダを各バージョンのExamplesフォルダに移動すればできます。


構成でPelease/Relese-MTとありますが、ランタイムライブラリのスイッチ(/MT/MD)が違うものです。それ以外は同じです。
PluginのBinaryはoutAEXに書き出されます。

PluginのBinaryはAfter EffectsのPlug-insへコピーしてください。

例)
"C:\Program Files\Adobe\Adobe After Effects CC 2019\Support Files\Plug-ins"

# デバッグ

1. デバッグ構成時のプロパティでバイナリの出力先をインストールされたAEの**Plug-ins**フォルダに設定します。SDKでは"[Program Files]\Adobe\Common\Plug-ins\[**version**]\MediaCore\"が推奨されていますが、バージョンがこっそり上がって困ったことがありました。
> C:\Program Files\Adobe\Adobe After Effects 2020\Support Files\Plug-ins\debug\
2. プロパティ「デバッグ」のコマンドをAEの実行ファイルにします。
> C:\Program Files\Adobe\Adobe After Effects 2020\Support Files\AfterFX.exe
3. その他必要な項目（作業ディレクトリ）も設定します。
4. 念のためにプラグインフォルダの設定をフルコントロールにしておきます。アクセス権が無くて書き出しができなことがあります。

以上の設定を行えば、デバッグが可能になります。



# ライセンス

This software is released under the MIT License, see LICENSE.

このソースコードを使用する時はMITライセンスに準じてください。
独自にビルドして映像制作使う場合は、使用プラグインリスト等にここの[url](https://github.com/bryful)を入れてもらえればOKです（まぁ入れなくても僕は気にしません）

このプログラムを映像制作に使用した場合も特に制限ありません。一応MITライセンスの条件である著作権表示および本許諾表示として

**プラグイン協力 bry-ful**

とクレジットしてくれると嬉しいです。プラグイン協力以外でも適当な肩書であれば別のものに変えても構いません。

P関係のゲーム会社の方へ
使用に関しては特に制限ありません。許諾も必要ありません。使用料も発生しません。
これは今後絶対に変わりません。



# Authors

bry-ful [Hiroshi Furuhashi]<br>
github: [https://github.com/bryful](https://github.com/bryful)<br>
twitter:[bryful](https://twitter.com/bryful)<br>
bryful@gmail.com

# Thanks

Nanae Furuhashi

My daughter,
May her soul rest in peace．



