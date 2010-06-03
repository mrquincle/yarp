# Copyright: (C) 2009, 2010 RobotCub Consortium
# Authors: Paul Fitzpatrick, Giorgio Metta, Lorenzo Natale
# CopyPolicy: Released under the terms of the GNU GPL v2.0.

#########################################################################
##
## This file provides a set of macros for building bundles of plugins.
## Sample use:
##
##  BEGIN_PLUGIN_LIBRARY(libname)
##    ADD_SUBDIRECTORY(plugin1)
##    ADD_SUBDIRECTORY(plugin2)
##  END_PLUGIN_LIBRARY(libname)
##  ADD_PLUGIN_LIBRARY_EXECUTABLE(libnamedev libname)
##
## This sample would create two CMake targets, "libname" (a library) 
## and libnamedev (an executable).  
##
## The library links with every library in the subdirectories 
## (which can be made individually optional using CMake options),
## and provides a method to initialize them all.
##
## The executable is a test program that links and initializes
## all plugins.
##
## Plugins come in two types, carriers and devices.
## To let YARP know how to initialize them, add lines like 
## this in the CMakeLists.txt files the plugin subdirectories:
##
##   PREPARE_DEVICE(microphone TYPE MicrophoneDeviceDriver 
##                  INCLUDE MicrophoneDeviceDriver.h WRAPPER grabber)
## (the WRAPPER is optional), or:
##   PREPARE_CARRIER(new_carrier TYPE TheCarrier INCLUDE ...)
##
#########################################################################


## Skip this whole file if it has already been included
IF (NOT COMMAND END_PLUGIN_LIBRARY)

# Helper macro to work around a bug in set_property in cmake 2.6.0
MACRO(get_property_fix localname _global _property varname)
  set(_exists_chk)
  get_property(_exists_chk GLOBAL PROPERTY ${varname})
  if (_exists_chk)
    set(${localname} ${_exists_chk})
  else (_exists_chk)
    set(${localname})
  endif (_exists_chk)
ENDMACRO(get_property_fix)

# Helper macro to work around a bug in set_property in cmake 2.6.0
MACRO(set_property_fix _global _property _append varname)
  get_property(_append_chk GLOBAL PROPERTY ${varname})
  if (_append_chk)
    set_property(GLOBAL APPEND PROPERTY ${varname} ${ARGN})
  else (_append_chk)
    set_property(GLOBAL PROPERTY ${varname} ${ARGN})
  endif (_append_chk)
ENDMACRO(set_property_fix)

#########################################################################
# BEGIN_PLUGIN_LIBRARY macro makes sure that all the hooks
# needed for creating a plugin library are in place.
#
MACRO(BEGIN_PLUGIN_LIBRARY bundle_name)

  # If we are nested inside a larger plugin block, we don't
  # have to do much.  If we are the outermost plugin block,
  # then we need to set up everything.
  IF (YARP_PLUGIN_MODE)

    MESSAGE(STATUS "nested library ${bundle_name}")

  ELSE (YARP_PLUGIN_MODE)

    # Declare that we are starting to compile the given plugin library
    MESSAGE(STATUS "starting plugin library: ${bundle_name}")
    SET(YARP_PLUGIN_LIB_NAME ${bundle_name})
    SET(YARP_PLUGIN_MODE TRUE)

    SET(YARP_PLUGIN_GEN ${CMAKE_BINARY_DIR}/generated_code)
    IF (NOT EXISTS ${YARP_PLUGIN_GEN})
      FILE(MAKE_DIRECTORY ${YARP_PLUGIN_GEN})
    ENDIF (NOT EXISTS ${YARP_PLUGIN_GEN})
    SET(YARP_PLUGIN_PREFIX "${bundle_name}_")
    SET(YARP_PLUGIN_LIB_FLAG EXCLUDE_FROM_ALL)

    # Set a flag to let individual modules know that they are being
    # compiled as part of a bundle, and not standalone.  Developers
    # use this flag to inhibit compilation of test programs and 
    # the like.
    SET(COMPILE_DEVICE_LIBRARY TRUE) # legacy flag
    SET(COMPILE_PLUGIN_LIBRARY TRUE)

    # Record the name of this outermost device bundle (needed because
    # nesting is allowed)
    SET(YARP_PLUGIN_MASTER ${bundle_name})

    SET_PROPERTY(GLOBAL PROPERTY YARP_BUNDLE_DEVICES)
    SET_PROPERTY(GLOBAL PROPERTY YARP_BUNDLE_LIBS)
    SET_PROPERTY(GLOBAL PROPERTY YARP_BUNDLE_CODE)
    SET_PROPERTY(GLOBAL PROPERTY YARP_BUNDLE_INCLUDE_DIRS)

    get_property(YARP_TREE_INCLUDE_DIRS GLOBAL PROPERTY YARP_TREE_INCLUDE_DIRS)
    if (YARP_TREE_INCLUDE_DIRS)
      set (YARP_FOUND TRUE)
      get_property_fix(YARP_INCLUDE_DIRS GLOBAL PROPERTY YARP_TREE_INCLUDE_DIRS)
      get_property_fix(YARP_LIBRARIES GLOBAL PROPERTY YARP_LIBS)
      get_property_fix(YARP_DEFINES GLOBAL PROPERTY YARP_DEFS)
    else (YARP_TREE_INCLUDE_DIRS)
      find_package(YARP REQUIRED)
    endif (YARP_TREE_INCLUDE_DIRS)

  ENDIF (YARP_PLUGIN_MODE)

