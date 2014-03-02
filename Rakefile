require "pathname"

def which(name)
	ret = ENV["PATH"].split(/:/).map {|i|
		File.expand_path(name, i)
	}.find {|i|
		File.exist?(i) && File.executable?(i)
	} or raise "#{name} is not in PATH"
end

ROOT = Pathname.pwd.expand_path

MRUBY_ROOT = Pathname("mruby").expand_path

ARMGCC_ROOT = Pathname(which("arm-none-eabi-gcc")).parent.parent

AOPS = "-mcpu=arm1176jzf-s -mfloat-abi=soft -mfpu=vfp --warn --fatal-warnings "

COPS = "-Wall -nostartfiles -ffreestanding"

CFLAGS = "-mcpu=arm1176jzf-s -mtune=arm1176jzf-s -mfloat-abi=softfp -mfpu=vfp -O1 -ggdb -g"

INCDIR  = "#{MRUBY_ROOT}/include"
ULIBDIR = "#{MRUBY_ROOT}/build/arm-eabi-raspberry-pi/lib"
LIB     = [
	"#{ARMGCC_ROOT}/arm-none-eabi/lib/",
	Dir["#{ARMGCC_ROOT}/lib/gcc/arm-none-eabi/*"].last,
].map {|i| "-L#{i}"}.join(" ")
ULIBS   = "-lmruby"

task :default => "kernel.img"

task :build => "mruby" do
	ENV['MRUBY_CONFIG'] = (ROOT + "build_config.rb").to_s

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
	rm_r "mruby/build/arm-eabi-raspberry-pi" rescue nil
	rm FileList["*.o"] rescue nil
	rm "bytecode.h" rescue nil
	rm "kernel.img" rescue nil
	rm ["main.elf", "main.map", "main.mrb"] rescue nil
end

file "mruby" do
	sh %{git clone git@github.com:mruby/mruby.git}
end

file "vectors.o" => "vectors.s" do
	sh %{ arm-none-eabi-as #{AOPS} vectors.s -o vectors.o }
end

file "bytecode.h" => "main.rb" do
	puts "MRBC main.rb"
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

namespace :jtag do
	task :run => 'main.elf' do
		require "socket"
		s = TCPSocket.open('localhost', 4444)
		th = Thread.start do
			while l = s.gets
				puts l
			end
		end
		def s.command(str)
			$stdout.puts ">>> #{str}"
			write("#{str}\r\n")
		end

		s.command "halt"
		s.command "load_image main.elf"
		s.command "resume 0"
		s.command "exit"

		th.join
	end

	task :server do
		sh %{ sudo kextunload /System/Library/Extensions/FTDIUSBSerialDriver.kext } rescue nil
		sh %{ openocd -d -f openocd/generic-ft4232h.cfg -f openocd/raspi.cfg }
		sh %{ sudo kextload /System/Library/Extensions/FTDIUSBSerialDriver.kext } rescue nil
	end
end

#namespace :bootloader do
#	task :setup => 'bootloader.img' do
#		copy_to = ENV["dest"] or raise "ENV['dest'] is required"
#		until File.exist?(copy_to)
#			print "Waiting for mounting #{copy_to}...\r"
#			$stdout.flush
#			sleep 1
#		end
#		cp "bootloader.img", copy_to + "/kernel.img"
#		cp %w"bootcode.bin start.elf", copy_to
#		sh %{ diskutil eject /Volumes/SD }
#	end
#
##	task :install => "main.elf" do
##		# brew install lrzsz
##		sh %{ arm-none-eabi-objcopy main.elf -O binary main.bin }
##		sh %{ lsx -bk main.bin < /dev/tty.usbserial-FTB3L9UG > /dev/tty.usbserial-FTB3L9UG }
##	end
#
#	file "bootloader.img" do
#		sh %{ wget https://github.com/dwelch67/raspberrypi/raw/master/bootloader05/kernel.img -O bootloader.img }
#	end
#
#end

#namespace :gcc do
#	task :install do
#		dest = Pathname("~/sdk/gcc-arm-none-eabi")
#		if dest.exist?
#			puts "Already installed at #{dest}"
#			exit 1
#		end
#
#		puts "Installing gcc-arm-none-eabi cross compiler to #{dest} OK? [yN]"
#		exit 1 unless $stdin.gets =~ /y/i
#		dest.parent.mkpath
#		Dir.chdir(dest.parent) do
#			file = "gcc-arm-none-eabi-4_8-2013q4-20131218-mac.tar.bz2"
#			unless File.exist?(file)
#				sh %{ wget https://launchpad.net/gcc-arm-embedded/4.8/4.8-2013-q4-major/+download/#{file} }
#			end
#			sh %{ tar xjvf #{file} }
#			mv "gcc-arm-none-eabi-4_8-2013q4-20131218-mac", dest
#		end
#		puts "Installed. You should add '#{dest}/bin' to PATH"
#	end
#end
