#ifndef __ZLG9518S_H
#define __ZLG9518S_H

#include "main.h"

#define UART_CHAN_0 0x00
#define UART_CHAN_1 0x01
#define UART_CHAN_2 0x02
#define UART_CHAN_3 0x03
#define UART_CHAN_4 0x04
#define UART_CHAN_5 0x05
#define UART_CHAN_6 0x06
#define UART_CHAN_7 0x07

#define ZLG9518S_LCRO_REG 1
#define S2UC_LCR0_PUT_STATE_BIT(S2UC_LCR0_ON_STATE) 0x80 //0b10000000
#define S2UC_LCR0_PUT_DLEN_BIT(S2UC_LCR0_8_BIT_DATA) 0x60 //0b01100000
#define S2UC_LCR0_PUT_PARITY_BIT(S2UC_LCR0_PARITY_NONE) 0x00 //0b00000000
#define S2UC_LCR0_PUT_STOP_BIT(S2UC_LCR0_STOP_1_BIT) 0x00 //0b00000000

#define ZLG9518S_LCR1_REG 2
//#define S2UC_LCR1_PUT_BAUD_BIT(S2UC_LCR1_BAUD_19200) 0x03 //0b00000011
#define S2UC_LCR1_PUT_BAUD_BIT(S2UC_LCR1_BAUD_115200) 0x07 //0b00000111
#define S2UC_LCR1_PUT_CTS_BIT(S2UC_LCR1_CTS_OFF) 0x00 //0b00000000
#define S2UC_LCR1_PUT_RTS_BIT(S2UC_LCR1_RTS_OFF) 0x00 //0b00000000
#define S2UC_LCR1_PUT_FIFO_ENINT_BIT(S2UC_LCR1_INT_EN) 0x20 //0b00100000
#define S2UC_LCR1_PUT_STREAM_ENINT_BIT(S2UC_LCR1_INT_DIS) 0x00 //0b00000000
#define S2UC_LCR1_PUT_ERR_ENINT_BIT(S2UC_LCR1_INT_DIS) 0x00 //0b00000000

#define ZLG9518S_TCR_REG 3
#define S2UC_TCR_PUT_STREAM_START_BIT(S2UC_TCR_STREAM_32_BYTE_MAX255) 0x20 //0b00100000
#define S2UC_TCR_PUT_STREAM_END_BIT(S2UC_TCR_STREAM_224_BYTE_MAX255) 0x0E //0b00001110

#define ZLG9518S_TLR_REG 4
#define S2UC_TLR_PUT_RFIFO_INT_BIT(S2UC_TCR_STREAM_224_BYTE_MAX255) 0xE0 //0b11100000
#define S2UC_TLR_PUT_TFIFO_INT_BIT(S2UC_TCR_STREAM_240_BYTE_MAX255) 0x0F //0b00001111

#define ZLG9518S_RHR_REG 10
#define ZLG9518S_THR_REG 11

#define S2UC_WRITE_FORM 0
#define S2UC_READ_FORM 1

#define S2UC_PUT_CHAN_BIT(uart_chan) uart_chan
#define S2UC_PUT_REG_BIT(reg) reg << 3
#define S2UC_PUT_RW_BIT(rw) rw << 7

#define GPIO_REN GPIOC
#define GPIO_Pin_REN GPIO_PIN_13

void ZLG9518S_Init(void);
void SPI3_Init(void);
void SPI3_SetSpeed(u8 SpeedSet);
void s2u_configure_one_uart(uint8_t uart_chan);
void zlg9518s_set_cmd(uint8_t rw, uint8_t reg, uint8_t uart_chan, uint8_t prm, uint8_t *cmd_buf);
void zlg9518s_send_write_cmd(uint8_t reg, uint8_t uart_chan, uint8_t prm, uint8_t *cmd_buf);
void zlg9518s_send_read_cmd(uint8_t reg, uint8_t uart_chan, uint8_t prm, uint8_t *cmd_buf);
uint8_t zlg9518s_read_reg_pack(uint8_t reg, uint8_t uart_chan, uint8_t prm);


#endif
