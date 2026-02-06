# Grain

よくあるノイズです。<br>
1980x1080だと１ピクセルなノイズだと雰囲気がでなくなってしまったので作ったやつです。

# 機能

* masterPercent<br>amount/accntCoutと連動しています。
* amount<br>ノイズの強さ
* size<br>ノイズの大きさ
* subAmountPercent<br>サブノイズの強さをamountに対するパーセントで指定します。
* subSizePercent<br>サブノイズの大きさをsizeに対するパーセントで指定します。
* accntCout<br>アクセントとなる粒の描画数
* accntLoLimitPercent<br>描画数の最低値を％で指定。0で0..accntCoutの範囲のランダム、n%でaccntCout*accntLoLimitPercent/100からaccntCoutのランダム、100でaccntCoutちょうど
* accntintensity<br>アクセントとなる粒のの濃度の最大値、100以上も指定できます。



