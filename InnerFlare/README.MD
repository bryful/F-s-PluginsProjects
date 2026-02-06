# InnerFlare

不透明部分を対象に内側にフレアを付けます。
おまけで外側もできます。

実行すると元絵は消えます。

## 機能

* color<br>フレアの色です。
* max<br>最大最小です。
* blur<br>ぼかしです。
* hyperbolic<br>アルファーチャンネルに双曲線補完
* reverce<br>onにすると外側フレアになります。

minmax -> blur -> hyperbolicという順番で処理がかかります。
