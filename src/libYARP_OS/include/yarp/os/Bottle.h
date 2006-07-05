// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-
#ifndef _YARP2_BOTTLE_
#define _YARP2_BOTTLE_

#include <yarp/os/ConstString.h>
#include <yarp/os/Portable.h>
#include <yarp/os/Value.h>

#define BOTTLE_TAG_INT 1
#define BOTTLE_TAG_VOCAB (1+8)
#define BOTTLE_TAG_DOUBLE (2+8)
#define BOTTLE_TAG_STRING (4)
#define BOTTLE_TAG_BLOB (4+8)
#define BOTTLE_TAG_LIST 256

namespace yarp {
    namespace os {
        class Bottle;
    }
}


/**
 * \ingroup key_class
 *
 * A simple collection of objects that can be
 * described and transmitted in a portable way.
 * Objects are stored in a list, which you can add to and access.
 * It is handy to use until you feel the need to make your own more 
 * efficient formats for transmission.
 *
 * Here's an example of using a Bottle:
 * \include example/os/bottle_add.cpp
 *
 * This class has a well-defined, documented representation in both
 * binary and text form.  The name of this class comes from the idea
 * of throwing a "message in a bottle" into the network and hoping it
 * will eventually wash ashore somewhere else.  In the very early days
 * of YARP, that is what communication felt like.
 */
class yarp::os::Bottle : public Portable, public Searchable {
public:
    using Searchable::check;

    /**
     * Constructor.  The bottle is initially empty.
     */
    Bottle();

    /**
     * Creates a bottle from a string. The string which should contain a textual
     * form of the bottle, e.g. text = "10 -5.3 1.0 \"hello there\"" 
     * would give a bottle with 4 elements: an integer, two floating
     * point numbers, and a string.
     * @param text the textual form of the bottle to be interpreted
     */
    Bottle(const char *text);

    /**
     * Copy constructor.
     * @param bottle The object to copy
     */
    Bottle(const Bottle& bottle);


    /**
     * Empties the bottle of any objects it contains.
     */
    void clear();

    /**
     * Places an integer in the bottle, at the end of the list.
     * @param x the integer to add
     */
    void addInt(int x);

    /**
     * Places a vocabulary item in the bottle, at the end of the list.
     * @param x the item to add
     */
    void addVocab(int x);

    /**
     * Places a floating point number in the bottle, at the end of the list.
     * @param x the number to add
     */
    void addDouble(double x);

    /**
     * Places a string in the bottle, at the end of the list.
     * @param str the string to add
     */
    void addString(const char *str);

    /**
     * Add a Value to the bottle, at the end of the list.
     * @param value the Value to add
     */
    void add(const Value& value);

    /**
     * Add a Value to the bottle, at the end of the list.
     * The object passed will be placed directly into the list, without
     * copying.  The Bottle will be responsible for deallocating it
     * when appropriate.
     * @param value the Value to add
     */
    void add(Value *value);

    /**
     * Places an empty nested list in the bottle, at the end of the list.
     * The list itself is represented as a bottle.
     * @return a reference to the newly added list
     */
    Bottle& addList();

    /**
     * Reads a Value v from a certain part of the list.  Methods like
     * v.isInt() or v.isString() can be used to check the type of the
     * result.  Methods like v.asInt() or v.asString() can be used to
     * access the result as a particular type.
     *
     * @param index the part of the list to read from
     * @return the Value v; if the index lies outside the range of elements
     * present, then v.isNull() will be true.  
     */
    Value& get(int index) const;

    /**
     * Gets the number of elements in the bottle
     * @return number of elements in the bottle
     */
    int size() const;



    /**
     * Initializes bottle from a string, which should contain a textual
     * form of the bottle, e.g. text = "10 -5.3 1.0 \"hello there\"" 
     * would give a bottle with 4 elements: an integer, two floating
     * point numbers, and a string.
     * @param text the textual form of the bottle to be interpreted
     */
    void fromString(const char *text);

    /**
     * Initializes bottle from a binary representation.
     * @param buf the binary form of the bottle to be interpreted
     * @param len the length of the binary form
     */
    void fromBinary(const char *buf, int len);

