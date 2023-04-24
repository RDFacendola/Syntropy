
/// \file compare_unit_test.inl
///
/// \author Raffaele D. Facendola - April 2021.

// ===========================================================================

namespace CompareUnitTest
{
    /************************************************************************/
    /* UNIT TEST                                                            */
    /************************************************************************/

    inline Sy::Bool
    MemberComparable
    ::IsEqual(Sy::Immutable<MemberComparable>) const
    {
        return false;
    }

    inline Sy::Bool
    MemberComparable
    ::IsEquivalent(Sy::Immutable<MemberComparable>) const
    {
        return false;
    }

    inline Sy::Ordering
    MemberComparable
    ::Compare(Sy::Immutable<MemberComparable>) const
    {
        return Sy::Ordering::kIncomparable;
    }

    inline Sy::Bool
    AreEqual(Sy::Immutable<NonMemberComparable>,
             Sy::Immutable<NonMemberComparable>)
    {
        return false;
    }

    inline Sy::Bool
    AreEquivalent(Sy::Immutable<NonMemberComparable>,
                  Sy::Immutable<NonMemberComparable>)
    {
        return false;
    }

    inline Sy::Ordering
    Compare(Sy::Immutable<NonMemberComparable>,
            Sy::Immutable<NonMemberComparable>)
    {
        return Sy::Ordering::kIncomparable;
    }

    // Non-member operators for OperatorComparable.

    inline Sy::Bool
    operator==(Sy::Immutable<OperatorComparable>,
               Sy::Immutable<OperatorComparable>)
    {
        return false;
    }

    inline Sy::Ordering
    operator<=>(Sy::Immutable<OperatorComparable>,
                Sy::Immutable<OperatorComparable>)
    {
        return Sy::Ordering::kIncomparable;
    }
}

// ===========================================================================
