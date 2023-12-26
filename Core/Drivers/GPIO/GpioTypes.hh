#ifndef __GPIOTYPES_H__
#define __GPIOTYPES_H__

#include <cstddef>
namespace GpioTypes{

    enum class GpioPorts : std::size_t { GpioA, GpioB, GpioC };

    enum class PinState : bool { low, high };
};

#endif // __GPIOTYPES_H__