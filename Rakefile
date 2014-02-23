require "pathname"

def which(name)
	ret = ENV["PATH"].split(/:/).map {|i|
		File.expand_path(name, i)
	}.find {|i|
		File.exist?(i) && File.executable?(i)
	} or raise "#{name} is not in PATH"
end

MRUBY_ROOT = Pathname("mruby").expand_path

ARMGCC_ROOT = Pathname(which("arm-none-eabi-gcc")).parent.parent

AOPS = "-mcpu=arm1176jzf-s -mfloat-abi=soft -mfpu=vfp --warn --fatal-warnings "

COPS = "-Wall -nostartfiles -ffreestanding"

CFLAGS = "-mcpu=arm1176jzf-s -mtune=arm1176jzf-s -mfloat-abi=soft -mfpu=vfp -O0 -ggdb -g"

INCDIR = MRUBY_ROOT + "include"
ULIBDIR = MRUBY_ROOT + "build/arm-eabi-raspberry-pi/lib"
LIB = "-L#{ARMGCC_ROOT}/arm-none-eabi/lib/ -L#{ARMGCC_ROOT}/lib/gcc/arm-none-eabi/4.7.3"
ULIBS = "-lmruby"

task :default => "kernel.img"

task :build => "mruby" do
	mruby = Pathname("mruby")
	symlinks = %w[build_config.rb] + Dir.glob("mrbgems/*")
	symlinks.map {|i| Pathname(i) }.each do |f|
		path = mruby + f
		path.unlink
		path.make_symlink(f.expand_path)
	end

	Dir.chdir("mruby") do
		sh %{ rake }
	end
end

task :install => %w"kernel.img bootcode.bin start.elf" do
	copy_to = ENV["dest"] or raise "ENV['dest'] is required"
	until File.exist?(copy_to)
		print "Waiting for mounting #{copy_to}...\r"
		$stdout.flush
		sleep 1
	end
	cp %w"kernel.img bootcode.bin start.elf", copy_to
	sh %{ diskutil eject /Volumes/SD }
end

task :clean => ["mruby"] do
#	Dir.chdir("mruby") do
#		sh %{ rake clean }
#	end
	rm_r "mruby/build/arm-eabi-raspberry-pi"
	rm FileList["*.o"]
	rm "bytecode.h"
	rm "kernel.img"
	rm ["main.map", "main.mrb"]
end

file "mruby" do
	sh %{git clone git@github.com:mruby/mruby.git}
end

file "vectors.o" => "vectors.s" do
	sh %{ arm-none-eabi-as #{AOPS} vectors.s -o vectors.o }
end

file "bytecode.h" => "main.rb" do
	bytes = `mruby/bin/mrbc -Bcode -o - main.rb`
	File.open("bytecode.h", "w") do |f|
		f.puts bytes
	end
end

file "main.o" => %w"main.c bytecode.h" do
	sh %{ arm-none-eabi-gcc #{COPS} #{CFLAGS}  -I #{INCDIR} -c main.c -o main.o }
end

file "syscalls.o" => "syscalls.c" do
	sh %{ arm-none-eabi-gcc #{COPS} #{CFLAGS} -c syscalls.c -o syscalls.o }
end

file "ruby/build/arm-eabi-raspberry-pi/lib/libmruby.a" => :build

file "main.elf" => %w"memmap vectors.o main.o syscalls.o mruby/build/arm-eabi-raspberry-pi/lib/libmruby.a" do
	sh %{ arm-none-eabi-ld vectors.o main.o syscalls.o -Map=main.map -T memmap -o main.elf -L#{ULIBDIR} #{ULIBS} #{LIB} -lc -lm -lgcc  }
end

file "kernel.img" => [:build, "main.elf"] do
	sh %{ arm-none-eabi-objcopy main.elf -O binary kernel.img }
	puts "kernel.img: #{File.size('kernel.img') / 1024}KB"
end
