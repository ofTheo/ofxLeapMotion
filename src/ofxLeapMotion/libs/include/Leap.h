/******************************************************************************\
* Copyright (C) 2012 Leap Motion, Inc. All rights reserved.                    *
* NOTICE: This developer release of Leap Motion, Inc. software is confidential *
* and intended for very limited distribution. Parties using this software must *
* accept the SDK Agreement prior to obtaining this software and related tools. *
* This software is subject to copyright.                                       *
\******************************************************************************/

#if !defined(__Leap_h__)
#define __Leap_h__

#include "LeapMath.h"

#include <string>
#include <vector>

// Define integer types for Visual Studio 2005
#if defined(_MSC_VER) && (_MSC_VER < 1600)
typedef __int32 int32_t;
typedef __int64 int64_t;
typedef unsigned __int32 uint32_t;
typedef unsigned __int64 uint64_t;
#else
#include <stdint.h>
#endif

// Define Leap export macros
#if defined(_WIN32) // Windows
#if LEAP_API_INTERNAL
#define LEAP_EXPORT
#elif LEAP_API_IMPLEMENTATION
#define LEAP_EXPORT __declspec(dllexport)
#else
#define LEAP_EXPORT __declspec(dllimport)
#endif
#define LEAP_EXPORT_CLASS
#define LEAP_EXPORT_PLUGIN __declspec(dllexport)
#elif defined(__APPLE__) // Mac OS
#define LEAP_EXPORT __attribute__((visibility("default")))
#define LEAP_EXPORT_CLASS __attribute__((visibility("default")))
#define LEAP_EXPORT_PLUGIN __attribute__((visibility("default")))
#else // GNU/Linux
#define LEAP_EXPORT
#define LEAP_EXPORT_CLASS
#define LEAP_EXPORT_PLUGIN
#endif

namespace Leap {

// Interface for internal use only
class LEAP_EXPORT_CLASS Interface {
  public:
    struct Implementation {
      LEAP_EXPORT virtual ~Implementation() {}
    };
  protected:
    LEAP_EXPORT Interface(void* owner);
    LEAP_EXPORT Interface(Implementation* reference, void* owner);
    LEAP_EXPORT Interface(const Interface& rhs);
    LEAP_EXPORT Interface& operator=(const Interface& rhs);
    LEAP_EXPORT virtual ~Interface();
    template<typename T> T* get() const { return static_cast<T*>(reference()); }
    class SharedObject* m_object;
  private:
    LEAP_EXPORT Implementation* reference() const;
};

// Forward declarations for internal use only
class PointableImplementation;
class FingerImplementation;
class ToolImplementation;
class HandImplementation;
class FrameImplementation;
class ControllerImplementation;
template<typename T> class ListBaseImplementation;

// Forward declarations
class PointableList;
class FingerList;
class ToolList;
class Hand;
class Frame;
class Listener;

/// The Pointable class reports the physical characteristics of a detected finger or tool.
///
/// Both fingers and tools are classified as Pointable objects. Use the Pointable::isFinger()
/// function to determine whether a Pointable object represents a finger. Use the
/// Pointable::isTool() function to determine whether a Pointable object represents a tool.
/// The Leap classifies a detected entity as a tool when it is thinner, straighter, and longer
/// than a typical finger.
///
/// Note that Pointable objects can be invalid, which means that they do not contain
/// valid tracking data and do not correspond to a physical entity. Invalid Pointable
/// objects can be the result of asking for a Pointable object using an ID from an
/// earlier frame when no Pointable objects with that ID exist in the current frame.
/// A Pointable object created from the Pointable constructor is also invalid.
/// Test for validity with the Pointable::isValid() function.
class Pointable : public Interface {
  public:
#if !defined(SWIG)
    // For internal use only.
    Pointable(PointableImplementation*);
    // For internal use only.
    Pointable(FingerImplementation*);
    // For internal use only.
    Pointable(ToolImplementation*);
#endif

    /// Constructs a Pointable object.
    ///
    /// An uninitialized pointable is considered invalid.
    /// Get valid Pointable objects from a Frame or a Hand object.
    LEAP_EXPORT Pointable();

    /// A unique ID assigned to this Pointable object, whose value remains the
    /// same across consecutive frames while the tracked finger or tool remains
    /// visible. If tracking is lost (for example, when a finger is occluded by
    /// another finger or when it is withdrawn from the Leap field of view), the
    /// Leap may assign a new ID when it detects the entity in a future frame.
    ///
    /// Use the ID value with the Frame::pointable() function to find this
    /// Pointable object in future frames.
    ///
    /// @returns The ID assigned to this Pointable object.
    LEAP_EXPORT int32_t id() const;

    /// The Frame associated with this Pointable object.
    ///
    /// @returns The associated Frame object, if available; otherwise,
    /// an invalid Frame object is returned.
    LEAP_EXPORT Frame frame() const;

    /// The Hand associated with this finger or tool.
    ///
    /// @returns The associated Hand object, if available; otherwise,
    /// an invalid Hand object is returned.
    LEAP_EXPORT Hand hand() const;

    /// The tip position in millimeters from the Leap origin.
    ///
    /// @returns The Vector containing the coordinates of the tip position.
    LEAP_EXPORT Vector tipPosition() const;

    /// The rate of change of the tip position in millimeters/second.
    ///
    /// @returns The Vector containing the coordinates of the tip velocity.
    LEAP_EXPORT Vector tipVelocity() const;

