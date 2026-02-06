# InnerFlareRot

内フレアの位置を調整して向きを指定できるものです。

やってることは

1.元画像のアルファー部分のみを向き(rot)と距離(length)で指定した位置に。
2.lengthにoffsetを足した位置にさらに重ねます。これを２回
3.これを反転
4. minmax -> blur -> hyperbolicと処理をかけます。
5.元画像の透明部分でマスクしてそれにcolorで色を付けて完了


reverseをonにすると3.の反転をしなくなります。

実行すると元絵は消えます。

## 機能

* color<br>フレアの色
* rot<br>ずらす方向
* length<br>ずらす距離（ピクセル）
* offset<br>さらにlength+offsetの距離でずらしたものを合成
* reverce<br>反転。
* minmax<br>最大最小。
* blur<br>ぼかし
* hyperbolic<br>アルファーチャンネルに双曲線補完
