#ifndef PS_INTRUSIVELIST_H_
#define PS_INTRUSIVELIST_H_

#include <cassert>
#include <cstddef>
#include <iterator>
#include <utility>

template <typename T>
class IntrusiveListHook {
public:
    IntrusiveListHook() = default;
    ~IntrusiveListHook() = default;

    IntrusiveListHook(const IntrusiveListHook& other) = delete;
    IntrusiveListHook(IntrusiveListHook&& other) noexcept;
    IntrusiveListHook& operator =(const IntrusiveListHook& other) = delete;
    IntrusiveListHook& operator =(IntrusiveListHook&& other) noexcept;

    T* next() const {return m_next;}
    T* prev() const {return m_prev;}

    void set_next(T* next) {
        m_next = next;
    }
    void set_prev(T* prev) {
        m_prev = prev;
    }

private:
    T* m_next = nullptr;
    T* m_prev = nullptr; 
};

template<typename T>
inline IntrusiveListHook<T>::IntrusiveListHook(IntrusiveListHook<T>&& other) noexcept { 
    using std::swap;
    swap(m_next, other.m_next);
    swap(m_prev, other.m_prev);
}

template<typename T>
inline IntrusiveListHook<T>& IntrusiveListHook<T>
        ::operator=(IntrusiveListHook<T>&& other) noexcept {
    m_next = other.m_next;
    m_prev = other.m_prev;
    other.m_next = nullptr;
    other.m_prev = nullptr;
    return *this;
}

struct IntrusiveListIteratorEndType {};
struct IntrusiveListIteratorValidType {};

template <typename T>
class IntrusiveListIterator {
public:
    using difference_type = std::ptrdiff_t;
    using value_type = std::remove_const_t<T>;
    using pointer = T*;
    using reference = T&;
    using iterator_category = std::bidirectional_iterator_tag;

    IntrusiveListIterator() {};
    explicit IntrusiveListIterator(T* item);
    ~IntrusiveListIterator() = default;

    IntrusiveListIterator(const IntrusiveListIterator& other) = default;
    IntrusiveListIterator(IntrusiveListIterator&& other) noexcept = default;
    IntrusiveListIterator& operator =(const IntrusiveListIterator& other) = default;
    IntrusiveListIterator& operator =(IntrusiveListIterator&& other) noexcept = default;

    bool operator ==(const IntrusiveListIterator<T>& rhs) const;
    bool operator !=(const IntrusiveListIterator<T>& rhs) const {
        return !(*this == rhs);
    }

    IntrusiveListIterator<T>& operator ++();
    IntrusiveListIterator<T> operator ++(int);
    IntrusiveListIterator<T>& operator --();
    IntrusiveListIterator<T> operator --(int);

    T& operator *() const;
    T& operator ->() const;
    
    T* get_item() const {
        return m_item;
    }

    operator IntrusiveListIterator<const T>() const {
        return IntrusiveListIterator<const T>(m_item);
    }
    

private:
    T* m_item = nullptr; 
};

template<typename T>
void swap(IntrusiveListIterator<T> lhs, IntrusiveListIterator<T> rhs) {
    using std::swap;
    swap(lhs, rhs);
}
 
template<typename T>
inline IntrusiveListIterator<T>::IntrusiveListIterator(T* item): 
    m_item(item) 
{}
 
template<typename T>
inline bool IntrusiveListIterator<T>::operator==(const IntrusiveListIterator<T>& rhs) const {
    return m_item == rhs.m_item; 
}
 
template<typename T>
inline IntrusiveListIterator<T>& IntrusiveListIterator<T>::operator++() {
    assert(m_item != nullptr && "Attempting to advance an end iterator");
    m_item = m_item->next(); 
    return *this; 
}
 
template<typename T>
inline IntrusiveListIterator<T> IntrusiveListIterator<T>::operator++(int) {
    assert(m_item != nullptr && "Attempting to advance an end iterator");
    auto out = *this;
    m_item = m_item->next();
    return out;
}
 
template<typename T>
inline IntrusiveListIterator<T>& IntrusiveListIterator<T>::operator--() {
    assert(m_item != nullptr && "Attempting to advance an end iterator");
    m_item = m_item->prev();
    return *this; 
}
 
template<typename T>
inline IntrusiveListIterator<T> IntrusiveListIterator<T>::operator--(int) {
    assert(m_item != nullptr && "Attempting to advance an end iterator");
    auto out = *this;
    m_item = m_item->prev();
    return out;
}
 
template<typename T>
inline T& IntrusiveListIterator<T>::operator*() const {
    assert(m_item != nullptr && "Dereferencing end iterator");
    return *m_item; 
}
 
