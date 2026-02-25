# Posterization

　俗にいう「ポスタリゼーション」です。階調落としです。<br>
　こいつは特にモニターグラフィックスでの「等高線」を描くのに特化しています。
　階調ごとに色を指定できたり、境界線を描くことができます。

# 機能

* level<br>階調数です。3..15
* Mode<br>描画モードです。
  - Posterization 階調落とし
  - With Border 階調落とし＆境界線
  - Border Only	 境界線のみ
* Colorise<br>onで階調ごとに色をつけます。
* Colors 階調ごとの色の指定
* Tween Start to End<br> startとendの色を中割補完します。
* Tween Start,Mid,End<br> startとmid、endの色を中割補完します。
* Pre-proccessing 前処理でバイラテラル処理を行います。
  - Bilateral ONで実行
  - Radius 処理半径
  - Sigma_Spatial 処理の重みです。Radiusと同じ値にすると普通のぼかしになります。
  - Sigma_Range 色の値の許容値です。
* Border 境界線
  - InnerSize 内側
  - OuterSize 外側
  - BorderColor 線の色


