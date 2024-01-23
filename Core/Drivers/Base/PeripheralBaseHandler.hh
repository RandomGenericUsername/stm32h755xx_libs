#ifndef __PERIPHERALBASEHANDLER_H__
#define __PERIPHERALBASEHANDLER_H__

//<------------------------------INCLUDES------------------------------>//
#include <Utils.hh> //
#include <ClassMembersWithTagHandler.hh>
#include <Register.hh>
#include <IPeripheralProperties.hh>
#include <IPeripheralRegisters.hh>
//<-------------------------------------------------------------------->//



template<typename PeripheralPropertiesPairs, typename RegisterAddressesPairs, typename PeripheralHandler>
requires (
    (Utils::IsTypeListOfPairs<PeripheralPropertiesPairs>) && 
    (Utils::IsTypeListOfPairs<RegisterAddressesPairs>)
)
class PeripheralHandlerBase 
{
private:

    PeripheralHandler* peripheralHandler{ nullptr };
    IPeripheralProperties <PeripheralPropertiesPairs> members;
    IPeripheralRegisters<RegisterAddressesPairs> registers;
    
protected:

public:

    // Make all instantiations of PeripheralHandlerBase friends of each other
    template<typename, typename, typename>
    friend class PeripheralHandlerBase;

    template<typename... RegisterAddressess>
    requires ((Utils::UnsignedIntegralPointerConcept<RegisterAddressess> && ...))
    constexpr explicit PeripheralHandlerBase(PeripheralHandlerBase* handler, RegisterAddressess&&... registerAddresses) 
        : peripheralHandler(handler), registers(registerAddresses...) {
    }


    ~PeripheralHandlerBase() { peripheralHandler = nullptr; }

    template <auto EnumValue>
    constexpr auto& getParam() {
        return this->members.template get<EnumValue>();
    }

    template <auto EnumValue, typename ValueType>
    constexpr void setParam(ValueType&& v) {
        this->members.template set<EnumValue, ValueType>(std::forward<ValueType>(v));
    }

    template<auto T>
    constexpr auto getRegisterValue() const { return registers.template get<T>(); }

    template<auto T>
    constexpr void setRegisterValue(auto&& value) { registers.template set<T>(value); }

    template<auto T>
    constexpr void clearRegisterValue() { registers.template clear<T>(); }

    template<auto T>
    constexpr bool checkBit(const std::size_t& position) const { return registers.template checkBit<T>(position); }

    template<auto T>
    constexpr bool checkBits(const std::size_t& bitsMask, const std::size_t& position = 0) const { return registers.template checkBits<T>(bitsMask, position); }

    template<auto T>
    constexpr void setBit(const std::size_t& position) { return registers.template setBit<T>(position); }

    template<auto T>
    constexpr void clearBit(const std::size_t& position) { return registers.template clearBit<T>(position); }

    template<auto T>
    constexpr void setBits(const std::size_t& bitsMask, const std::size_t& position = 0) { registers.template setBits<T>(bitsMask, position); }

    template<auto T>
    constexpr std::size_t const getLowestIndex() { return registers.template getLowestIndex<T>(); }

    template<auto T>
    constexpr std::size_t const getHighestIndex() { return registers.template getHighestIndex<T>(); }

    template<auto T>
    constexpr auto getAddress() const { return registers.template getAddress<T>();}


    //Actually i want something like this:
    template<typename ExtendedPeripheralPropertiesPairs, typename ExtendedRegisterAddressesPairs>
    using ExtendedClass_t = PeripheralHandlerBase<
        Utils::ConcatenateTypeList<PeripheralPropertiesPairs, ExtendedPeripheralPropertiesPairs>, 
        Utils::ConcatenateTypeList<RegisterAddressesPairs, ExtendedRegisterAddressesPairs>,
        PeripheralHandler
    >;

    template<typename ExtendedPeripheralPropertiesPairs, typename ExtendedRegisterAddressesPairs, typename... T>
    requires (Utils::UnsignedIntegralPointerConcept<T> && ...)
    static constexpr ExtendedClass_t<ExtendedPeripheralPropertiesPairs,ExtendedRegisterAddressesPairs> getExtendedInstance(T&&... registerAddresses) { return ExtendedClass_t<ExtendedPeripheralPropertiesPairs,ExtendedRegisterAddressesPairs>{registerAddresses...}; }
};


#endif // __PERIPHERALBASEHANDLER_H__