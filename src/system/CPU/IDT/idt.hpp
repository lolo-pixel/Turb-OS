#pragma once

#include <stdint.h>
#include <lib/cpu/cpu.hpp>

namespace turbo::idt {

	#define SYSCALL 0x80

	enum IRQS{
		IRQ0 = 32,
		IRQ1 = 33,
		IRQ2 = 34,
		IRQ3 = 35,
		IRQ4 = 36,
		IRQ5 = 37,
		IRQ6 = 38,
		IRQ7 = 39,
		IRQ8 = 40,
		IRQ9 = 41,
		IRQ10 = 42,
		IRQ11 = 43,
		IRQ12 = 44,
		IRQ13 = 45,
		IRQ14 = 46,
		IRQ15 = 47,
	};

	struct [[gnu::packed]] idtEntry_t{
		uint16_t offset_1;
		uint16_t selector;
		uint8_t ist;
		uint8_t type_attr;
		uint16_t offset_2;
		uint32_t offset_3;
		uint32_t zero;
	};

	struct [[gnu::packed]] idtr_t{
		uint16_t limit;
		uint64_t base;
	};
	
	struct [[gnu::packed]] registers_t{
		uint64_t r15, r14, r13, r12, r11, r10, r9, r8;
		uint64_t rbp, rdi, rsi, rdx, rcx, rbx, rax;
		uint64_t int_no, error_code, rip, cs, rflags, rsp, ss;
	};

	using intHandler_t = void (*)(registers_t *);

	extern idtEntry_t idt[];
	extern idtr_t idtr;

	extern intHandler_t interrupt_handlers[];

	extern bool isInit;

	void reload();

	void init();
	void registerInterruptHandler(uint8_t vector, intHandler_t handler);

	extern "C" void int_handler(registers_t *regs);
}