    /// The direction in which this finger or tool is pointing.
    ///
    /// The direction is expressed as a unit vector pointing in the same
    /// direction as the tip.
    ///
    /// \image html images/Leap_Finger_Model.png
    ///
    /// @returns The Vector pointing in the same direction as the tip of this
    /// Pointable object.
    LEAP_EXPORT Vector direction() const;

    /// The estimated width of the finger or tool in millimeters.
    ///
    /// The reported width is the average width of the visible portion of the
    /// finger or tool from the hand to the tip. If the width isn't known,
    /// then a value of 0 is returned.
    ///
    /// @returns The estimated width of this Pointable object.
    LEAP_EXPORT float width() const;

    /// The estimated length of the finger or tool in millimeters.
    ///
    /// The reported length is the visible length of the finger or tool from the
    /// hand to tip. If the length isn't known, then a value of 0 is returned.
    ///
    /// @returns The estimated length of this Pointable object.
    LEAP_EXPORT float length() const;

    /// Whether or not the Pointable is believed to be a finger.
    /// Fingers are generally shorter, thicker, and less straight than tools.
    ///
    /// @returns True, if this Pointable is classified as a finger.
    LEAP_EXPORT bool isFinger() const;

    /// Whether or not the Pointable is believed to be a tool.
    /// Tools are generally longer, thinner, and straighter than fingers.
    ///
    /// @returns True, if this Pointable is classified as a tool.
    LEAP_EXPORT bool isTool() const;

    /// Reports whether this is a valid Pointable object.
    ///
    /// @returns True, if this Pointable object contains valid tracking data.
    LEAP_EXPORT bool isValid() const;

    /// Returns an invalid Pointable object.
    ///
    /// You can use the instance returned by this function in comparisons testing
    /// whether a given Pointable instance is valid or invalid. (You can also use the
    /// Pointable::isValid() function.)
    ///
    /// @returns The invalid Pointable instance.
    LEAP_EXPORT static const Pointable& invalid();

    /// Compare Pointable object equality.
    /// Two Pointable objects are equal if and only if both Pointable objects represent the
    /// exact same physical entities in the same frame and both Pointable objects are valid.
    LEAP_EXPORT bool operator==(const Pointable&) const;

    /// Compare Pointable object inequality.
    /// Two Pointable objects are equal if and only if both Pointable objects represent the
    /// exact same physical entities in the same frame and both Pointable objects are valid.
    LEAP_EXPORT bool operator!=(const Pointable&) const;

    /// Writes a brief, human readable description of the Pointable object to an output stream.
    LEAP_EXPORT friend std::ostream& operator<<(std::ostream&, const Pointable&);

    /// A string containing a brief, human readable description of the Pointable object.
    ///
    /// @returns A description of the Pointable object as a string.
    LEAP_EXPORT std::string toString() const;
};

/// The Finger class represents a tracked finger.
///
/// Fingers are Pointable objects that the Leap has classified as a finger.
/// Get valid Finger objects from a Frame or a Hand object.
///
/// Note that Finger objects can be invalid, which means that they do not contain
/// valid tracking data and do not correspond to a physical finger. Invalid Finger
/// objects can be the result of asking for a Finger object using an ID from an
/// earlier frame when no Finger objects with that ID exist in the current frame.
/// A Finger object created from the Finger constructor is also invalid.
/// Test for validity with the Finger::isValid() function.
class Finger : public Pointable {
  public:
#if !defined(SWIG)
    // For internal use only.
    Finger(FingerImplementation*);
#endif

    /// Constructs a Finger object.
    ///
    /// An uninitialized finger is considered invalid.
    /// Get valid Finger objects from a Frame or a Hand object.
    LEAP_EXPORT Finger();

    /// If the specified Pointable object represents a finger, creates a copy
    /// of it as a Finger object; otherwise, creates an invalid Finger object.
    LEAP_EXPORT Finger(const Pointable&);

    /// Returns an invalid Finger object.
    ///
    /// You can use the instance returned by this function in comparisons testing
    /// whether a given Finger instance is valid or invalid. (You can also use the
    /// Finger::isValid() function.)
    ///
    /// @returns The invalid Finger instance.
    LEAP_EXPORT static const Finger& invalid();

    /// A string containing a brief, human readable description of the Finger object.
    ///
    /// @returns A description of the Finger object as a string.
    LEAP_EXPORT std::string toString() const;
};

/// The Tool class represents a tracked tool.
///
/// Tools are Pointable objects that the Leap has classified as a tool.
/// Tools are longer, thinner, and straighter than a typical finger.
/// Get valid Tool objects from a Frame or a Hand object.
///
/// \image html images/Leap_Tool.png
///
/// Note that Tool objects can be invalid, which means that they do not contain
/// valid tracking data and do not correspond to a physical tool. Invalid Tool
/// objects can be the result of asking for a Tool object using an ID from an
/// earlier frame when no Tool objects with that ID exist in the current frame.
/// A Tool object created from the Tool constructor is also invalid.
/// Test for validity with the Tool::isValid() function.
class Tool : public Pointable {
  public:
#if !defined(SWIG)
    // For internal use only.
    Tool(ToolImplementation*);
#endif

    /// Constructs a Tool object.
    ///
    /// An uninitialized tool is considered invalid.
    /// Get valid Tool objects from a Frame or a Hand object.
    LEAP_EXPORT Tool();

    /// If the specified Pointable object represents a tool, creates a copy
    /// of it as a Tool object; otherwise, creates an invalid Tool object.
    LEAP_EXPORT Tool(const Pointable&);

