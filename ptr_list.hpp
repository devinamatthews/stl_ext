#ifndef _AQUARIUS_STL_EXT_HPP_
#define _AQUARIUS_STL_EXT_HPP_

#include <initializer_list>
#include <iterator>
#include <list>
#include <memory>

#include "global_ptr.hpp"
#include "type_traits.hpp"

namespace stl_ext
{

namespace detail
{

template <typename list_>
class ptr_list_
{
    private:
        template <typename ptr_>
        class iterator_
        {
            friend class ptr_list_;

            public:
                typedef typename std::iterator_traits<ptr_>::iterator_category iterator_category;
                typedef typename std::iterator_traits<ptr_>::value_type value_type;
                typedef typename std::iterator_traits<ptr_>::difference_type difference_type;
                typedef typename std::iterator_traits<ptr_>::pointer pointer;
                typedef typename std::iterator_traits<ptr_>::reference reference;

                typedef conditional_t<is_same<ptr_,value_type*>::value,
                                      typename list_::iterator,
                                      typename list_::const_iterator> ptr_iterator;

                iterator_() = default;

                iterator_(const iterator_& other)
                : it_(other.it_) {}

                template <typename=enable_if_const_pointer_t<pointer>>
                iterator_(const iterator_<value_type*>& other)
                : it_(other.it_) {}

                iterator_& operator=(const iterator_& other)
                {
                    it_ = other.it_;
                    return *this;
                }

                template <typename=enable_if_const_pointer_t<pointer>>
                iterator_& operator=(const iterator_<value_type*>& other)
                {
                    it_ = other.it_;
                    return *this;
                }

                bool operator==(const iterator_<value_type*>& x)
                {
                    return it_ == x.it_;
                }

                bool operator!=(const iterator_<value_type*>& x)
                {
                    return it_ != x.it_;
                }

                bool operator==(const iterator_<const value_type*>& x)
                {
                    return it_ == x.it_;
                }

                bool operator!=(const iterator_<const value_type*>& x)
                {
                    return it_ != x.it_;
                }

                reference operator*() const
                {
                    return **it_;
                }

                pointer operator->() const
                {
                    return it_->get();
                }

                iterator_& operator++()
                {
                    ++it_;
                    return *this;
                }

                iterator_& operator--()
                {
                    --it_;
                    return *this;
                }

                iterator_ operator++(int x)
                {
                    return iterator_(it_++);
                }

                iterator_ operator--(int x)
                {
                    return iterator_(it_--);
                }

                friend void swap(iterator_& a, iterator_& b)
                {
                    using aquarius::swap;
                    swap(a.it_, b.it_);
                }

                ptr_iterator base()
                {
                    return it_;
                }

            protected:
                iterator_(ptr_iterator it) : it_(it) {}

                ptr_iterator it_;
        };

        /*
         * Workaround for lack of support for const_iterator in
         * erase(), splice(), etc.
         */
        typename list_::iterator ci2i(list_& l, typename list_::const_iterator ci)
        {
            auto i = l.begin();
            while (&*i != &*ci) ++i;
            return i;
        }

    public:
        typedef typename list_::value_type ptr_type;
        typedef decay_t<decltype(*declval<ptr_type>())> value_type;
        typedef const value_type* const_pointer;
        typedef conditional_t<is_same<ptr_type,const_pointer>::value,
                              const value_type*,
                              value_type*> pointer;
        typedef const value_type& const_reference;
        typedef conditional_t<is_same<ptr_type,const_pointer>::value,
                              const value_type&,
                              value_type&> reference;
        typedef iterator_<pointer> iterator;
        typedef iterator_<const_pointer> const_iterator;
        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
        typedef typename std::iterator_traits<iterator>::difference_type difference_type;
        typedef typename list_::size_type size_type;

        typedef typename list_::iterator ptr_iterator;
        typedef typename list_::const_iterator const_ptr_iterator;
        typedef typename list_::reverse_iterator reverse_ptr_iterator;
        typedef typename list_::const_reverse_iterator const_reverse_ptr_iterator;

        ptr_list_() {}

        explicit ptr_list_(size_type n) : impl_(n) {}

        ptr_list_(size_type n, const value_type& val)
        {
            assign(n, val);
        }

        ptr_list_(size_type n, value_type&& val)
        {
            assign(n, forward<value_type>(val));
        }

        ptr_list_(const ptr_list_&) = default;

        ptr_list_(ptr_list_&&) = default;

