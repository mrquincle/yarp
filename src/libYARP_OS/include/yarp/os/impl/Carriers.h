// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/*
 * Copyright (C) 2006 Paul Fitzpatrick
 * CopyPolicy: Released under the terms of the GNU GPL v2.0.
 *
 */

#ifndef _YARP2_CARRIERS_
#define _YARP2_CARRIERS_

#include <yarp/os/impl/String.h>
#include <yarp/os/Bytes.h>
#include <yarp/os/impl/Address.h>
#include <yarp/os/impl/Face.h>
#include <yarp/os/impl/OutputProtocol.h>
#include <yarp/os/impl/Carrier.h>

#include <ace/Vector_T.h>

namespace yarp {
    namespace os {
        namespace impl {
            class Carriers;
        }
    }
}

/**
 * Collection of carriers, a singleton. 
 * This is the starting point for creating connections
 * between ports.
 */
class yarp::os::impl::Carriers {
public:

    /**
     *
     * Select a carrier by name.
     *
     * @param name the name of the desired carrier.
     * @return the desired carrier, or NULL if not found.
     *
     */
    static Carrier *chooseCarrier(const String& name);

    /**
     *
     * Select a carrier by 8-byte header.
     *
     * @param bytes the 8-byte header describing the desired carrier.
     * @return the desired carrier, or NULL if not found.
     *
     */
    static Carrier *chooseCarrier(const Bytes& bytes);

    /**
     *
     * Create a "proto-carrier" interface object that waits for
     * incoming connections prior to a carrier being selected via
     * handshaking.
     *
     * Currently, this is always a tcp server socket.  There is no
     * reason why it couldn't be any kind of stream, and this would be
     * the method to change if you want to do something imaginative
     * here.
     *
     * @param address the address (including initial carrier type) to
     * listen to.
     *
     * @return the interface object.
     *
     */
    static Face *listen(const Address& address);

    /**
     *
     * Initiate a connection to an address.
     *
     * @param address the address (including desired carrier type) to
     * connect to.
     *
     * @return the protocol object.
     *
     */
    static OutputProtocol *connect(const Address& address);

    /**
     *
     * Destructor.
     *
     */
    virtual ~Carriers();


    /**
     *
     * Add a new connection type.
     *
     * @param carrier a prototype of the desired connection type.
     * The YARP library will be responsible for destroying it on shutdown.
     *
     * @return true on success.
     *
     */
    static bool addCarrierPrototype(Carrier *carrier);

private:
    ACE_Vector<Carrier *> delegates;

    Carriers();
  
    static Carriers instance;
    static Carriers& getInstance() {
        return instance;
    }

    Carrier *chooseCarrier(const String * name, const Bytes * bytes);  
};


#endif

