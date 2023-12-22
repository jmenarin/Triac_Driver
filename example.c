#include "stm8s.h"
#include "stm8s_flash.h"
#include "triac_driver.h"

#define TRIAC_CONF GPIOC
#define ZERO_CONF GPIOB
#define TRIAC GPIO_PIN_3
#define ZERO GPIO_PIN_5

void GPIO_Config(void);
void triac_off_config(void);
void triac_on_config(void);
bool read_zero(void);

main()
{

	GPIO_Config();

	triac_config(read_zero, triac_on_config, triac_off_config); // Pass the addresses of the functions as parameters

	while (1)
	{

		turn_on_triac(90); // Turn on the TRIAC at a 90-degree angle
	}
}

/*Variable used to configure the pins of the application*/
void GPIO_Config(void)
{
	GPIO_Init(TRIAC_CONF, TRIAC, GPIO_MODE_IN_PU_NO_IT);
	GPIO_Init(ZERO_CONF, ZERO, GPIO_MODE_OUT_PP_LOW_FAST);
}

/*The address of this function is passed as a parameter to turn on the TRIAC.*/
void triac_on_config(void)
{
	GPIO_WriteLow(TRIAC_CONF, TRIAC);
}

/*The address of this function is passed as a parameter to turn on the TRIAC.*/
void triac_off_config(void)
{
	GPIO_WriteHigh(TRIAC_CONF, TRIAC);
}

/*The address of this function is passed as a parameter to read the ZeroCross signal*/
bool read_zero(void)
{
	if (GPIO_ReadInputPin(ZERO_CONF, ZERO) == RESET)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}
