#pragma once
#include <cstdlib>
#include <new>
#include <limits>
#include <iostream>
#include <algorithm>
#include <iterator>

template<class T>
class Allocator
{
public:
    using value_type = T;
    using pointer = T*;
    using size_type = size_t;

    pointer allocate(size_type count)
    {
        return static_cast<pointer>(std::malloc(count * sizeof(value_type)));
    }

    void deallocate(pointer ptr, size_type count)
    {
        if (count > 0)
            std::free(ptr);
    }

    template<class... Args>
    void construct(pointer ptr, Args&&... args)
    {
        ::new ((void*)ptr) value_type(std::forward<Args>(args)...);
    }

    void destroy(pointer ptr)
    {
        ptr->~value_type();
    }

    size_t max_size() const noexcept
    {
        return std::numeric_limits<size_type>::max() / sizeof(value_type);
    }
};

template <class T>
class Iterator: public std::iterator<std::random_access_iterator_tag, T>
{
public:
    using size_type = size_t;
    using value_type = T;
    using pointer = T*;
    using reference = T&;
    using iterator = Iterator<value_type>;

    pointer ptr_;

    explicit Iterator(pointer ptr): ptr_(ptr) {}

    Iterator(const Iterator& other) : ptr_(other.ptr_) {}

    bool operator==(const Iterator<value_type>& other) const
    {
        return ptr_ == other.ptr_;
    }

    bool operator!=(const Iterator<value_type>& other) const
    {
        return !(*this == other);
    }

    reference operator*() const
    {
        return *ptr_;
    }

    iterator& operator++()
    {
        ++ptr_;
        return *this;
    }

    iterator& operator--()
    {
        --ptr_;
        return *this;
    }

    iterator& operator+(size_type count)
    {
        ptr_ += count;
        return *this;
    }

    iterator& operator-(size_type count)
    {
        ptr_ -= count;
        return *this;
    }

    bool operator<(const Iterator<value_type>& other) const
    {
        return ptr_ < other.ptr_;
    }

    bool operator>(const Iterator<value_type>& other) const
    {
        return ptr_ > other.ptr_;
    }

    bool operator<=(const Iterator<value_type>& other) const
    {
        return ptr_ <= other.ptr_;
    }

    bool operator>=(const Iterator<value_type>& other) const
    {
        return ptr_ >= other.ptr_;
    }

    iterator& operator+=(size_type count)
    {
        ptr_ += count;
        return *this;
    }

    iterator& operator-=(size_type count)
    {
        ptr_ -= count;
        return *this;
    }

    reference operator[](size_type pos) const
    {
        return ptr_[pos];
    }
};

template<class T, class Alloc = Allocator<T>>
class Vector
{
public:
    using size_type = size_t;
    using value_type = T;
    using pointer = T*;
    using reference = T&;
    using const_reference = const T&;
    using allocator_type = Alloc;
    using iterator = Iterator<T>;
    using reverse_iterator = std::reverse_iterator<iterator>;

    Vector(): size_(0), capacity_(0) {}

    Vector(size_type count): size_(0), capacity_(count)
    {
         ptr_ = alloc_.allocate(count);
    }

    ~Vector()
    {
        for (size_t i = 0; i < size_; ++i)
            alloc_.destroy(ptr_ + i);
        alloc_.deallocate(ptr_, capacity_);
    }

    reference operator[](size_type pos)
    {
        return ptr_[pos];
    }

    const_reference operator[](size_type pos) const
    {
        return ptr_[pos];
    }

    void push_back(value_type&& value)
    {
        if (capacity_ == 0)
        {
            ptr_ = alloc_.allocate(1);
            capacity_ = 1;
        }
        else if (capacity_ == size_)
        {
            increase_size(2 * capacity_);
            capacity_ *= 2;
        }

        ptr_[size_++] = std::move(value);
    }

    void push_back(const value_type& value)
    {
        if (capacity_ == 0)
        {
            ptr_ = alloc_.allocate(1);
            capacity_ = 1;
        }
        else if (capacity_ == size_)
        {
            increase_size(2 * capacity_);
            capacity_ *= 2;
        }

        ptr_[size_++] = value;
    }

    void pop_back()
    {
        if (size_ == 0)
            throw(std::runtime_error("Empty vector"));

        alloc_.destroy(ptr_ + size_ - 1);
        --size_;
    }

    bool empty() const noexcept
    {
        return size_ == 0;
    }

    size_type size() const noexcept
    {
        return size_;
    }

    size_type capacity() const noexcept
    {
        return capacity_;
    }

    void clear() noexcept
    {
        for (size_t i = 0; i < size_; ++i)
            alloc_.destroy(ptr_ + i);
        size_ = 0;
    }

    iterator begin() noexcept
    {
        return iterator(ptr_);
    }

    iterator end() noexcept
    {
        return iterator(ptr_ + size_);
    }

    reverse_iterator rbegin() noexcept
    {
        return reverse_iterator(end());
    }

    reverse_iterator rend() noexcept
    {
        return reverse_iterator(begin());
    }

    void resize(size_type new_size)
    {
        if (new_size < size_)
        {
            for (size_t i = new_size; i < size_; ++i)
                alloc_.destroy(ptr_ + i);
        }
        else if (new_size > size_)
        {
            if (new_size > capacity_)
            {
                increase_size(new_size);
                capacity_ = new_size;
            }

            for (size_t i = size_; i < new_size; ++i)
                alloc_.construct(ptr_ + i);
        }
        size_ = new_size;
    }

    void reserve(size_type new_capacity)
    {
        if (new_capacity > capacity_)
        {
            increase_size(new_capacity);
            capacity_ = new_capacity;
        }
    }
private:
    size_type size_;
    size_type capacity_;
    allocator_type alloc_;
    pointer ptr_;

    void increase_size(size_type new_size)
    {
        pointer tmp = alloc_.allocate(new_size);

        for (size_t i = 0; i < size_; ++i)
            alloc_.construct(tmp + i, ptr_[i]);

        for (size_t i = 0; i < size_; ++i)
            alloc_.destroy(ptr_ + i);

        alloc_.deallocate(ptr_, capacity_);
        ptr_ = tmp;
    }
};