// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/*
 * Copyright: (C) 2011 Paul Fitzpatrick
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 */

#include "Hello.h"

#include <yarp/os/SharedLibraryClass.h>
#include <yarp/os/SharedLibrary.h>

#include <stdio.h>

using namespace yarp::os;

int main(int argc, char *argv[]) {
    SharedLibrary lib;
    if (!lib.open("hello")) {
        fprintf(stderr,"Failed to open library\n");
        fprintf(stderr,"Make sure libhello.so/hello.dll/... is in your library path\n");
        //return 1;
    }
    void *fn = lib.getSymbol("hello_there");
    if (fn==NULL) {
        fprintf(stderr,"Failed to find factory in library\n");
        //return 1;
    }
    SharedLibraryClassFactory<Hello> helloFactory;
	helloFactory.useFactoryFunction(fn);
	SharedLibraryClass<Hello> phello(helloFactory);
    if (!phello.isValid()) {
        fprintf(stderr,"Failed to create Hello object\n");
        //return 1;
    } else {
		Hello& hello = phello.getContent();
		hello.write(42);
		printf("42 + 1 = %d\n", hello.read());
	}


	// Start over, using more helper functions and defaults
    SharedLibraryClassFactory<Hello> helloFactory2("hello");
	if (!helloFactory2.isValid()) {
		printf("Library problem: (%s)\n", Vocab::decode(helloFactory2.getStatus()).c_str());
		return 1;
	}

	SharedLibraryClass<Hello> hello2(helloFactory2);
    hello2->write(100);
    printf("100 + 1 = %d\n", hello2->read());
    return 0;
}

