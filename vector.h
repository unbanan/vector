#pragma once
#include <algorithm>
#include <cassert>

template <typename T>
class vector {
public:
  using value_type = T;

  using reference = T&;
  using const_reference = const T&;

  using pointer = T*;
  using const_pointer = const T*;

  using iterator = pointer;
  using const_iterator = const_pointer;

public:
  vector() noexcept : data_(nullptr), size_(0), capacity_(0) {}

private:
  vector(const vector& other, std::size_t new_capacity)
      : data_(new_capacity == 0 ? nullptr : static_cast<T*>(operator new(sizeof(T) * new_capacity))),
        size_(other.size_),
        capacity_(new_capacity) {
    assert(new_capacity >= other.size_);
    for (size_t i = 0; i < other.size_; i++) {
      try {
        new (begin() + i) T(other.data_[i]);
      } catch (...) {
        while (i--) {
          data_[i].~T();
        }
        operator delete(data_);
        throw;
      }
    }
  }

public:
  vector(const vector& other) : vector(other, other.size_) {}

  vector& operator=(const vector& other) {
    if (this != &other) {
      vector tmp(other);
      swap(tmp);
    }
    return *this;
  }

  ~vector() noexcept {
    clear();
    operator delete(data_);
  }

  reference operator[](size_t index) {
    return data_[index];
  }

  const_reference operator[](size_t index) const {
    return data_[index];
  }

  pointer data() noexcept {
    return data_;
  }

  const_pointer data() const noexcept {
    return data_;
  }

  size_t size() const noexcept {
    return size_;
  }

  reference front() {
    return data_[0];
  }

  const_reference front() const {
    return data_[0];
  }

  reference back() {
    return data_[size_ - 1];
  }

  const_reference back() const {
    return data_[size_ - 1];
  }

  void push_back(const T& element) {
    if (size_ == capacity_) {
      vector tmp(*this, capacity_ == 0 ? 1 : capacity_ * 2);
      tmp.push_back(element);
      swap(tmp);
    } else {
      new (end()) T(element);
      size_++;
    }
  }

  void pop_back() {
    assert(!empty());
    data_[--size_].~T();
  }

  bool empty() const noexcept {
    return size_ == 0;
  }

  size_t capacity() const noexcept {
    return capacity_;
  }

  void reserve(size_t new_capacity) {
    if (capacity_ < new_capacity) {
      vector tmp = vector(*this, new_capacity);
      swap(tmp);
    }
  }

  void shrink_to_fit() {
    if (size_ < capacity_) {
      vector tmp = vector(*this, size_);
      swap(tmp);
    }
  }

  void clear() noexcept {
    while (!empty()) {
      pop_back();
    }
  }

  void swap(vector& other) noexcept {
    std::swap(data_, other.data_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
  }

  iterator begin() noexcept {
    return data_;
  }

  iterator end() noexcept {
    return data_ + size_;
  }

  const_iterator begin() const noexcept {
    return data_;
  }

  const_iterator end() const noexcept {
    return data_ + size_;
  }

  void rotate(iterator begin, iterator first, iterator end) {
    while (begin != first && first != end) {
      iterator beginCopy = first;
      for (; begin != first && beginCopy != end; ++begin, ++beginCopy) {
        std::iter_swap(begin, beginCopy);
      }
      if (begin == first) {
        first = beginCopy;
      }
    }
  }

  iterator insert(const_iterator pos, const T& value) {
    ptrdiff_t offset = pos - begin();
    push_back(value);
    iterator new_pos = begin() + offset;
    rotate(new_pos, end() - 1, end());
    return new_pos;
  }

  iterator erase(const_iterator pos) noexcept {
    return erase(pos, pos + 1);
  }

  iterator erase(const_iterator first, const_iterator last) noexcept {
    iterator p = begin() + (first - begin());
    ptrdiff_t n = last - first;
    ptrdiff_t add = first - begin();
    rotate(p, p + n, end());
    while (n--) {
      pop_back();
    }
    return (begin() + add);
  }

private:
  pointer data_;
  size_t size_;
  size_t capacity_;
};