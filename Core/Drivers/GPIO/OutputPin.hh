#ifndef __OUTPUTPIN_H__
#define __OUTPUTPIN_H__

//<------------------------------INCLUDES------------------------------>//
#include <IPeripheral.hh>
//<-------------------------------------------------------------------->//

class IOutputPin : public IPeripheral
{
public:
    virtual void write(const bool status) = 0;
};

#endif // __OUTPUTPIN_H__