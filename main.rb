#!mruby/bin/mruby 

#module IRQHandler
#	def tim2
#	end
#end
#
#GC.interval_ratio = 20
#GC.step_ratio = 200

pin = 16

GPIO.direction(pin, :out)

bool = false
loop do
	bool = !bool
	GPIO.write(pin, bool)
	GPIO.delay_us(1e6)
#	5000.times {}
end
