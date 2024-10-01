#pragma once
#include "TFutureResult.hpp"
#include "Tuple.hpp"

namespace impl {

template <size_t...>
struct index_sequence {};

template <size_t N, size_t... Is>
struct make_index_sequence : make_index_sequence<N - 1, N - 1, Is...> {};

template <size_t... Is>
struct make_index_sequence<0, Is...> : index_sequence<Is...> {};

template <typename T>
T ExtractFirst(const T& target) {
    return target;
}

template <typename T>
auto ExtractFirst(const TFutureResult<T>& target) {
    try {
        return target.get();
    } catch (const std::bad_cast&) {
        throw std::bad_cast();
    }
}

template <typename Func, typename Tuple, size_t... I>
auto ApplyImpl(Func func, Tuple args, index_sequence<I...>) {
    try {
        return func(ExtractFirst(args.template get<I>())...);
    } catch (const std::bad_cast&) {
        throw std::bad_cast();
    }
}

template <typename Func, typename... Args>
auto Apply(Func func, const impl::tuple<const Args&...>& args) {
    try {
        return ApplyImpl(func, args, make_index_sequence<sizeof...(Args)>{});
    } catch (const std::bad_cast&) {
        throw std::bad_cast();
    }
}

}  // namespace impl