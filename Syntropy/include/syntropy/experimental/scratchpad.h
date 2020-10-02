// VARIABLE TEMPLATE _Tuple_conditional_explicit_v
template <bool _Same, class _Dest, class... _Srcs>
_INLINE_VAR constexpr bool _Tuple_conditional_explicit_v0 = false;

template <class... _Dests, class... _Srcs>
_INLINE_VAR constexpr bool _Tuple_conditional_explicit_v0<true, tuple<_Dests...>, _Srcs...> =
!conjunction_v<is_convertible<_Srcs, _Dests>...>;

template <class _Dest, class... _Srcs>
_INLINE_VAR constexpr bool _Tuple_conditional_explicit_v =
_Tuple_conditional_explicit_v0<tuple_size_v<_Dest> == sizeof...(_Srcs), _Dest, _Srcs...>;

// VARIABLE TEMPLATE _Tuple_constructible_v
template <bool _Same, class _Dest, class... _Srcs>
_INLINE_VAR constexpr bool _Tuple_constructible_v0 = false;

template <class... _Dests, class... _Srcs>
_INLINE_VAR constexpr bool _Tuple_constructible_v0<true, tuple<_Dests...>, _Srcs...> =
conjunction_v<is_constructible<_Dests, _Srcs>...>;

template <class _Dest, class... _Srcs>
_INLINE_VAR constexpr bool _Tuple_constructible_v =
_Tuple_constructible_v0<tuple_size_v<_Dest> == sizeof...(_Srcs), _Dest, _Srcs...>;

template <class _Dest, class... _Srcs>
struct _Tuple_constructible_val : bool_constant<_Tuple_constructible_v<_Dest, _Srcs...>> {};

// VARIABLE TEMPLATE _Tuple_nothrow_constructible_v
template <bool _Same, class _Dest, class... _Srcs>
_INLINE_VAR constexpr bool _Tuple_nothrow_constructible_v0 = false;

template <class... _Dests, class... _Srcs>
_INLINE_VAR constexpr bool _Tuple_nothrow_constructible_v0<true, tuple<_Dests...>, _Srcs...> =
conjunction_v<is_nothrow_constructible<_Dests, _Srcs>...>;

template <class _Dest, class... _Srcs>
_INLINE_VAR constexpr bool _Tuple_nothrow_constructible_v =
_Tuple_nothrow_constructible_v0<tuple_size_v<_Dest> == sizeof...(_Srcs), _Dest, _Srcs...>;

// VARIABLE TEMPLATE _Tuple_assignable_v
template <bool _Same, class _Dest, class... _Srcs>
_INLINE_VAR constexpr bool _Tuple_assignable_v0 = false;

template <class... _Dests, class... _Srcs>
_INLINE_VAR constexpr bool _Tuple_assignable_v0<true, tuple<_Dests...>, _Srcs...> =
conjunction_v<is_assignable<_Dests&, _Srcs>...>; // note _Dests& instead of _Dests

template <class _Dest, class... _Srcs>
_INLINE_VAR constexpr bool _Tuple_assignable_v =
_Tuple_assignable_v0<tuple_size_v<_Dest> == sizeof...(_Srcs), _Dest, _Srcs...>;

template <class _Dest, class... _Srcs>
struct _Tuple_assignable_val : bool_constant<_Tuple_assignable_v<_Dest, _Srcs...>> {};

// STRUCT TEMPLATE _Tuple_convert_copy_val
// Constrain tuple's converting copy constructor (LWG-2549)
template <class _Myself, class... _Other>
struct _Tuple_convert_copy_val : true_type {};

template <class _This, class _Uty>
struct _Tuple_convert_copy_val<tuple<_This>, _Uty>
    : bool_constant<!disjunction_v<is_same<_This, _Uty>, is_constructible<_This, const tuple<_Uty>&>,
    is_convertible<const tuple<_Uty>&, _This>>> {};

// STRUCT TEMPLATE _Tuple_convert_move_val
// Constrain tuple's converting move constructor (LWG-2549)
template <class _Myself, class... _Other>
struct _Tuple_convert_move_val : true_type {};