        ptr_list_(std::initializer_list<value_type> il)
        {
            assign(il);
        }

        ptr_list_(std::initializer_list<pointer> il)
        {
            assign(il);
        }

        ptr_list_& operator=(const ptr_list_&) = default;

        ptr_list_& operator=(ptr_list_&&) = default;

        iterator begin()
        {
            return iterator(impl_.begin());
        }

        iterator end()
        {
            return iterator(impl_.end());
        }

        const_iterator begin() const
        {
            return const_iterator(impl_.begin());
        }

        const_iterator end() const
        {
            return const_iterator(impl_.end());
        }

        reverse_iterator rbegin()
        {
            return reverse_iterator(impl_.rbegin());
        }

        reverse_iterator rend()
        {
            return reverse_iterator(impl_.rend());
        }

        const_reverse_iterator rbegin() const
        {
            return const_reverse_iterator(impl_.rbegin());
        }

        const_reverse_iterator rend() const
        {
            return const_reverse_iterator(impl_.rend());
        }

        const_iterator cbegin() const
        {
            return const_iterator(impl_.begin());
        }

        const_iterator cend() const
        {
            return const_iterator(impl_.end());
        }

        const_reverse_iterator crbegin() const
        {
            return const_reverse_iterator(impl_.rbegin());
        }

        const_reverse_iterator crend() const
        {
            return const_reverse_iterator(impl_.rend());
        }

        ptr_iterator pbegin()
        {
            return impl_.begin();
        }

        ptr_iterator pend()
        {
            return impl_.end();
        }

        const_ptr_iterator pbegin() const
        {
            return impl_.begin();
        }

        const_ptr_iterator pend() const
        {
            return impl_.end();
        }

        reverse_ptr_iterator rpbegin()
        {
            return impl_.rbegin();
        }

        reverse_ptr_iterator rpend()
        {
            return impl_.rend();
        }

        const_reverse_ptr_iterator rpbegin() const
        {
            return impl_.rbegin();
        }

        const_reverse_ptr_iterator rpend() const
        {
            return impl_.rend();
        }

        const_ptr_iterator cpbegin() const
        {
            return impl_.begin();
        }

        const_ptr_iterator cpend() const
        {
            return impl_.end();
        }

        const_reverse_ptr_iterator crpbegin() const
        {
            return impl_.rbegin();
        }

        const_reverse_ptr_iterator crpend() const
        {
            return impl_.rend();
        }

        size_type size() const
        {
            return impl_.size();
        }

        size_type max_size() const
        {
            return impl_.max_size();
        }

        void resize(size_type n)
        {
            impl_.resize(n);
        }

        void resize(size_type n, const value_type& x)
        {
            if (n <= impl_.size())
            {
                impl_.resize(n);
            }
            else
            {
                impl_.reserve(n);

                for (size_type i = impl_.size();i < n;i++)
                {
                    impl_.emplace_back(new value_type(x));
                }
            }
        }

        void resize(size_type n, value_type&& x)
        {
            if (n <= impl_.size())
            {
                impl_.resize(n);
            }
            else
            {
                impl_.reserve(n);

                impl_.emplace_back(new value_type(move(x)));

                for (size_type i = impl_.size();i < n;i++)
                {
                    impl_.emplace_back(new value_type(x));
                }
            }
        }

        bool empty() const
        {
            return impl_.empty();
        }

        reference front()
        {
            return *impl_.front();
        }

        reference back()
        {
            return *impl_.back();
        }

        const_reference front() const
        {
            return *impl_.front();
        }

        const_reference back() const
        {
            return *impl_.back();
        }

        ptr_type& pfront()
        {
            return impl_.front();
        }

        ptr_type& pback()
        {
            return impl_.back();
        }

        const ptr_type& pfront() const
        {
            return impl_.front();
        }

        const ptr_type& pback() const
        {
            return impl_.back();
        }

        void assign(size_type n, const value_type& val)
        {
            impl_.clear();
            resize(n, val);
        }

        void assign(size_type n, value_type&& val)
        {
            impl_.clear();
            resize(n, move(val));
        }

        void assign(const ptr_list_& x)
        {
            *this = x;
        }

        void assign(ptr_list_&& x)
        {
            *this = move(x);
        }

        void assign(std::initializer_list<value_type> il)
        {
            impl_.clear();

            for (auto& val : il)
            {
                impl_.emplace_back(new value_type(val));
            }
        }

