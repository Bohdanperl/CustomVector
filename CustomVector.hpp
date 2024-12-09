#include <stdexcept>
#include <iostream>
#include <algorithm>


template<typename T>
class Vector {
private:
    T* m_data {nullptr};
    size_t m_size {0};
    size_t m_capacity {0};

private:    
    void resize() 
    {
        m_capacity = (m_capacity == 0) ? 1 : m_capacity * 2;
        T* new_data = new T[m_capacity];
        for (size_t i = 0; i < m_size; ++i) {
            new_data[i] = std::move(m_data[i]);
        }
        delete[] m_data;
        m_data = new_data;
    }

public:
    class Iterator : public std::iterator<std::random_access_iterator_tag, T> 
    {
        private:
            T* ptr;

        public:
            Iterator(T* p);
            T& operator*();
            const T& operator*() const;
            Iterator& operator++();
            Iterator operator++(int);
            bool operator==(const Iterator& other) const;
            bool operator!=(const Iterator& other) const;
            Iterator& operator--();
            Iterator operator--(int);
            Iterator operator+(std::ptrdiff_t n) const;
            Iterator operator-(std::ptrdiff_t n) const;
            std::ptrdiff_t operator-(const Iterator& other) const;
    };

    Vector();
    explicit Vector(size_t initial_capacity);
    Vector(const Vector& other);
    Vector& operator=(const Vector& other);
    Vector(Vector&& other) noexcept;
    Vector& operator=(Vector&& other) noexcept;
    ~Vector();

    void push_back(const T& value);
    void push_back(T&& value);
    void erase(size_t index);
    void reserve(size_t new_capacity);
    void clear();
    void reset();
    void shrink_to_fit();
    bool empty() const;
    size_t size() const;
    size_t capacity() const;

    T& operator[](size_t index);
    const T& operator[](size_t index) const;

    Iterator begin();
    Iterator end();
    const Iterator begin() const;
    const Iterator end() const;

    template<typename U>
    friend std::ostream& operator<<(std::ostream& os, const Vector<U>& vec);
};

template<typename T>
Vector<T>::Iterator::Iterator(T* p) : ptr(p) {}

template<typename T>
T& Vector<T>::Iterator::operator*() { return *ptr; }

template<typename T>
const T& Vector<T>::Iterator::operator*() const { return *ptr; }

template<typename T>
typename Vector<T>::Iterator& Vector<T>::Iterator::operator++() {
    ++ptr;
    return *this;
}

template<typename T>
typename Vector<T>::Iterator Vector<T>::Iterator::operator++(int) 
{
    Iterator tmp = *this;
    ++ptr;
    return tmp;
}

template<typename T>
bool Vector<T>::Iterator::operator==(const Iterator& other) const 
{ 
    return ptr == other.ptr; 
}

template<typename T>
bool Vector<T>::Iterator::operator!=(const Iterator& other) const 
{ 
    return ptr != other.ptr; 
}

template<typename T>
typename Vector<T>::Iterator& Vector<T>::Iterator::operator--() 
{
    --ptr;
    return *this;
}

template<typename T>
typename Vector<T>::Iterator Vector<T>::Iterator::operator--(int) 
{
    Iterator tmp = *this;
    --ptr;
    return tmp;
}

template<typename T>
typename Vector<T>::Iterator Vector<T>::Iterator::operator+(std::ptrdiff_t n) const 
{ 
    return Iterator(ptr + n); 
}

template<typename T>
typename Vector<T>::Iterator Vector<T>::Iterator::operator-(std::ptrdiff_t n) const 
{ 
    return Iterator(ptr - n); 
}

template<typename T>
std::ptrdiff_t Vector<T>::Iterator::operator-(const Iterator& other) const 
{ 
    return ptr - other.ptr; 
}

template<typename T>
Vector<T>::Vector() = default;

template<typename T>
Vector<T>::Vector(size_t initial_capacity) 
    : m_data(new T[initial_capacity]), m_size(0), m_capacity(initial_capacity) {}

template<typename T>
Vector<T>::Vector(const Vector& other) 
    : m_data(new T[other.m_capacity]), m_size(other.m_size), m_capacity(other.m_capacity) 
{
    std::copy(other.m_data, other.m_data + m_size, m_data);
}

