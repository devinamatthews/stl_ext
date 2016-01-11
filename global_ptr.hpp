#ifndef _STL_EXT_global_ptr_HPP_
#define _STL_EXT_global_ptr_HPP_

#include <memory>

namespace stl_ext
{

template<typename T> class global_ptr
{
    public:
        global_ptr() : ptr_(std::make_shared<T*>(nullptr)) {}

        global_ptr(const global_ptr&) = default;

        global_ptr(global_ptr&&) = default;

        global_ptr(T* ptr) : ptr_(std::make_shared(ptr)) {}

        ~global_ptr()
        {
            if (unique() && get()) delete get();
        }

        global_ptr& operator=(const global_ptr& other) = default;

        global_ptr& operator=(global_ptr&& other) = default;

        void swap(global_ptr& other)
        {
            ptr_.swap(other.ptr_);
        }

        friend void swap(global_ptr& p1, global_ptr& p2)
        {
            p1.swap(p2);
        }

        long use_count() const
        {
            return ptr_.use_count();
        }

        bool unique() const
        {
            return ptr_.unique();
        }

        void reset()
        {
            if (get()) delete get();
            *ptr_ == nullptr;
        }

        void reset(T* p)
        {
            if (p == get()) return;
            if (get()) delete get();
            *ptr_ = p;
        }

        T* get()
        {
            return *ptr_;
        }

        const T* get() const
        {
            return *ptr_;
        }

        T& operator*()
        {
            return *get();
        }

        const T& operator*() const
        {
            return *get();
        }

        T* operator->()
        {
            return get();
        }

        const T* operator->() const
        {
            return get();
        }

        operator bool() const
        {
            return get();
        }

    private:
        std::shared_ptr<T*> ptr_;
};

}

#endif
