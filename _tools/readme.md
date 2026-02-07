# tools
プラグイン作るときに便利なアプリ・スクリプトを入れてあります。<br>
ソースは[https://github.com/bryful/AE_Helper](https://github.com/bryful/AE_Helper)にあります。<br>

## PowerShellスクリプトの実行について
PowerShellスクリプトを実行するには、PowerShellの実行ポリシーを変更する必要があります。<br>
以下のコマンドをPowerShellで実行してください。<br>
```
Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope Process
```
この設定はPowerShellを閉じると元に戻ります。
僕は面倒なので基本設定にしてますが、業務に使うPCでは上記の手順を踏んだほうが良いです。<br>
<br>
F's Pluginsのソリューションのフォルダをエクスプローラーで開き、URL欄に"powershell"と入力してEnterを押すと、そのフォルダでPowerShellが起動します。<br>
```
./_tools/WarningLevel.ps1
```
./_と打ち込んでTabキーで補完できます。


## Add-DateToAex.exe / Add-DateToAex.ps1
aexの末尾に日付を付けます。<br>
-clearオプションで削除。<br>
```
Usage: Add-DateToAex.exe <TargetDir> [-clear]

  <TargetDir>  : 対象ディレクトリのパス (必須)
  -clear       : 日付を削除するモード (オプション)

Examples:
  Add-DateToAex.exe C:\Plugins
  Add-DateToAex.exe C:\Plugins -clear
  ```

## aeVersion.exe
aeVersion.exeはプラグインのバージョン値を計算するアプリです。計算結果は標準出力されます<br>


```
[aeVersion.exe] プラグインのバージョン値の計算
使い方: aeVersion <1:vers> <2:subvers> <3:bugvers> <4:stage> <5:build>
  stage: DEVELOP or D, ALPHA or A, BETA or B, RELEASE or R
例: aeVersion 1 0 0 DEVELOP 0
```

例)

```
>aeVersion.exe 1 0 0 R 0

#define MAJOR_VERSION   1
#define MINOR_VERSION   0
#define BUG_VERSION     0
#define STAGE_VERSION   PF_Stage_RELEASE
#define BUILD_VERSION   0
#define MY_VERSION 525824

```

## AE_FlagManager.ps1 / outflags.exe

outflagsの計算を行うPowerShellスクリプト
outflags.exeは全く同じ機能を持つC++製のGUIアプリケーションです。<br>

-write/-outflag/-outflag2で設定ファイルを書き出し。<br>
設定ファイルを編集。 行頭の;を外すとその項目が有効になる<br>
必要に応じてファイル名を変えます。<br>
-calcでそのファイルを指定すると結果が出力される。<br>
<br>
-routflag/-routflag2で数値を指定すれば、その値のリストが書き出される。

プロジェクトフォルダでpowershellを起動（エクスプローラーで上のurl欄に"powershell"と入力してEnter）<br>
まず最初に

```
Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope Process
```
を実行して、スクリプトの実行を許可してください。<br>
その後

```
.\_tools\AE_FlagManager.ps1 -write
```
で実行。".\\_"と打ち込んだらTabキーで補完できます。<br>

```
AE_FlagManager - After Effects SDK Flag Utility
Usage:
  .\AE_FlagManager.ps1 -write               : 全リストを書き出し (; 付き)
  .\AE_FlagManager.ps1 -outflag             : outFlags1 を書き出し (; 付き)
  .\AE_FlagManager.ps1 -outflag2            : outFlags2 を書き出し (; 付き)
  .\AE_FlagManager.ps1 -calc <File>         : 有効な行の値を合算
  .\AE_FlagManager.ps1 -routflag <Value>    : 値に含まれるFlagの ";" を外して書き出し
  .\AE_FlagManager.ps1 -routflag2 <Value>   : 値に含まれるFlag2の ";" を外して書き出し
```

## MaintainProjects.ps1
C++プロジェクトファイルを修正するスクリプト。実行時は注意

## Organize-Aex.exe / Organize-Aex.ps1
aexファイルをカテゴリ別に整理する
```
Usage: Organize-Aex.exe <TargetDir> [-restore]

  <TargetDir>  : 対象ディレクトリのパス (必須)
  -restore     : 元に戻すモード (オプション)

Examples:
  Organize-Aex.exe C:\Plugins
  Organize-Aex.exe C:\Plugins -restore
  Organize-Aex.exe -restore C:\Plugins
 ```
## outDir.ps1
Debugの出力ディレクトリを一括変更するPowerShellスクリプトです。<br>
現在 "..\..\Fs_Plugins_debug\"が設定されているので必要に応じて書き換えてください<br>

## outflags.exe

```
AE_FlagManager - After Effects SDK Flag Utility

Usage:
  outflags -write               : 全リストをカレントに書き出し (; 付き)
  outflags -outflag             : outFlags1 をカレントに書き出し (; 付き)
  outflags -outflag2            : outFlags2 をカレントに書き出し (; 付き)
  outflags -calc <File>         : 有効な行の値を合算
  outflags -routflag <Value>    : 値に含まれるFlagの ";" を外してカレントに書き出し
  outflags -routflag2 <Value>   : 値に含まれるFlag2の ";" を外してカレントに書き出し

Options:
  -json <Path>                  : JSONファイルのパスを明示的に指定
```

## outflagsUI
上記のoutflags.exeのUI付き、￥。ファイルサイズがGithubの限界を超えてるのでzipにしてあります。

## projectDup.exe

```
projectDup <targetDirPath> <DupDirPath> <srcWord> <dstword><br>
```
projectDup.exeは複製しながら名前の変更を行うC++コンソールアプリです。<br>


## renPrj.ps1

```
./renPrj.ps1 <targetDirPath> <srcWord> <dstword><br>
```
プロジェクトのあるフォルダーのリネームを行うPowerShellスクリプト。<br>
ファイル名のリネームと同時にテキストの中身の置換も行います。<br>



## WarningLevel.ps1
WarningLevel.ps1はVisual Studioのプロジェクトファイル(*.vcxproj)のWarning Levelを一括変更するPowerShellスクリプトです。<br>
現在はLevel3に変更する機能のみ実装しています。必要に応じて書き換えてください<br>


