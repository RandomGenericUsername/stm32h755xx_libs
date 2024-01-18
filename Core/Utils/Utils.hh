/**
 * @file Utils.h
 * @brief Utility constructs for type manipulation and checks in C++.
 *
 * This header file provides a range of utility constructs such as concepts, type lists,
 * and compile-time checks and operations useful for metaprogramming in C++.
 */

#ifndef __UTILS_H__
#define __UTILS_H__

//<------------------------------INCLUDES------------------------------>//
#include <concepts>
#include <type_traits>
#include <tuple>
//<-------------------------------------------------------------------->//

namespace Utils
{
    template<typename Derived, typename Base>
    concept IsDerivedFrom = std::is_base_of<Base, Derived>::value && std::is_convertible<const volatile Derived*, const volatile Base*>::value;


    //<-------------------------------------------------------------------->//
    template <typename T>
    concept UnsignedIntegralPointerConcept = 
        (std::is_pointer_v<T> && std::is_unsigned_v<std::remove_pointer_t<std::remove_cv_t<T>>> && std::is_integral_v<std::remove_pointer_t<std::remove_cv_t<T>>>);
    //<-------------------------------------------------------------------->//

    //<-------------------------------------------------------------------->//
    template <typename T>
    concept ArrayOfUnsignedIntegralPointersConcept = 
        std::is_array_v<T> && UnsignedIntegralPointerConcept<std::remove_extent_t<T>>;

    //<-------------------------------------------------------------------->//
 
    //<-------------------------------------------------------------------->//

    //<-------------------------------------------------------------------->//
    // Helper structure to find the index of an enum value in a type list.
    template <auto EnumValue, typename... Pairs>
    struct indexOfEnumValue;

    template <auto EnumValue, std::size_t Index, typename... Types>
    struct indexOfEnumValueHelper;

    // Primary template
    template <auto EnumValue, std::size_t Index, typename First, typename... Rest>
    struct indexOfEnumValueHelper<EnumValue, Index, First, Rest...> {
        static constexpr std::size_t value = std::is_same_v<typename First::tag, std::integral_constant<decltype(EnumValue), EnumValue>>
            ? Index
            : indexOfEnumValueHelper<EnumValue, Index + 1, Rest...>::value;
    };

    // Specialization for the termination case
    template <auto EnumValue, std::size_t Index>
    struct indexOfEnumValueHelper<EnumValue, Index> 
    { 
        static constexpr std::size_t value = Index; // Fallback index if not found.
    };

    template <auto EnumValue, typename... Pairs>
    struct indexOfEnumValue {
        static constexpr std::size_t value = indexOfEnumValueHelper<EnumValue, 0, Pairs...>::value;
    };  

    //<-------------------------------------------------------------------->//
    // Concept to check if a given enum value is in the list of pairs
    template<auto EnumValue, typename... Pairs>
    concept EnumInPairs = ((std::is_same_v<typename std::integral_constant<decltype(EnumValue), EnumValue>, typename Pairs::tag>) || ...);

    //<-------------------------------------------------------------------->//
    /**
     * @brief Concept to check if a type is an enumeration.
     *
     * This concept validates if the specified type is an enum type. It is primarily
     * used to ensure type safety and specialization for enum types in templates.
     *
     * Example usage:
     * ```
     * template<typename T>
     * requires IsEnumConcept<T>
     * void processEnum(T enumValue) {
     *     // Implementation specific to enum types
     * }
     * ```
     *
     * @tparam Tp The type to check.
     */
    template <typename Tp>
    concept IsEnumConcept = std::is_enum_v<Tp>;
    //<-------------------------------------------------------------------->//

    //<-------------------------------------------------------------------->//
    /**
     * @brief A compile-time container for holding a list of types.
     *
     * This struct is a part of metaprogramming utilities and acts as a type-safe container for an arbitrary number of types.
     * It is primarily used in template metaprogramming to manipulate and interact with lists of types, allowing for compile-time
     * type checks and operations.
     *
     * Usage example:
     * ```
     * using MyTypeList = Utils::TypeList<int, float, double>;
     * ```
     *
     * @tparam Types A variadic template parameter pack representing the types to be included in the type list.
     */
    template <typename... Types>
    struct TypeList
    {
        /**
         * @brief Marker type alias for identifying a TypeList.
         *
         * This alias is used in metaprogramming techniques, especially in concepts, to determine if a given type is a TypeList.
         * It is a form of self-referencing alias intended for compile-time checks.
         */
        using TypeListIdentifier = TypeList<Types...>;
    };


