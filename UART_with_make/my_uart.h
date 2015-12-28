#ifndef MY_UART_H
#define MY_UART_H

#define __RX_PIN_NUMBER 11 //use this only in defining other things in this file
#define __TX_PIN_NUMBER 9 //use this only in defining other things in this file

#define RXPIN (1 << __RX_PIN_NUMBER)
#define RXPINCFG PIN_CNF[__RX_PIN_NUMBER]
#define TXPIN (1 << __TX_PIN_NUMBER)
#define TXPINCFG PIN_CNF[__TX_PIN_NUMBER]

#define UART_PIN_DISCONNECTED 0xFFFFFFFF//see p. 156 of ref man

void uart_init(void);
void uart_write_str(const char *str);

#endif /*MY_UART_H*/