ENDMACRO(BEGIN_PLUGIN_LIBRARY bundle_name)


#########################################################################
# ADD_PLUGIN_NORMALIZED macro is an internal command to convert a 
# plugin declaration to code, and to set up CMake flags for controlling
# compilation of that device.  This macro is called by PREPARE_PLUGIN
# which is the user-facing macro.  PREPARE_PLUGIN parses
# a flexible set of arguments, then passes them to ADD_PLUGIN_NORMALIZED
# in a clean canonical order.
#
MACRO(ADD_PLUGIN_NORMALIZED plugin_name type include wrapper category)

  # Append the current source directory to the set of include paths.
  # Developers seem to expect #include "foo.h" to work if foo.h is
  # in their module directory.
  INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

  # Figure out a decent filename for the code we are about to 
  # generate.  If all else fails, the code will get dumped in
  # the current binary directory.
  SET(fdir ${YARP_PLUGIN_GEN})
  IF(NOT fdir)
    SET(fdir ${CMAKE_CURRENT_BINARY_DIR})
  ENDIF(NOT fdir)

  # We'll be expanding the code in template/yarp_plugin_*.cpp.in using 
  # the following variables:

  SET(YARPDEV_NAME "${plugin_name}")
  SET(YARPDEV_TYPE "${type}")
  SET(YARPDEV_INCLUDE "${include}")
  SET(YARPDEV_WRAPPER "${wrapper}")
  SET(YARPDEV_CATEGORY "${category}")
  SET(ENABLE_YARPDEV_NAME "1")

  # Go ahead and prepare some code to wrap this plugin.  
  SET(fname ${fdir}/yarpdev_add_${plugin_name}.cpp)
  CONFIGURE_FILE(${YARP_MODULE_PATH}/template/yarp_plugin_${category}.cpp.in
    ${fname} @ONLY  IMMEDIATE)
 
  # Set up a flag to enable/disable compilation of this plugin.
  SET(MYNAME "${YARP_PLUGIN_PREFIX}${plugin_name}")
  IF (NOT COMPILE_BY_DEFAULT)
    SET (COMPILE_BY_DEFAULT FALSE)
  ENDIF (NOT COMPILE_BY_DEFAULT)
  SET(ENABLE_${MYNAME} ${COMPILE_BY_DEFAULT} CACHE BOOL "Enable/disable compilation of ${MYNAME}")

  # Set some convenience variables based on whether the plugin
  # is enabled or disabled.
  SET(ENABLE_${plugin_name} ${ENABLE_${MYNAME}})
  IF (ENABLE_${plugin_name})
    SET(SKIP_${plugin_name} FALSE)
    SET(SKIP_${MYNAME} FALSE)
  ELSE (ENABLE_${plugin_name})
    SET(SKIP_${plugin_name} TRUE)
    SET(SKIP_${MYNAME} TRUE)
  ENDIF (ENABLE_${plugin_name})

  # If the plugin is enabled, add the appropriate source code into
  # the library source list.
  IF (ENABLE_${MYNAME})
    set_property_fix(GLOBAL APPEND PROPERTY YARP_BUNDLE_DEVICES ${plugin_name})
    set_property_fix(GLOBAL APPEND PROPERTY YARP_BUNDLE_CODE ${fname})
    set_property_fix(GLOBAL APPEND PROPERTY YARP_BUNDLE_INCLUDE_DIRS 
      ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/include)
    SET(YARP_PLUGIN_ACTIVE TRUE)
    MESSAGE(STATUS " +++ plugin ${plugin_name}, ENABLE_${plugin_name} is set")
  ELSE (ENABLE_${MYNAME})
    MESSAGE(STATUS " +++ plugin ${plugin_name}, SKIP_${plugin_name} is set")
  ENDIF (ENABLE_${MYNAME})

  # We are done!

