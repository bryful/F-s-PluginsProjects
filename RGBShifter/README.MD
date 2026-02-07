# RGBShifter

 RGB値を制御します。<br>
 RGBAControl/YUVControlの後継です。<br>
## ShiftMode
* RGB Channel<br>RGB各チャンネルを個別にシフトします。
* YUV Channel<br>YUV各チャンネルを個別にシフトします。
* Luma Identity<br>画像のLuma成分をそのままにして、TargetColorのUとVを適応させます。<br>平面をカラー合成した感じです。

## パラメータ
* ShiftMode<br>モード切替
* RedShift<br>Redチャンネルのシフト量
* GreenShift<br>Greenチャンネルのシフト量
* BlueShift<br>Blueチャンネルのシフト量
* Y_Luminance<br>Yチャンネルのシフト量
* U_Chroma<br>Uチャンネルのシフト量
* V_Chroma<br>Vチャンネルのシフト量
* TargetColor<br>Luma Identityモードで使用するターゲットカラー
* Blend with original<br>元画像を混ぜることができます。<br>0%で完全にシフト後の画像、100%で元画像そのままです。<br>
