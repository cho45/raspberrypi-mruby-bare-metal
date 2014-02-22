// Peripherals Data Sheet
// http://www.raspberrypi.org/wp-content/uploads/2012/02/BCM2835-ARM-Peripherals.pdf
//
// Bare metal info:
// https://github.com/dwelch67/raspberrypi
// Bare metal mruby:
// https://github.com/yamanekko/mruby/blob/rubyconf/mrbgems/mruby-rs-led/src/rs_led.c
// https://github.com/yamanekko/raspberrypi/blob/master/README.ja.md

#include "mruby.h"
#include "mruby/variable.h"
#include "gpio.h"

extern void PUT32 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );

// static uint16_t micro_second = 0;

//void TIM2_IRQHandler ( void ) {
//	micro_second++;
//	// mrb_funcall(mrb, mrb_obj_value(interrupt_module), "tim2", 0);
//}

static inline void mrb_mruby_raspberrypi_gpio_gem_alternate_function_select(uint8_t pin, uint8_t func) {
	func &= 0b111;
	unsigned int reg;
	reg = (pin < 10) ? GPFSEL0 :
	      (pin < 20) ? GPFSEL1 :
	      (pin < 30) ? GPFSEL2 :
	      (pin < 40) ? GPFSEL3 :
	      (pin < 50) ? GPFSEL4 :
	                   GPFSEL5 ;
	pin = (pin % 10) * 3;
	PUT32(reg, (GET32(reg) & ~(0b111 << pin)) | func);
}

static mrb_value mrb_mruby_raspberrypi_gpio_gem_write(mrb_state* mrb,  mrb_value self) {
	mrb_int pin; mrb_bool state;
	mrb_get_args(mrb, "ib", &pin, &state);

	if (state) {
		GPSET(pin);
	} else {
		GPCLR(pin);
	}

	return mrb_nil_value();
}

static mrb_value mrb_mruby_raspberrypi_gpio_gem_read(mrb_state* mrb,  mrb_value self) {
	mrb_int pin;
	mrb_get_args(mrb, "i", &pin);

	return mrb_bool_value(GPLEV(pin));
}

static mrb_value mrb_mruby_raspberrypi_gpio_gem_direction(mrb_state* mrb,  mrb_value self) {
	mrb_value state; // state is symbol
	mrb_int pin;
	mrb_get_args(mrb, "io", &pin, &state);

	if (mrb_intern_cstr(mrb, "in") == mrb_symbol(state)) {
		mrb_mruby_raspberrypi_gpio_gem_alternate_function_select(pin, 0b000);
	} else
	if (mrb_intern_cstr(mrb, "out") == mrb_symbol(state)) {
		mrb_mruby_raspberrypi_gpio_gem_alternate_function_select(pin, 0b001);
	} else
	if (mrb_intern_cstr(mrb, "high") == mrb_symbol(state)) {
		mrb_mruby_raspberrypi_gpio_gem_alternate_function_select(pin, 0b001);
		GPSET(pin);
	} else
	if (mrb_intern_cstr(mrb, "low") == mrb_symbol(state)) {
		mrb_mruby_raspberrypi_gpio_gem_alternate_function_select(pin, 0b001);
		GPCLR(pin);
	}

	return mrb_nil_value();
}

void mrb_mruby_raspberrypi_gpio_gem_init(mrb_state* mrb) {
	struct RClass* GPIO;

	// define ruby module
	GPIO = mrb_define_module(mrb, "GPIO");

	mrb_define_module_function(mrb, GPIO, "write", mrb_mruby_raspberrypi_gpio_gem_write, ARGS_REQ(2));
	mrb_define_module_function(mrb, GPIO, "read", mrb_mruby_raspberrypi_gpio_gem_read, ARGS_REQ(1));
	mrb_define_module_function(mrb, GPIO, "direction", mrb_mruby_raspberrypi_gpio_gem_direction, ARGS_REQ(2));
}

void mrb_mruby_raspberrypi_gpio_gem_final(mrb_state* mrb) {
}