ENDMACRO(ADD_PLUGIN_NORMALIZED plugin_name type include wrapper)



#########################################################################
# PREPARE_PLUGIN macro lets a developer declare a plugin using a 
# statement like:
#    PREPARE_PLUGIN(foo CATEGORY device TYPE FooDriver INCLUDE FooDriver.h)
# or
#    PREPARE_PLUGIN(moto CATEGORY device TYPE Moto INCLUDE moto.h WRAPPER controlboard)
# This macro is just a simple parser and calls ADD_PLUGIN_NORMALIZED to
# do the actual work.
#
MACRO(PREPARE_PLUGIN plugin_name)
  SET(EXPECT_TYPE FALSE)
  SET(EXPECT_INCLUDE FALSE)
  SET(THE_TYPE "")
  SET(THE_INCLUDE "")
  SET(THE_WRAPPER "")
  FOREACH(arg ${ARGN})
    IF(EXPECT_TYPE)
      SET(THE_TYPE ${arg})
      SET(EXPECT_TYPE FALSE)
    ENDIF(EXPECT_TYPE)
    IF(EXPECT_INCLUDE)
      SET(THE_INCLUDE ${arg})
      SET(EXPECT_INCLUDE FALSE)
    ENDIF(EXPECT_INCLUDE)
    IF(EXPECT_WRAPPER)
      SET(THE_WRAPPER ${arg})
      SET(EXPECT_WRAPPER FALSE)
    ENDIF(EXPECT_WRAPPER)
    IF(EXPECT_CATEGORY)
      SET(THE_CATEGORY ${arg})
      SET(EXPECT_CATEGORY FALSE)
    ENDIF(EXPECT_CATEGORY)
    IF(arg STREQUAL "TYPE")
      SET(EXPECT_TYPE TRUE)
    ENDIF(arg STREQUAL "TYPE")
    IF(arg STREQUAL "INCLUDE")
      SET(EXPECT_INCLUDE TRUE)
    ENDIF(arg STREQUAL "INCLUDE")
    IF(arg STREQUAL "WRAPPER")
      SET(EXPECT_WRAPPER TRUE)
    ENDIF(arg STREQUAL "WRAPPER")
    IF(arg STREQUAL "CATEGORY")
      SET(EXPECT_CATEGORY TRUE)
    ENDIF(arg STREQUAL "CATEGORY")
  ENDFOREACH(arg ${ARGN})
  IF(THE_TYPE AND THE_INCLUDE)
    ADD_PLUGIN_NORMALIZED(${plugin_name} ${THE_TYPE} ${THE_INCLUDE} "${THE_WRAPPER}" "${THE_CATEGORY}")
  ELSE(THE_TYPE AND THE_INCLUDE)
    MESSAGE(STATUS "Not enough information to create ${plugin_name}")
    MESSAGE(STATUS "  type:    ${THE_TYPE}")
    MESSAGE(STATUS "  include: ${THE_INCLUDE}")
    MESSAGE(STATUS "  wrapper: ${THE_WRAPPER}")
    MESSAGE(STATUS "  category: ${THE_CATEGORY}")
  ENDIF(THE_TYPE AND THE_INCLUDE)
ENDMACRO(PREPARE_PLUGIN plugin_name)

MACRO(PREPARE_DEVICE)
  PREPARE_PLUGIN(${ARGN} CATEGORY device)
ENDMACRO(PREPARE_DEVICE)

MACRO(PREPARE_CARRIER)
  PREPARE_PLUGIN(${ARGN} CATEGORY carrier)
ENDMACRO(PREPARE_CARRIER)



#########################################################################
# Lightly redefine ADD_LIBRARY
#
MACRO(ADD_LIBRARY LIBNAME)
  IF (NOT YARP_PLUGIN_MODE)
    # pass on call without looking at it
    _ADD_LIBRARY(${LIBNAME} ${ARGN})
  ELSE (NOT YARP_PLUGIN_MODE)
    set(IS_IMPORTED FALSE)
    foreach(arg ${ARGN})
      if ("${arg}" STREQUAL "IMPORTED")
        set(IS_IMPORTED TRUE)
      endif()
    endforeach()
    if (NOT IS_IMPORTED)
      get_property_fix(srcs GLOBAL PROPERTY YARP_BUNDLE_CODE)
      _ADD_LIBRARY(${LIBNAME} ${srcs} ${ARGN})
      set_property(GLOBAL PROPERTY YARP_BUNDLE_CODE)
      set_property_fix(GLOBAL APPEND PROPERTY YARP_BUNDLE_LIBS ${LIBNAME})
      if (YARP_TREE_INCLUDE_DIRS)
        install(TARGETS ${LIBNAME} EXPORT YARP COMPONENT runtime 
                DESTINATION lib)
      endif (YARP_TREE_INCLUDE_DIRS)
    endif (NOT IS_IMPORTED)
  ENDIF (NOT YARP_PLUGIN_MODE)
