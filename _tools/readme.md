# tools

## renPrj.ps1

./renPrj.ps1 <targetDIrPath> <srcWord> <dstword><br>
<br>
プロジェクトのあるフォルダーのリネームを行います。<br>
ファイル名のリネームと同時にテキストの中身の置換も行います。<br>

## AE_FlagManager.ps1

outflagsの計算を行うスクリプト

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
