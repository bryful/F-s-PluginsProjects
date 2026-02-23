# SputteringPosition

 スパッタリングエフェクトです。俗にいう「たたき」です。<br>
 SputteringPositionは位置を指定して行うもので、あとScaleで位置の変更も行えます。<br>

# 機能


* Mode<br> 描画する所
  - Count 描画を行う回数。これにSpawn_Countを掛け算したものが実際の描画数になります。
  - Style 描画位置の指定
    - Sphere 球形
    - Rectangle　矩形
  - Center/Radius 球形の中心、球形の半径。ランダムなので目安です。
  - StartPoint/EndPoint 矩形の指定。やや中心に偏るようになっています。
  - Scalable スケールを有効にする
  - Scale スケール。
  - AnchorPointEnabled アンカーポイントを有効に。非有効時は範囲の中心になります。
  - AnchorPoint スケールの起点の指定
* Textture 外部ファイルを指定する項目
  - Sputtering_Texture_Layer このレイヤを読み込む。なしだと内蔵データを使います。
  - Size %大きさ指定。元が48x48で、それのスケールになります。
  - Size_Random 大きさをばらつかせます。
  - TimeSampling
    - Random 画像の中からランダムで選ぶ
    - Loop 画像の左上から右へ順番に使う
  - koma コマ落としします。
	
* Parameters 発生条件
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

