# Mult

 CreateAlpha/AlphaFix/Premultiply等をまとめたプラグインです。

# 機能

* Unmult<br>Unmultです。アルファーのない画像から明るさをもとにアルファーを作成します。
* Mult<br>アルファー値を掛け算してから全てを不透明にします。まぁ下に黒い画像おいて合成した感じです。
* ToStraight<br>Premultiplited(マット合成)な状態のアルファー付き画像をStraight（ストレート合成）に戻します。
* ToPremultiplited<br>下で説明。


Premultiplitedはストレート（通常のRGB）の値にアルファー値を掛け算したものです。<br>
全然わからない^^;<br>
<br>
「フッテージの変換」の「アルファー」の「ストレートマット無し」と「合成チャンネル」の違いだと思ってください。<br>
<br>
画像処理の重要な概念なので調べて覚えておくとかなり幸せになります。

