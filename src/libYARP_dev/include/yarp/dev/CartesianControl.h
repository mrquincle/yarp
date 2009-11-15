// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/*
* Copyright (C) 2008 RobotCub Consortium
* Authors: Lorenzo Natale and Ugo Pattacini
* CopyPolicy: Released under the terms of the GNU GPL v2.0.
*/

#ifndef __YARPCARTESIANCONTROLINTERFACES__
#define __YARPCARTESIANCONTROLINTERFACES__

#include <yarp/dev/DeviceDriver.h>
#include <yarp/sig/Vector.h>

/*! \file CartesianController.h define control board standard interfaces*/

namespace yarp {
    namespace dev {
        class ICartesianControl;
    }
}


/**
 * @ingroup dev_iface_motor
 *
 * Interface for a cartesian controller.
 */
class yarp::dev::ICartesianControl
{
public:
    /**
     * Destructor.
     */
    virtual ~ICartesianControl() {}

    /**
    * Set the controller in tracking or non-tracking mode. [wait for
    * reply] 
    * @param f: true for tracking mode, false otherwise. 
    * \note In tracking mode when the controller reachs the target, 
    *       it keeps on running in order to mantain the limb in the
    *       desired pose. In non-tracking mode the controller
    *       releases the limb as soon as the desired pose is
    *       reached.
    * @return true/false on success/failure.
    */
    virtual bool setTrackingMode(const bool f)=0;

    /**
    * Get the current controller mode. [wait for reply]
    * @param f: here is returned true if controller is in tracking 
    *         mode, false otherwise.
    * @return true/false on success/failure. 
    */
    virtual bool getTrackingMode(bool *f)=0;

    /**
    * Get the current pose. [do not wait for reply]
    * @param x: a 3-d vector which is filled with the actual position x,y,z (meters)
    * @param od: a 4-d vector which is filled with the actual orientation
    * using axis-angle representation xa, ya, za, theta (meters and 
    * radians). 
    * @return true/false on success/failure.
    */
    virtual bool getPose(yarp::sig::Vector &x, yarp::sig::Vector &o)=0;

    /**
    * Move the end effector to a specified pose (position
    * and orientation) in cartesian space. [do not wait for reply]
    * @param xd: a 3-d vector which contains the desired position x,y,z
    * @param od: a 4-d vector which contains the desired orientation
    * using axis-angle representation (xa, ya, za, theta). 
    * @param t: set the trajectory duration time (seconds). If t<=0 
    *         (as by default) the current execution time is kept.
    * @return true/false on success/failure.
    */
    virtual bool goToPose(const yarp::sig::Vector &xd, const yarp::sig::Vector &od, const double t=0.0)=0;

    /**
    * Move the end effector to a specified position in cartesian space, 
    * ignore the orientation. [do not wait for reply]
    * @param xd: a 3-d vector which contains the desired position x,y,z 
    * @param t: set the trajectory duration time (seconds). If t<=0 
    *         (as by default) the current execution time is kept. 
    * @return true/false on success/failure.
    */
    virtual bool goToPosition(const yarp::sig::Vector &xd, const double t=0.0)=0;

    /**
    * Move the end effector to a specified pose (position
    * and orientation) in cartesian space. [wait for reply]
    * @param xd: a 3-d vector which contains the desired position x,y,z
    * @param od: a 4-d vector which contains the desired orientation
    * using axis-angle representation (xa, ya, za, theta). 
    * @param t: set the trajectory duration time (seconds). If t<=0 
    *         (as by default) the current execution time is kept.
    * @return true/false on success/failure.
    */
    virtual bool goToPoseSync(const yarp::sig::Vector &xd, const yarp::sig::Vector &od, const double t=0.0)=0;

    /**
    * Move the end effector to a specified position in cartesian space, 
    * ignore the orientation. [wait for reply]
    * @param xd: a 3-d vector which contains the desired position x,y,z 
    * @param t: set the trajectory duration time (seconds). If t<=0 
    *         (as by default) the current execution time is kept. 
    * @return true/false on success/failure.
    */
    virtual bool goToPositionSync(const yarp::sig::Vector &xd, const double t=0.0)=0;

