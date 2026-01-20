# tools

## renPrj.ps1

```
./renPrj.ps1 <targetDirPath> <srcWord> <dstword><br>
```
プロジェクトのあるフォルダーのリネームを行うPowerShellスクリプト。<br>
ファイル名のリネームと同時にテキストの中身の置換も行います。<br>

## projectDup.exe

```
projectDup <targetDirPath> <DupDirPath> <srcWord> <dstword><br>
```
projectDup.exeは複製しながら名前の変更を行うC++コンソールアプリです。<br>

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


