Raspberry Pi 上で mruby を Linux 抜きで動かす
=============================================

ファイル
--------

* Rakefile : Makefile の変わりに全部 Rakefile にビルド定義を書いてある
* build_config.rb : mruby のビルド設定ファイル。ビルド時に mruby のディレクトリに symlink している
* main.rb : Ruby レベルのエントリポイント。コンパイルされたバイトコードが使われる
* main.c : C 言語レベルのエントリポイント。mruby_open() して実際にバイトコードを実行するプログラム
* bytecode.h : コンパイル済みの main.rb を C のヘッダファイルにしたもの。バイトコードデータファイル
* memmap : リンカスクリプト (メモリ上にオブジェクトをどう配置するかの定義)
* syscalls.c : newlib が要求するシステムコールのモック的実装 (ほぼ参考文献からのコピー)
* vectors.s : アセンブラレベルのローレベル処理定義
* mrbgems/mruby-raspberrypi-gpio/ : Raspberry Pi の GPIO を使うための mrbgem。ビルド時に mruby の mrbgems ディレクトリに symlink している
* kernel.img : ARM 実行形式のファイル。これを SD カードにコピーして実行させる
* .gdbinit :  arm-none-eabi-gdb を実行したときにコンパイル済み main.elf を実行するためのもの


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
