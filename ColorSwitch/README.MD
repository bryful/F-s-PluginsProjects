# ColorSwitch

F's ColorChangeの代替えになります。機能は色替えだけでなく

* Replace 色変え
* Key 抜き
* Extract 抽出

と同じようなものをまとめてあります。

## 機能

* EnableAll<br>これをonにしないと何もしません。ちゃんと変わってるかON/OFFして確認するためのボタンです。
* ActiveParamCount<br>有効な色数です。標準は4色ですが、1色から32色まで指定できます。
* Mode<br>機能選択です。"Replace/Key/Extractが選べます。
* TurnON?<br>この項目を実行するかしないか選べます。
* Old?<br>元の色です。
* New?<br>ReplaceでOldな色がNewに変わります。Key/Extractでは指定しても無視されます。
* All turn off<br>オスとすべてのTurnONがOFFになります。32項目全てです。
* Save Params<br>パラメータをファイルに保存します。
* Load Params<br>パラメータファイルを読み込みます。

## ColorSwitchEdit.exe

このプラグインのパラメータファイルを読み込んで順番を変えたりと編集できます。<br>
色の編集は面倒なのでPNGファイルに書き出してPhotoshop等でした方が幸せです。
PNGはサイズを変えないように注意してください。<br>
<br>
[https://github.com/bryful/AE_Helper](https://github.com/bryful/AE_Helper)ここにソース等あります。

## CSJ ColorSwitchJsonファイル

普通のテキストファイルです。Json形式なので直接メモ帳で編集しても構いません。

```
{
    "active_param_count": 4,
    "enable_all": 1,
    "mode": 1,
    "new_colors": [
        [
            191,
            233,
            231
        ],
        [
            158,
            197,
            190
        ],
   (略)
        [
            0,
            0,
            0
        ]
    ],
    "old_colors": [
        [
            154,
            189,
            187
        ],
   (略)
        [
            0,
            0,
            0
        ]
    ],
    "turnon_colors": [
        1,
        1,
   (略)
        0
    ]
}
```
