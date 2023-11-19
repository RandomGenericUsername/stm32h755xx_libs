#ifndef __REGISTER_H__
#define __REGISTER_H__

//<------------------------------INCLUDES------------------------------>//
#include <concepts>
#include <type_traits>
#include <cstdlib> // Include for calloc
//<-------------------------------------------------------------------->//


template <typename T>
concept UnsignedIntegralConcept = std::is_unsigned_v<T> && std::is_integral_v<T>;

/**
 * @brief Concept defining an a pointer to a unsigned integral type.
 */
template <typename T>
concept UnsignedIntegralPointerConcept = 
    (std::is_pointer_v<T> && std::is_unsigned_v<std::remove_pointer_t<std::remove_cv_t<T>>> && std::is_integral_v<std::remove_pointer_t<std::remove_cv_t<T>>>);



/**
 * @brief Interface for a generic register.
 * @tparam UnsignedIntegralPtr The type of pointer to a unsigned integral type.
 */
template <UnsignedIntegralPointerConcept UnsignedIntegralPtr>
class IRegister {
public:
    using ValueType = std::remove_pointer_t<UnsignedIntegralPtr>;
    /**
     * @brief Virtual destructor using the default implementation.
     */
    virtual ~IRegister() = default;

    /**
     * @brief Get the current value of the register.
     * @return UnsignedIntegralPtr Current value of the register.
     */
    virtual ValueType get() = 0;

    /**
     * @brief Set the value of the register.
     * @param value New value to set.
     */
    virtual void set(ValueType value) = 0;

    /**
     * @brief Clear the register (set all bits to 0).
     */
    virtual void clear() = 0;

    /**
     * @brief Check if a specific bit is set.
     * @param position Position of the bit to check.
     * @return true If the bit is set.
     * @return false If the bit is not set.
     */
    virtual bool checkBit(std::size_t position) = 0;

    /**
     * @brief Check if specific bits at a position are set.
     * @param bitsMask Bits mask to check.
     * @param position Starting position to check from.
     * @return true If the bits are set.
     * @return false If the bits are not set.
     */
    virtual bool checkBits(ValueType bitsMask, std::size_t position) = 0;

    /**
     * @brief Set a specific bit at a given position.
     * @param position Starting position to set bit.
     */
    virtual void setBit(std::size_t position) = 0;

    /**
     * @brief Clear a specific bit at a given position.
     * @param position Starting position to clear bit.
     */
    virtual void clearBit(std::size_t position) = 0;

    /**
     * @brief Set specific bits at a position.
     * @param bitsMask Bits mask to set.
     * @param position Starting position to set from.
     */
    virtual void setBits(ValueType bitsMask, std::size_t position = 0) = 0;

    /**
     * @brief Get the position of the lowest set bit.
     * @return UnsignedIntegralPtr Position of the lowest set bit.
     */
    virtual std::size_t getLowestIndex() = 0;

    /**
     * @brief Get the position of the highest set bit.
     * @return UnsignedIntegralPtr Position of the highest set bit.
     */
    virtual std::size_t getHighestIndex() = 0;
};


/**
 * @brief Represents a Register with various utility functions.
 * @tparam UnsignedIntegralPtr The type of the register, must be an unsigned integral type.
 */
template <UnsignedIntegralPointerConcept UnsignedIntegralPtr>
class RegisterImpl : public IRegister<UnsignedIntegralPtr>{

    public:

        using ValueType = typename IRegister<UnsignedIntegralPtr>::ValueType;

        /**
         * @brief Construct a new Register object.
         * @param n Initial value of the register.
         */
        explicit RegisterImpl(UnsignedIntegralPtr n) : value(n) {}
        ~RegisterImpl() { }

        /**
         * @brief Get the value of the register.
         * @return UnsignedIntegralPtr Value of the register.
         */
        ValueType get() override { return *(this->value); }

        /**
         * @brief Set the value of the register.
         * @param n New value to set.
         */
        void set(const ValueType n) override { *(this->value) = n; }

        /**
         * @brief Clear the register (set all bits to 0).
         */
        void clear() override{ *(this->value) = 0; }

        /**
         * @brief Check if a specific bit is set.
         * @param position Position of the bit to check.
         * @return true If the bit is set.
         * @return false If the bit is not set.
         */
        bool checkBit(const std::size_t position) override {
            return (0x1 << position) & *(this->value);
        }

        /**
         * @brief Check if specific bits at a position are set.
         * @param bitsMask Bits mask to check.
         * @param position Starting position to check from.
         * @return true If the bits are set.
         * @return false If the bits are not set.
         */
        bool checkBits(const ValueType bitsMask, const std::size_t position) override {
            ValueType isolatedBits = (*(this->value) >> position) & ((1 << (position + 1)) - 1);
            return isolatedBits == bitsMask;
        }

