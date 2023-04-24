/// \file simple_cxx20.h
/// Contains functionalities using c++20.
///
/// \author Raffaele D. Facendola - January 2021

// ===========================================================================

#pragma once

template <typename T>
concept FooConcept = requires(const T& t)
{
   { t.DoFoo() };
};

struct Foo
{
   void DoFoo() const;
};

struct Bar
{
   void DoBar() const;
};

/*****************************************************************************/
/* IMPLEMENTATION                                                            */
/*****************************************************************************/

// Foo.
// ====

inline void Foo::DoFoo() const
{
   // Foo satisfies FooConcept.
}

// Bar.
// ====

inline void Bar::DoBar() const
{
   // Bar does not satisfy FooConcept.
}

// ============================================================================
