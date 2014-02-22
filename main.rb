#!mruby/bin/mruby 

module IRQHandler
	def tim2
	end
end

GC.interval_ratio = 20
GC.step_ratio = 200

pin = 16

GPIO.direction(pin, :low)
bool = false
loop do
	bool = !bool
	GPIO.write(pin, bool)
	10000.times { }
end

