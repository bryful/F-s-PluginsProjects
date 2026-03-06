# Ligtning

 「稲妻(高度)」をアニメチックにしたものです。<br>
かなり独特なパラメータなので、良い形を作るのが結構難しいです。<br>
<br>
モードがいくつかあります。

# 共通のパラメータ。

* mode モードを選びます。
* autoSeed勝手に動かします。
* seed シードです。
* wipe 0-100-200で描画範囲を指定します。
* startWeight スタート点の太さ
* endWeight ラスト点の大きさ
* weight 上記以外の線の太さ
* cmplexity 折り返しの数です。増やせば細かくなります。
* jaggedness 折り返し点の大きさです。
* comple_angle 折り返し点を回転させます。
* fork 線をfork数分描画します。
* color 線の色


## 2Points
 ２点で指定しまっす。
* start
* end

## path
パスマスクを選んでその頂点をとおる稲妻を描画します。

* path パスを選ぶ

## 2-2Points
点４点を使って描画します。

* startPoint1
* startPoint2
* endPoint1
* endPoint2
* 2_2_count 描画する本数。
* startRand スタート位置を適当に動かします。
* endRand エンド位置を適当に動かします。

## 2-2Points
３点を使って描画します。一番稲妻っぽい奴です。

* secPoint スタート
* impactLine1/impactLine2
* impactRand 到着点を適当に動かします。
* branch 枝分かれの数。

## radical
中心を指定して放射状に描きます。

* r_count 描く本数。
* r_angle 回転させます。適当に逆回転するものもあります。
* r_Cente 中心
* r_radius 半径
* radiusRand 半径の乱雑さ

