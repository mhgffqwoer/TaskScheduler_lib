#pragma once
#include <iostream>

namespace impl {

class Any {
   private:
    class PlaceHolder {
       public:
        virtual ~PlaceHolder() {}
        virtual const std::type_info& type() const = 0;
        virtual PlaceHolder* clone() const = 0;
    };

    template <typename T>
    class Holder : public PlaceHolder {
       public:
        Holder(const T& data) : data_(data) {}
        const std::type_info& type() const { return typeid(T); }
        PlaceHolder* clone() const { return new Holder(data_); }

        T data_;
    };

    PlaceHolder* data_;

   public:
    Any() : data_(nullptr) {}

    template <typename T>
    Any(const T& data) : data_(new Holder<T>(data)) {}

    Any(const Any& other)
        : data_(other.data_ ? other.data_->clone() : nullptr) {}

    Any& operator=(const Any& other) {
        if (this != &other) {
            delete data_;
            data_ = other.data_ ? other.data_->clone() : nullptr;
        }
        return *this;
    }

    template <typename T>
    T& cast() const {
        if (typeid(T) != data_->type()) {
            throw std::bad_cast();
        }
        return dynamic_cast<Holder<T>*>(data_)->data_;
    }

    ~Any() { delete data_; }

    bool empty() const { return !data_; }
};

}  // namespace impl