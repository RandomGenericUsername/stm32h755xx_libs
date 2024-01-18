#ifndef __REGISTER_H__
#define __REGISTER_H__

//<------------------------------INCLUDES------------------------------>//
#include <concepts>     //For concepts
#include <type_traits>  //For typetraits for concepts definitions
#include <vector>       //For std::vector
#include <memory>       //For std::shared_ptr
#include <algorithm>    //For this for std::find_if
//<-------------------------------------------------------------------->//

/**
 * @brief Interface for a generic register.
 * @tparam UnsignedIntegralPtr The type of pointer to a unsigned integral type.
 */
template <Utils::UnsignedIntegralPointerConcept UnsignedIntegralPtr>
class IRegister {
public:
    using ValueType = std::remove_pointer_t<UnsignedIntegralPtr>;
    /**
     * @brief Virtual destructor using the default implementation.
     */
    constexpr virtual ~IRegister() = default;

    /**
     * @brief Get the current value of the register.
     * @return UnsignedIntegralPtr Current value of the register.
     */
    constexpr virtual ValueType const get() const = 0;

    /**
     * @brief Set the value of the register.
     * @param value New value to set.
     */
    constexpr virtual void set(ValueType value) = 0;

    /**
     * @brief Clear the register (set all bits to 0).
     */
    constexpr virtual void clear() = 0;

    /**
     * @brief Check if a specific bit is set.
     * @param position Position of the bit to check.
     * @return true If the bit is set.
     * @return false If the bit is not set.
     */
    constexpr virtual bool checkBit(std::size_t position) const = 0;

    /**
     * @brief Check if specific bits at a position are set.
     * @param bitsMask Bits mask to check.
     * @param position Starting position to check from.
     * @return true If the bits are set.
     * @return false If the bits are not set.
     */
    constexpr virtual bool checkBits(ValueType bitsMask, std::size_t position = 0) const = 0;

    /**
     * @brief Set a specific bit at a given position.
     * @param position Starting position to set bit.
     */
    constexpr virtual void setBit(std::size_t position) = 0;

    /**
     * @brief Clear a specific bit at a given position.
     * @param position Starting position to clear bit.
     */
    constexpr virtual void clearBit(std::size_t position) = 0;

    /**
     * @brief Set specific bits at a position.
     * @param bitsMask Bits mask to set.
     * @param position Starting position to set from.
     */
    constexpr virtual void setBits(ValueType bitsMask, std::size_t position = 0) = 0;

    /**
     * @brief Get the position of the lowest set bit.
     * @return UnsignedIntegralPtr Position of the lowest set bit.
     */
    constexpr virtual std::size_t const getLowestIndex() const = 0;

    /**
     * @brief Get the position of the highest set bit.
     * @return UnsignedIntegralPtr Position of the highest set bit.
     */
    constexpr virtual std::size_t const getHighestIndex() const = 0;


    constexpr virtual UnsignedIntegralPtr const getAddress() const = 0;

};


/**
 * @brief Represents a Register with various utility functions.
 * @tparam UnsignedIntegralPtr The type of the register, must be an unsigned integral type.
 */
template <Utils::UnsignedIntegralPointerConcept UnsignedIntegralPtr>
class Register : public IRegister<UnsignedIntegralPtr>{

    public:

        //Type of the value pointed to by the pointer
        using ValueType = typename IRegister<UnsignedIntegralPtr>::ValueType;

        /**
         * @brief Construct a new Register object.
         * @param n Initial value of the register.
         */
        constexpr explicit Register(UnsignedIntegralPtr n) : address(n) {}
        constexpr ~Register() { }

        /**
         * @brief Get the value of the register.
         * @return UnsignedIntegralPtr Value of the register.
         */
        constexpr ValueType const get() const override { return *(this->address); }

        /**
         * @brief Set the value of the register.
         * @param n New value to set.
         */
        constexpr void set(const ValueType n) override { *(this->address) = n; }

        /**
         * @brief Clear the register (set all bits to 0).
         */
        constexpr void clear() override{ *(this->address) = 0; }