    /// Returns an invalid Tool object.
    ///
    /// You can use the instance returned by this function in comparisons testing
    /// whether a given Tool instance is valid or invalid. (You can also use the
    /// Tool::isValid() function.)
    ///
    /// @returns The invalid Tool instance.
    LEAP_EXPORT static const Tool& invalid();

    /// A string containing a brief, human readable description of the Tool object.
    ///
    /// @returns A description of the Tool object as a string.
    LEAP_EXPORT std::string toString() const;
};

/// The Hand class reports the physical characteristics of a detected hand.
///
/// Hand tracking data includes a palm position and velocity; vectors for
/// the palm normal and direction to the fingers; properties of a sphere fit
/// to the hand; and lists of the attached fingers and tools.
///
/// Note that Hand objects can be invalid, which means that they do not contain
/// valid tracking data and do not correspond to a physical entity. Invalid Hand
/// objects can be the result of asking for a Hand object using an ID from an
/// earlier frame when no Hand objects with that ID exist in the current frame.
/// A Hand object created from the Hand constructor is also invalid.
/// Test for validity with the Hand::isValid() function.
class Hand : public Interface {
  public:
#if !defined(SWIG)
    // For internal use only.
    Hand(HandImplementation*);
#endif

    /// Constructs a Hand object.
    ///
    /// An uninitialized hand is considered invalid.
    /// Get valid Hand objects from a Frame object.
    LEAP_EXPORT Hand();

    /// A unique ID assigned to this Hand object, whose value remains the same
    /// across consecutive frames while the tracked hand remains visible. If
    /// tracking is lost (for example, when a hand is occluded by another hand
    /// or when it is withdrawn from or reaches the edge of the Leap field of view),
    /// the Leap may assign a new ID when it detects the hand in a future frame.
    ///
    /// Use the ID value with the Frame::hand() function to find this Hand object
    /// in future frames.
    ///
    /// @returns The ID of this hand.
    LEAP_EXPORT int32_t id() const;

    /// The Frame associated with this Hand.
    ///
    /// @returns The associated Frame object, if available; otherwise,
    /// an invalid Frame object is returned.
    LEAP_EXPORT Frame frame() const;

    /// The list of Pointable objects (fingers and tools) detected in this frame
    /// that are associated with this hand, given in arbitrary order. The list
    /// can be empty if no fingers or tools associated with this hand are detected.
    ///
    /// Use the Pointable::isFinger() function to determine whether or not an
    /// item in the list represents a finger. Use the Pointable::isTool() function
    /// to determine whether or not an item in the list represents a tool.
    /// You can also get only fingers using the Hand::fingers() function or
    /// only tools using the Hand::tools() function.
    ///
    /// @returns The PointableList containing all Pointable objects associated with this hand.
    LEAP_EXPORT PointableList pointables() const;

    /// The Pointable object with the specified ID associated with this hand.
    ///
    /// Use the Hand::pointable() function to retrieve a Pointable object
    /// associated with this hand using an ID value obtained from a previous frame.
    /// This function always returns a Pointable object, but if no finger or tool
    /// with the specified ID is present, an invalid Pointable object is returned.
    ///
    /// Note that ID values persist across frames, but only until tracking of a
    /// particular object is lost. If tracking of a finger or tool is lost and subsequently
    /// regained, the new Pointable object representing that finger or tool may have a
    /// different ID than that representing the finger or tool in an earlier frame.
    ///
    /// @param id The ID value of a Pointable object from a previous frame.
    /// @returns The Pointable object with the matching ID if one exists for this
    /// hand in this frame; otherwise, an invalid Pointable object is returned.
    LEAP_EXPORT Pointable pointable(int32_t id) const;

    /// The list of Finger objects detected in this frame that are attached to
    /// this hand, given in arbitrary order.
    /// The list can be empty if no fingers attached to this hand are detected.
    ///
    /// @returns The FingerList containing all Finger objects attached to this hand.
    LEAP_EXPORT FingerList fingers() const;

    /// The Finger object with the specified ID attached to this hand.
    ///
    /// Use the Hand::finger() function to retrieve a Finger object attached to
    /// this hand using an ID value obtained from a previous frame.
    /// This function always returns a Finger object, but if no finger
    /// with the specified ID is present, an invalid Finger object is returned.
    ///
    /// Note that ID values persist across frames, but only until tracking of a
    /// particular object is lost. If tracking of a finger is lost and subsequently
    /// regained, the new Finger object representing that finger may have a
    /// different ID than that representing the finger in an earlier frame.
    ///
    /// @param id The ID value of a Finger object from a previous frame.
    /// @returns The Finger object with the matching ID if one exists for this
    /// hand in this frame; otherwise, an invalid Finger object is returned.
    LEAP_EXPORT Finger finger(int32_t id) const;

    /// The list of Tool objects detected in this frame that are held by this
    /// hand, given in arbitrary order.
    /// The list can be empty if no tools held by this hand are detected.
    ///
    /// @returns The ToolList containing all Tool objects held by this hand.
    LEAP_EXPORT ToolList tools() const;

