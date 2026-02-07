# LogicGray

 LogicGrayはカラー画像をグレースケールに変換します。<br>
 ToGrayの後継です。<br>
## 機能

### LogicType
* Luma(YUV)<br>。YuvのY成分を使ってグレースケールに変換します。<br>
* Lightness(HLS)<br>HLSのLightness成分を使ってグレースケールに変換します。<br>
* Average(RGB)<br>RGBの平均値を使ってグレースケールに変換します。<br>
* Red Channel<br>RGBのR成分を使ってグレースケールに変換します。<br>
* Green Channel<br>RGBのG成分を使ってグレースケールに変換します。<br>
* Blue Channel<br>RGBのB成分を使ってグレースケールに変換します。<br>
* Max(RGB)<br>RGBの最大値を使ってグレースケールに変換します。<br>
* Perceptual(Lab)<br>LabのL成分を使ってグレースケールに変換します。<br>

### パラメータ
まあ見ればわかります。

### Blend with original
元画像を混ぜることができます。<br>
0%で完全にグレースケール、100%で元画像そのままです。<br>
