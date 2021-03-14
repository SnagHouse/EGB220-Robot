#ifndef List_h__
#define List_h__

#include <string.h>
#include <stdint.h>

/**
 * A lightweight implementation of a dynamically allocated List.
 * It is analogous to std::vector from STL.
 **/

/**
 * Move implementation.
 * Just casts to a R-value reference.
 **/
template<typename T>
T&& move(T &o) { return static_cast<T&&>(o); }

/**
 * Templated overload for alloc().
 * 
 * Allocate an uninitialized buffer to contain 'count' number of elements of type T.
 * The buffer size will be 'sizeof(T) * count'
 * 
 * @param count The number of elements to be allocated.
 * @return      A pointer to the allocated memory.
 **/
template<typename T>
T* malloc(uint32_t count) { return (T*)malloc(sizeof(T) * count); }

template<typename T>
void moveConstruct(T * pDest, T * pSrc, uint32_t count, bool reverse = false) {
  if (reverse) {
    pDest += count;
    pSrc  += count;
    while (count-- > 0) {
      new (--pDest) T(move(*--pSrc));
    }
  }
  else {
    while (count-- > 0) {
      new (pDest++) T(move(*pSrc++));
    }
  }
}

template<typename T>
void copyConstruct(T * pDest, T * pSrc, uint32_t count, bool reverse = false) {
  if (reverse) {
    pDest += count;
    pSrc  += count;
    while (count-- > 0) {
      new (--pDest) T(*--pSrc);
    }
  }
  else {
    while (count-- > 0) {
      new (pDest++) T(*pSrc++);
    }
  }
}

template<typename T>
void destruct(T * pDest, uint32_t count) {
  while (count-- > 0) {
    (pDest++)->~T();
  }
}

/**
 * Implement an optimized move for an array of a built-in type. 
 **/
#define ImplementBasicMoveAndCopy(type)\
template<> void moveConstruct<type>(type *pDest, type *pSrc, uint32_t count, bool reverse) { memmove(pDest, pSrc, count * sizeof(type)); }\
template<> void copyConstruct<type>(type *pDest, type *pSrc, uint32_t count, bool reverse) { memmove(pDest, pSrc, count * sizeof(type)); }\
template<> void destruct<type>(type *pDest, uint32_t count) { pDest, count; }\

ImplementBasicMoveAndCopy(bool)
ImplementBasicMoveAndCopy(float)
ImplementBasicMoveAndCopy(double)
ImplementBasicMoveAndCopy(int8_t)
ImplementBasicMoveAndCopy(int16_t)
ImplementBasicMoveAndCopy(int32_t)
ImplementBasicMoveAndCopy(int64_t)
ImplementBasicMoveAndCopy(uint8_t)
ImplementBasicMoveAndCopy(uint16_t)
ImplementBasicMoveAndCopy(uint32_t)
ImplementBasicMoveAndCopy(uint64_t)

template <typename T>
class List
{
public:
  List(int capacity = 0)
  {
    reserve(capacity);
  }

  ~List()
  {
    clear();
    free(m_pData);
  }

  /**
   * Add an item to the back of the list.
   **/
  void add(T value)
  {
    insert(size(), move(value));
  }
  
  /**
   * Insert an item into the list.
   **/
  void insert(uint32_t index, T value)
  {
    grow(size() + 1);

    T *pDest = begin() + index;
    uint32_t remaining = (uint32_t)(end() - pDest);

    // Copy the buffer, iterating in the reverse order
    moveConstruct(pDest, pDest + 1, remaining, true);
    moveConstruct(pDest, &value, 1); // Move the item into pDest

    ++m_size;
  }

  /**
   * Remove an item from the list by value.
   **/
  bool remove(T const &value)
  {
    return erase(find(value));
  }

  /**
   * Remove an item from the list by index
   **/
  bool erase(uint32_t index)
  {
    // Check the index is valid
    if (index < 0 || index >= size())
      return false;

    T * pElement = m_pData + index;
    T * pNext = pElement + 1;

    destruct(pElement, 1);

    // Shift elements down 1 index
    uint32_t remaining = (uint32_t)(end() - pNext);
    moveConstruct(pElement, pNext, remaining);

    --m_size;

    return true;
  }

  /**
   * Find the index of a value in the list.
   **/
  uint32_t find(T const &value)
  {
    for (uint32_t i = 0; i < size(); ++i)
      if (at(i) == value)
        return i;
    return -1;
  }

  /**
   * Pre-allocate memory for the list.
   **/
  void reserve(uint32_t capacity)
  {
    if (m_capacity < capacity)
    {
      T * pNewBuffer = malloc<T>(capacity);
      if (m_pData)
        moveConstruct(pNewBuffer, m_pData, m_size);
      free(m_pData);
      m_pData = pNewBuffer;
      m_capacity = capacity;
    }
  }

  /**
   * Get the number of elements in the list.
   **/
  uint32_t size() const
  {
    return m_size;
  }

  /**
   * Get the capacity of the list.
   **/
  uint32_t capacity() const
  {
    return m_capacity;
  }

  /**
   * Remove all items from the list
   **/
  void clear()
  {
    destruct(begin(), size());
    m_size = 0;
  }

  /**
   * Get a pointer to the internal buffer.
   **/
  T * data()
  {
    return m_pData;
  }

  /**
  * Get a pointer to the internal buffer.
  **/
  T const * data() const
  {
    return m_pData;
  }

  /**
   * Access an element in the list by index.
   **/
  T &at(uint32_t index)
  {
    return m_pData[index];
  }

  T const & at(uint32_t index) const
  {
    return m_pData[index];
  }

  T &operator[](uint32_t index)
  {
    return at(index);
  }

  T const &operator[](uint32_t index) const
  {
    return at(index);
  }

  /**
   * Iterators for range based for loops .
   **/
  T *begin() {
    return data();
  }

  T *end() {
    return begin() + size();
  }

  T const *begin() const {
    return data();
  }

  T const *end() const {
    return begin() + size();
  }

protected:
  void grow(uint32_t desiredCapacity)
  {
    if (desiredCapacity > m_capacity)
      reserve((m_capacity == 0 ? 1 : m_capacity) * 2);
  }

  T *m_pData          = nullptr;
  uint32_t m_size     = 0;
  uint32_t m_capacity = 0;
};

#endif // List_h__