    /// The Tool object with the specified ID held by this hand.
    ///
    /// Use the Hand::tool() function to retrieve a Tool object held by
    /// this hand using an ID value obtained from a previous frame.
    /// This function always returns a Tool object, but if no tool
    /// with the specified ID is present, an invalid Tool object is returned.
    ///
    /// Note that ID values persist across frames, but only until tracking of a
    /// particular object is lost. If tracking of a tool is lost and subsequently
    /// regained, the new Tool object representing that tool may have a
    /// different ID than that representing the tool in an earlier frame.
    ///
    /// @param id The ID value of a Tool object from a previous frame.
    /// @returns The Tool object with the matching ID if one exists for this
    /// hand in this frame; otherwise, an invalid Tool object is returned.
    LEAP_EXPORT Tool tool(int32_t id) const;

    /// The center position of the palm in millimeters from the Leap origin.
    ///
    /// @returns The Vector representing the coordinates of the palm position.
    LEAP_EXPORT Vector palmPosition() const;

    /// The rate of change of the palm position in millimeters/second.
    ///
    /// @returns The Vector representing the coordinates of the palm velocity.
    LEAP_EXPORT Vector palmVelocity() const;

    /// The normal vector to the palm. If your hand is flat, this vector will
    /// point downward, or "out" of the front surface of your palm.
    ///
    /// \image html images/Leap_Palm_Vectors.png
    ///
    /// The direction is expressed as a unit vector pointing in the same
    /// direction as the palm normal (that is, a vector orthogonal to the palm).
    ///
    /// @returns The Vector normal to the plane formed by the palm.
    LEAP_EXPORT Vector palmNormal() const;

    /// The direction from the palm position toward the fingers.
    ///
    /// The direction is expressed as a unit vector pointing in the same
    /// direction as the directed line from the palm position to the fingers.
    ///
    /// @returns The Vector pointing from the palm position toward the fingers.
    LEAP_EXPORT Vector direction() const;

    /// The center of a sphere fit to the curvature of this hand.
    ///
    /// This sphere is placed roughly as if the hand were holding a ball.
    ///
    /// \image html images/Leap_Hand_Ball.png
    ///
    /// @returns The Vector representing the center position of the sphere.
    LEAP_EXPORT Vector sphereCenter() const;

    /// The radius of a sphere fit to the curvature of this hand.
    ///
    /// This sphere is placed roughly as if the hand were holding a ball. Thus the
    /// size of the sphere decreases as the fingers are curled into a fist.
    /// @returns The radius of the sphere in millimeters.
    LEAP_EXPORT float sphereRadius() const;

    /// Reports whether this is a valid Hand object.
    ///
    /// @returns True, if this Hand object contains valid tracking data.
    LEAP_EXPORT bool isValid() const;

    /// Returns an invalid Hand object.
    ///
    /// You can use the instance returned by this function in comparisons testing
    /// whether a given Hand instance is valid or invalid. (You can also use the
    /// Hand::isValid() function.)
    ///
    /// @returns The invalid Hand instance.
    LEAP_EXPORT static const Hand& invalid();

    /// Compare Hand object equality.
    /// Two Hand objects are equal if and only if both Hand objects represent the
    /// exact same physical hand in the same frame and both Hand objects are valid.
    LEAP_EXPORT bool operator==(const Hand&) const;

    /// Compare Hand object inequality.
    /// Two Hand objects are equal if and only if both Hand objects represent the
    /// exact same physical hand in the same frame and both Hand objects are valid.
    LEAP_EXPORT bool operator!=(const Hand&) const;

    /// Writes a brief, human readable description of the Hand object to an output stream.
    LEAP_EXPORT friend std::ostream& operator<<(std::ostream&, const Hand&);

    /// A string containing a brief, human readable description of the Hand object.
    ///
    /// @returns A description of the Hand as a string.
    LEAP_EXPORT std::string toString() const;
};

// For internal use only.
template<typename L, typename T>
class ConstListIterator {
  public:
    ConstListIterator<L,T>(const L& list, int index) : m_list(list), m_index(index) {}

    const T operator*() const { return m_list[m_index]; }
    const ConstListIterator<L,T>& operator++() { ++m_index; return *this; }
    bool operator!=(const ConstListIterator<L,T>& rhs) const { return m_index != rhs.m_index; }

  private:
    const L& m_list;
    int m_index;
};

/// The PointableList class represents a list of Pointable objects.
///
/// Pointable objects include entities that can be pointed, such as fingers and tools.
///
/// Get a PointableList object by calling Frame::pointables().
class PointableList : public Interface {
  public:
    // For internal use only.
    PointableList(const ListBaseImplementation<Pointable>&);

    /// Constructs an empty list of pointable entities.
    LEAP_EXPORT PointableList();

    /// Returns the number of pointable entities in this list.
    /// @returns The number of pointable entities in this list.
    LEAP_EXPORT int count() const;

    /// Reports whether the list is empty.
    /// @returns True, if the list has no members.
    LEAP_EXPORT bool empty() const;

    /// Access a list member by its position in the list.
    /// @param index The zero-based list position index.
    /// @returns The Pointable object at the specified index.
    LEAP_EXPORT Pointable operator[](int index) const;

    /// Appends the members of the specifed PointableList to this PointableList.
    /// @param other A PointableList object containing Pointable objects
    /// to append to the end of this PointableList.
    LEAP_EXPORT PointableList& append(const PointableList& other);

    /// Appends the members of the specifed FingerList to this PointableList.
    /// @param other A FingerList object containing Finger objects
    /// to append to the end of this PointableList.
    LEAP_EXPORT PointableList& append(const FingerList& other);

    /// Appends the members of the specifed ToolList to this PointableList.
    /// @param other A ToolList object containing Tool objects
    /// to append to the end of this PointableList.
    LEAP_EXPORT PointableList& append(const ToolList& other);

