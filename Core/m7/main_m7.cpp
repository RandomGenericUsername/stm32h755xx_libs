#define CORE_CM7
#include <stm32h755xx.h>
#include <InputPin.hh>
#include <Register.hh>

extern "C"{

    void SystemInit(void);
}

volatile uint32_t* a[2] = {&GPIOA->ODR, &GPIOA->AFR[0]};
InputPinHandler A{a};


int main(void)
{
    //A.setParam<InputPinProperties::pinState>(true);
    A.getParam<InputPinProperties::pinState>();
    while (1)
    {
        /* code */
    }
    
    return 0;
}


void SystemInit(void)
{
    
}
    