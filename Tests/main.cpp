#include <iostream>

//<------------------------------INCLUDES------------------------------>//
/**
 * @brief Concept to assert if a template parameter is an enum 
 */
template <typename T>
concept EnumType = std::is_enum_v<T>;
#include <type_traits>
#include <Register.hh>
//<-------------------------------------------------------------------->//


class IPeripheral
{
private:
    /* data */
public:

    virtual ~IPeripheral() = default;
    virtual void init() = 0;
    virtual void reset() = 0;
};


class IOPin : public IPeripheral
{
    private:
    public:
        virtual ~IOPin() = default;
        virtual void write(const bool status) = 0;
        virtual bool read() = 0;
        virtual void setInputMode() = 0;
        virtual void setOutputMode() = 0;
};



template <UnsignedIntegralPointerConcept UnsignedIntegralPtr>
class IOPinBase : public IOPin
{
private:
    Register<UnsignedIntegralPtr> instance;
    const uint8_t pinNumber;
public:
};

    //constexpr explicit IOPin (const uint8_t pinNumber) : pinNumber(pinNumber) { };
    //~IOPin() { }

    //void init() override { }
    //void reset() override { }

    //void write(const bool status) override { }
    //bool read() override { }
    //void setInputMode() override { }
    //void setOutputMode() override { }




volatile uint32_t a = 32;
Register<volatile uint32_t*> *aR = Register<volatile uint32_t*>::getInstance(&a);
uint32_t b {12};


int main(void)
{
    uint32_t bb {12};
    std::cout << aR->get() << std::endl;
    return 0;
}