        void assign(std::initializer_list<pointer> il)
        {
            impl_.clear();

            for (auto& ptr : il)
            {
                impl_.emplace_back(ptr);
            }
        }

        void push_front(const value_type& x)
        {
            impl_.emplace_front(new value_type(x));
        }

        void push_front(value_type&& x)
        {
            impl_.emplace_front(new value_type(move(x)));
        }

        void push_front(const ptr_type& x)
        {
            impl_.push_front(x);
        }

        void push_front(ptr_type&& x)
        {
            impl_.push_front(move(x));
        }

        template <typename=enable_if_not_same_t<pointer,ptr_type>>
        void push_front(pointer x)
        {
            impl_.emplace_front(x);
        }

        void pop_front()
        {
            impl_.pop_front();
        }

        void push_back(const value_type& x)
        {
            impl_.emplace_back(new value_type(x));
        }

        void push_back(value_type&& x)
        {
            impl_.emplace_back(new value_type(move(x)));
        }

        void push_back(const ptr_type& x)
        {
            impl_.push_back(x);
        }

        void push_back(ptr_type&& x)
        {
            impl_.push_back(move(x));
        }

        template <typename=enable_if_not_same_t<pointer,ptr_type>>
        void push_back(pointer x)
        {
            impl_.emplace_back(x);
        }

        void pop_back()
        {
            impl_.pop_back();
        }

        iterator insert(const_iterator position, const value_type& val)
        {
            impl_.emplace(ci2i(impl_, position.it_), new value_type(val));
            return *this;
        }

        iterator insert(const_iterator position, value_type&& val)
        {
            impl_.emplace(ci2i(impl_, position.it_), new value_type(move(val)));
            return *this;
        }

        iterator insert(const_iterator position, const ptr_type& val)
        {
            impl_.insert(ci2i(impl_, position.it_), val);
            return *this;
        }

        iterator insert(const_iterator position, ptr_type&& val)
        {
            impl_.insert(ci2i(impl_, position.it_), move(val));
            return *this;
        }

        iterator insert(const_iterator position, pointer val)
        {
            impl_.emplace(ci2i(impl_, position.it_), val);
            return *this;
        }

        iterator insert(const_iterator position, size_type n, const value_type& val)
        {
            while (n-- > 0) impl_.emplace(ci2i(impl_, position.it_), val);
            return *this;
        }

        iterator insert(const_iterator position, size_type n, value_type&& val)
        {
            if (n > 0)
            {
                while (n-- > 1) impl_.emplace(ci2i(impl_, position.it_), val);
                impl_.emplace(ci2i(impl_, position.it_), move(val));
            }
            return *this;
        }

        iterator insert(const_iterator position, std::initializer_list<value_type> il)
        {
            for (auto& val : il)
            {
                impl_.emplace(ci2i(impl_, position.it_), new value_type(val));
            }
            return *this;
        }

        iterator insert(const_iterator position, std::initializer_list<pointer> il)
        {
            for (auto& ptr : il)
            {
                impl_.emplace(ci2i(impl_, position.it_), ptr);
            }
            return *this;
        }

        iterator erase(const_iterator position)
        {
            return iterator(impl_.erase(ci2i(impl_, position.it_)));
        }

        iterator erase(const_iterator first, const_iterator last)
        {
            return iterator(impl_.erase(ci2i(impl_, first.it_), ci2i(impl_, last.it_)));
        }

        ptr_iterator perase(const_ptr_iterator position)
        {
            return impl_.erase(ci2i(impl_, position));
        }

        ptr_iterator perase(const_ptr_iterator first, const_ptr_iterator last)
        {
            return impl_.erase(ci2i(impl_, first), ci2i(impl_, last));
        }

        void swap(ptr_list_& x)
        {
            impl_.swap(x.impl_);
        }

        void clear()
        {
            impl_.clear();
        }

        template <typename... Args>
        void emplace(const_iterator position, Args&&... args)
        {
            impl_.emplace(ci2i(impl_, position.it_), new value_type(forward<Args>(args)...));
        }

        template <typename... Args>
        void emplace_front(Args&&... args)
        {
            impl_.emplace_front(new value_type(forward<Args>(args)...));
        }

        template <typename... Args>
        void emplace_back(Args&&... args)
        {
            impl_.emplace_back(new value_type(forward<Args>(args)...));
        }

        friend bool operator==(const ptr_list_& lhs, const ptr_list_& rhs)
        {
            if (lhs.size() != rhs.size()) return false;
            return equal(lhs.begin(), lhs.end(), rhs.begin());
        }

