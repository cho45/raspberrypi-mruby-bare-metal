#include "mruby.h"
#include "mruby/proc.h"
#include "mruby/data.h"
#include "mruby/compile.h"
#include "mruby/string.h"
#include "mruby/numeric.h"
#include <math.h>
#include <stdlib.h>

mrb_state* mrb;
// struct RClass* interrupt_module;

#include "mrbgems/mruby-raspberrypi-gpio/src/gpio.h"
#include "bytecode.h"

extern void PUT32 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );
extern void dummy ( unsigned int );

void TIM2_IRQHandler ( void ) {
}

volatile unsigned int n;
#define L(x) {\
	GPCLR(16);  \
	for (n = 0; n < (0x50000 * x); n++) dummy(n); \
	GPSET(16); \
	for (n = 0; n < 0x50000; n++) dummy(n); \
	}
// #define L(x) 

int main( void ) {
	mrb = mrb_open();
	if (mrb == NULL) {
		return 0;
	}

	// interrupt_module = mrb_module_get(mrb, "IRQHandler");

	mrb_load_irep(mrb, code);

	while (1) {
		L(1);
	}

	mrb_close(mrb);

	return 0;
}
