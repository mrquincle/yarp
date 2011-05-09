// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/*
 * Copyright (C) 2011 Paul Fitzpatrick, Anne van Rossum
 * CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 *
 */

#ifndef _YARP2_PLATFORMTHREAD_
#define _YARP2_PLATFORMTHREAD_

#include <yarp/conf/system.h>
#ifdef YARP_HAS_ACE
#  include <ace/Thread.h>
#  define Platform_hthread_t ACE_hthread_t
#  define Platform_thread_t ACE_thread_t
#  define PLATFORM_THREAD_SELF() ACE_Thread::self()
#  define PLATFORM_THREAD_RETURN unsigned
#  define PLATFORM_THREAD_JOIN(x) ACE_Thread::join(x)
#else
#  include <pthread.h>
#  define Platform_hthread_t pthread_t
#  define Platform_thread_t long int
#  define PLATFORM_THREAD_SELF() pthread_self()
#  define PLATFORM_THREAD_RETURN void *
#  define PLATFORM_THREAD_JOIN(x) pthread_join(x,0/*NULL*/)
#endif

#endif