// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/*
 * Copyright (C) 2006 Paul Fitzpatrick
 * CopyPolicy: Released under the terms of the GNU GPL v2.0.
 *
 */

#ifndef _YARP2_OUTPUTPROTOCOL_
#define _YARP2_OUTPUTPROTOCOL_

#include <yarp/os/impl/Route.h>
#include <yarp/os/ConnectionWriter.h>
#include <yarp/os/impl/SizedWriter.h>

namespace yarp {
    namespace os {
        namespace impl {
            class OutputProtocol;
            class InputProtocol;
        }
    }
}

/**
 * The output side of an active connection between two ports.
 */
class yarp::os::impl::OutputProtocol {
public:
    // all can throw IOException

    virtual ~OutputProtocol() {}

    virtual bool open(const Route& route) = 0;
    virtual void rename(const Route& route) = 0;
    virtual void close() = 0;

    virtual const Route& getRoute() = 0;
    virtual bool isActive() = 0;
    virtual bool isTextMode() = 0;
    virtual bool isConnectionless() = 0;
    virtual bool canEscape() = 0;
    virtual bool supportReply() = 0;
    virtual bool isLocal() = 0;

    virtual bool checkStreams() = 0;

    virtual void write(SizedWriter& writer) = 0;

    virtual void interrupt() = 0;

    // some connections are capable of ping-ponging
    virtual InputProtocol& getInput() = 0;
  
    // direct access
    virtual OutputStream& getOutputStream() = 0;
    virtual InputStream& getInputStream() = 0;
};

#endif