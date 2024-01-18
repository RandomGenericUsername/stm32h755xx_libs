#ifndef __IPERIPHERALPROPERTIES_H__
#define __IPERIPHERALPROPERTIES_H__


//<------------------------------INCLUDES------------------------------>//
#include <Utils.hh> //
#include <ClassMembersWithTagHandler.hh>
//<-------------------------------------------------------------------->//


template<typename PeripheralPropertiesPairs>
requires((Utils::IsTypeListOfPairs<PeripheralPropertiesPairs>))
class IPeripheralProperties
{
    private:
        TypeListClassMembersWithTags<PeripheralPropertiesPairs> properties;

        // Pure virtual function making this an abstract class
        //virtual void abstractFunction() = 0;
    public:

        constexpr explicit IPeripheralProperties() { }
        template<typename... T>
        constexpr explicit IPeripheralProperties(T&&... propertiesValues) : properties(propertiesValues...) { }

        template <auto EnumValue>
        constexpr auto& get() const {
            return this->properties.template get<EnumValue>();
        }

        template <auto EnumValue, typename ValueType>
        constexpr void set(ValueType&& v) {
            this->properties.template set<EnumValue, ValueType>(std::forward<ValueType>(v));
        }
};


#endif // __IPERIPHERALPROPERTIES_H__