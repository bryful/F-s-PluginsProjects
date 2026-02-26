# TouchLines

 対象に対して効果線を加えます。<br>
 直線・放射状が指定できます。。


# 機能

* value<br>線の数
* threshold
  * luminance 輝度の変化点で
  * alpha アルファーの境界線で
  * color 選択した色で。
* lunmLevel 輝度の閾値
* alphaLevel アルファーの閾値
* targetColor 選択する色
* colorLevel 色判別の閾値
*show target mask<br>　上記で抽出したマスクの表示。Debug用でしたが、便利そうなので
*params
  * mode 動作モード
    * straight 直線
    * radial 放射状
  * angle線の向き
　* center 放射状の中心
  * innerLength 内側の長さ
  * outerLength 外側の長さ
  * lengthRand 線の長さのばらつき
  * size<br>　線の太さ
  * sizeRand<br> 線の太さのばらつき
  * opacity<br>　不透明度
  * opacityRand<br> 不透明度のばらつき
  * blue<br> ぼかす
  * noize<br> ノイズ。
  * autoSeed<br>　ONで自動的に動きます
  * seed<br>　randomseedです。この値を動かせば適当に動きます。
* color
  * colorCount<br> 使う色数。
  * color1..8<br> 色。８色まで
* blend with orogonal<br> 元絵を合成します。


