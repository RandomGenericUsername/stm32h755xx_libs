#ifndef __IINPUTPIN_H__
#define __IINPUTPIN_H__


//<------------------------------INCLUDES------------------------------>//
#include <GpioTypes.hh>
#include <PeripheralBaseHandler.hh>
//<-------------------------------------------------------------------->//


#ifdef A
template<typename InputPinHandler>
using InputPinHandlerParent = IPinHandler<InputPinHandler, InputPinPropertiesTypeList, InputPinRegistersTypeList> ;

class InputPinHandler : public InputPinHandlerParent<InputPinHandler>
{
    private:
    public:
        template<typename ...RegisterAddress>
        requires((Utils::UnsignedIntegralPointerConcept<RegisterAddress> && ...))
        constexpr explicit InputPinHandler(RegisterAddress&&... addresses) 
            : InputPinHandlerParent<InputPinHandler>(std::forward<RegisterAddress>(addresses)...) {
                setParam<IPinHandlerProperties::mode>(IPinModes::input);
        }
};

template <std::size_t PinNumber>
class IInputPin : public IPin<PinNumber>
{
public:
    virtual bool read() = 0;
};

#endif // __IINPUTPIN_H__
#endif // __IINPUTPIN_H__