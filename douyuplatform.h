#ifndef DOUYUPLATFORM_H
#define DOUYUPLATFORM_H

#include "Platform.hpp"

class DouyuPlatform : public Platform
{
public:
    DouyuPlatform();

    virtual QString getName() const override;
    virtual QString getLiveURL(const Broadcaster& broadcaster) const override;
    virtual int getLiveState(const Broadcaster& broadcaster) const override;
};

#endif // DOUYUPLATFORM_H
