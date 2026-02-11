# F's Plugins - Next / NF-Plugins
Adone After EffectsのEffectsPlugin集のソース一式とWindowsバイナリです。<br>
(**No macOS support** / **不支持 macOS**)<br>

昔から趣味でコツコツと作っていたものです。<br>
趣味といっても僕自身日本のアニメ制作者なので業務に使えます。<br>
<br>
***
**2026/02/05以降新しいF's Pluginsの追加はありません。**<br>
プロジェクトフォルダをLegacyフォルダに移動しました。<br>

**今後NF-Pluginsとして新しいエフェクトを追加していきます。**<br>
アップグレード・バグフィックス時にはNFへの以降も同時に行います<br>
<br>
**Deprecatedカテゴリに入っているF's Pluginsは近日中にバイナリの配布は中止します。**<br>
残してほしいものがあれば連絡ください。<br>
<br>
過去の資産のために互換性を重視してましたが、AfterEffects本体のほうが互換が無くなってきた・動く古いPCが少なくなってきた等、もうそろそろ限界かなと思ってます。<br>
***

SDKをAfterEffectsSDK_25.6_61_winに変えました。いつの間にかSDK2025がバージョンアップしてたみたいです。<br>
<br>

## ダウンロード
ダウンロードは今ページの右上あたりにある<b>[Releases](https://github.com/bryful/F-s-PluginsProjects/releases)</b>でできます。<br>
2026/02/07 更新しました。<br>
個別にほしい時は[NF-Plugins_release](https://github.com/bryful/F-s-PluginsProjects/tree/master/NF-Plugins_release)ディレクトリから直接ダウンロードできます。[ここ](https://github.com/bryful/F-s-PluginsProjects/tree/master/NF-Plugins_release)にあるやつは最新版ですがバグが残ってる可能性があるので注意してください。
<br>

## <b>**残念なお知らせ**</b><br>
2025年度からAdobeCCの料金がかなり高価に値上がりしたので、プライベートではとっても払えないので次回の更新はしないことにしました。
その為F's Pluginsの更新は今以上にかなりスローペースになります。<br>
一応僕も仕事でたまには使うのでAfter Effects自体のバージョンに合わせて再ビルドは行うつもりです(SDKの更新に合わせます)<br>

## 今後の予定
今後F's Pluginsの開発はメンテナンスにとどめて、NF-Pluginsという新しいプラグイン集を開発します。<br>
NF-Pluginsはあえて互換性(matchName)を無くしてモダンな開発環境を目指します。<br>
F’sは、ラッパー関数の嵐でコードが読みにくくなっているので、NF-Pluginsではシンプルで他のプラグインへの移植性を重視する予定です。<br>


***

最近モチベーションが全然無くてメンテナンスさぼり気味です。自分でほとんど使わなくなってしまったせいですかな？<br>
少しでもやる気が出るようにAmazonの欲しいものリストを試しに公開してみます。<br>
* [bry-fulの欲しいものリスト(https://www.amazon.co.jp/hz/wishlist/ls/2ME5VSS8WJOX8?ref_=wl_sha)<br>


## _tools フォルダについて<br>
_toolsフォルダ内にはプラグイン開発に便利なツールを入れています。<br>
今までC#で作ってましたが、PowerShellスクリプトに置き換えました。<br>
詳細は_toolsフォルダ内のREADME.mdを見てください。<br>
<br>
ここにソースがあります。
[AE_Helper](https://github.com/bryful/AE_Helper)

## Legacyフォルダについて

F's PluginsのプロジェクトはLegacy/Projectに中にまとめてあります。<br>
もうSDKをアップさせる気がないのでインクルードファイルの修正が面倒だったのでSDKも含めています。<br>
ResourcesフォルダにあるはずのPiPLtool.exeがGitではじかれる場合があるのでその場合はLegacy/Resources/PiPLtool.zipを展開してexeを取り出してください。
<br>
考えてみるとSDK込みでGitに入れたのでビルドの成功率はかなり高くなってるはずです。<br>

## 関連プロジェクト ##
F-s-PluginsProjects_TLed<br>
(Oops, sorry!　^^;) 詳細はリンク先で<br>
[https://github.com/ilyasok/F-s-PluginsProjects_TLed](https://github.com/ilyasok/F-s-PluginsProjects_TLed)
<br>
F's Plugins for MacOS<br>
詳細はリンク先で<br>
[https://github.com/CubeZeero/F-s-PluginsProjects_forMac](https://github.com/CubeZeero/F-s-PluginsProjects_forMac)<br>

DaVinci Resolve - Fusion 移植版<br>
詳細はリンク先で<br>
[https://github.com/akahito-ot/Fs-Plugins-Fusion-Ports](https://github.com/akahito-ot/Fs-Plugins-Fusion-Ports)<br>

chinese translated version<br>
[https://www.lookae.com/fsplugins/](https://www.lookae.com/fsplugins/)<br>

## ビルド時の注意 ##

**SDKのExamplesフォルダにこのリポジトリのフォルダを入れてください**<br>
ヘッダーのインクルードが一番楽です。<br>
<br>

今回から**Directory.Build.props**/**Directory.Build.Targets**を使ってプロジェクト設定の一括変更を行っています。<br>
VS2026でSDK2025を使うとものすごいWarnigが出るのでDirectory.Build.propsで抑制しています。プロパティのUIで変更しても上書きされるので注意です。<br>
<br>
出力ファイル名にDirectory.Build.Targetで指定して自動的に日付が入るようにしました。Debug時の出力先もここで変更しています。<br>
<br>
こんな便利な機能あるの知らなかった。<br>
<br>
SDKは**AfterEffectsSDK_25.6_61_win**を使用しています。<br>
<br>
Legacyフォルダにあるプロジェクトはもう変える予定がないのでSDKも同梱してあります。なのでビルドは簡単にできると思います。

## 進捗
バージョン管理の方法をいろいろ考えましたが、プラグインファイル名に日付を入れるという一番チープな方法を採用しました。<br>
<br>
Visual studio 2026に変更。SDKを2025に変更（まだ動作確認できていません）<br>
Visual studioのプロジェクト設定を一括で変更できるDirectory.Build.props/Directory.Build.targetsを使って、出力ファイル名に日付を入れるようにしました。<br>
<br>
CellLineEraset.aexの高速化<br>
MaxFast.aexの追加。<br>
[FsCellLineEraser_20260123.zip](https://github.com/bryful/F-s-PluginsProjects/raw/refs/heads/master/_DL_windowsbinary/FsCellLineEraser_20260123.zip)<br>
<br>
TargetGrad.aex/TargetGradradical.aex<br>
グラデーションエフェクトを追加<br>
[FsTargetGrad_finalBeta.zip](https://github.com/bryful/F-s-PluginsProjects/raw/refs/heads/master/_DL_windowsbinary/FsTargetGrad_finalBeta.zip) からDLしてください。近いうちに正式版出します。
<br>
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



<br>

# 変更点
2026/0２/01<br>
とりあえず今回の更新はこれで最後にします。<br>
2025/07/21<br>
プラグインファイル名の規則を変更。詳細は添付ファイル内の「必ず読んでください.txt」を見てください。<br>
<br>
2024/04/14<br>
SDK2023でbuildし直しました。<br>
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
Visual studio 2026 Community C++

AfterEffectsSDK CC2025
SDKはCC2025を使用しています。

# Setup
プロジェクト等はSDKフォルダ内のExampleフォルダ内へ配置してください。

こんな感じです。

        /AfterEffectsSDK
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

SDKはCC2025を想定しています。

**NFsLibrary**ではCC2020以降のサポートとなります。

F's PluginsProjectsフォルダを各バージョンのExamplesフォルダに移動すればできます。


PluginのBinaryはAfter EffectsのPlug-insへコピーしてください。

例)
"C:\Program Files\Adobe\Adobe After Effects CC 2024\Support Files\Plug-ins"

# デバッグ

1. デバッグ構成時のプロパティでバイナリの出力先をインストールされたAEの**Plug-ins**フォルダに設定します。SDKでは"[Program Files]\Adobe\Common\Plug-ins\[**version**]\MediaCore\"が推奨されていますが、バージョンがこっそり上がって困ったことがありました。
> C:\Program Files\Adobe\Adobe After Effects 2040\Support Files\Plug-ins\debug\
2. プロパティ「デバッグ」のコマンドをAEの実行ファイルにします。
> C:\Program Files\Adobe\Adobe After Effects 2040\Support Files\AfterFX.exe
3. その他必要な項目（作業ディレクトリ）も設定します。
4. 念のためにプラグインフォルダの設定をフルコントロールにしておきます。アクセス権が無くて書き出しができなことがあります。

以上の設定を行えば、デバッグが可能になります。



# ライセンス

This software is released under the MIT License, see LICENSE.

このソースコードを使用する時はMITライセンスに準じてください。
独自にビルドして映像制作使う場合は、使用プラグインリスト等にここの[url](https://github.com/bryful)を入れてもらえればOKです（まぁ入れなくても僕は気にしません）

このプログラムを映像制作に使用した場合も特に制限ありません。一応MITライセンスの条件である著作権表示および本許諾表示として

**プラグイン協力 bry-ful**

とクレジットしてくれると嬉しいです。プラグイン協力以外でも適当な肩書であれば別のものに変えても構いません。<br>

パチンコパチスロ遊技機開発会社の方へ<br>
使用に関しては特に制限ありません。許諾も必要ありません。使用料も発生しません。
これは今後絶対に変わりません。



# Authors

bry-ful [Hiroshi Furuhashi]<br>
github: [https://github.com/bryful](https://github.com/bryful)<br>
twitter:[bryful](https://twitter.com/bryful)<br>

# Thanks

Nanae Furuhashi

My daughter,
May her soul rest in peace．



