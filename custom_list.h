#include <iostream>
#include <memory>
#include <vector>

template <typename T, typename Alloc=std::allocator<T>>
class CustomList {

    struct Node
    {
        explicit Node(const T& val, Node* next = nullptr) {
            this->next = next;
            this->val = val;
        }
        Node* next;
        T val;
    };

    struct Iterator
    {
        using iterator_category = std::forward_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using value_type        = Node;
        using pointer           = Node*;  // or also value_type*
        using reference         = Node&;  // or also value_type&

        explicit Iterator(pointer ptr) : m_ptr(ptr) {}
        reference operator*() const { return *m_ptr; }
        pointer operator->() const { return m_ptr; }

        // Prefix increment
        Iterator& operator++() { m_ptr = m_ptr->next; return *this; }

        // Postfix increment
        Iterator operator++(int) { Iterator tmp = *this; ++m_ptr; return tmp; }

        friend bool operator==(const Iterator& a, const Iterator& b) { return a.m_ptr == b.m_ptr; };
        friend bool operator!=(const Iterator& a, const Iterator& b) { return a.m_ptr != b.m_ptr; };
        friend bool operator<(const Iterator& a, const Iterator& b) { return a.m_ptr < b.m_ptr; };

    private:
        pointer m_ptr;
    };

public:

    CustomList() {
        Head_ = nullptr;
        Last_ = nullptr;
    }

    void Add(const T& val) {
        Node* newNode = RebindAlloc_.allocate(1);
        std::allocator_traits<RebindAlloc>::construct(RebindAlloc_, newNode, val);
        if (!Head_) {
            Head_ = newNode;
        } else {
            Last_->next = newNode;
        }
        Last_ = newNode;
        Size_ ++;
    }

    void Iterate() {
        Node* cur = Head_;
        while (cur != nullptr) {
            std::cout << cur->val << " ";
            cur = cur->next;
        }
        std::cout << std::endl;
    }

    bool empty() {
        return Head_ == nullptr;
    }

    size_t size() {
        return size;
    }

    Iterator begin() noexcept {
        return Iterator(Head_);
    }

    Iterator end() noexcept {
        return Iterator(Last_->next);
    }

private:
    using RebindAlloc = typename std::allocator_traits<Alloc>::template rebind_alloc<Node>;

    Node* Head_;
    Node* Last_;
    RebindAlloc RebindAlloc_;
    size_t Size_ = 0;
};