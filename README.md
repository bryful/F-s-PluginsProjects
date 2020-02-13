# F's Plugins

Adone After EffectsのEffectsPlugin集のソース一式です。  
  
昔から趣味でコツコツと作っていたものです。  
趣味といっても僕自身日本のアニメ制作者なので業務に使ってます。


# Dependency

Visual studio 2017 Community 2017 C++
Adobe After Effects CC 15.0 Win SDK

SDKはCC2018を使用していますが、CS4/CS6のSDKでもbuild可能です。

# Setup
プロジェクト等はSDKフォルダ内のExampleフォルダ内へ配置してください。

こんな感じです。

        /ADOBE AFTER EFFECTS CC 15.0 WIN SDK  
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

# Usage

SDKはCC2018を想定していますが、CS4/CS6及びCC2018までのSDKでビルド可能です。
  
F's PluginsProjectsフォルダを各バージョンのExamplesフォルダに移動すればできます。
  
プラットホームはCS4のみx86(Win32)で、それ以降はx64になります。

構成でPelease/Relese-MDとありますが、ランタイムライブラリのスイッチ(/MT/MD)が違うものです。それ以外は同じです。  
  
PluginのBinaryはoutAEXもしくはoutAEX_MDに書き出されます。  

PluginのBinaryはAfter EffectsのPlug-insへコピーしてください。  
  
例)  
"C:\Program Files\Adobe\Adobe After Effects CC 2018\Support Files\Plug-ins"


# License

This software is released under the MIT License, see LICENSE 
  
このプログラムを映像制作に使用した場合は、MITライセンスの条件である著作権表示および本許諾表示は特に必要ありません。  
ライセンスを気になさる方は、社内書類等でこのプラグインが明記されているところに、MITのライセンス表記を行えば大丈夫です。
映像等に表示は必要ありません！

まぁ、出来ればスタッフリスト等に

**プラグイン協力 bry-ful**  

と表記してもらえると嬉しいです。 プラグイン協力以外でも適当な肩書であれば別のものに変えても構いません。  
**上記の表記は強制ではありませんのでクレジットに明記できなくても自由に使ってください。 **

P関係のゲーム会社の方へ  
使用に関しては特に制限ありません。許諾も必要ありません。使用料も発生しません。スタッフリストへの表記も必要ありません。  
これは今後絶対に変わりません。



# Authors

bry-ful(Hiroshi Furuhashi) http://bryful.yuzu.bz/  
twitter:[bryful](https://twitter.com/bryful)  
bryful@gmail.com  

# References

