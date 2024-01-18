#define CORE_CM7
#include <iostream>
#include <InputPin.hh>
#include <PeripheralBaseHandler.hh>
#include <stm32h755xx.h>

using namespace std::string_literals;

volatile uint32_t MODER = 0b11110101101010101100110000001111;
volatile uint32_t ODR = 124;

InputPinHandler fdsa {&MODER, &ODR};

int main(void)
{
    fdsa.getRegisterValue<IPinHandlerProperties::mode>();
    fdsa.clearRegisterValue<IPinHandlerProperties::mode>();
    auto _1 { fdsa.getParam<InputPinProperties::pinState>() };
    auto _2 { fdsa.getParam<IPinHandlerProperties::pinNumber>() };
    
    return 0;
}

#ifdef compile


template<typename T>
struct Cont
{
    public:
        using t = T;
        T v;
        Cont() {}
        Cont(T&& v) : v(v) {}
         // Copy constructor
        Cont(const Cont& other) : v(other.v) {}
        // Move constructor
        Cont(Cont&& other) noexcept : v(std::move(other.v)) {}
        ~Cont(){}
        //Cont<T>& operator=(T&& other) { v = std::move(other); return *this; }
        //Cont<T>& operator=(const T& other) { v = other; return *this; }
        // Move assignment operator
        Cont& operator=(Cont&& other) noexcept {
            if (this != &other) {
                v = std::move(other.v);
            }
            return *this;
        }
        Cont<T>& operator=(const Cont<T>& other) {
            if (this != &other) { // Guard against self-assignment
                v = other.v; 
            }
            return *this;
        }
};

#endif