        /**
         * @brief Check if a specific bit is set.
         * @param position Position of the bit to check.
         * @return true If the bit is set.
         * @return false If the bit is not set.
         */
        constexpr bool checkBit(const std::size_t position) const override {
            return (0x1 << position) & (*(this->address));
        }

        /**
         * @brief Check if specific bits at a position are set.
         * @param bitsMask Bits mask to check.
         * @param position Starting position to check from.
         * @return true If the bits are set.
         * @return false If the bits are not set.
         */
        constexpr bool checkBits(const ValueType bitsMask, const std::size_t position = 0) const override {
            ValueType mask = bitsMask << position; // Shift the bitsMask to the correct position
            ValueType isolatedBits = (*(this->address) & mask); // Isolate the bits from the register value
            return isolatedBits == mask; // Check if isolated bits match the shifted bitsMask
        }

        /**
         * @brief Set a specific bit at a given position
         * @param Starting position to set bit
        */
        constexpr void setBit(const std::size_t position) override {
            //*(this->address) |= 0x1 << position;
            ValueType val = *(this->address);
            //val |= 0x1 << position;
            val = val | (0x1 << position);
            *(this->address) = val;
        }


        /**
         * @brief Clear a specific bit at a given position
         * @param Starting position to clear bit
        */
        constexpr void clearBit(const std::size_t position) override {
            //*(this->address) &= ~(0x1 << position);
            ValueType val = *(this->address);
            val = val & ~(0x1 << position);
            *(this->address) = val;
        }

        /**
         * @brief Set specific bits at a position.
         * @param bitsMask Bits mask to set.
         * @param position Starting position to set from.
         */
        constexpr void setBits(const ValueType bitsMask, const std::size_t position = 0) override {
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
            //ValueType mask = bitsMask << position;
            //*(this->address) &= ~mask;
            //*(this->address) |= mask;
            ValueType val = *(this->address);
            ValueType mask = bitsMask << position;
            val = (val & ~mask) | mask;
            *(this->address) = val;
        }

        /**
         * @brief Get the position of the lowest set bit.
         * @return UnsignedIntegralPtr Position of the lowest set bit.
         */
        constexpr std::size_t const getLowestIndex() const override { return getLowestIndexHelper<0>(); }

        /**
         * @brief Get the position of the highest set bit.
         * @return UnsignedIntegralPtr Position of the highest set bit.
         */
        constexpr std::size_t const getHighestIndex() const override { return getHighestIndexHelper<sizeof(ValueType) * 8 - 1>(); }



        constexpr UnsignedIntegralPtr const getAddress() const override{ return address; }


    private:

        template<std::size_t I = 0>
        constexpr const std::size_t getLowestIndexHelper() const {
            if constexpr (I >= sizeof(ValueType) * 8) // Base case: Check all bits
                return 0;
            else if (*(this->address) & (0x1 << I))
                return I + 1;
            else
                return getLowestIndexHelper<I+1>();
        }

        template<std::size_t I = sizeof(ValueType) * 8 - 1>
        constexpr const std::size_t getHighestIndexHelper() const {
            if constexpr (I == static_cast<std::size_t>(-1)) // Base case: Checked all bits
                return 0;
            else if (*(this->address) & (0x1 << I))
                return I + 1;
            else
                return getHighestIndexHelper<I-1>();
        }

        protected:
            //The pointer to the register cant be changed
            UnsignedIntegralPtr const address;

        // Declare SRegister as a friend
        //template <Utils::UnsignedIntegralPointerConcept>
        //friend class SRegister;

        template <Utils::UnsignedIntegralPointerConcept>
        friend class UniquePtrRegister;
};



template <Utils::UnsignedIntegralPointerConcept UnsignedIntegralPtr>
class UniquePtrRegister : public IRegister<UnsignedIntegralPtr> {
public:

    //Type of the value pointed to by the pointer
    using ValueType = typename IRegister<UnsignedIntegralPtr>::ValueType;

    constexpr ValueType const get() const override { return registerPtr->get(); }

    constexpr void set(ValueType value) override {
        registerPtr->set(value);
    }

    constexpr void clear() override {
        registerPtr->clear();
    }

    constexpr bool checkBit(std::size_t position) const override {
        return registerPtr->checkBit(position);
    }

