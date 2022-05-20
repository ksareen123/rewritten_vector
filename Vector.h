#ifndef VECTOR_H
#define VECTOR_H

#include <algorithm> // std::random_access_iterator_tag
#include <cstddef> // size_t
#include <stdexcept> // std::range_error
#include <iostream> // print

template <class T>
class Vector {
public:
    class iterator;
private:
    T* array;
    size_t _capacity, _size;

    // You may want to write a function that grows the vector
    void grow() { // resize function
        _capacity*= 2;
        T* arr2 = new T[_capacity];
        //using std::move, move array 
        for (size_t i = 0; i < _size; i++) {
            arr2[i] = std::move(array[i]);
        }
        delete[] array;
        array = arr2;
    };

public:
    Vector() noexcept { //default constructor so set everything to default/zero value
        array = nullptr;
        _capacity = 0;
        _size = 0;
    };

    Vector(size_t count, const T& value) { // constructor, initialized size and capacity 
        _size = count;
        _capacity = count;
        array = new T[_capacity];
        //every element needs to be the same
        for (size_t i = 0; i < _size; i++) {
            array[i] = value;
        }
    };

    explicit Vector(size_t count) { // create default array of specified size
        _size = count;
        _capacity = count;
        array = new T[count] {};
    };

    Vector(const Vector& other){ // copy constructor, don't move previous array
        _size = other.size();
        _capacity = other.capacity();
        array = new T[_capacity];
        for (size_t i = 0; i < _size; i++) {
            array[i] = other.at(i);
        }
    };

    Vector(Vector&& other) noexcept { // move constructor, empty previous array
        _size = other.size();
        _capacity = other.capacity();
        array = other.array;
        other.array = nullptr; // don't delete, keep space for new array
        other._size = 0;
        other._capacity = 0;
    };

    ~Vector() { //destructor
        delete[] array;
        _size = 0;
        _capacity = 0;
    };

    Vector& operator=(const Vector& other) { // copy assignment operator
        if (this != &other) { //only proceed if they are not equal
            _size = other.size();
            _capacity = other.capacity();
            delete[] array; // in case there's something already there
            array = new T[_capacity];
            for (size_t i = 0; i < _size; i++) { //fill element by element
                array[i] = other.at(i);
            }
        }

        return *this;
    };

    Vector& operator=(Vector&& other) noexcept { //move assignment operator
        if (this != &other) { // only proceed if not equal
            _size = other.size();
            _capacity = other.capacity();
            delete[] array; // in case there's something already there
            array = other.array;
            other.array = nullptr; //set to nullptr instead of deleting
            other._size = 0;
            other._capacity = 0;
        }

        return *this;
    };

    iterator begin() noexcept { //begin, returns iterator to first element in vector
        return iterator(array);
    };

    iterator end() noexcept { //end, returns iterator to last element in vector
        return iterator(array+_size);
    };

    [[nodiscard]] bool empty() const noexcept {
        return (_size == 0);
    };
    size_t size() const noexcept {
        return _size;
    };
    size_t capacity() const noexcept {
        return _capacity;
    };

    T& at(size_t pos) {
        if (pos >= _size || pos < 0) {
            throw std::out_of_range("out of range");
        }
        return array[pos];
    };
    const T& at(size_t pos) const {
        if (pos >= _size || pos < 0) {
            throw std::out_of_range("out of range");
        }
        return array[pos];
    };
    
    T& operator[](size_t pos) {
        return array[pos];
    };
    const T& operator[](size_t pos) const {
        return array[pos];
    };
    
    T& front() {
        return array[0];
    };
    const T& front() const {
        return array[0];
    };
    T& back() {
        return array[_size-1];
    };
    const T& back() const {
        return array[_size-1];
    };

    void push_back(const T& value) {
        if (!array || _capacity == 0) {
            delete[] array;
            array = new T[1] {};
            _capacity = 1;
            _size = 0;
        }
        else if (_size >= _capacity) {
            grow();
        }
        array[_size] = value;
        _size+= 1;
    };
    void push_back(T&& value) {
        if (!array || _capacity == 0) {
            delete[] array;
            array = new T[1] {};
            _capacity = 1;
            _size = 0;
        }
        else if (_size >= _capacity) {
            grow();
        }
        array[_size] = std::move(value);
        _size+= 1;
    };

    void pop_back() {
        _size--;
    };

    iterator insert(iterator pos, const T& value) {
        if (pos > end()) {
            throw std::out_of_range("iterator out of range");
        }
        if (_size >= _capacity) {
            ptrdiff_t offset = pos - begin();
            grow();
            pos = begin() + offset;
        }
        for (iterator i = end() - 1; i >= pos; --i) { // move backwards to push everything to the right
            //std::cout << "moving " << *i << std::endl;
            *(i + 1) = std::move(*i);
        }
        *pos = value;
        //std::cout << "inserting " << value << std::endl;
        _size++;
        return pos;
    };

