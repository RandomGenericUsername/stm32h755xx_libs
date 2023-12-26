#ifndef __REGISTER_H__
#define __REGISTER_H__

//<------------------------------INCLUDES------------------------------>//
#include <concepts>     //For concepts
#include <type_traits>  //For typetraits for concepts definitions
#include <vector>       //For std::vector
#include <memory>       //For std::shared_ptr
#include <algorithm>    //For this for std::find_if
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
    virtual ValueType const get() const = 0;

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
    virtual bool checkBit(std::size_t position) const = 0;

    /**
     * @brief Check if specific bits at a position are set.
     * @param bitsMask Bits mask to check.
     * @param position Starting position to check from.
     * @return true If the bits are set.
     * @return false If the bits are not set.
     */
    virtual bool checkBits(ValueType bitsMask, std::size_t position) const = 0;

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
    virtual std::size_t const getLowestIndex() const = 0;

    /**
     * @brief Get the position of the highest set bit.
     * @return UnsignedIntegralPtr Position of the highest set bit.
     */
    virtual std::size_t const getHighestIndex() const = 0;


    virtual UnsignedIntegralPtr const getAddress() const = 0;

};


/**
 * @brief Represents a Register with various utility functions.
 * @tparam UnsignedIntegralPtr The type of the register, must be an unsigned integral type.
 */
template <UnsignedIntegralPointerConcept UnsignedIntegralPtr>
class Register : public IRegister<UnsignedIntegralPtr>{

    public:

        //Type of the value pointed to by the pointer
        using ValueType = typename IRegister<UnsignedIntegralPtr>::ValueType;

        /**
         * @brief Construct a new Register object.
         * @param n Initial value of the register.
         */
        explicit Register(UnsignedIntegralPtr n) : address(n) {}
        ~Register() { }

        /**
         * @brief Get the value of the register.
         * @return UnsignedIntegralPtr Value of the register.
         */
        ValueType const get() const override { return *(this->address); }

        /**
         * @brief Set the value of the register.
         * @param n New value to set.
         */
        void set(const ValueType n) override { *(this->address) = n; }

        /**
         * @brief Clear the register (set all bits to 0).
         */
        void clear() override{ *(this->address) = 0; }

        /**
         * @brief Check if a specific bit is set.
         * @param position Position of the bit to check.
         * @return true If the bit is set.
         * @return false If the bit is not set.
         */
        bool checkBit(const std::size_t position) const override {
            return (0x1 << position) & *(this->address);
        }

        /**
         * @brief Check if specific bits at a position are set.
         * @param bitsMask Bits mask to check.
         * @param position Starting position to check from.
         * @return true If the bits are set.
         * @return false If the bits are not set.
         */
        bool checkBits(const ValueType bitsMask, const std::size_t position) const override {
            ValueType isolatedBits = (*(this->address) >> position) & ((1 << (position + 1)) - 1);
            return isolatedBits == bitsMask;
        }

        /**
         * @brief Set a specific bit at a given position
         * @param Starting position to set bit
        */
        void setBit(const std::size_t position) override {
            //*(this->address) |= 0x1 << position;
            ValueType val = *(this->address);
            val |= 0x1 << position;
            *(this->address) = val;
        }


        /**
         * @brief Clear a specific bit at a given position
         * @param Starting position to clear bit
        */
        void clearBit(const std::size_t position) override {
            //*(this->address) &= ~(0x1 << position);
            ValueType val = *(this->address);
            val &= ~(0x1 << position);
            *(this->address) = val;
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
        std::size_t const getLowestIndex() const override { return getLowestIndexHelper<0>(); }

        /**
         * @brief Get the position of the highest set bit.
         * @return UnsignedIntegralPtr Position of the highest set bit.
         */
        std::size_t const getHighestIndex() const override { return getHighestIndexHelper<sizeof(ValueType) * 8 - 1>(); }



        UnsignedIntegralPtr const getAddress() const override{ return address; }


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
        //template <UnsignedIntegralPointerConcept>
        //friend class SRegister;

        template <UnsignedIntegralPointerConcept>
        friend class UniquePtrRegister;
};



template <UnsignedIntegralPointerConcept UnsignedIntegralPtr>
class UniquePtrRegister : public IRegister<UnsignedIntegralPtr> {
public:

    //Type of the value pointed to by the pointer
    using ValueType = typename IRegister<UnsignedIntegralPtr>::ValueType;

    ValueType const get() const override { return registerPtr->get(); }

    void set(ValueType value) override {
        registerPtr->set(value);
    }

    void clear() override {
        registerPtr->clear();
    }

    bool checkBit(std::size_t position) const override {
        return registerPtr->checkBit(position);
    }

    bool checkBits(ValueType bitsMask, std::size_t position) const override {
        return registerPtr->checkBits(bitsMask, position);
    }

    void setBit(std::size_t position) override {
        registerPtr->setBit(position);
    }

    void clearBit(std::size_t position) override {
        registerPtr->clearBit(position);
    }

    void setBits(ValueType bitsMask, std::size_t position = 0) override {
        registerPtr->setBits(bitsMask, position);
    }

    std::size_t const getLowestIndex() const override {
        return registerPtr->getLowestIndex();
    }

    std::size_t const getHighestIndex() const override {
        return registerPtr->getHighestIndex();
    }

    UnsignedIntegralPtr const getAddress() const override { return registerPtr->getAddress(); }

private:

    explicit UniquePtrRegister(UnsignedIntegralPtr address)
            : registerPtr(std::make_unique<Register<UnsignedIntegralPtr>>(address)) {}

    std::unique_ptr<Register<UnsignedIntegralPtr>> registerPtr;

    template <UnsignedIntegralPointerConcept>
    friend class SRegister;

};


template <UnsignedIntegralPointerConcept UnsignedIntegralPtr>
class SRegister : public UniquePtrRegister<UnsignedIntegralPtr> {
//
    private:

        explicit SRegister(UnsignedIntegralPtr n) : UniquePtrRegister<UnsignedIntegralPtr>(n) {}
        //Construct On First Use Idiom
        // Function to access the instances vector
        static std::vector<UniquePtrRegister<UnsignedIntegralPtr>*>& getInstances() {
            // Local static variable
            static std::vector<UniquePtrRegister<UnsignedIntegralPtr>*> instances;
            return instances;
        }
        static UniquePtrRegister<UnsignedIntegralPtr>* createInstance(UnsignedIntegralPtr n) 
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
        ~SRegister() override = default;
        constexpr static UniquePtrRegister<UnsignedIntegralPtr>* getInstance(UnsignedIntegralPtr n) 
        {
            auto it = SRegister::findInstance(n);
            if (it != nullptr) {
                return it;
            }
            return SRegister::createInstance(n);
        } 
};




#endif // __REGISTER_H__

