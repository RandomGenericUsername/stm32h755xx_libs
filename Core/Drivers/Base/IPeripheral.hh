#ifndef __IPERIPHERAL_H__
#define __IPERIPHERAL_H__

/**
 * @brief Basic interface followed by all peripherals
 */
class IPeripheral
{
private:
    /* data */
public:
    /**
     * @brief Method to initialize hardware, i.e write the hardware register.
     */
    virtual void init() = 0;

    /**
     * @brief Method to reset hardware to default values.
     */
    virtual void reset() = 0;
};

#endif // __IPERIPHERAL_H__