    iterator insert(iterator pos, T&& value) { // insert move
        if (pos > end()) {
            throw std::out_of_range("iterator out of range");
        }
        if (_size >= _capacity) {
            ptrdiff_t offset = pos - begin();
            grow();
            pos = begin() + offset;
        }
        for (iterator i = end() - 1; i >= pos; --i) { // move backwards to push everything to the right
            //std::cout << "moving " << *i << std::endl;
            *(i + 1) = std::move(*i);
        }
        *pos = std::move(value);
        //std::cout << "inserting " << value << std::endl;
        _size++;
        return pos;
    };

    iterator insert(iterator pos, size_t count, const T& value) {
        if (pos > end()) {
            throw std::out_of_range("iterator out of range");
        }
        std::cout << "preparing for new size " << _size + count << std::endl;
        while (_size + count > _capacity) {
            ptrdiff_t offset = pos - begin();
            grow();
            std::cout << "Grew to size " << _capacity << std::endl;
            pos = begin() + offset;
        }
        iterator n = end() + count - 1;
        std::cout << "n: " << n - begin() << std::endl;
        iterator off = n - (end() - pos);
        for ( ; n > off; n--) {
            std::cout << "new n: " << n - begin() << std::endl;
            *n = std::move(*(n - count));
        }
        for (off = pos; off < pos + count; off++) {
            std::cout << "entered at " << off - begin() << std::endl;
            *off = value;
        }
        _size+= count;
        return pos;
    };

    iterator erase(iterator pos) {
        if (pos == begin() && pos == end()) {
            iterator ret = pos;
            _size--;
            return ret;
        }
        if (pos >= end()) { // have to throw if the position is past the end
            throw std::out_of_range("iterator out of range");
        }

        for (iterator i = pos; i < end() - 1; i++) { // moving everything to the left 
            *i = *(i + 1);
        }
        _size--; // never actually have to delete anything
        return pos;
    };
    
    iterator erase(iterator first, iterator last) {
        if (first < begin() || last > end() || first > last) {
            throw std::out_of_range("iterator out of range");
        }
        if (first == last) {
            return first;
        }
        ptrdiff_t offset = last - first;
        for (iterator i = first; i < end() - offset; i++) {
            *i = *(i + offset);
        }
        _size-= offset;
        return first;
    };

    class iterator {
    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type        = T;
        using difference_type   = ptrdiff_t;
        using pointer           = T*;
        using reference         = T&;
    private:
        T* p;
        // Add your own data members here
        // HINT: For random_access_iterator, the data member is a pointer 99.9% of the time
    public:
        iterator() {
            p = nullptr;
        };
        iterator(T* ptr) {
            p = ptr;
        }
        //create copy constructor
        

        // This assignment operator is done for you, please do not add more
        iterator& operator=(const iterator&) noexcept = default;

        [[nodiscard]] reference operator*() const noexcept {
            return *p;
        };
        [[nodiscard]] pointer operator->() const noexcept {
            return p;
        };

        // Prefix Increment: ++a
        iterator& operator++() noexcept {
            p++;
            return *this;
        };
        // Postfix Increment: a++
        iterator operator++(int) noexcept {
            iterator old(p);
            p++;
            return old;
        };
        // Prefix Decrement: --a
        iterator& operator--() noexcept {
            p--;
            return *this;
        };
        // Postfix Decrement: a--
        iterator operator--(int) noexcept{
            iterator old(p);
            p--;
            return old;
        };

        iterator& operator+=(difference_type offset) noexcept {
            p += offset;
            return *this;
        };
        [[nodiscard]] iterator operator+(difference_type offset) const noexcept {
            return iterator(p + offset);
        };
        
        iterator& operator-=(difference_type offset) noexcept {
            p -= offset;
            return *this;
        };

        [[nodiscard]] iterator operator-(difference_type offset) const noexcept {
            return iterator(p - offset);
        };

        [[nodiscard]] difference_type operator-(const iterator& rhs) const noexcept {
            return (p - rhs.p);
        };

        [[nodiscard]] reference operator[](difference_type offset) const noexcept{
            return *(p + offset);
        };

        [[nodiscard]] bool operator==(const iterator& rhs) const noexcept {
            return (p == rhs.p); 
        };
        [[nodiscard]] bool operator!=(const iterator& rhs) const noexcept {
            return (p != rhs.p);
        };
        [[nodiscard]] bool operator<(const iterator& rhs) const noexcept {
            if (p < rhs.p) {
                return true;
            }
            return false;
        };
        [[nodiscard]] bool operator>(const iterator& rhs) const noexcept {
            if (p > rhs.p) {
                return true;
            }
            return false;
        };
        [[nodiscard]] bool operator<=(const iterator& rhs) const noexcept {
            if (p <= rhs.p) {
                return true;
            }
            return false;
        };
        [[nodiscard]] bool operator>=(const iterator& rhs) const noexcept {
            if (p >= rhs.p) {
                return true;
            }
            return false;
        };
    };

    void clear() noexcept {
        delete[] array;
        array = nullptr;
        _size = 0;
    };
};

template <class T>
[[nodiscard]] typename Vector<T>::iterator operator+(typename Vector<T>::iterator::difference_type offset, typename Vector<T>::iterator iterator) noexcept;

#endif