template <class _This, class _Uty>
struct _Tuple_convert_move_val<tuple<_This>, _Uty>
    : bool_constant<!disjunction_v<is_same<_This, _Uty>, is_constructible<_This, tuple<_Uty>>,
    is_convertible<tuple<_Uty>, _This>>> {};

// STRUCT TEMPLATE _Tuple_perfect_val
// Constrain tuple's perfect forwarding constructor (LWG-3121)
template <class _Myself, class _This2, class... _Rest2>
struct _Tuple_perfect_val : true_type {};

template <class _Myself, class _This2>
struct _Tuple_perfect_val<_Myself, _This2>
    : bool_constant<!is_same_v<_Myself, remove_const_t<remove_reference_t<_This2>>>> {};



template <class _This, class... _Rest>
class tuple<_This, _Rest...> : private tuple<_Rest...>
{
    // ***********************************************************************************************
    // ***********************************************************************************************
    // ***********************************************************************************************

    template <class _This2 = _This,
        enable_if_t<conjunction_v<is_default_constructible<_This2>, is_default_constructible<_Rest>...>, int> = 0>
    tuple();

    template <class _This2 = _This,
        enable_if_t<_Tuple_constructible_v<tuple, const _This2&, const _Rest&...>, int> = 0>
    tuple(const _This& _This_arg, const _Rest&... _Rest_arg);

    template <class _This2, class... _Rest2,
        enable_if_t<conjunction_v<_Tuple_perfect_val<tuple, _This2, _Rest2...>, _Tuple_constructible_val<tuple, _This2, _Rest2...>>,int> = 0>
    tuple(_This2&& _This_arg, _Rest2&&... _Rest_arg);

    template <class... _Other,
        enable_if_t<conjunction_v<_Tuple_constructible_val<tuple, const _Other&...>, _Tuple_convert_copy_val<tuple, _Other...>>, int> = 0>
    tuple(const tuple<_Other...>& _Right);

    template <class... _Other,
        enable_if_t<conjunction_v<_Tuple_constructible_val<tuple, _Other...>, _Tuple_convert_move_val<tuple, _Other...>>, int> = 0>
    tuple(tuple<_Other...>&& _Right);

    // ***********************************************************************************************
    // ***********************************************************************************************
    // ***********************************************************************************************

    tuple& operator=(const volatile tuple&) = delete;

    template <class _Myself = tuple, class _This2 = _This, enable_if_t<conjunction_v<_Is_copy_assignable_no_precondition_check<_This2>, _Is_copy_assignable_no_precondition_check<_Rest>...>, int> = 0>
    tuple& operator=(_Identity_t<const _Myself&> _Right);

    template <class _Myself = tuple, class _This2 = _This, enable_if_t<conjunction_v<_Is_move_assignable_no_precondition_check<_This2>, _Is_move_assignable_no_precondition_check<_Rest>...>, int> = 0>
    tuple& operator=(_Identity_t<_Myself&&> _Right);

    template <class... _Other, enable_if_t<conjunction_v<negation<is_same<tuple, tuple<_Other...>>>, _Tuple_assignable_val<tuple, const _Other&...>>, int> = 0>
    tuple& operator=(const tuple<_Other...>& _Right);

    template <class... _Other, enable_if_t<conjunction_v<negation<is_same<tuple, tuple<_Other...>>>, _Tuple_assignable_val<tuple, _Other...>>, int> = 0>
    tuple& operator=(tuple<_Other...>&& _Right);

    template <class _First, class _Second, enable_if_t<_Tuple_assignable_v<tuple, const _First&, const _Second&>, int> = 0>
    tuple& operator=(const pair<_First, _Second>& _Right);

    template <class _First, class _Second, enable_if_t<_Tuple_assignable_v<tuple, _First, _Second>, int> = 0>
    tuple& operator=(pair<_First, _Second>&& _Right);

    // ***********************************************************************************************
    // ***********************************************************************************************
    // ***********************************************************************************************
};
