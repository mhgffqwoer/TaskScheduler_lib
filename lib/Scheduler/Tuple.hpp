namespace impl {

template <typename... Types>
struct tuple {};

template <typename Head, typename... Tail>
struct tuple<Head, Tail...> {
    Head head;
    tuple<Tail...> tail;

    tuple(Head h, Tail... t) : head(h), tail(t...) {}

    template <size_t I>
    auto get() {
        if constexpr (I == 0) {
            return head;
        } else {
            return tail.template get<I - 1>();
        }
    }
};

}  // namespace impl