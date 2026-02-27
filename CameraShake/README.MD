# CameraShake

 画面動です。<br>
 パラメータを整理して使いやすくしました（自信ない）<br>
 おまけで手ぶれもできます。<br>

# 機能

* camera_value<br>　画面動の大きさ
* cameraShake
  - valueRand　画面動の乱雑さ
  - subShake アクセントとして入る揺れの入る確率
  - angle 画面動の方向
  - angleRand 方向の乱雑さ。最大45度のランダムな値がangleに
* hand_value<br>　手ぶれの全体の大きさ
* hand_speed<br>　手ぶれの全体の速度
* hand_seed<br>　手ぶれの全体のSEED（起点）

* handShake
  - amplitude 振幅の大きさ
  - x_value 横方向の大きさ
  - x_speed1 横揺らぎ1の速度
  - x_speed2 横揺らぎ2の速度
  - x_seed 横揺らぎのSEED
  - y_value 縦方向の大きさ
  - y_speed1 縦揺らぎ1の速度
  - y_speed2 縦揺らぎ2の速度
  - y_seed 縦揺らぎのSEED

# camera_valueについて
画面のブレピクセルは、画像の横縦サイズの小さい方の5%をcamera_valueの100%にしています。
大抵の仕上げは110%のゆとりがあるのでその範囲内で揺らす事ができます。10％の半分（左右上下）で5%です。

# amplitude
手ぶれのピクセルはcamera_valueと同じ値ですが、amplitudeはそれに対するパーセントです。


