#pragma once

#include <tuple>

#include "Any.hpp"
#include "Apply.hpp"
#include "TFutureResult.hpp"

struct TTaskBase {
    using TTaskId = size_t;

   public:
    virtual ~TTaskBase() = default;
    virtual void Run() = 0;
    virtual TTaskId Id() const = 0;
    virtual const impl::Any& getResult() const = 0;
};

template <typename Func, typename... Args>
class TTask : public TTaskBase {
   private:
    TTaskId id;
    Func func;
    impl::tuple<const Args&...> args;
    impl::Any result;

   public:
    TTask(const size_t& id, const Func& f, const Args&... a)
        : func(f), args(a...), id(id), result(nullptr) {}

    void Run() override {
        try {
            result = impl::Apply(func, args);
        } catch (const std::bad_cast&) {
            std::cerr << "\nError: wrong future result type. Process id = " << id << "." << "\n";
            exit(EXIT_FAILURE);
        }
    }

    TTaskId Id() const override { return id; }

    const impl::Any& getResult() const override {
        return result;
    }
};