#define CORE_CM7
#include <stm32h755xx.h>
#include <Register.hh>

extern "C"{

    void SystemInit(void);
}

class testClass
{
private:
    /* data */
public:
    testClass(/* args */);
    ~testClass();
};

testClass::testClass(/* args */)
{
}

testClass::~testClass()
{
}

testClass asdf;

int main(void)
{

    testClass b;
    while (1)
    {
        /* code */
    }
    
    return 0;
}


void SystemInit(void)
{
    
}
    