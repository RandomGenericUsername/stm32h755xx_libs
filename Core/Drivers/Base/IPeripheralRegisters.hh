#ifndef __IPERIPHERALREGISTERS_H__
#define __IPERIPHERALREGISTERS_H__


//<------------------------------INCLUDES------------------------------>//
#include <Utils.hh> //
#include <ClassMembersWithTagHandler.hh>
#include <Register.hh>
//<-------------------------------------------------------------------->//

// Helper function to transform raw address to IRegister instance
template <typename T>
requires Utils::UnsignedIntegralPointerConcept<T>
IRegister<T>* createRegisterInstance(T address) {
    return SRegister<T>::getInstance(address);
}


// Metafunction to transform a single pair
template<typename Pair>
struct TransformPair;

template<auto EnumValue, typename RawPointerType>
requires ((Utils::UnsignedIntegralPointerConcept<RawPointerType>))
struct TransformPair<pair<EnumValue, RawPointerType>> {
    using type = pair<EnumValue, IRegister<RawPointerType>*>;
};

// Apply the transformation to each type in the TypeList
template<typename TypeList>
struct TransformPeripheralRegistersPairs;

template<template<typename...> class TypeList, typename... Pairs>
struct TransformPeripheralRegistersPairs<TypeList<Pairs...>> {
    using type = TypeList<typename TransformPair<Pairs>::type...>;
};


// Your IPeripheralRegisters class
template<typename PeripheralRegistersPairs>
requires ((Utils::IsTypeListOfPairs<PeripheralRegistersPairs>))
class IPeripheralRegisters {
    private:

        TypeListClassMembersWithTags<
            typename TransformPeripheralRegistersPairs<PeripheralRegistersPairs>::type
        > registers;

    public:
        // Constructor
        template<typename... Addresses>
        constexpr explicit IPeripheralRegisters(Addresses&&... addresses)
            : registers(createRegisterInstance(addresses)...) {
        }

        template<auto T>
        constexpr auto get() const { return registers.template get<T>()->get(); }

        template<auto T>
        constexpr void set(auto&& value) { registers.template get<T>()->set(value); }

        template<auto T>
        constexpr void clear() { registers.template get<T>()->clear(); }

        template<auto T>
        constexpr bool checkBit(const std::size_t& position) const { return registers.template get<T>()->checkBit(position); }

        template<auto T>
        constexpr bool checkBits(const std::size_t& bitsMask, const std::size_t& position = 0) const { return registers.template get<T>()->checkBits(bitsMask, position); }

        template<auto T>
        constexpr void setBit(const std::size_t& position) { return registers.template get<T>()->setBit(position); }

        template<auto T>
        constexpr void clearBit(const std::size_t& position) { return registers.template get<T>()->clearBit(position); }

        template<auto T>
        constexpr void setBits(const std::size_t& bitsMask, const std::size_t& position = 0) { registers.template get<T>()->setBits(bitsMask, position); }

        template<auto T>
        constexpr std::size_t const getLowestIndex() { return registers.template get<T>()->getLowestIndex(); }

        template<auto T>
        constexpr std::size_t const getHighestIndex() { return registers.template get<T>()->getHighestIndex(); }

        template<auto T>
        constexpr auto getAddress() const { return registers.template get<T>()->getAddress();}

        // Other methods and functionality...
};

#endif // __IPERIPHERALREGISTERS_H__