    /// A C++ iterator type for PointableList objects.
    typedef ConstListIterator<PointableList, Pointable> const_iterator;

    /// The C++ iterator set to the beginning of this PointableList.
    LEAP_EXPORT const_iterator begin() const;

    /// The C++ iterator set to the end of this PointableList.
    LEAP_EXPORT const_iterator end() const;
};

/// The FingerList class represents a list of Finger objects.
///
/// Get a FingerList object by calling Frame::fingers().
class FingerList : public Interface {
  public:
    // For internal use only.
    FingerList(const ListBaseImplementation<Finger>&);

    /// Constructs an empty list of fingers.
    LEAP_EXPORT FingerList();

    /// Returns the number of fingers in this list.
    /// @returns The number of fingers in this list.
    LEAP_EXPORT int count() const;

    /// Reports whether the list is empty.
    /// @returns True, if the list has no members.
    LEAP_EXPORT bool empty() const;

    /// Access a list member by its position in the list.
    /// @param index The zero-based list position index.
    /// @returns The Finger object at the specified index.
    LEAP_EXPORT Finger operator[](int index) const;

    /// Appends the members of the specifed FingerList to this FingerList.
    /// @param other A FingerList object containing Finger objects
    /// to append to the end of this FingerList.
    LEAP_EXPORT FingerList& append(const FingerList& other);

    /// A C++ iterator type for FingerList objects.
    typedef ConstListIterator<FingerList, Finger> const_iterator;

    /// The C++ iterator set to the beginning of this FingerList.
    LEAP_EXPORT const_iterator begin() const;

    /// The C++ iterator set to the end of this FingerList.
    LEAP_EXPORT const_iterator end() const;

    friend class PointableList;
};

/// The ToolList class represents a list of Tool objects.
///
/// Get a ToolList object by calling Frame::tools().
class ToolList : public Interface {
  public:
    // For internal use only.
    ToolList(const ListBaseImplementation<Tool>&);

    /// Constructs an empty list of tools.
    LEAP_EXPORT ToolList();

    /// Returns the number of tools in this list.
    /// @returns The number of tools in this list.
    LEAP_EXPORT int count() const;

    /// Reports whether the list is empty.
    /// @returns True, if the list has no members.
    LEAP_EXPORT bool empty() const;

    /// Access a list member by its position in the list.
    /// @param index The zero-based list position index.
    /// @returns The Tool object at the specified index.
    LEAP_EXPORT Tool operator[](int index) const;

    /// Appends the members of the specifed ToolList to this ToolList.
    /// @param other A ToolList object containing Tool objects
    /// to append to the end of this ToolList.
    LEAP_EXPORT ToolList& append(const ToolList& other);

    /// A C++ iterator type for ToolList objects.
    typedef ConstListIterator<ToolList, Tool> const_iterator;

    /// The C++ iterator set to the beginning of this ToolList.
    LEAP_EXPORT const_iterator begin() const;

    /// The C++ iterator set to the end of this ToolList.
    LEAP_EXPORT const_iterator end() const;

    friend class PointableList;
};

/// The HandList class represents a list of Hand objects.
///
/// Get a HandList object by calling Frame::hands().
class HandList : public Interface {
  public:
    // For internal use only.
    HandList(const ListBaseImplementation<Hand>&);

    /// Constructs an empty list of hands.
    LEAP_EXPORT HandList();

    /// Returns the number of hands in this list.
    /// @returns The number of hands in this list.
    LEAP_EXPORT int count() const;

    /// Reports whether the list is empty.
    /// @returns True, if the list has no members.
    LEAP_EXPORT bool empty() const;

    /// Access a list member by its position in the list.
    /// @param index The zero-based list position index.
    /// @returns The Hand object at the specified index.
    LEAP_EXPORT Hand operator[](int index) const;

    /// Appends the members of the specifed HandList to this HandList.
    /// @param other A HandList object containing Hand objects
    /// to append to the end of this HandList.
    LEAP_EXPORT HandList& append(const HandList& other);

    /// A C++ iterator type for this HandList objects.
    typedef ConstListIterator<HandList, Hand> const_iterator;

    /// The C++ iterator set to the beginning of this HandList.
    LEAP_EXPORT const_iterator begin() const;

    /// The C++ iterator set to the end of this HandList.
    LEAP_EXPORT const_iterator end() const;
};

/// The Frame class represents a set of hand and finger tracking data detected
/// in a single frame.
///
/// The Leap detects hands, fingers and tools within the tracking area, reporting
/// their positions and orientations in frames generated at the Leap frame rate.
///
/// Access Frame objects through an instance of a Leap Controller. Implement a
/// Listener subclass to receive a callback event when a new Frame is available.
class Frame : public Interface {
  public:
#if !defined(SWIG)
    // For internal use only.
    Frame(FrameImplementation*);
#endif

    /// Constructs a Frame object.
    ///
    /// Frame instances created with this constructor are invalid.
    /// Get valid Frame objects by calling the Controller::frame() function.
    LEAP_EXPORT Frame();

    /// A unique ID for this Frame. Consecutive frames processed by the Leap
    /// have consecutive increasing values.
    ///
    /// @returns The frame ID.
    LEAP_EXPORT int64_t id() const;

    /// The frame capture time in microseconds elapsed since the Leap started.
    ///
    /// @returns The timestamp in microseconds.
    LEAP_EXPORT int64_t timestamp() const;

