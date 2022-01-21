#include <kernel/main.hpp>
#include <stivale2.h>
#include <stddef.h>
#include <lib/cpu/cpu.hpp>
#include <kernel/kernel.hpp>

uint8_t kernelStack[STACK_SIZE] = {[0 ... STACK_SIZE - 1] = 'A'};

static struct stivale2_header_tag_terminal terminal_hdr_tag = {
    .tag = {
        .identifier = STIVALE2_HEADER_TAG_TERMINAL_ID,
        .next = 0
    },
    .flags = 0,
    .callback = 0
};

static struct stivale2_header_tag_smp smp_hdr_tag = {
    .tag = {
        .identifier = STIVALE2_HEADER_TAG_SMP_ID,
        .next = (uint64_t)&terminal_hdr_tag
    },
    .flags = 0
};

static struct stivale2_header_tag_framebuffer framebuffer_hdr_tag = {
    .tag = {
        .identifier = STIVALE2_HEADER_TAG_FRAMEBUFFER_ID,
        .next = (uint64_t)&smp_hdr_tag
    },
    .framebuffer_width = 0,
    .framebuffer_height = 0,
    .framebuffer_bpp = 0,
    .unused = 0
};

#if (LVL5_PAGING != 0)
static struct stivale2_tag lvl5_hdr_tag = {
    .identifier = STIVALE2_HEADER_TAG_5LV_PAGING_ID,
    .next = reinterpret_cast<uint64_t>(&framebuffer_hdr_tag)
};
#endif

[[gnu::section(".stivale2hdr"), gnu::used]]
static struct stivale2_header stivale_hdr = {
    .entry_point = 0,
    .stack = (uintptr_t)kernelStack + STACK_SIZE,
    .flags = (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4),
    #if(LV5_PAGING != 0)
        .tags = (uintptr_t)&lvl5_hdr_tag
    #else
        .tags = (uintptr_t)&framebuffer_hdr_tag
    #endif
};

void *stivale2_get_tag(stivale2_struct *stivale, uint64_t id){
    stivale2_tag *current_tag = (stivale2_tag*)stivale->tags;
    while(true){
        if (current_tag == NULL){
            return NULL;
        }

        if(current_tag->identifier == id){
            return current_tag;
        }

        current_tag = (stivale2_tag*)current_tag->next;
    }
}

extern "C" void _start(stivale2_struct *stivale2_struct){
    enableSSE();

    turbo::main(stivale2_struct);

    while(true){
        asm volatile ("hlt");
    }
}