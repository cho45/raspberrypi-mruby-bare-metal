module GPIO
	def sleep(n)
		GPIO.delay_us(n * 1e6)
	end

	extend self
end

