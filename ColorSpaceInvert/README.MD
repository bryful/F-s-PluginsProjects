# ColorSpaceInvert

RGB/YUV/HSLの各カラースペースにおける色反転を行います。

# 機能

* mode<br>RGBChannels|InvertLuma(Y)|InvertHue(H)
* value<br>反転具合。100で完全反転、0で反転なし。

RGBだと俗にいう「補色」反転になります。<br>
YUVのLuma反転は明るさを反転。<br>
HSLのHue反転は色相を反転します。<br>

100%状態ならRGB/YUV/HSLでかなり色が変わるので、用途によって使い分けられます。<br>
ただHSL以外は５０％時にグレーになってしまうので注意してください。<br>
早いタイミングならどれでも大丈夫ですが、遅い場合グレーが目立ちます。