template<typename T>
Vector<T>& Vector<T>::operator=(const Vector& other) 
{
    if (this != &other) {
        Vector temp(other);
        std::swap(m_data, temp.m_data);
        std::swap(m_size, temp.m_size);
        std::swap(m_capacity, temp.m_capacity);
    }
    return *this;
}

template<typename T>
Vector<T>::Vector(Vector&& other) noexcept 
    : m_data(other.m_data), m_size(other.m_size), m_capacity(other.m_capacity) 
{
    other.m_data = nullptr;
    other.m_size = 0;
    other.m_capacity = 0;
}

template<typename T>
Vector<T>& Vector<T>::operator=(Vector&& other) noexcept 
{
    if (this != &other) {
        delete[] m_data;
        m_data = other.m_data;
        m_size = other.m_size;
        m_capacity = other.m_capacity;

        other.m_data = nullptr;
        other.m_size = 0;
        other.m_capacity = 0;
    }
    return *this;
}

template<typename T>
Vector<T>::~Vector() { delete[] m_data; }

template<typename T>
void Vector<T>::push_back(const T& value) 
{
    if (m_size == m_capacity) { resize(); }
    m_data[m_size++] = value;
}

template<typename T>
void Vector<T>::push_back(T&& value) 
{
    if (m_size == m_capacity) { resize(); }
    m_data[m_size++] = std::move(value);
}

template<typename T>
void Vector<T>::erase(size_t index) 
{
    if (index >= m_size) {
        throw std::out_of_range("Index out of range");
    }
    for (size_t i = index; i < m_size - 1; ++i) {
        m_data[i] = std::move(m_data[i + 1]);
    }
    --m_size;
}

template<typename T>
void Vector<T>::reserve(size_t new_capacity) 
{
    if (new_capacity > m_capacity) {
        T* new_data = new T[new_capacity];
        for (size_t i = 0; i < m_size; ++i) {
            new_data[i] = std::move(m_data[i]);
        }
        delete[] m_data;
        m_data = new_data;
        m_capacity = new_capacity;
    }
}

template<typename T>
void Vector<T>::clear() { m_size = 0; }

template<typename T>
void Vector<T>::reset() 
{
    delete[] m_data;
    m_data = nullptr;
    m_size = 0;
    m_capacity = 0;
}

template<typename T>
void Vector<T>::shrink_to_fit() 
{
    if (m_size < m_capacity) {
        T* new_data = new T[m_size];
        for (size_t i = 0; i < m_size; ++i) {
            new_data[i] = std::move(m_data[i]);
        }
        delete[] m_data;
        m_data = new_data;
        m_capacity = m_size;
    }
}

template<typename T>
bool Vector<T>::empty() const { return m_size == 0; }

template<typename T>
size_t Vector<T>::size() const { return m_size; }

template<typename T>
size_t Vector<T>::capacity() const { return m_capacity; }

template<typename T>
T& Vector<T>::operator[](size_t index) 
{
    if (index >= m_size) {
        throw std::out_of_range("Index out of range");
    }
    return m_data[index];
}

template<typename T>
const T& Vector<T>::operator[](size_t index) const 
{
    if (index >= m_size) {
        throw std::out_of_range("Index out of range");
    }
    return m_data[index];
}

template<typename T>
typename Vector<T>::Iterator Vector<T>::begin() { return Iterator(m_data); }

template<typename T>
typename Vector<T>::Iterator Vector<T>::end() { return Iterator(m_data + m_size); }

template<typename T>
const typename Vector<T>::Iterator Vector<T>::begin() const { return Iterator(m_data); }

template<typename T>
const typename Vector<T>::Iterator Vector<T>::end() const { return Iterator(m_data + m_size); }

template<typename T>
std::ostream& operator<<(std::ostream& os, const Vector<T>& vec) 
{
    if (vec.empty()) {
        os << "[]";
        return os;
    }
    os << "[";
    for (size_t i = 0; i < vec.size(); ++i) {
        os << vec[i];
        if (i < vec.size() - 1) os << ", ";
    }
    os << "]";
    return os;
}