    /// The list of Hand objects detected in this frame, given in arbitrary order.
    /// The list can be empty if no hands are detected.
    ///
    /// @returns The HandList containing all Hand objects detected in this frame.
    LEAP_EXPORT HandList hands() const;

    /// The Hand object with the specified ID in this frame.
    ///
    /// Use the Frame::hand() function to retrieve the Hand object from
    /// this frame using an ID value obtained from a previous frame.
    /// This function always returns a Hand object, but if no hand
    /// with the specified ID is present, an invalid Hand object is returned.
    ///
    /// Note that ID values persist across frames, but only until tracking of a
    /// particular object is lost. If tracking of a hand is lost and subsequently
    /// regained, the new Hand object representing that physical hand may have
    /// a different ID than that representing the physical hand in an earlier frame.
    ///
    /// @param id The ID value of a Hand object from a previous frame.
    /// @returns The Hand object with the matching ID if one exists in this frame;
    /// otherwise, an invalid Hand object is returned.
    LEAP_EXPORT Hand hand(int32_t id) const;

    /// The list of Pointable objects (fingers and tools) detected in this frame,
    /// given in arbitrary order. The list can be empty if no fingers or tools are detected.
    ///
    /// @returns The PointableList containing all Pointable objects detected in this frame.
    LEAP_EXPORT PointableList pointables() const;

    /// The Pointable object with the specified ID in this frame.
    ///
    /// Use the Frame::pointable() function to retrieve the Pointable object from
    /// this frame using an ID value obtained from a previous frame.
    /// This function always returns a Pointable object, but if no finger or tool
    /// with the specified ID is present, an invalid Pointable object is returned.
    ///
    /// Note that ID values persist across frames, but only until tracking of a
    /// particular object is lost. If tracking of a finger or tool is lost and subsequently
    /// regained, the new Pointable object representing that finger or tool may have
    /// a different ID than that representing the finger or tool in an earlier frame.
    ///
    /// @param id The ID value of a Pointable object from a previous frame.
    /// @returns The Pointable object with the matching ID if one exists in this frame;
    /// otherwise, an invalid Pointable object is returned.
    LEAP_EXPORT Pointable pointable(int32_t id) const;

    /// The list of Finger objects detected in this frame, given in arbitrary order.
    /// The list can be empty if no fingers are detected.
    ///
    /// @returns The FingerList containing all Finger objects detected in this frame.
    LEAP_EXPORT FingerList fingers() const;

    /// The Finger object with the specified ID in this frame.
    ///
    /// Use the Frame::finger() function to retrieve the Finger object from
    /// this frame using an ID value obtained from a previous frame.
    /// This function always returns a Finger object, but if no finger
    /// with the specified ID is present, an invalid Finger object is returned.
    ///
    /// Note that ID values persist across frames, but only until tracking of a
    /// particular object is lost. If tracking of a finger is lost and subsequently
    /// regained, the new Finger object representing that physical finger may have
    /// a different ID than that representing the finger in an earlier frame.
    ///
    /// @param id The ID value of a Finger object from a previous frame.
    /// @returns The Finger object with the matching ID if one exists in this frame;
    /// otherwise, an invalid Finger object is returned.
    LEAP_EXPORT Finger finger(int32_t id) const;

    /// The list of Tool objects detected in this frame, given in arbitrary order.
    /// The list can be empty if no tools are detected.
    ///
    /// @returns The ToolList containing all Tool objects detected in this frame.
    LEAP_EXPORT ToolList tools() const;

    /// The Tool object with the specified ID in this frame.
    ///
    /// Use the Frame::tool() function to retrieve the Tool object from
    /// this frame using an ID value obtained from a previous frame.
    /// This function always returns a Tool object, but if no tool
    /// with the specified ID is present, an invalid Tool object is returned.
    ///
    /// Note that ID values persist across frames, but only until tracking of a
    /// particular object is lost. If tracking of a tool is lost and subsequently
    /// regained, the new Tool object representing that tool may have a
    /// different ID than that representing the tool in an earlier frame.
    ///
    /// @param id The ID value of a Tool object from a previous frame.
    /// @returns The Tool object with the matching ID if one exists in this frame;
    /// otherwise, an invalid Tool object is returned.
    LEAP_EXPORT Tool tool(int32_t id) const;

    /// Reports whether this Frame instance is valid.
    ///
    /// A valid Frame is one generated by the Leap::Controller object that contains
    /// tracking data for all detected entities. An invalid Frame contains no
    /// actual tracking data, but you can call its functions without risk of a
    /// null pointer exception. The invalid Frame mechanism makes it more
    /// convenient to track individual data across the frame history. For example,
    /// you can invoke:
    ///
    /// \code{.cpp}
    /// Finger finger = Controller::frame(n).finger(fingerID);
    /// \endcode
    ///
    /// for an arbitrary Frame history value, "n", without first checking whether
    /// frame(n) returned a null object. (You should still check that the
    /// returned Finger instance is valid.)
    ///
    /// @returns True, if this is a valid Frame object; false otherwise.
    LEAP_EXPORT bool isValid() const;

    /// Returns an invalid Frame object.
    ///
    /// You can use the instance returned by this function in comparisons testing
    /// whether a given Frame instance is valid or invalid. (You can also use the
    /// Frame::isValid() function.)
    ///
    /// @returns The invalid Frame instance.
    LEAP_EXPORT static const Frame& invalid();

