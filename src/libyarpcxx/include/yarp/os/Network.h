// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/*
 * Copyright (C) 2010 Paul Fitzpatrick
 * CopyPolicy: Released under the terms of the GNU GPL v2.0.
 *
 */

#ifndef YARPCXX_Network_INC
#define YARPCXX_Network_INC

#include <yarp/yarpcxx.h>

namespace yarpcxx {
    namespace os {
        class Network;
    }
}

class yarpcxx::os::Network {
public:
    Network() {
        impl = yarpNetworkCreate();
        YARPCXX_VALID(impl);
    }

    ~Network() {
        yarpNetworkFree(impl);
    }

    bool setLocalMode(int isLocal) {
        return yarpNetworkSetLocalMode(impl,isLocal) == 0;
    }

    bool connect(const char *src, const char *dest, const char *carrier) {
        return yarpNetworkConnect(impl,src,dest,carrier) == 0;
    }
private:
    yarpNetworkPtr impl;
};

#endif