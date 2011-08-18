# Copyright: (C) 2009 RobotCub Consortium
# Author: Paul Fitzpatrick
# CopyPolicy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT

# This file is the official location of the current YARP version number.

set(YARP_VERSION_MAJOR "2")
set(YARP_VERSION_MINOR "3")
set(YARP_VERSION_PATCH "9")
set(YARP_VERSION_MODIFIER "")
set(YARP_VERSION_ABI "1")

set(CPACK_PACKAGE_VERSION_MAJOR "${YARP_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${YARP_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${YARP_VERSION_PATCH}${YARP_VERSION_MODIFIER}")

set(YARP_GENERIC_VERSION "${YARP_VERSION_MAJOR}.${YARP_VERSION_MINOR}.${YARP_VERSION_PATCH}")
set(YARP_GENERIC_SOVERSION "${YARP_VERSION_ABI}")
