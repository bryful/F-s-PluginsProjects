# BurstLines

 放射状に飛び散る線を描く物です。

# 機能

* center<br>　放射線の中心
* lineCount<br>　線の数。実際の数は、この値とsubCountをかけたものになります。
* distance<br>　中心から線の先端への距離
* length<br>　線の長さ
* params
  - subcCount サブ線の数
  - subAngle サブ線の散らばる角度。lineCountを１にして変えると実感できます。
  - distanceRand 距離の散らばり具合。これはlengthにも影響を与えます。
  - lengthRand 線の長さのばらつき
  - tickness 線の太さ
  - ticknessRand 線の太さのばらつき
  - thck_start 線の先端の太さ。パーセントです。
  - thck_end 線の後ろの太さ。パーセントです。
  - opacity_start 線の先端の不透明度。200%まで指定できます。
  - opacity_end 線の後ろの不透明度。
  - direction 線の回転。angleRandが100の時は単純な回転。
  - angleRange 線の飛ぶ範囲
* colors
  - colorCount 使用する色数。
  - color1..8 色
* autoSeed
* seed