        friend bool operator!=(const ptr_list_& lhs, const ptr_list_& rhs)
        {
            return !(lhs == rhs);
        }

        friend bool operator<(const ptr_list_& lhs, const ptr_list_& rhs)
        {
            return lexicographical_compare(lhs.begin(), lhs.end(),
                                           rhs.begin(), rhs.end());
        }

        friend bool operator>(const ptr_list_& lhs, const ptr_list_& rhs)
        {
            return rhs < lhs;
        }

        friend bool operator<=(const ptr_list_& lhs, const ptr_list_& rhs)
        {
            return !(rhs < lhs);
        }

        friend bool operator>=(const ptr_list_& lhs, const ptr_list_& rhs)
        {
            return !(lhs < rhs);
        }

        friend void swap(ptr_list_&& a, ptr_list_&& b)
        {
            a.swap(b);
        }

        friend void swap(ptr_list_& a, ptr_list_& b)
        {
            a.swap(b);
        }

        void splice(const_iterator position, ptr_list_& x)
        {
            impl_.splice(ci2i(impl_, position.it_), x.impl_);
        }

        void splice (const_iterator position, ptr_list_&& x)
        {
            impl_.splice(ci2i(impl_, position.it_), move(x.impl_));
        }

        void splice (const_iterator position, ptr_list_& x, const_iterator i)
        {
            impl_.splice(ci2i(impl_, position.it_), x.impl_, ci2i(x, i.it_));
        }

        void splice (const_iterator position, ptr_list_&& x, const_iterator i)
        {
            impl_.splice(ci2i(impl_, position.it_), move(x.impl_), ci2i(x, i.it_));
        }

        void splice (const_iterator position, ptr_list_& x,
                     const_iterator first, const_iterator last)
        {
            impl_.splice(ci2i(impl_, position.it_), x.impl_, ci2i(x, first.it_), ci2i(x, last.it_));
        }

        void splice (const_iterator position, ptr_list_&& x,
                     const_iterator first, const_iterator last)
        {
            impl_.splice(ci2i(impl_, position.it_), move(x.impl_), ci2i(x, first.it_), ci2i(x, last.it_));
        }

        void remove(const value_type& val)
        {
            impl_.remove_if([&val](const ptr_type& ptr) { return *ptr == val; });
        }

        template <typename Predicate>
        void remove_if(Predicate pred)
        {
            impl_.remove_if([&pred](const ptr_type& ptr) { return pred(*ptr); });
        }

        void unique()
        {
            impl_.unique([](const ptr_type& a, const ptr_type& b) { return *a == *b; });
        }

        template <class BinaryPredicate>
        void unique(BinaryPredicate binary_pred)
        {
            impl_.unique([&binary_pred](const ptr_type& a, const ptr_type& b)
                             { return binary_pred(*a, *b); });
        }

        void merge(ptr_list_& x)
        {
            impl_.merge(x.impl_, [](const ptr_type& a, const ptr_type& b)
                                    { return *a < *b; });
        }

        void merge(ptr_list_&& x)
        {
            impl_.merge(move(x.impl_), [](const ptr_type& a, const ptr_type& b)
                                            { return *a < *b; });
        }

        template <class Compare>
        void merge(ptr_list_& x, Compare comp)
        {
            impl_.merge(x.impl_, [&comp](const ptr_type& a, const ptr_type& b)
                                        { return comp(*a, *b); });
        }

        template <class Compare>
        void merge(ptr_list_&& x, Compare comp)
        {
            impl_.merge(move(x.impl_), [&comp](const ptr_type& a, const ptr_type& b)
                                            { return comp(*a, *b); });
        }

        void sort()
        {
            impl_.sort([](const ptr_type& a, const ptr_type& b)
                           { return *a < *b; });
        }

        template <class Compare>
        void sort(Compare comp)
        {
            impl_.sort([&comp](const ptr_type& a, const ptr_type& b)
                            { return comp(*a, *b); });
        }

        void reverse() noexcept
        {
            impl_.reverse();
        }

    private:
        list_ impl_;
};

}

template <typename T>
using ptr_list = detail::ptr_list_<std::list<T*>>;

template <typename T>
using unique_list = detail::ptr_list_<std::list<std::unique_ptr<T>>>;

template <typename T>
using shared_list = detail::ptr_list_<std::list<std::shared_ptr<T>>>;

template <typename T>
using global_list = detail::ptr_list_<std::list<global_ptr<T>>>;

}

#endif