    /// Compare Frame object equality.
    /// Two Frame objects are equal if and only if both Frame objects represent
    /// the exact same frame of tracking data and both Frame objects are valid.
    LEAP_EXPORT bool operator==(const Frame&) const;

    /// Compare Frame object inequality.
    /// Two Frame objects are equal if and only if both Frame objects represent
    /// the exact same frame of tracking data and both Frame objects are valid.
    LEAP_EXPORT bool operator!=(const Frame&) const;

    /// Writes a brief, human readable description of the Frame object to an output stream.
    LEAP_EXPORT friend std::ostream& operator<<(std::ostream&, const Frame&);

    /// A string containing a brief, human readable description of the Frame object.
    ///
    /// @returns A description of the Frame as a string.
    LEAP_EXPORT std::string toString() const;
};

/// The Config class provides access to Leap system configuration information.
///
/// Reserved for future use; the keys necessary to obtain configuration values
/// are not yet documented.
class Config : public Interface {
  public:
    /// Constructs a Config object.
    LEAP_EXPORT Config();

    /// Enumerates the possible data types for configuration values.
    ///
    /// The Config::type() function returns an item from the ValueType enumeration.
    enum ValueType {
      TYPE_UNKNOWN, ///< The data type is unknown.
      TYPE_BOOLEAN, ///< A boolean value.
      TYPE_INT32,   ///< A 32-bit integer.
      TYPE_INT64,   ///< A 64-bit integer.
      TYPE_UINT32,  ///< A 32-bit unsigned integer.
      TYPE_UINT64,  ///< A 64-bit unsigned integer.
      TYPE_FLOAT,   ///< A floating-point number.
      TYPE_DOUBLE,  ///< A double precision floating-point number.
      TYPE_STRING   ///< A string of characters.
    };

    /// Reports the natural data type for the value related to the specified key.
    ///
    /// @param key The key for the looking up the value in the configuration dictionary.
    /// @returns The native data type of the value, that is, the type that does not
    /// require a data conversion.
    LEAP_EXPORT ValueType type(const std::string& key) const;

    /// Reports whether the value is an array of homogeneous objects. The type
    /// of the objects in the array is returned using the Config::type() function.
    LEAP_EXPORT bool isArray(const std::string& key) const;

    /// Gets the boolean representation for the specified key.
    LEAP_EXPORT bool getBool(const std::string& key) const;

    /// Gets the 32-bit integer representation for the specified key.
    LEAP_EXPORT int32_t getInt32(const std::string& key) const;

    /// Gets the 64-bit integer representation for the specified key.
    LEAP_EXPORT int64_t getInt64(const std::string& key) const;

    /// Gets the unsigned 32-bit integer representation for the specified key.
    LEAP_EXPORT uint32_t getUInt32(const std::string& key) const;

    /// Gets the unsigned 64-bit integer representation for the specified key.
    LEAP_EXPORT uint64_t getUInt64(const std::string& key) const;

    /// Gets the floating point representation for the specified key.
    LEAP_EXPORT float getFloat(const std::string& key) const;

    /// Gets the double precision representation for the specified key.
    LEAP_EXPORT double getDouble(const std::string& key) const;

    /// Gets the string representation for the specified key.
    LEAP_EXPORT std::string getString(const std::string& key) const;

    /// Gets the boolean array representation for the specified key.
    LEAP_EXPORT std::vector<bool> getBoolArray(const std::string& key) const;

    /// Gets the 32-bit integer array representation for the specified key.
    LEAP_EXPORT std::vector<int32_t> getInt32Array(const std::string& key) const;

    /// Gets the 64-bit integer array representation for the specified key.
    LEAP_EXPORT std::vector<int64_t> getInt64Array(const std::string& key) const;

    /// Gets the unsigned 32-bit integer array representation for the specified key.
    LEAP_EXPORT std::vector<uint32_t> getUInt32Array(const std::string& key) const;

    /// Gets the unsigned 64-bit integer array representation for the specified key.
    LEAP_EXPORT std::vector<uint64_t> getUInt64Array(const std::string& key) const;

    /// Gets the floating point array representation for the specified key.
    LEAP_EXPORT std::vector<float> getFloatArray(const std::string& key) const;

    /// Gets the double precision array representation for the specified key.
    LEAP_EXPORT std::vector<double> getDoubleArray(const std::string& key) const;

    /// Gets the string array representation for the specified key.
    LEAP_EXPORT std::vector<std::string> getStringArray(const std::string& key) const;
};

/// The Controller class is your main interface to the Leap device.
///
/// Create an instance of this Controller class to access frames of tracking
/// data and configuration information. Frame data can be polled at any time
/// using the Controller::frame() function. Call frame() or frame(0) to get the
/// most recent frame. Set the history parameter to a positive integer to access
/// previous frames. A controller stores up to 60 frames in its frame history.
///
/// Polling is an appropriate strategy for applications which already have an
/// intrinsic update loop, such as a game. You can also add an instance of a
/// subclass of Leap::Listener to the controller to handle events as they occur.
/// The Leap dispatches events to the listener upon initialization and exiting,
/// on connection changes, and when a new frame of tracking data is available.
/// When these events occur, the controller object invokes the appropriate
/// callback function defined in your subclass of Listener.
///
/// To access frames of tracking data as they become available:
/// 1. Implement a subclass of the Listener class and override the
///    Listener::onFrame() function.
/// 2. In your Listener::onFrame() function, call the Controller::frame()
///    function to access the newest frame of tracking data.
/// 3. To start receiving frames, create a Controller object and add an instance
///    of the Listener subclass to the Controller::addListener() function.
///
/// When an instance of a Listener subclass is added to a Controller object,
/// it calls the Listener::onInit() function when the listener is ready for use.
/// When a connection is established between the controller and the Leap, the
/// controller calls the Listener::onConnect() function. At this point, your
/// application will start receiving frames of data. The controller calls the
/// Listener::onFrame() function each time a new frame is available. If the
/// controller loses its connection with the Leap software or device for any
/// reason, it calls the Listener::onDisconnect() function. If the listener is
/// removed from the controller or the controller is destroyed, it calls the
/// Listener::onExit() function. At that point, unless the listener is added to
/// another controller again, it will no longer receive frames of tracking data.
class Controller : public Interface {
  public:
#if !defined(SWIG)
    // For internal use only.
    Controller(ControllerImplementation*);
#endif

