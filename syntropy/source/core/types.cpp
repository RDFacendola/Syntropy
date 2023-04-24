
// ################################################################################

// @author Raffaele D. Facendola - April 2023

// ################################################################################

module syntropy.types;

// ################################################################################

// ================================================================================
// CONVERSIONS
// ================================================================================

constexpr sy::Int sy::ToInt(sy::Fix8 rhs)
{
    return static_cast<sy::Int>(rhs);
}

constexpr sy::Int sy::ToInt(sy::Fix16 rhs)
{
    return static_cast<sy::Int>(rhs);
}

constexpr sy::Int sy::ToInt(sy::Fix32 rhs)
{
    return static_cast<sy::Int>(rhs);
}

constexpr sy::Int sy::ToInt(sy::Fix64 rhs)
{
    return static_cast<sy::Int>(rhs);
}

// ################################################################################
