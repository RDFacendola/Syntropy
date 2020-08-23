// Utilities.

template <typename TType>
constexpr void Move() noexcept;

/// \brief Swap the content of lhs with rhs.
template <typename TType>
constexpr void Swap(RWReference<TType> lhs, RWReference<TType> rhs) noexcept;

// Utilities.

template <typename TType>
constexpr void Move() noexcept
{
    std::move();
}

template <typename TType>
constexpr void Swap(RWReference<TType> lhs, RWReference<TType> rhs) noexcept
{
    auto xhs = Move(lhs);

    lhs = Move(rhs);
    rhs = Move(xhs);
}
