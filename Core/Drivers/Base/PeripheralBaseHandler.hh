#ifndef __PERIPHERALBASEHANDLER_H__
#define __PERIPHERALBASEHANDLER_H__

//<------------------------------INCLUDES------------------------------>//
#include <Utils.hh> //
#include <ClassMembersWithTagHandler.hh>
#include <Register.hh>
//<-------------------------------------------------------------------->//

template<typename PeripheralPropertiesPairs, typename RegisterAddresses, typename PeripheralHandler>
requires ((Utils::IsTypeList<PeripheralPropertiesPairs>) && (Utils::IsTypeList<RegisterAddresses>))
class PeripheralHandlerBase 
{
private:

    virtual void temp() = 0;

    PeripheralHandler* peripheralHandler;
    const TypeListClassMembersWithTags<PeripheralPropertiesPairs> members;
    const TypeListClassMembersWithTags<RegisterAddresses> regs;
    //Register<RegisterAddress>* instance { nullptr };

public:
    constexpr explicit PeripheralHandlerBase() { }
    template<typename... T>
    constexpr explicit PeripheralHandlerBase(T&&... a) : regs(a...) { }
    //explicit PeripheralHandlerBase(PeripheralHandler* peripheralHandler) : peripheralHandler(peripheralHandler) { }
    ~PeripheralHandlerBase() { peripheralHandler = nullptr; }

    template <auto EnumValue>
    constexpr auto& getParam() {
        return this->members.template get<EnumValue>();
    }

    template <auto EnumValue, typename ValueType>
    constexpr void setParam(ValueType&& v) {
        this->members.template set<EnumValue, ValueType>(std::forward<ValueType>(v));
    }

    
};

#endif // __PERIPHERALBASEHANDLER_H__