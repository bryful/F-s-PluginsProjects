# F's Plugins New and Next
Adone After EffectsのEffectsPlugin集のソース一式とWindowsバイナリです。

昔から趣味でコツコツと作っていたものです。
趣味といっても僕自身日本のアニメ制作者なので業務に使えます。

***
**重要なお知らせ**

AE2022のマルチフレームレンダーに対応させました。<br>
ただし、対応フラグをONにしただけでコードの変更はしていません。
対応条件をSDKで調べる限り元々対応しているコーディングをしていたみたいです。<br>
なので、下のアーカイブはAE2022のSDKで再ビルドを行っただけの物です。<br>
<br>
普段なら全部を僕自身で動作確認するのですが、仕事でほとんど撮影を行っていないのでそれをする時間がありません。<br>
忘れてるだけで未対応なコードが残っている可能性があります。<br>
<br>
**このバージョンのF's Pluginsは人柱バグ取りバージョンと考えてください。**<br>
出来れば各個で動作確認して大丈夫あるいはNGであるかを教えてもらえると助かります<br>
<br>
twitterがMailでお願いします。<br>
<br>
**そういうわけで仕事業務で使用する時はそのことを踏まえて使用してください。**<br>

* [FsPlugins20220223.zip](https://bit.ly/351YokT)<br>
***



CC2019用からgithubでバイナリーの配布も行います。
**_DL_windowsbinary**フォルダの中に入っています。

* [FsPlugins20201111.zip](https://bit.ly/2KknG31)<br>
11月11日版です。NFsライブラリのひな型になります。
* [FsPlugins20200816.zip](https://bit.ly/2PZqemC)<br>
8月16日版です。VS2020に変更しました。
* [Fsplugins20200726.zip](https://bit.ly/2OXxZJj)<br>
7月26日版です。SDKをCC2020に変えてビルドしただけのものです。

# 変更点
2020/11/11
NFsライブラリのひな型を作成しました。

2020/08/15
コンパイラを VS2017からVS2019へ変更。
それに伴い、構造体メンバーのアライメントを16byteに変更。/Zp16

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



