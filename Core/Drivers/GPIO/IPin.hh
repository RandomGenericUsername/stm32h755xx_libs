#ifndef __IPIN_H__
#define __IPIN_H__

//<------------------------------INCLUDES------------------------------>//
#include <cstddef>
#include <IPeripheral.hh>
#include <PeripheralBaseHandler.hh>
//<-------------------------------------------------------------------->//


template <std::size_t PinNumber>
class IPin : IPeripheral
{
private:
protected:
    const std::size_t pinNumber{PinNumber};

public:
    //constexpr IPin() = default;
    //constexpr ~IPin() override = default;
};


enum class IPinModes { input, output, alternateFunction };
enum class IPinHandlerProperties { pinNumber, mode };
using PinNumberPair = pair<IPinHandlerProperties::pinNumber, uint32_t>;
using PinModePair = pair<IPinHandlerProperties::mode, IPinModes>;
using IPinHandlerPropertiesTypeList = Utils::TypeList<PinNumberPair, PinModePair>;
using IPinHandlerModeRegisterPair = pair<IPinHandlerProperties::mode, volatile uint32_t*>;
using IPinHandlerRegistersTypeList = Utils::TypeList<IPinHandlerModeRegisterPair>;


template<typename PinHandler>
using IPinHandlerParentt = PeripheralHandlerBase<IPinHandlerPropertiesTypeList, IPinHandlerRegistersTypeList, PinHandler>;

template<typename PinHandler, typename ExtendedPeripheralPropertiesPairs, typename ExtendedRegisterAddressesPairs>
using IPinHandlerParent = typename IPinHandlerParentt<PinHandler>::template ExtendedClass_t<ExtendedPeripheralPropertiesPairs, ExtendedRegisterAddressesPairs>;

template<typename PinHandler, typename ExtendedPeripheralPropertiesPairs, typename ExtendedRegisterAddressesPairs>
class IPinHandler : public IPinHandlerParent<PinHandler, ExtendedPeripheralPropertiesPairs, ExtendedRegisterAddressesPairs>
{

    protected:

    public:
        template<typename ...RegisterAddress>
        requires((Utils::UnsignedIntegralPointerConcept<RegisterAddress> && ...))
        constexpr explicit IPinHandler(RegisterAddress&&... addresses) 
            : IPinHandlerParent<PinHandler, ExtendedPeripheralPropertiesPairs, ExtendedRegisterAddressesPairs>(std::forward<RegisterAddress>(addresses)...) { }

        void setMode() { 
            
            auto mode { static_cast<std::size_t>(this->template getParam<IPinHandlerProperties::mode>()) };
            auto pinNumber { static_cast<std::size_t>(this->template getParam<IPinHandlerProperties::pinNumber>()) };
            IPinHandlerParent<PinHandler, ExtendedPeripheralPropertiesPairs, ExtendedRegisterAddressesPairs>::template setBits<IPinHandlerProperties::mode>(mode, pinNumber);
        }
};


#endif // __IPIN_H__