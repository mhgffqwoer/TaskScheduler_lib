#pragma once
#include "Any.hpp"

template <typename T>
class TFutureResult {
    using TTaskId = size_t;

   private:
    const impl::Any& result_;
    TTaskId id_ = 0;

   public:
    TFutureResult(const impl::Any& result, const TTaskId& id)
        : result_(result), id_(id){};

    TTaskId Id() const { return id_; }

    T get() const {
        try {
            return result_.template cast<T>();
        } catch (const std::bad_cast&) {
            throw std::bad_cast();
        }
    }
};