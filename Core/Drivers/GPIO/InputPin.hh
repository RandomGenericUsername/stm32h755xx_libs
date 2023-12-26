#ifndef __INPUTPIN_H__
#define __INPUTPIN_H__

//<------------------------------INCLUDES------------------------------>//
#include <IPin.hh>
#include <type_traits>
#include <GpioTypes.hh>
#include <PeripheralBaseHandler.hh>
//<-------------------------------------------------------------------->//

enum class InputPinProperties { pinState, otherProp };
using pinStatePair = pair<InputPinProperties::pinState, bool>;
using otherPropPair = pair<InputPinProperties::otherProp, bool>;
using InputPinSettings = Utils::TypeList<pinStatePair, otherPropPair>;


template<typename RegisterAddresses>
requires Utils::IsTypeList<RegisterAddresses>
class InputPinHandler : public PeripheralHandlerBase< InputPinSettings, RegisterAddresses, InputPinHandler<RegisterAddresses>>{};

template<template <typename...>class TypeList, typename... RegisterAddress>
requires Utils::IsTypeList<TypeList<RegisterAddress...>>
class InputPinHandler<TypeList<RegisterAddress...>> : public PeripheralHandlerBase< InputPinSettings, TypeList<RegisterAddress...>, InputPinHandler<TypeList<RegisterAddress...>> >
{
    private:
        void temp() { }
    public:
        constexpr explicit InputPinHandler(const RegisterAddress&... addresses) :PeripheralHandlerBase< InputPinSettings, TypeList<RegisterAddress...>, InputPinHandler<TypeList<RegisterAddress...>> >(addresses...) { }

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

public:
    InputPin() {}
    ~InputPin() {}

    void init() override {}
    void reset() override {}
    bool read() override { return true; }
};

#endif // __INPUTPIN_H__