F's TargetGrad.aex / F's TargetGradRadical.aex

ターゲットの色の部分のみにグラデーションを描画。
元絵は消えます。アニメの二値化ペイントされたセルを想定しています。




ベータ版です。今考えている質感エフェクト用にグラデーションを作ったのでそのおまけ。意外と使えそうなので作ったもの


# F's TargetGrad.aex

* target - targatColors/AlphaON グラデーション描画の対象。色か不透明部分か選べる

* targetEnabled1-8 targatColors　targatColorsした時の対象色
  target1

* gradColor グラデの色

* swap グラデのAlpah値を反転（start/leastの入れ替えにしました）

* hyperbolic 双曲線処理を行ってグラデの中間を薄くします。

* 2Point  
  start/last　グラデーションの開始終了点。autoPosがonだと無視される

* autoPos ターゲットの範囲に対して適当にグラデ位置うぃ決める

* rot autoPos時のグラデの傾き

* startPercent/lastPecent グラデ位置を％指定で補正。０で元の位置、100で内側、負数で外側。

* offsetX/offsetY　単純に位置をずらす。

* guideDraw グラデーションの描画位置を計りやすくガイド線を描画

* guideColor ガイド線の色

* ColorTable load/save カラー情報の読み込み・書き出しボタン


# F's TargetGradRadical.aex

* target - targatColors/AlphaON グラデーション描画の対象。色か不透明部分か選べる

* targetEnabled1-8 targatColors　targatColorsした時の対象色
  target1

* gradColor グラデの色

* invert 白黒反転

* center グラデの中心

* radius グラデの半径

* feather ぼかし具合

* hyperbolic 双曲線処理を行ってグラデの中間を薄くします。

* aspect 左右比

* angle 回転

* ColorTable load/save カラー情報の読み込み・書き出しボタン


その他の機能
カラーデータファイルをリネームして以下の場所に置くと、初期値が反映される。

　　C:\Users\＜ユーザー名＞\AppData\Local\FsPlugins\TargetGrad.pref

　　C:\Users\＜ユーザー名＞\AppData\Local\FsPlugins\TargetGradRadical.pref


カラーデータはただのJSONテキストファイルで、直接メモ帳で書き換えても大丈夫。

bry-ful



