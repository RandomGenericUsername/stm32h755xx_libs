#ifndef __CLASSMEMBERSHANDLER_H__
#define __CLASSMEMBERSHANDLER_H__

/**
 * @file ClassMembersHandler.hh
 * @brief Defines a template class for managing a collection of members with various types.
 *
 * This class template is specialized to handle a collection of member variables of different types,
 * encapsulated within a tuple. It provides methods for setting and getting these member variables efficiently,
 * supporting perfect forwarding to optimize for lvalue and rvalue references.
 *
 */

//<------------------------------INCLUDES------------------------------>//
#include <Utils.hh>
#include <utility> // For std::forward
#include <tuple>   // For std::tuple
#include <type_traits>
//<-------------------------------------------------------------------->//

/**
 * @brief Defines a template class for managing a collection of members with various types.
 *
 * This class template is specialized to handle a collection of member variables of different types,
 * encapsulated within a tuple. It provides methods for setting and getting these member variables efficiently,
 * supporting perfect forwarding to optimize for lvalue and rvalue references.
 *
 * @tparam TypeList A Utils::TypeList containing the types of the members.
 */

template <typename TypeList>
    requires Utils::IsTypeList<TypeList>
class ClassMembers;

/**
 * @brief Specialization of ClassMembers for a specific set of member types.
 *
 * This specialization handles a collection of members where each member's type
 * is specified in the TypeList. It utilizes a std::tuple to store the members
 * and provides methods to access and modify them.
 *
 * @tparam MemberTypes The types of the members included in the class.
 */
template <typename... MemberTypes>
class ClassMembers<Utils::TypeList<MemberTypes...>>
{
private:
    std::tuple<MemberTypes...> members; ///< Tuple storing the member values.

    /**
     * @brief Retrieves a member by its type using compile-time index lookup.
     *
     * This function uses the index of the type in the MemberTypes pack to access the corresponding
     * member in the tuple. It is a compile-time operation, ensuring efficient access to members.
     *
     * @tparam T The type of the member to be accessed.
     * @tparam Is Index sequence for unpacking the tuple.
     * @return A reference to the member of type T.
     *
     * Example:
     * @code{.cpp}
     * ClassMembers<Utils::TypeList<int, std::string>> members(42, "Hello");
     * auto& intMember = members.getMemberByType<int>(std::index_sequence_for<int, std::string>{});
     * @endcode
     */
    template <typename T, std::size_t... Is>
    constexpr T &getMemberByType(std::index_sequence<Is...>)
    {
        return std::get<Utils::indexOfTypeInTypeList<T, Utils::TypeList<MemberTypes...>>::value>(members);
    }

    /**
     * @brief Updates a member of the specified type with the given value.
     *
     * This function checks if the type T is present in MemberTypes and, if so, updates the corresponding
     * member in the tuple. It uses perfect forwarding to efficiently handle both lvalues and rvalues.
     *
     * @tparam T The type of the member to be updated.
     * @param value The value to set the member to.
     *
     * Example:
     * @code{.cpp}
     * ClassMembers<Utils::TypeList<int, std::string>> members(42, "Initial");
     * members.updateMember(std::string("Updated"));
     * auto& stringMember = members.get<std::string>();
     * // stringMember now holds the value "Updated"
     * @endcode
     */

    template <typename T, bool STRICT = true>
    constexpr void updateMember(T &&value)
    {
        using DecayedType = std::decay_t<T>;
        constexpr bool isType{ Utils::isTypeIn<DecayedType, MemberTypes...>::value };
        static_assert(isType || !STRICT, "[INVALID TYPE]: Updating invalid member on 'set' method @ 'ClassMemberHandler' class");
        if constexpr(isType)
            get<DecayedType>() = std::forward<T>(value);
    }



public:
    /**
     * @brief Default constructor for ClassMembers.
     *
     * Initializes an instance of ClassMembers with default-constructed members.
     */
    constexpr explicit ClassMembers() {}

    /**
     * @brief Constructor that initializes members with given values.
     *
     * This constructor accepts a list of values corresponding to the types in MemberTypes.
     * It uses perfect forwarding to efficiently initialize the members, supporting both lvalues and rvalues.
     *
     * @tparam Args Parameter pack corresponding to the types in MemberTypes.
     * @param args The values to initialize the members with.
     *
     * Example:
     * @code{.cpp}
     * ClassMembers<Utils::TypeList<int, std::string>> members(42, "Hello");
     * @endcode
     */
    // SFINAE-enabled constructor
    template<typename... Args, typename = std::enable_if_t<std::conjunction_v<std::is_same<MemberTypes, std::decay_t<Args>>...>>>
    constexpr explicit ClassMembers(Args&&... args) 
        : members(std::forward<Args>(args)...) {}
    //constexpr explicit ClassMembers(MemberTypes &&...members) : members(std::forward<MemberTypes>(members)...) {}

    /**
     * @brief Accesses a member by its type.
     *
     * This function provides type-safe access to members of the tuple. It statically asserts
     * if the requested type is not in MemberTypes, ensuring correct usage.
     *
     * @tparam T The type of the member to access.
     * @return A reference to the member of type T.
     *
     * Example:
     * @code{.cpp}
     * auto& intValue = members.get<int>();
     * @endcode
     */
    template <typename T>
    constexpr T &get()
    {
        static_assert(Utils::isTypeIn<T, MemberTypes...>::value, "[INVALID TYPE]: Invalid type on 'get' method @ 'ClassMembersHandler' ");
        return getMemberByType<T>(std::index_sequence_for<MemberTypes...>{});
    }

    /**
     * @brief Sets one or more members by passing their values.
     *
     * This function allows setting multiple members at once. It iterates over each argument and updates the corresponding
     * member. This method is particularly efficient with rvalues, as it utilizes perfect forwarding.
     *
     * @tparam T The types of the values to set.
     * @param values The values to set the members to.
     *
     * Example:
     * @code{.cpp}
     * members.set(25, "World");
     * @endcode
     */
    template <typename... T>
    constexpr void set(T &&...values)
    {
        (updateMember(std::forward<T>(values)), ...);
    }
};

#endif // __CLASSMEMBERSHANDLER_H__
