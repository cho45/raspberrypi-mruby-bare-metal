Raspberry Pi 上で mruby を Linux 抜きで動かす
=============================================

動かしかた
----------

### クロスコンパイル環境をつくる

これが一番面倒……

https://github.com/jsnyder/arm-eabi-toolchain を使って ARM のクロスコンパイル環境をつくる。
~/arm-cs-tools/bin に PATH を通す

### ビルド

rake を実行すると mruby のクローンからビルドまで一気にやり kernel.img ができる。

### SD カードへコピー

FAT16 でフォーマットした SD カードを入れて以下のコマンドを実行する

```
rake install dest=/Volumes/SD
```

すると必要なファイルが(必要に応じてダウンロードされて)SDカードにコピーされ、SDカードはイジェクトされる。

### Raspberry Pi で実行

Raspberry Pi に挿しこんで、電源を挿す。
基板上にある ACT LED (緑色) (電源を挿しただけで光る PWR LED の隣) が点灯する。

最初すこし ACT が点滅するのは、バイナリがロードされているとき光るようになっているからっぽい。

それが終わるとこちら側のコードが動く。ACT LED は GPIO 16 に繋っているので、GPIO 16 に対していろいろやってる。


TODO
----

vfp を使った場合うまく動かすことができていない。
コードの問題というより環境構築がうまくいってない。

JTAG の環境をつくれてないのでデバッグが面倒。

参考文献
--------
 
 * https://github.com/yamanekko/mruby/tree/rubyconf/mrbgems/mruby-rs-led
 * https://github.com/yamanekko/raspberrypi/
 * [Raspberry Pi ARM based bare metal examples]( https://github.com/dwelch67/raspberrypi )
 * [BCM2835-ARM-Peripherals (チップの仕様書)]( http://www.raspberrypi.org/wp-content/uploads/2012/02/BCM2835-ARM-Peripherals.pdf )
