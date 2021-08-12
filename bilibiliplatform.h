#ifndef BILIBILIPLATFORM_H
#define BILIBILIPLATFORM_H

#include "Platform.hpp"

class BilibiliPlatform : public Platform
{
public:
    BilibiliPlatform();

    virtual QString getName() const override;
    virtual QString getLiveURL(const Broadcaster& broadcaster) const override;
    virtual int getLiveState(const Broadcaster& broadcaster) const override;
};

#endif // BILIBILIPLATFORM_H
