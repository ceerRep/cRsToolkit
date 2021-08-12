#ifndef HUYAPLATFORM_H
#define HUYAPLATFORM_H

#include "Platform.hpp"

class HuyaPlatform : public Platform
{
public:
    HuyaPlatform();

    virtual QString getName() const override;
    virtual QString getLiveURL(const Broadcaster& broadcaster) const override;
    virtual int getLiveState(const Broadcaster& broadcaster) const override;
};

#endif // HUYAPLATFORM_H