    /// Constructs a Controller object.
    ///
    /// When creating a Controller object, you may optionally pass in a
    /// reference to an instance of a subclass of Leap::Listener. Alternatively,
    /// you may add a listener using the Controller::addListener() function.
    ///
    /// @param listener An instance of Leap::Listener implementing the callback
    /// functions for the Leap events you want to handle in your application.
    LEAP_EXPORT Controller();
    LEAP_EXPORT virtual ~Controller();
    LEAP_EXPORT Controller(Listener& listener);

    /// Adds a listener to this Controller.
    ///
    /// The Controller dispatches Leap events to each associated listener. The
    /// order in which listener callback functions are invoked is arbitrary. If
    /// you pass a listener to the Controller's constructor function, it is
    /// automatically added to the list and can be removed with the
    /// Controller::removeListener() function.
    ///
    /// @param listener A subclass of Leap::Listener implementing the callback
    /// functions for the Leap events you want to handle in your application.
    /// @returns Whether or not the listener was successfully added to the list
    /// of listeners.
    LEAP_EXPORT bool addListener(Listener& listener);

    /// Remove a listener from the list of listeners that will receive Leap
    /// events. A listener must be removed if its lifetime is shorter than the
    /// controller to which it is listening.
    ///
    /// @param listener The listener to remove.
    /// @returns Whether or not the listener was successfully removed from the
    /// list of listeners.
    LEAP_EXPORT bool removeListener(Listener& listener);

    /// Returns a frame of tracking data from the Leap. Use the optional
    /// history parameter to specify which frame to retrieve. Call frame() or
    /// frame(0) to access the most recent frame; call frame(1) to access the
    /// previous frame, and so on. If you use a history value greater than the
    /// number of stored frames, then the controller returns an invalid frame.
    ///
    /// @param history The age of the frame to return, counting backwards from
    /// the most recent frame (0) into the past and up to the maximum age (59).
    /// @returns The specified frame; or, if no history parameter is specified,
    /// the newest frame. If a frame is not available at the specified history
    /// position, an invalid Frame is returned.
    LEAP_EXPORT Frame frame(int history = 0) const;

    /// Returns a Config object, which you can use to query the Leap system for
    /// configuration information. Reserved for future use.
    LEAP_EXPORT Config config() const;
};

/// The Listener class defines a set of callback functions that you can
/// override in a subclass to respond to events dispatched by the Leap.
///
/// To handle Leap events, create an instance of a Listener subclass and assign
/// it to the Controller instance. The Controller calls the relevant Listener
/// callback function when an event occurs, passing in a reference to itself.
/// You do not have to implement callbacks for events you do not want to handle.
class Listener {
  public:
    /// Constructs a Listener object.
    LEAP_EXPORT Listener() {}

    /// Destructs this Listener object.
    LEAP_EXPORT virtual ~Listener() {}

    /// Called once, when this Listener object is newly added to a Controller.
    ///
    /// @param controller The Controller object invoking this callback function.
    LEAP_EXPORT virtual void onInit(const Controller&) {}

    /// Called when the Controller object connects to the Leap software, or when
    /// this Listener object is added to a Controller that is alrady connected.
    ///
    /// @param controller The Controller object invoking this callback function.
    LEAP_EXPORT virtual void onConnect(const Controller&) {}

    /// Called when the Controller object disconnects from the Leap software.
    /// The controller can disconnect when the Leap device is unplugged, the
    /// user shuts the Leap software down, or the Leap software encounters an
    /// unrecoverable error.
    ///
    /// @param controller The Controller object invoking this callback function.
    LEAP_EXPORT virtual void onDisconnect(const Controller&) {}

    /// Called when this Listener object is removed from the Controller object
    /// or the controller is destroyed.
    ///
    /// @param controller The Controller object invoking this callback function.
    LEAP_EXPORT virtual void onExit(const Controller&) {}

    /// Called when a new frame of hand and finger tracking data is available.
    /// Access the new frame data using the Controller::frame() function.
    ///
    /// Note, the Controller skips any pending onFrame events while your
    /// onFrame handler executes. If your implementation takes too long to return,
    /// one or more frames can be skipped. The Controller still inserts the skipped
    /// frames into the frame history. You can access recent frames by setting
    /// the history parameter when calling the Controller::frame() function.
    /// You can determine if any pending onFrame events were skipped by comparing
    /// the ID of the most recent frame with the ID of the last received frame.
    ///
    /// @param controller The Controller object invoking this callback function.
    LEAP_EXPORT virtual void onFrame(const Controller&) {}
};

}

#endif // __Leap_h__