ENDMACRO(ADD_LIBRARY LIBNAME)


#########################################################################
# Lightly redefine FIND_PACKAGE
#
MACRO(FIND_PACKAGE LIBNAME)
  IF (NOT YARP_PLUGIN_MODE)
    # pass on call without looking at it
    _FIND_PACKAGE(${LIBNAME} ${ARGN})
  ELSE (NOT YARP_PLUGIN_MODE)
  ENDIF (NOT YARP_PLUGIN_MODE)
    IF ("${LIBNAME}" STREQUAL "YARP")
      # Skipping requests for YARP, we already have it
    ELSE ("${LIBNAME}" STREQUAL "YARP")
      _FIND_PACKAGE(${LIBNAME} ${ARGN})
    ENDIF ("${LIBNAME}" STREQUAL "YARP")
ENDMACRO(FIND_PACKAGE LIBNAME)



#########################################################################
# END_PLUGIN_LIBRARY macro prepares a plugin library if this is
# the outermost plugin library block, otherwise it propagates
# all collected information to the plugin library block that wraps
# it.
#
macro(END_PLUGIN_LIBRARY bundle_name)
  message(STATUS "ending plugin library: ${bundle_name}")
  if ("${bundle_name}" STREQUAL "${YARP_PLUGIN_MASTER}")
    set(YARP_LIB_NAME ${YARP_PLUGIN_MASTER})
    get_property_fix(devs GLOBAL PROPERTY YARP_BUNDLE_DEVICES)
    set(YARP_CODE_PRE)
    set(YARP_CODE_POST)
    foreach(dev ${devs})
      set(YARP_CODE_PRE "${YARP_CODE_PRE}\nextern void add_${dev}();")
      set(YARP_CODE_POST "${YARP_CODE_POST}\n    add_${dev}();")
    endforeach()
    configure_file(${YARP_MODULE_PATH}/template/yarpdev_lib.cpp.in
      ${YARP_PLUGIN_GEN}/add_${YARP_PLUGIN_MASTER}_plugins.cpp @ONLY IMMEDIATE)
    configure_file(${YARP_MODULE_PATH}/template/yarpdev_lib.h.in
      ${YARP_PLUGIN_GEN}/add_${YARP_PLUGIN_MASTER}_plugins.h @ONLY  IMMEDIATE)
    get_property_fix(code GLOBAL PROPERTY YARP_BUNDLE_CODE)
    #get_property(dirs GLOBAL PROPERTY YARP_BUNDLE_INCLUDE_DIRS)
    #include_directories(${YARP_INCLUDE_DIRS} ${dirs})
    include_directories(${YARP_INCLUDE_DIRS})
    _ADD_LIBRARY(${YARP_PLUGIN_MASTER} ${code} ${YARP_PLUGIN_GEN}/add_${YARP_PLUGIN_MASTER}_plugins.cpp)
    target_link_libraries(${YARP_PLUGIN_MASTER} ${YARP_LIBRARIES})
    get_property_fix(libs GLOBAL PROPERTY YARP_BUNDLE_LIBS)
    target_link_libraries(${YARP_PLUGIN_MASTER} ${libs})
    set(${YARP_PLUGIN_MASTER}_LIBRARIES ${libs})
    set(YARP_PLUGIN_MODE FALSE) # neutralize redefined methods 
  endif ("${bundle_name}" STREQUAL "${YARP_PLUGIN_MASTER}")
endmacro(END_PLUGIN_LIBRARY bundle_name)

MACRO(ADD_PLUGIN_LIBRARY_EXECUTABLE exename bundle_name)
  CONFIGURE_FILE(${YARP_MODULE_PATH}/template/yarpdev_lib_main.cpp.in
    ${YARP_PLUGIN_GEN}/yarpdev_${bundle_name}.cpp @ONLY  IMMEDIATE)
    ADD_EXECUTABLE(${exename} ${YARP_PLUGIN_GEN}/yarpdev_${bundle_name}.cpp)
    TARGET_LINK_LIBRARIES(${exename} ${bundle_name})
ENDMACRO(ADD_PLUGIN_LIBRARY_EXECUTABLE)


## We skipped this whole file if it was already included
ENDIF (NOT COMMAND END_PLUGIN_LIBRARY)

