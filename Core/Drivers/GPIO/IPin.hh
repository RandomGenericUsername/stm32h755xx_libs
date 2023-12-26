#ifndef __IPIN_H__
#define __IPIN_H__

//<------------------------------INCLUDES------------------------------>//
#include <cstddef>
#include <IPeripheral.hh>
//<-------------------------------------------------------------------->//

template <std::size_t PinNumber>
class IPin : IPeripheral
{
private:
protected:
    const std::size_t pinNumber{PinNumber};

public:
    constexpr IPin() = default;
    constexpr ~IPin() override = default;
};

#endif // __IPIN_H__