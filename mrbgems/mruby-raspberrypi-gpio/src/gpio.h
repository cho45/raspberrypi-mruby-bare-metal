// BUS address -> ARM physical address
#define BUS_ADDRESS(bus_address) (bus_address - 0x5e000000)

// GPIO Alternate function select register
#define GPFSEL0 BUS_ADDRESS(0x7E200000)
#define GPFSEL1 BUS_ADDRESS(0x7E200004)
#define GPFSEL2 BUS_ADDRESS(0x7E200008)
#define GPFSEL3 BUS_ADDRESS(0x7E20000C)
#define GPFSEL4 BUS_ADDRESS(0x7E200010)
#define GPFSEL5 BUS_ADDRESS(0x7E200014)

// GPIO Pin Output Set Registers
#define GPSET0  BUS_ADDRESS(0x7E20001C)
#define GPSET1  BUS_ADDRESS(0x7E200020)
#define GPSET(pin) PUT32(pin < 32 ? GPSET0 : GPSET1, 1 << pin)

// GPIO Pin Output Clear Registers
#define GPCLR0  BUS_ADDRESS(0x7E200028)
#define GPCLR1  BUS_ADDRESS(0x7E20002C)
#define GPCLR(pin) PUT32(pin < 32 ? GPCLR0 : GPCLR1, 1 << pin)

// GPIO Pin Level Registers
#define GPLEV0  BUS_ADDRESS(0x7E200034)
#define GPLEV1  BUS_ADDRESS(0x7E200038)
#define GPLEV(pin) ((GET32(pin < 32 ? GPLEN0 : GPLEN1) & (1 << pin)) != 0)

// GPIO Event Detect Status Registers
#define GPEDS0  BUS_ADDRESS(0x7E200040)
#define GPEDS1  BUS_ADDRESS(0x7E200044)

// GPIO Rising Edge Detect Enable Registers
#define GPREN0  BUS_ADDRESS(0x7E20004C)
#define GPREN1  BUS_ADDRESS(0x7E200050)

// GPIO Falling Edge Detect Enable Registers
#define GPFEN0  BUS_ADDRESS(0x7E200058)
#define GPFEN1  BUS_ADDRESS(0x7E20005C)

// GPIO High Detect Enable Registers
#define GPHEN0  BUS_ADDRESS(0x7E200064)
#define GPHEN1  BUS_ADDRESS(0x7E200068)

// GPIO Low Detect Enable Registers
#define GPLEN0  BUS_ADDRESS(0x7E200070)
#define GPLEN1  BUS_ADDRESS(0x7E200074)

// GPIO Asynchronous rising Edge Detect Enable Registers
#define GPAREN0 BUS_ADDRESS(0x7E20007C)
#define GPAREN1 BUS_ADDRESS(0x7E200080)

// GPIO Asynchronous Falling Edge Detect Enable Registers
#define GPAFEN0 BUS_ADDRESS(0x7E200088)
#define GPAFEN1 BUS_ADDRESS(0x7E20008C)

// GPIO Pull-up/down Register
#define GPPUD   BUS_ADDRESS(0x7E200094)

// GPIO Pull-up/down Clock Registers
#define GPPUDCLK0 BUS_ADDRESS(0x7E200098)
#define GPPUDCLK1 BUS_ADDRESS(0x7E20009C)