        /**
         * @brief Set a specific bit at a given position
         * @param Starting position to set bit
        */
        void setBit(const std::size_t position) override {
            *(this->value) |= 0x1 << position;
        }


        /**
         * @brief Clear a specific bit at a given position
         * @param Starting position to clear bit
        */
        void clearBit(const std::size_t position) override {
            *(this->value) &= ~(0x1 << position);
        }

        /**
         * @brief Set specific bits at a position.
         * @param bitsMask Bits mask to set.
         * @param position Starting position to set from.
         */
        void setBits(const ValueType bitsMask, const std::size_t position = 0) override {
            //bool condition = position > sizeof(UnsignedIntegralPtr) * 8 - 1;
            /**
             *  bits -> 0b101 
             *   pos -> 1
             *  mask -> 0b1010 (bits << pos)
             * 
             *   n   -> 0b01101
             *  mask -> 0b01010
             * 
             * !mask -> 0b10101
             *   n   -> 0b01101  &
             * ____________________
             *   n   -> 0b00101
             * 
             *  mask -> 0b01010
             *   n   -> 0b00101  |
             * ____________________
             *   n   -> 0b01111
             * 
            */
            ValueType mask = bitsMask << position;
            *(this->value) &= ~mask;
            *(this->value) |= mask;
        }

        /**
         * @brief Get the position of the lowest set bit.
         * @return UnsignedIntegralPtr Position of the lowest set bit.
         */
        std::size_t getLowestIndex() override { return getLowestIndexHelper<0>(); }

        /**
         * @brief Get the position of the highest set bit.
         * @return UnsignedIntegralPtr Position of the highest set bit.
         */
        std::size_t getHighestIndex() override { return getHighestIndexHelper<sizeof(ValueType) * 8 - 1>(); }


    private:

        template<std::size_t I = 0>
        std::size_t getLowestIndexHelper() {
            if constexpr (I >= sizeof(ValueType) * 8) // Base case: Check all bits
                return 0;
            else if (*(this->value) & (0x1 << I))
                return I + 1;
            else
                return getLowestIndexHelper<I+1>();
        }

        template<std::size_t I = sizeof(ValueType) * 8 - 1>
        std::size_t getHighestIndexHelper() {
            if constexpr (I == static_cast<std::size_t>(-1)) // Base case: Checked all bits
                return 0;
            else if (*(this->value) & (0x1 << I))
                return I + 1;
            else
                return getHighestIndexHelper<I-1>();
        }

        //The pointer to the register cant be changed
        protected:
            UnsignedIntegralPtr const value;

};


template <UnsignedIntegralPointerConcept UnsignedIntegralPtr>
class Register : public RegisterImpl<UnsignedIntegralPtr>{

    private:
        static Register<UnsignedIntegralPtr>** instances;
        static std::size_t instancesCount;
        static std::size_t instancesCapacity;
        explicit Register(UnsignedIntegralPtr n) : RegisterImpl<UnsignedIntegralPtr>(n) {}
    public:
        Register(const Register&) = delete;
        Register& operator=(const Register&) = delete;

        ~Register() { }
        constexpr static Register* getInstance(UnsignedIntegralPtr n) 
        {
            for (std::size_t i = 0; i < instancesCount; i++)
            {
                if (instances[i]->value == n) // Assuming value is also volatile
                    return instances[i];
            }
            if (instancesCount == instancesCapacity)
            {
                instancesCapacity = instancesCapacity == 0 ? 1 : instancesCapacity * 2;
                instances = reinterpret_cast<Register<UnsignedIntegralPtr>**>(std::realloc(instances, instancesCapacity * sizeof(Register<UnsignedIntegralPtr>*)));
            }
            Register* newInstance = new Register<UnsignedIntegralPtr>(n);
            instances[instancesCount++] = newInstance;
            return newInstance;
        } 
};


template <UnsignedIntegralPointerConcept UnsignedIntegralPtr>
Register<UnsignedIntegralPtr>** Register<UnsignedIntegralPtr>::instances = {nullptr};

template <UnsignedIntegralPointerConcept UnsignedIntegralPtr>
std::size_t Register<UnsignedIntegralPtr>::instancesCount = 0;

template <UnsignedIntegralPointerConcept UnsignedIntegralPtr>
std::size_t Register<UnsignedIntegralPtr>::instancesCapacity = 0;


#endif // __REGISTER_H__

