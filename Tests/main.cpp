#include <iostream>
#include <InputPin.hh>
#define CORE_CM7
#include <stm32h755xx.h>
#include <ClassMembersWithTagHandler.hh>

using namespace std::string_literals;

using GpioStateRegisterPair = pair<InputPinProperties::pinState, volatile unsigned int*>;
using GpioOtherPropRegisterPair = pair<InputPinProperties::otherProp, volatile unsigned int*>;
GpioStateRegisterPair a{&(GPIOA->ODR)};
GpioOtherPropRegisterPair b{&(GPIOA->AFR[0])};
using GpioSettings = Utils::TypeList<GpioStateRegisterPair, GpioOtherPropRegisterPair>;
InputPinHandler<GpioSettings> A{a, b};



int main(void)
{
    return 0;
}

#ifdef compile


enum class GpioProps { clockFrequency, outputState, container, instances, objects };


using ClockFreqPair = pair<GpioProps::clockFrequency, int>;
using OutputStatePair = pair<GpioProps::outputState, bool>;
using ContainerPair32 = pair<GpioProps::container, Cont<uint32_t>>;


Cont<uint32_t> a;
Cont<uint32_t> b{616};
classMembersWithTags<ClockFreqPair, OutputStatePair> AB{32, false};
classMembersWithTags<ClockFreqPair, OutputStatePair, ContainerPair32> AV{32, false, b};

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