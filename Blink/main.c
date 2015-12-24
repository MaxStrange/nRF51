#include <stdbool.h>
#include <stdint.h>
#include "nrf_delay.h"
#include "boards.h"

#define PIN21 (1 << 21)
#define PIN22 (1 << 22)
#define PIN23 (1 << 23)
#define PIN24 (1 << 24)

int main(void)
{
	NRF_GPIO->DIR |= PIN21 | PIN22 | PIN23 | PIN24;

	while (true)
	{
		NRF_GPIO->OUT ^= PIN21 | PIN22 | PIN23 | PIN24;
		nrf_delay_ms(500);
	}
}