    /**
     * Gives a human-readable textual representation of the bottle, suitable 
     * for passing to Bottle::fromString (see that method for examples)
     * @return a textual representation of the bottle
     */
    ConstString toString() const;

    /**
     * Output a representation of the bottle to a network connection.
     * @param writer the interface to the network connection for writing
     * @result true iff the representation was written successfully.
     */
    bool write(ConnectionWriter& writer);

    /**
     * Set the bottle's value based on input from a network connection.
     * @param reader the interface to the network connection for reading
     * @return true iff the bottle was read successfully.
     */
    bool read(ConnectionReader& reader);

    virtual bool check(const char *key) {
        Bottle& val = findGroup(key);
        return !val.isNull();
    }

    virtual Value& find(const char *key);

    Bottle& findGroup(const char *key);



    /**
     * Assignment operator.
     * @param bottle The object to copy
     * @return the Bottle itself
     */
    const Bottle& operator = (const Bottle& bottle);


    /**
     * Destructor.
     */
    virtual ~Bottle();



    /**
     * Copy all or part of another Bottle
     * @param alt The object to copy
     * @param first The index of the first element to copy
     * @param len The number of elements to copy (-1 for all)
     */
    void copy(const Bottle& alt, int first = 0, int len = -1);

    /**
     * A special Bottle with no content
     * @return the special invalid "null" Bottle.
     */
    static Bottle& getNullBottle();


    /**
     * Equality test.
     * @param alt the value to compare against
     * @result true iff the values are equal
     */
    virtual bool operator == (const Bottle& alt);

    /**
     * Inequality test.
     * @param alt the value to compare against
     * @result true iff the values are not equal
     */
    virtual bool operator != (const Bottle& alt) {
        return !((*this)==alt);
    }
    
    





#ifndef DOXYGEN_SHOULD_SKIP_THIS
    // this is needed for implementation reasons -- could go away
    // with judicious use of friend declarations
    void specialize(int subCode);
    int getSpecialization();
    void setNested(bool nested);
#endif


private:

    Value& findGroupBit(const char *key);

    virtual Bottle *create() {
        return new Bottle();
    }

    virtual Bottle *clone();

    //Value& find(const char *key);
    void *implementation;


    ///////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////
    // the methods below are no longer used
    ///////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////

    /**
     * Reads an integer from a certain part of the list.
     * @param index the part of the list to read from
     * @return the integer - result is undefined if it is not actually an
     *         integer - see Bottle::isInt
     */
    int getInt(int index);

    /**
     * Reads a string from a certain part of the list.
     * @param index the part of the list to read from
     * @return the string - result is undefined if it is not actually an
     *         string - see Bottle::isString
     */
    ConstString getString(int index);

    /**
     * Reads a floating point number from a certain part of the list.
     * @param index the part of the list to read from
     * @return the floating point number - result is undefined if it is not
     *         actually a floating point number - see Bottle::isDouble
     */
    double getDouble(int index);

    /**
     * Reads a nested list from a certain part of the list.
     * @param index the part of the list to read from
     * @return a pointer to the nested list, or NULL if it isn't a list -
     * see Bottle::isList
     */
    Bottle *getList(int index);

    /**
     * Checks if a certain part of the list is an integer.
     * @param index the part of the list to check
     * @return true iff that part of the list is indeed an integer
     */
    bool isInt(int index);

    /**
     * Checks if a certain part of the list is a floating point number.
     * @param index the part of the list to check
     * @return true iff that part of the list is indeed a floating point number
     */
    bool isDouble(int index);

    /**
     * Checks if a certain part of the list is a string.
     * @param index the part of the list to check
     * @return true iff that part of the list is indeed a string
     */
    bool isString(int index);

    /**
     * Checks if a certain part of the list is a nested list.
     * @param index the part of the list to check
     * @return true iff that part of the list is indeed a nested list
     */
    bool isList(int index);


    virtual bool isList() { return true; }

    //virtual Bottle *asList() { 
    //  return this; 
    //}


};

#endif

