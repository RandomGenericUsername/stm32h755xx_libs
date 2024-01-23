#ifndef __GPIOTYPES_H__
#define __GPIOTYPES_H__

#include <concepts>
#include <Utils.hh>
#include <PeripheralBaseHandler.hh>

#include <cstddef>
namespace GpioTypes{

    enum class GpioPorts : std::size_t { GpioA, GpioB, GpioC, GpioD, GpioE, GpioF, GpioG, GpioH };
    enum class PinState : bool { low, high };
    enum class PinModes : std::size_t { input, output, alternateFunction };

    enum class IPinProperties { pinNumber, mode };
    using IPinHandlerPinNumberPair = pair<IPinProperties::pinNumber, std::size_t>;
    using IPinHandlerPinModePair = pair<IPinProperties::mode, PinModes>;
    using IPinHandlerPropertiesTypeList = Utils::TypeList<IPinHandlerPinNumberPair, IPinHandlerPinModePair>;

    template<typename Tp>
    requires((Utils::UnsignedIntegralPointerConcept<Tp>))
    using IPinHandlerModeRegisterPair = pair<IPinProperties::mode, Tp>;

    template<typename Tp>
    requires((Utils::UnsignedIntegralPointerConcept<Tp>))
    using IPinHandlerRegistersTypeList = Utils::TypeList<IPinHandlerModeRegisterPair<Tp>>;


//
//    enum class InputPinProperties { pinState };
//
//    template<typename PinHandler>
//    using IPinHandlerParentt = PeripheralHandlerBase<IPinHandlerPropertiesTypeList, IPinHandlerRegistersTypeList, PinHandler>;
//    
//    template<typename PinHandler, typename ExtendedPeripheralPropertiesPairs, typename ExtendedRegisterAddressesPairs>
//    using IPinHandlerParent = typename IPinHandlerParentt<PinHandler>::template ExtendedClass_t<ExtendedPeripheralPropertiesPairs, ExtendedRegisterAddressesPairs>;
//
//
//
//
//    using PinStatePair = pair<InputPinProperties::pinState, bool>;
//    using InputPinPropertiesTypeList = Utils::TypeList<PinStatePair>;
//    using InputPinPinStateRegisterPair = pair<InputPinProperties::pinState, volatile uint32_t*>;
//    using InputPinRegistersTypeList = Utils::TypeList<InputPinPinStateRegisterPair>;
};

#endif // __GPIOTYPES_H__