#include <iostream>
#include <memory>
#include <unordered_set>

template <class T, size_t kSize>
struct PoolAllocator {
    typedef T value_type;

    PoolAllocator() {
        Pool_ = static_cast<T*>(::operator new(kSize * sizeof(T)));
        Used_ = new int[kSize];
        for (size_t i = 0; i < kSize; i++) {
            Used_[i] = 0;
        }
    }

    ~PoolAllocator() {
        ::operator delete(Pool_);
    }

    T* allocate(size_t n) {
        if (n > kSize) {
            std::cout << "Can not allocate" << std::endl;
            return nullptr;
        }
        for (size_t i = 0; i < kSize - n + 1; i++) {
            bool free = true;
            for (size_t j = 0; j < n; j++) {
                if (Used_[i + j] == 1) {
                    free = false;
                    break;
                }
            }
            if (free) {
                auto res = &Pool_[i];
                std::cout << "allocate at " << i << " " << n << std::endl;
                for (size_t j = 0; j < n; j++) {
                    Used_[i + j] = 1;
                }
                return res;
            }
        }

        std::cout << "Can not allocate" << std::endl;
        return nullptr;
    }

    void deallocate(T* p, std::size_t n) {
        for (size_t i = 0; i < n; i++) {
            size_t idx = p - Pool_ + i;
            Used_[idx] = 0;
        }
//        std::cout << "deallocate " << p << " " << n << std::endl;
//        ::operator delete(p);
    }

    // Not needed because of allocator_traits
//    void construct (T* p, const T& value) {
//        // initialize memory with placement new
//        std::cout << "construct" << std::endl;
//        new((void*)p)T(value);
//    }
//
//    // destroy elements of initialized storage p
//    void destroy (T* p) {
//        // destroy objects by calling their destructor
//        std::cout << "destroy" << std::endl;
//        p->~T();
//    }

    template<class U>
    struct rebind {
        using other = PoolAllocator<U, kSize>;
    };

    T* GetPool() const {
        return Pool_;
    }

private:
    T* Pool_;
    int* Used_;
};

template <class T, size_t kSize>
constexpr bool operator==(const PoolAllocator<T, kSize>& a1, const PoolAllocator<T, kSize>& a2) noexcept
{
    return a1.GetPool() == a2.GetPool();
}

template <class T, size_t kSize>
constexpr bool operator!=(const PoolAllocator<T, kSize>& a1, const PoolAllocator<T, kSize>& a2) noexcept
{
    return a1.GetPool() != a2.GetPool();
}