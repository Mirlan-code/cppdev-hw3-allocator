#include <iostream>
#include <memory>

template <class T, size_t kSize>
struct AllocatorWithoutDeallocation {
    typedef T value_type;

    AllocatorWithoutDeallocation() {
        Pool_ = static_cast<T*>(::operator new(kSize * sizeof(T)));
    }
    ~AllocatorWithoutDeallocation() {
        ::operator delete(Pool_);
    }

    T* allocate(size_t n) {
        auto res = &Pool_[Idx_];
        Idx_ += n;
        return res;
    }

    void deallocate(T*, std::size_t) {
    }

    template<class U>
    struct rebind {
        using other = AllocatorWithoutDeallocation<U, kSize>;
    };

private:
    size_t Idx_ = 0;
    T* Pool_;
};

template <class T, size_t kSize>
constexpr bool operator==(const AllocatorWithoutDeallocation<T, kSize>& a1, const AllocatorWithoutDeallocation<T, kSize>& a2) noexcept
{
    return a1.Pool_ == a2.Pool_;
}

template <class T, size_t kSize>
constexpr bool operator!=(const AllocatorWithoutDeallocation<T, kSize>& a1, const AllocatorWithoutDeallocation<T, kSize>& a2) noexcept
{
    return a1.Pool_ != a2.Pool_;
}