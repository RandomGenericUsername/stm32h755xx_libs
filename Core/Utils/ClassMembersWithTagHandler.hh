#ifndef __CLASSMEMBERSWITHTAGHANDLER_H__
#define __CLASSMEMBERSWITHTAGHANDLER_H__

#include <type_traits>
#include <Utils.hh>

template<typename T, template <auto, typename> class PairType>
concept IsPair = requires {
    typename T::tag;   // Ensure T has a nested 'tag' type
    typename T::type;  // Ensure T has a nested 'type' type
    requires std::is_same_v<T, PairType<T::tag::value, typename T::type>>;
};


template<auto EnumValue, typename Type>
requires Utils::IsEnumConcept<decltype(EnumValue)>
class pair {
    

    private:
        Type value;
    public:
        pair() = default;
        ~pair() = default;
        template<typename U, typename = std::enable_if_t<std::is_same_v<U, Type>>>
        explicit pair(U&& v) : value(v) { }

        Type get() { return value; }
        template<typename T, typename = std::enable_if<std::is_same_v<T, Type>>>
        void set(T&& v) { this->value = std::move(v); }

        using tag = std::integral_constant<decltype(EnumValue), EnumValue>;
        using type = Type;
};


template<typename... Pairs>
    requires (IsPair<Pairs, pair> && ...)
class classMembersWithTags {
    private:
        std::tuple<typename Pairs::type...> members;
    public:
        constexpr explicit classMembersWithTags() {}

        template<typename... Types>
        constexpr explicit classMembersWithTags(Types&&... values)
        requires ((std::is_same_v<std::decay_t<Types>, typename Pairs::type> || 
               std::is_constructible_v<typename Pairs::type, Types>) && ...)
            : members(std::forward<Types>(values)...) { }

         // Set method
        template <auto EnumValue, typename ValueType>
        void set(ValueType&& value) {
            using Pair = pair<EnumValue, typename std::decay<ValueType>::type>;
            constexpr std::size_t index = Utils::indexOfTypeInTypeList<Pair, Utils::TypeList<Pairs...>>::value;
            std::get<index>(members) = std::forward<ValueType>(value);
        }

        template <auto EnumValue>
        requires Utils::EnumInPairs<EnumValue, Pairs...> 
        constexpr auto& get() {
            constexpr std::size_t index = Utils::indexOfEnumValue<EnumValue, Pairs...>::value;
            return std::get<index>(members);
        }
};

template<typename T>
struct TypeListClassMemberWithTagsConstructor;

template<template<typename...> class TypeList, typename... Types>
struct TypeListClassMemberWithTagsConstructor<TypeList<Types...>> {
    using type = classMembersWithTags<Types...>;
};

template<typename TypeList>
using TypeListClassMembersWithTags = typename TypeListClassMemberWithTagsConstructor<TypeList>::type;


//enum class GpioProps { clockFrequency, outputState, msgObj, instances, objects };
//using ClockFreqPair = pair<GpioProps::clockFrequency, int>;
//using OutputStatePair = pair<GpioProps::outputState, bool>;
//using msgObjPair = pair<GpioProps::msgObj, II>;
//classMembersWithTags<ClockFreqPair, OutputStatePair, msgObjPair> AB;
//classMembersWithTags<ClockFreqPair, OutputStatePair, msgObjPair> AV{32, false, i};

//template<typename T>
//struct Cont
//{
//    public:
//        using t = T;
//        T v;
//        Cont() {}
//        Cont(T&& v) : v(v) {}
//         // Copy constructor
//        Cont(const Cont& other) : v(other.v) {}
//        // Move constructor
//        Cont(Cont&& other) noexcept : v(std::move(other.v)) {}
//        ~Cont(){}
//        //Cont<T>& operator=(T&& other) { v = std::move(other); return *this; }
//        //Cont<T>& operator=(const T& other) { v = other; return *this; }
//        // Move assignment operator
//        Cont& operator=(Cont&& other) noexcept {
//            if (this != &other) {
//                v = std::move(other.v);
//            }
//            return *this;
//        }
//        Cont<T>& operator=(const Cont<T>& other) {
//            if (this != &other) { // Guard against self-assignment
//                v = other.v; 
//            }
//            return *this;
//        }
//};

#endif // __CLASSMEMBERSWITHTAGHANDLER_H__