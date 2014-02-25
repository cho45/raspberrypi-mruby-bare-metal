#!mruby/bin/mruby 

pin = 16

GPIO.direction(pin, :out)

bool = false
time = 0.2
loop do
	bool = !bool
	GPIO.write(pin, bool)
	GPIO.sleep time
    time += 0.2
end
