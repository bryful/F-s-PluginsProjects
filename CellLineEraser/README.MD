# CellLineEraser

MainLineRepaint/MainLineRepaint_oldの後継になります。
もともとギャグ系だったのが、一番使われる物になりました。

主線を消しますが、50ピクセルを超える線や。周囲に有効な色がないと諦めます。

MainLineRepaint_oldは諦めたら適当な色で塗ります。
MainLineRepaintは周囲に有効な色がないとそのピクセルは消します。

## 機能

* TargetColorCount<br>有効な対象色の数です
* STR_Color1-8<br>対象色。この色を消します。
* KeepPixels<br>ONであきらめてもそのピクセルを消しません。
* Fill unremoved pixels<br>ONだとあきらめた時FillColorで塗りつぶします。OFFは元の色です。
* FillColor<br>Fill unremoved pixelsがonの時塗る色です。。
* Make White Transparent<br>白を透明にします。

## 直す気がないバグ

画像がすでに透明部分(Alphaが0)があった時

Make White TransparentがOFF
KeepPixelsがON

の時あきらめたピクセルが白くなります。
本来は周囲のサーチして透明にするべきですが、それやると処理時間が２倍になるのにそんなにメリットないのでそのままにしてます。