    template <typename T>
    struct TypeListSize;
    
    template <typename... Types>
    struct TypeListSize<TypeList<Types...>> {
        static constexpr size_t value = sizeof...(Types);
    };

    /**
     * @brief A concept that checks if a type is a TypeList.
     *
     * This concept is used to constrain template parameters to be of TypeList type. It is especially useful
     * in templates where operations are specific to TypeList and its semantics. The concept uses the presence
     * of TypeListIdentifier within a type as a condition for the type to be considered a TypeList.
     *
     * Usage example:
     * ```
     * template <typename T>
     * requires Utils::IsTypeList<T>
     * void myFunction() {
     *     // Function implementation
     * }
     * ```
     *
     * @tparam T The type to be checked against the TypeList concept.
     */
    template <typename T>
    concept IsTypeList = requires { typename T::TypeListIdentifier; };

    // Concept to check if a type is a pair
    template<typename T, template <auto, typename> class PairType>
    concept IsPair = requires {
        typename T::tag;   // Ensure T has a nested 'tag' type
        typename T::type;  // Ensure T has a nested 'type' type
        requires std::is_same_v<T, PairType<T::tag::value, typename T::type>>;
    };

    // Helper to extract types from TypeList
    template <typename T>
    struct ExtractTypesFromTypeList;

    template <template <typename...> class TypeList, typename... _Types>
    struct ExtractTypesFromTypeList<TypeList<_Types...>> {
        // Using a type alias for the parameter pack itself
        using Types = TypeList<_Types...>;
    };

    template <typename T>
    struct AreAllPairs;

    template <template <auto, typename> class PairType, auto... EnumValues, typename... Types>
    struct AreAllPairs<TypeList<PairType<EnumValues, Types>...>> {
        static constexpr bool value = (... && IsPair<PairType<EnumValues, Types>, PairType>);
    };

    // Concept to check if a type is a TypeList of pairs.
    template <typename T>
    concept IsTypeListOfPairs = requires {
        typename T::TypeListIdentifier; // Ensure T is a TypeList
        //typename ExtractTypesFromTypeList<T>::Types; // Extract types from TypeList
        requires AreAllPairs<typename ExtractTypesFromTypeList<T>::Types>::value; // Check if all types in TypeList are pairs
    };


    //<-------------------------------------------------------------------->//

    //<-------------------------------------------------------------------->//
    /**
     * @brief Checks if a type is in a TypeList.
     *
     * Utilizes template metaprogramming to determine if a specific type is present within a TypeList.
     *
     * Example usage:
     * ```
     * constexpr bool isInList = IsInTypeList<int, TypeList<float, int, double>>::value; // true
     * ```
     *
     * @tparam T The type to search for.
     * @tparam TypeList The list of types to search within.
     */
    template <typename T, typename TypeList>
    struct IsInTypeList : std::false_type
    {
        // Base case: type not found.
    };

    // Recursive case: Check the head and then the tail.
    template <typename T, typename Head, typename... Tail>
    struct IsInTypeList<T, TypeList<Head, Tail...>>
        : std::conditional<std::is_same<T, Head>::value, std::true_type, IsInTypeList<T, TypeList<Tail...>>>::type
    {
    };
    //<-------------------------------------------------------------------->//


    //<-------------------------------------------------------------------->//
    /**
     * @brief Checks if a type is in a std::tuple.
     *
     * This struct recursively checks if a specific type is part of the given std::tuple.
     *
     * Example usage:
     * ```
     * constexpr bool isInTuple = isTypeInTuple<int, std::tuple<float, int, double>>::value; // true
     * ```
     *
     * @tparam T The type to search for.
     * @tparam Tuple The tuple to be searched.
     */
    template <typename T, typename Tuple>
    struct isTypeInTuple;

