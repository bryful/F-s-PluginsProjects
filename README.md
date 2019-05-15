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

This software is released under the MIT License, see LICENSE.txt.




# Authors

bry-ful (Hiroshi Furuhashi)  http://bryful.yuzu.bz/
  
bryful@twitter  
  
bryful@gmail.com  

# References

