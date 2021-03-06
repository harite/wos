/*
 * traps.c
 *
 * (c) 2011	wzt
 *
 */

#include <wos/gdt.h>
#include <wos/idt.h>
#include <asm/asm.h>
#include <wos/task.h>

extern void divide_error();
extern void debug();
extern void default_int(void);
extern void nmi();
extern void int3();
extern void overflow();
extern void bounds();
extern void invalid_op();
extern void coprocessor_segment_overrun();
extern void reserved();
extern void irq13();
extern void double_fault();
extern void invalid_TSS();
extern void segment_not_present();
extern void stack_segment();
extern void general_protection();
extern void device_not_available();
extern void timer_interrupt();
extern void parallel_interrupt();
extern void keyboard_interrupt();
extern void system_call();

struct idt_desc {
        unsigned int a;
        unsigned int b;
};

struct idt_desc new_idt[256];

void set_idt_desc(unsigned int base, unsigned int segment_sel, int type, int idx)
{
        new_idt[idx].a = (base & 0x0000ffff) | (segment_sel << 16);
        new_idt[idx].b = (base & 0xffff0000) | type;
}

void setup_idt(void)
{
        int i;

        for (i = 0; i < 256; i++)
		set_intr_gate((unsigned int)default_int, i);
}

void do_divide_error(unsigned int esp)
{
	panic("divide", esp);
}

void do_debug(unsigned int esp)
{
	panic("debug", esp);
}

void do_nmi(unsigned int esp)
{
	panic("nmi", esp);
}

void do_int3(unsigned int esp)
{
	panic("int3", esp);
}

void do_overflow(unsigned int esp)
{
	panic("overflow", esp);
}

void do_bounds(unsigned int esp)
{
	panic("bounds", esp);
}

void do_invalid_op(unsigned int esp)
{
	panic("invalid_op", esp);
}

void do_coprocessor_segment_overrun(unsigned int esp)
{
	panic("coprocessor_segment_overrun", esp);
}

void do_reserved(unsigned int esp)
{
	panic("reserved", esp);
}

void do_irq13(unsigned int esp)
{
	panic("do_irq13", esp);
}

void do_double_fault(unsigned int esp)
{
	panic("double_fault", esp);
}
 
void do_invalid_tss(unsigned int esp)
{
	panic("invalid_tss", esp);
}

void do_segment_not_present(unsigned int esp)
{
	panic("segment_not_present", esp);
}

void do_stack_segment(unsigned int esp)
{
	panic("stack_segment", esp);
}

void do_general_protection(unsigned int esp)
{
	panic("general_protection", esp);
}

void do_device_not_available(unsigned int esp)
{
	panic("device_not_available", esp);
}

void init_trap(void)
{
	int i = 0;

	set_trap_gate((unsigned int)divide_error, 0);
	set_trap_gate((unsigned int)debug, 1);
	set_trap_gate((unsigned int)nmi, 2);
	set_system_gate((unsigned int)int3, 3);
	set_system_gate((unsigned int)overflow, 4);
	set_system_gate((unsigned int)bounds, 5);
	set_trap_gate((unsigned int)invalid_op, 6);
	set_trap_gate((unsigned int)device_not_available, 7);
	set_trap_gate((unsigned int)coprocessor_segment_overrun, 9);
	set_trap_gate((unsigned int)reserved, 15);
	set_trap_gate((unsigned int)irq13, 45);

	set_trap_gate((unsigned int)double_fault, 8);
	set_trap_gate((unsigned int)invalid_TSS, 10);
	set_trap_gate((unsigned int)segment_not_present, 11);
	set_trap_gate((unsigned int)stack_segment, 12);
	set_trap_gate((unsigned int)general_protection, 13);

	for (i = 0x22; i <= 0x2f; i++)
		set_system_gate((unsigned int)default_int, i);
}

void io_delay(void)
{
	asm("nop\n\t"
		"nop\n\t"
		"nop\n\t"
		"nop\n\t"
		"nop\n\t"
		"nop\n\t"
		"nop\n\t"
		"nop\n\t"
		"nop\n\t"
		"nop\n\t"
		"nop\n\t"
		"nop\n\t"
		"nop\n\t"
		"nop\n"::);
}

void init_8259A(void)
{
	outb(0x11, 0x20);
	io_delay();
	outb(0x11, 0xa0);
	io_delay();

	outb(0x20, 0x21);
	io_delay();
	outb(0x28, 0xa1);
	io_delay();

	outb(0x04, 0x21);
	io_delay();
	outb(0x02, 0xa1);
	io_delay();

	outb(0x01, 0x21);
	io_delay();
	outb(0x01, 0xa1);
	io_delay();

	outb(0xff, 0x21);
	io_delay();
	outb(0xff, 0xa1);
	io_delay();
}
