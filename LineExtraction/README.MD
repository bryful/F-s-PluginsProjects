# LineExtraction

 線抽出エフェクトです。<br>
 線の色を指定できます。F's LineDetectionの後継になります。<br>

# 機能

* Pre-processing前処理
 - Bilateral バイラテラルフィルタを実行します。
 - Radius ぼかし半径
 - Sigma_Spatial 空間の広がり 
 - Sigma_Range 輝度の許容差 / 輝度感度
* Outer Sampling<br> 外側の線幅
* Inner Sampling<br> 内側の線幅
* Color<br>線の色
* Post Level<br>　後処理のレベル補正
* Level_lo
* Lebel_Hi
* Blend with Original

Outer SamplingとInner Samplingを両方０にするとPre-processingのみ行います。

