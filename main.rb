#!mruby/bin/mruby 

#module IRQHandler
#	def tim2
#	end
#end
#
#GC.interval_ratio = 20
#GC.step_ratio = 200

pin = 16

def sleep(n)
	GPIO.delay_us(n * 1e6)
end

GPIO.direction(pin, :out)

bool = false
loop do
	bool = !bool
	GPIO.write(pin, bool)
	GPIO.sleep 1
end
