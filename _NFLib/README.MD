# _NFLib

NF-Pluginsの中核であるファイルが収納されています。<br>
AdobeのSDKはチンプンカンプンなのでわかりやすくするためのラッパーがほとんどになります。<br>
<br>


* AE_SDK.h<br>SDKでインクルードしないといけないファイルをここでインクルードしてます。
* NF_AE.h<br>プラグイン実行に必要な段取りを行うクラス**NF_AE**が定義されてます。
* NF_Version.h<br>バージョンの定義。面倒なのでここで一括管理
* NF_CATEGORY.h<br>カテゴリーの定義
* NF_ParamsSetup.h<br>UI登録を行うクラス。別に使わなくてもいい。
* NF_Utils.h<br>使うと便利なマクロ定義が入ってます。SDKによってマクロが変わって困るので作ったもの
* NF_Settings.h/NF_Settings.cpp<br>プラグインの設定関係。ファイル操作
* NF_Json.cpp/NF_Json.h<br>使ってません^^;
* dialog.jsx/dialog_jsx.h/txt_to_h.ps1<br>スクリプトファイルをC++内で使うためのPowerShellスクリプトとサンプル


## fxフォルダ
描画関係のライブラリが入ってます。依存をなくしているので他のプロジェクトに持って行っても使えます。


## 使わせてもらっているライブラリ

便利です。感謝！

* json.hpp<br>Json関係<br>[https://github.com/nlohmann/json](https://github.com/nlohmann/json)
* tinyfiledialogs.c/tinyfiledialogs.h<br>ダイアログ関係<br>[https://github.com/groutoutlook/tinyfiledialogs](https://github.com/groutoutlook/tinyfiledialogs)

