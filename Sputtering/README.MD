# Sputtering

 スパッタリングエフェクトです。俗にいう「たたき」です。<br>

# 機能

* Targetr<br> 描画する所
  - Target_mode
    - Alpha 不透明部分で
    - Color1-8　指定した色で
* Border_Direction<br>指定したエリアに指定した方向にのみ発生させる。
  - Diection_Angle 発生させる向き
* Textture 外部ファイルを指定する項目
  - Sputtering_Texture_Layer このレイヤを読み込む。なしだと内蔵データを使います。
  - Size %大きさ指定。元が48x48で、それのスケールになります。
  - Size_Random 大きさをばらつかせます。
  - TimeSampling
    - Random 画像の中からランダムで選ぶ
    - Loop 画像の左上から右へ順番に使う
  - koma コマ落としします。
	
* Parameters 発生条件
  - Area_Rate  平面部分での発生率。Border_DirectionがONの時は使いません（0になる）
  - Border_Rate 境界部分での発生率
  - Spawn_Count 発生ノード１個につき描画する回数。
  - Spawn_Offset 発生ノード位置から離れる距離
  - Opacity 不透明度
  - Opacity_random 不透明度のばらつき
  - Auto_Seed 自動的にSeedを変化させます。
  - Seed ランダムのシード 
* Colors<br> 描画するものの色
  - Color_Count 使用する色数
  - Coior_1 .. 8 色。８色まで使えます。
* Blend with original<br> ONで元絵を合成します。

# スッパッタリングデータ。

48x48ピクセルサイズの元画像を複数グリッド状に並べたものです。<br>
<br>
<img src=".\Sample.png"><br>
<br>
作るのが大変なので専用のソフト作りました。<br>
グレースケールです。色が付いたものでも読み込めますが、Gチャンネルしか読んでいません。アルファーチャンネルがついていたらそれも考慮します。<br>
<br>

## PicTiler.exe

アーカイブに同梱していますが、なかった場合は[ここ](https://github.com/bryful/F-s-PluginsProjects/raw/refs/heads/master/_DL_windowsbinary/PicTiler.zip)からDLできます。<br>
使い方は一緒にある[README.MD](https://github.com/bryful/AE_Helper/tree/master/PicTiler)を参照してください。

### PicTiler.exeの使い方。

1. 適当なデータを作る。48x48ピクセル。違うサイズで作ってもPicTilerが勝手に縮小します。
2. リストに登録。"File:AddImage"メニューで登録。ドラッグ＆ドロップでも可能。
3. Up/Down/Remove/Clearで順番等を修正。
4. 書き出すサイズ（グリッドの数）を設定。余った場合は繰り返しで自動配置します。
5. Exportで書き出し。

です。