    template <typename T>
    struct isTypeInTuple<T, std::tuple<>> : std::false_type
    {
    };

    template <typename T, typename U, typename... Rest>
    struct isTypeInTuple<T, std::tuple<U, Rest...>>
        : std::conditional_t<std::is_same<T, U>::value, std::true_type, isTypeInTuple<T, std::tuple<Rest...>>>
    {
    };
    //<-------------------------------------------------------------------->//


    //<-------------------------------------------------------------------->//
    /**
     * @brief Finds the index of a type in a tuple.
     *
     * Computes the zero-based index of a given type within a tuple, facilitating access to tuple elements
     * based on type.
     *
     * Example usage:
     * ```
     * constexpr size_t index = indexOfInTuple<double, std::tuple<int, float, double>>::value; // 2
     * ```
     *
     * @tparam T The type to find.
     * @tparam Tuple The tuple to search within.
     */
    template <typename T, typename Tuple>
    struct indexOfInTuple;

    template <typename T, typename... Types>
    struct indexOfInTuple<T, std::tuple<T, Types...>> : std::integral_constant<std::size_t, 0>
    {
    };

    template <typename T, typename U, typename... Types>
    struct indexOfInTuple<T, std::tuple<U, Types...>> : std::integral_constant<std::size_t, 1 + indexOfInTuple<T, std::tuple<Types...>>::value>
    {
    };
    //<-------------------------------------------------------------------->//

    //<-------------------------------------------------------------------->//

    /**
     * @brief Finds the index of a type in a TypeList.
     *
     * Computes the zero-based index of a given type within a TypeList, facilitating access to TypeList elements
     * based on type.
     *
     * Example usage:
     * ```
     * constexpr size_t index = indexOfTypeInTypeList<double, TypeList<int, float, double>>::value; // 2
     * ```
     *
     * @tparam T The type to find.
     * @tparam TypeList The TypeList to search within.
     */
    template <typename T, typename TypeList>
    struct indexOfTypeInTypeList;  // Forward declaration
    

    // Base case: the type is not found in the list.
    template <typename T>
    struct indexOfTypeInTypeList<T, TypeList<>> : std::integral_constant<std::size_t, 0>
    {
    };

    // Recursive case: Check the head and then the tail.
    template <typename T, typename Head, typename... Tail>
    struct indexOfTypeInTypeList<T, TypeList<Head, Tail...>> : std::integral_constant<std::size_t, std::is_same<T, Head>::value ? 0 : 1 + indexOfTypeInTypeList<T, TypeList<Tail...>>::value>
    {
    };

    //<-------------------------------------------------------------------->//
     /**
     * @brief Checks if a type is in a set of types.
     *
     * Utilizes template metaprogramming to determine if a given type is part of a set of types.
     *
     * Example usage:
     * ```
     * constexpr bool isIntInList = isTypeIn<int, double, char, int>::value; // true
     * ```
     *
     * @tparam T The type to search for.
     * @tparam Args The set of types to search within.
     */
    template <typename T, typename... Args>
    struct isTypeIn : std::false_type
    {
    };

    template <typename T, typename... Rest>
    struct isTypeIn<T, T, Rest...> : std::true_type
    {
    };

    template <typename T, typename U, typename... Rest>
    struct isTypeIn<T, U, Rest...> : isTypeIn<T, Rest...>
    {
    };
    //<-------------------------------------------------------------------->//

    //<-------------------------------------------------------------------->//
    template<typename... Lists>
    struct TypeListConcat;

    template<typename... Types1, typename... Types2>
    struct TypeListConcat<Utils::TypeList<Types1...>, Utils::TypeList<Types2...>> {
        using type = Utils::TypeList<Types1..., Types2...>;
    };

    template<typename... Lists>
    using ConcatenateTypeList = typename TypeListConcat<Lists...>::type;
    //<-------------------------------------------------------------------->//

    template<typename EnumProp>
    requires((IsEnumConcept<EnumProp>) && std::is_convertible_v<EnumProp, std::size_t>)
    std::size_t CastEnum(const EnumProp& prop){ return static_cast<std::size_t>(prop); }

};

#endif // __UTILS_H__