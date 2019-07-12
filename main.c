#include <stdint-gcc.h>
#include <stdio.h>

typedef struct {
    uint8_t REG_8;
    uint16_t REG_16;
    uint32_t REG_32;
    union {
        uint32_t REG_32BITFIELD;
        struct {
            unsigned int field1:1;
            unsigned int field2:2;
            unsigned int field3:3;
            unsigned int field4:4;
            unsigned int :4;
            unsigned int field5:5;
            unsigned int field6:6;
            unsigned int field7:7;
        };
    };
} __attribute__((packed)) FAKE_PERIPHERAL_TYPE;

FAKE_PERIPHERAL_TYPE *FAKE_PERIPHERAL = (FAKE_PERIPHERAL_TYPE *)0x20001800;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

int main(void) {
    printf("Size of the peripheral struct %d\n", sizeof(*FAKE_PERIPHERAL));

    FAKE_PERIPHERAL->REG_32BITFIELD = 0xABCDEF79;
    FAKE_PERIPHERAL->REG_8 = 0xCC;
    FAKE_PERIPHERAL->REG_16 = 0xBCBC;
    FAKE_PERIPHERAL->REG_32 = 0xAA55C396;
    FAKE_PERIPHERAL->field5 = 9;
    for (int i = 0; 1; i++) {
        FAKE_PERIPHERAL->REG_16++;
        printf("Hello, World!!! %x\n", i);
        for (long  j = 0; j < 100000000L; j++) {
            __asm__("nop");
        }
    }
}

#pragma clang diagnostic pop


// Redirect file handlers to UART0
// TI LM3S-specific address is used
// Nor readiness check, nor configuration - this stuff works fine under qemu

#define UART0_DR *((uint32_t *) 0x4000c000)

__attribute__((unused)) int _write(__attribute__((unused))int file, const char *data, int len) {
    for (int i = 0; i < len; i++) {
        UART0_DR = data[i];
    }
    return len;
}

