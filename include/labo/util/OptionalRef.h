/// @file OptionalRef.h
/// @author FPR (funny.pig.run __ ATMARK__ gmail.com)
///
/// @copyright Copyright (c) 2020
///
/// Part of the LaboHouse tool. Proprietary and confidential.
/// See the licenses directory for details.
#pragma once
#include <labo/debug/Log.h>

namespace labo {
template<class T>
class OptionalRef
{
    /// Our reference.
    T* t;

  public:
    /// Create an empty reference.
    explicit OptionalRef();

    /// Create an valid reference.
    /// @param t The passed reference
    OptionalRef(T& t);

    /// Check if the reference is valid.
    /// @return true If reference is valid.
    /// @return false
    operator bool() const;

    /// Get the reference. Note that it is a fatal error to call this function
    /// on an invalid reference.
    /// @return T&
    T& get();
};

// Definitions

template<class T>
OptionalRef<T>::OptionalRef()
  : t{ nullptr }
{}

template<class T>
OptionalRef<T>::OptionalRef(T& t)
  : t{ &t }
{}

template<class T>
OptionalRef<T>::operator bool() const
{
    return t;
}

template<class T>
T&
OptionalRef<T>::get()
{
    if (!*this) {
        failure();
    }
    return *t;
}
};