    /**
    * Get the actual desired pose and joints configuration as result
    * of kinematic inversion. [wait for reply] 
    * @param xdcap: a 3-d vector which is filled with the actual 
    *          desired position x,y,z (meters); it may differ from
    *          the commanded xd.
    * @param odcap: a 4-d vector which is filled with the actual 
    * desired orientation using axis-angle representation xa, ya, 
    * za, theta (meters and radians); it may differ from the 
    * commanded od. 
    * @param qdcap: the joints configuration through which the
    *             couple (xdcap,odcap) is achieved (degrees).
    * @return true/false on success/failure.
    */
    virtual bool getDesired(yarp::sig::Vector &xdcap, yarp::sig::Vector &odcap,
                            yarp::sig::Vector &qdcap)=0;

    /**
    * Get the current DOF configuration of the limb. [wait for
    * reply] 
    * @param curDof: a vector which is filled with the actual DOF 
    *           configuration.
    * \note The vector lenght is equal to the number of limb's 
    *       joints; each vector's position is filled with 1 if the
    *       associated joint is controlled (i.e. it is an actuated
    *       DOF), 0 otherwise.
    * @return true/false on success/failure. 
    */
    virtual bool getDOF(yarp::sig::Vector &curDof)=0;

    /**
    * Set a new DOF configuration for the limb. [wait for reply]
    * @param newDof: a vector which contains the new DOF 
    *            configuration.
    * @param curDof: a vector where the DOF configuration is 
    *              returned as it has been processed after the
    *              request (it may differ from newDof due to the
    *              presence of some internal limb's constraints).
    * \note Eeach vector's position shall contain 1 if the 
    *       associated joint can be actuated, 0 otherwise. The
    *       special value 2 indicates that the joint status won't be
    *       modified (useful as a placeholder).
    * @return true/false on success/failure. 
    */
    virtual bool setDOF(const yarp::sig::Vector &newDof, yarp::sig::Vector &curDof)=0;

    /**
    * Get the current range for the axis. [wait for reply]
    * @param axis: joint index (regardless if it is actuated or 
    *            not).
    * @param min: where the minimum value is returned [deg].
    * @param max: where the maximum value is returned [deg].
    * @return true/false on success/failure. 
    */
    virtual bool getLimits(int axis, double *min, double *max)=0;

    /**
    * Set new range for the axis. Allowed range shall be a valid 
    * subset of the real control limits. [wait for reply]
    * @param axis: joint index (regardless it it is actuated or 
    *            not).
    * @param min: the new minimum value [deg]. 
    * @param max: the new maximum value [deg]. 
    * @return true/false on success/failure. 
    */
    virtual bool setLimits(int axis, const double min, const double max)=0;

    /**
    * Get the current trajectory duration. [wait for reply]
    * @param t: time (seconds).
    * @return true/false on success/failure. 
    */
    virtual bool getTrajTime(double *t)=0;

    /**
    * Set the duration of the trajectory. [wait for reply]
    * @param t: time (seconds).
    * @return true/false on success/failure. 
    */
    virtual bool setTrajTime(const double t)=0;

    /**
    * Return tolerance for in-target check. [wait for reply]
    * \note The trajectory is supposed to be completed as soon as 
    *       norm(xd-end_effector)<tol.
    * @return tolerance.
    */
    virtual bool getInTargetTol(double *tol)=0;

    /**
    * Set tolerance for in-target check. [wait for reply]
    * \note The trajectory is supposed to be completed as soon as 
    *       norm(xd-end_effector)<tol.
    * @param tol is the tolerance.
    */
    virtual bool setInTargetTol(const double tol)=0;

    /** Check if the current trajectory is terminated. [wait for
    *   reply]
    * @param f: where the result is returned.
    * @return true if the trajectory is terminated, false otherwise
    */
    virtual bool checkMotionDone(bool *f)=0;

    /** Ask for an immediate stop motion. [wait for reply]
    * \note the control is completely release, i.e. a direct switch 
    *       to non-tracking mode is executed.     
    * @return true/false on success/failure. 
    */
    virtual bool stopControl()=0;
};

#endif