    constexpr bool checkBits(ValueType bitsMask, std::size_t position = 0) const override {
        return registerPtr->checkBits(bitsMask, position);
    }

    constexpr void setBit(std::size_t position) override {
        registerPtr->setBit(position);
    }

    constexpr void clearBit(std::size_t position) override {
        registerPtr->clearBit(position);
    }

    constexpr void setBits(ValueType bitsMask, std::size_t position = 0) override {
        registerPtr->setBits(bitsMask, position);
    }

    constexpr std::size_t const getLowestIndex() const override {
        return registerPtr->getLowestIndex();
    }

    constexpr std::size_t const getHighestIndex() const override {
        return registerPtr->getHighestIndex();
    }

    constexpr UnsignedIntegralPtr const getAddress() const override { return registerPtr->getAddress(); }

private:

    constexpr explicit UniquePtrRegister(UnsignedIntegralPtr address)
            : registerPtr(std::make_unique<Register<UnsignedIntegralPtr>>(address)) {}

    std::unique_ptr<Register<UnsignedIntegralPtr>> registerPtr;

    template <Utils::UnsignedIntegralPointerConcept>
    friend class SRegister;

};


template <Utils::UnsignedIntegralPointerConcept UnsignedIntegralPtr>
class SRegister : public UniquePtrRegister<UnsignedIntegralPtr> {
//
    private:

        constexpr explicit SRegister(UnsignedIntegralPtr n) : UniquePtrRegister<UnsignedIntegralPtr>(n) {}

        // Function to access the instances vector
        static std::vector<UniquePtrRegister<UnsignedIntegralPtr>*>& getInstances() {
            // Local static variable
            //Construct On First Use Idiom
            static std::vector<UniquePtrRegister<UnsignedIntegralPtr>*> instances;
            return instances;
        }

        constexpr static UniquePtrRegister<UnsignedIntegralPtr>* createInstance(UnsignedIntegralPtr n) 
        {
            #ifdef DEBUG_PRINT
                std::cout << "Created instance to register: " << std::hex << reinterpret_cast<std::uintptr_t>(n) << std::dec << std::endl;
            #endif
            UniquePtrRegister<UnsignedIntegralPtr>* newInstance = new UniquePtrRegister<UnsignedIntegralPtr>(n);
            auto& instances = SRegister::getInstances();
            instances.push_back(newInstance);
            return newInstance;
        }
        constexpr static UniquePtrRegister<UnsignedIntegralPtr>* findInstance(UnsignedIntegralPtr const n){
            #ifdef DEBUG_PRINT
                std::cout << "Searching for register: " << std::hex << reinterpret_cast<std::uintptr_t>(n) << std::dec << std::endl;
            #endif
            auto instances = SRegister::getInstances();
            auto it = std::find_if(
                instances.begin(), 
                instances.end(), 
                [&n](UniquePtrRegister<UnsignedIntegralPtr>* const instance) -> bool {
                    return instance->getAddress() == n ;
                }
            );
            if (it != instances.end()) {
                #ifdef DEBUG_PRINT
                    std::cout << "Register: " << std::hex << reinterpret_cast<std::uintptr_t>(n) << std::dec << " found!!!" << std::endl;
                #endif
                return *it;
            }
            #ifdef DEBUG_PRINT
                std::cout << "Register: " << std::hex << reinterpret_cast<std::uintptr_t>(n) << std::dec << " not found" << std::endl;
            #endif
            return nullptr; // Return nullptr if instance not found
        }

    public:
        SRegister(const SRegister&) = delete;
        SRegister& operator=(const SRegister&) = delete;
        constexpr ~SRegister() override = default;
        constexpr static UniquePtrRegister<UnsignedIntegralPtr>* getInstance(UnsignedIntegralPtr n) 
        {
            auto it = SRegister::findInstance(n);
            if (it != nullptr) {
                return it;
            }
            return SRegister::createInstance(n);
        } 
};


template<typename T>
requires ((Utils::UnsignedIntegralPointerConcept<T>))
IRegister<T>* getRegisterInstance(T i)
{
    return SRegister<T>::getInstance(i);
}



#endif // __REGISTER_H__