template<typename T>
inline T& IntrusiveListIterator<T>::operator->() const {
    assert(m_item != nullptr && "Dereferencing end iterator");
    return *m_item; 
}
 

template <typename T>
class IntrusiveList {
public:
    using iterator = IntrusiveListIterator<T>;
    using const_iterator = IntrusiveListIterator<const T>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    IntrusiveList() = default;
    ~IntrusiveList() = default;

    IntrusiveList(const IntrusiveList& other) = delete;
    IntrusiveList(IntrusiveList&& other) noexcept;
    IntrusiveList& operator =(const IntrusiveList& other) = delete;
    IntrusiveList& operator =(IntrusiveList&& other) noexcept;

    iterator begin() {return iterator(m_head);}
    iterator end() {return iterator(nullptr);}
    const_iterator begin() const {return const_iterator(m_head);}
    const_iterator end() const {return const_iterator(nullptr);}
    const_iterator cbegin() const {return begin();}
    const_iterator cend() const {return end();}
    reverse_iterator rbegin() {return reverse_iterator(m_tail);}
    reverse_iterator rend() {return reverse_iterator(nullptr);}
    const_reverse_iterator rbegin() const {return const_reverse_iterator(m_tail);}
    const_reverse_iterator rend() const {return const_reverse_iterator(nullptr);}
    const_reverse_iterator crbegin() const {return rbegin();}
    const_reverse_iterator crend() const {return rend();}

    size_type size() const {return m_size;} 
    bool empty() const {return m_size==0;}

    T& front() {return m_head;}
    T& back() {return m_tail;}

    iterator insert(iterator pos, T* object);
    iterator push_back(T* item);
    iterator erase(iterator pos);
    iterator erase(T* item);
    iterator erase(T& item);

    void clear();


private: 
    T* m_head = nullptr;
    T* m_tail = nullptr;
    size_type m_size = 0;
};

template<typename T>
inline IntrusiveList<T>::IntrusiveList(IntrusiveList<T>&& other) noexcept {
    using std::swap;
    swap(m_head, other.m_head);
    swap(m_tail, other.m_tail);
    swap(m_size, other.m_size); 
}
 
template<typename T>
inline IntrusiveList<T>& IntrusiveList<T>::operator=(IntrusiveList<T>&& other) noexcept {
    m_head = nullptr;
    m_tail = nullptr;
    m_size = 0;
    swap(*this, other);
    return *this;
}

template<typename T>
inline void swap(IntrusiveList<T>& lhs, IntrusiveList<T>& rhs) noexcept {
    using std::swap;
    swap(lhs.m_head, rhs.m_head);
    swap(lhs.m_tail, rhs.m_tail);
    swap(lhs.m_size, rhs.m_size);
}
 
template<typename T>
inline typename IntrusiveList<T>::iterator IntrusiveList<T>::erase(iterator pos) {
    auto item = pos.get_item();
    return erase(item);
}
 
template<typename T>
inline typename IntrusiveList<T>::iterator IntrusiveList<T>::erase(T* item) {
    auto prev_item = item->prev();
    auto next_item = item->next();
    if(prev_item != nullptr) {
        prev_item->set_next(next_item);
    } else {
        m_head = next_item;
    }
    if(next_item != nullptr) {
        next_item->set_prev(prev_item);
    } else {
        m_tail = prev_item;
    }
    m_size -= 1;
    item->set_next(nullptr);
    item->set_prev(nullptr);
    return iterator(next_item);
}
 
template<typename T>
inline typename IntrusiveList<T>::iterator IntrusiveList<T>::erase(T& item) {
    return erase(&item); 
}
 
template<typename T>
inline void IntrusiveList<T>::clear() {
    m_head = nullptr;
    m_tail = nullptr; 
    m_size = 0;
}
 
template<typename T>
inline typename IntrusiveList<T>::iterator 
        IntrusiveList<T>::insert(iterator pos, T* object) {
    assert(pos != end() && "Trying to insert after end of container"); 

    object->set_next(pos->next());
    object->set_prev(pos.get_item());
    pos->set_next(object);
    m_size += 1;
    return iterator(object);
}
 
template<typename T>
inline typename IntrusiveList<T>::iterator IntrusiveList<T>::push_back(T* item) {
    if(m_tail != nullptr) {
        m_tail->set_next(item);
    } else {
        m_head = item;
    }

    item->set_prev(m_tail);
    item->set_next(nullptr);
    m_tail = item; 
    m_size += 1;
    return iterator(item);
}

#endif
