#ifndef __INPUTPIN_H__
#define __INPUTPIN_H__

//<------------------------------INCLUDES------------------------------>//
#include <IPin.hh>
#include <type_traits>
#include <GpioTypes.hh>
#include <PeripheralBaseHandler.hh>
#include <cstddef>
//<-------------------------------------------------------------------->//


enum class InputPinProperties { pinState };
using PinStatePair = pair<InputPinProperties::pinState, bool>;
using InputPinPropertiesTypeList = Utils::TypeList<PinStatePair>;
using InputPinPinStateRegisterPair = pair<InputPinProperties::pinState, volatile uint32_t*>;
using InputPinRegistersTypeList = Utils::TypeList<InputPinPinStateRegisterPair>;
//using InputPinHandlerParent = IPinHandler::ExtendedClass_t<InputPinPropertiesTypeList, InputPinRegistersTypeList>;

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
                setMode();
        }
};


template <std::size_t PinNumber>
class IInputPin : public IPin<PinNumber>
{
public:
    virtual bool read() = 0;
};

template <std::size_t PinNumber>
class InputPin : public IInputPin<PinNumber>
{
private:
    InputPinHandler _handler;

public:
    template<typename ModeRegister, typename StateRegister>
    requires((Utils::UnsignedIntegralPointerConcept<ModeRegister> && Utils::UnsignedIntegralPointerConcept<StateRegister>))
    explicit InputPin(ModeRegister&& moder, StateRegister&& idr) : _handler(InputPinHandler{std::forward<ModeRegister>(moder), std::forward<StateRegister>(idr)}) {
        _handler.setParam<IPinHandlerProperties::pinNumber>(PinNumber);
    }
    ~InputPin() {}

    void init() override {}
    void reset() override {}
    bool read() override {
        //return _handler.checkBit<InputPinProperties::pinState>(PinNumber); 
        return true;
    }
};

#ifdef COMPIE

#endif


#endif // __INPUTPIN_H__