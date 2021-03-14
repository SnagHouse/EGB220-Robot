#ifndef List_h__
#define List_h__

#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <new.h>

template<typename T> T utilMin(T const & a, T const & b)
{
  return a < b ? a : b;
}

template<typename T> T utilMax(T const & a, T const & b)
{
  return a > b ? a : b;
}

template<typename T> T utilClamp(T const & value, T const & a, T const & b)
{
  return utilMax(a, utilMin(b, value));
}

/**
 * A lightweight implementation of a dynamically allocated List.
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
void listMoveConstruct(T * pDest, T * pSrc, uint32_t count, bool reverse = false) {
  if (reverse)
  {
    pDest += count;
    pSrc += count;

    while (count-- > 0)
      new (--pDest) T(move(*--pSrc));
  }
  else
  {
    while (count-- > 0)
      new (pDest++) T(move(*pSrc++));
  }
}

template<typename T>
void listCopyConstruct(T * pDest, T const * pSrc, uint32_t count, bool reverse = false) {
  if (reverse)
  {
    pDest += count;
    pSrc += count;

    while (count-- > 0)
      new (--pDest) T(*--pSrc);
  }
  else
  {
    while (count-- > 0)
      new (pDest++) T(*pSrc++);
  }
}

template<typename T>
void listDestruct(T * pDest, uint32_t count) {
  while (count-- > 0) {
    (pDest++)->~T();
  }
}

/**
 * Implement an optimized move for an array of a built-in type. 
 **/
#define ImplementBasicMoveAndCopy(type)\
template<> inline void listMoveConstruct<type>(type *pDest, type *pSrc, uint32_t count, bool reverse)        { memmove(pDest, pSrc, count * sizeof(type)); }\
template<> inline void listCopyConstruct<type>(type *pDest, type const * pSrc, uint32_t count, bool reverse) { memmove(pDest, pSrc, count * sizeof(type)); }\
template<> inline void listDestruct<type>(type *pDest, uint32_t count)                                       { pDest, count; }

ImplementBasicMoveAndCopy(char)
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
  /**
   * Construct a list with an initial capacity
   **/
  List(int capacity = 0)
  {
    reserve(capacity);
  }

  /**
   * Construct a list by moving data from another list
   **/
  List(List &&o)
  {
    *this = move(o);
  }

  /**
   * Construct a list by copying data from another list
   **/
  List(List const &o)
  {
    *this = o;
  }

  /**
   * Assign one list to another.
   **/
  List& operator=(List const &o)
  {
    clear();
    reserve(o.size());
    listCopyConstruct(data(), o.data(), o.size());
    m_size = o.size();
    return *this;
  }

  /**
   * Assign one list to another using move semantics.
   **/
  List& operator=(List &&o)
  {
    clear();
    free(m_pData);
    m_pData = o.m_pData;
    m_capacity = o.m_capacity;
    m_size = o.m_size;
    o.m_pData = nullptr;
    o.m_capacity = 0;
    o.m_size = 0;
    return *this;
  }

  /**
   * Destroy the list and cleanup memory.
   **/
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
    listMoveConstruct(pDest, pDest + 1, remaining, true);
    listMoveConstruct(pDest, &value, 1); // Move the item into pDest

    ++m_size;
  }

  void addRange(T const * pBegin, T const *pEnd)
  {
    insertRange(size(), pBegin, pEnd);
  }

  void insertRange(uint32_t index, T const *pBegin, T const * pEnd)
  {
    uint32_t count = uint32_t(pEnd - pBegin);
    if (count > 0)
    {
      grow(size() + count);

      // Get a pointer to where the items will be inserted
      T * pDest = begin() + index;

      // Get the number of items in the list after 'index'
      uint32_t remaining = (uint32_t)(end() - pDest);

      // Move items, making make space for the new onces
      listCopyConstruct(pDest, pDest + count, remaining, true);
      listCopyConstruct(pDest, pBegin, count); // Move the items into pDest

      m_size += count; // Increase the size
    }
  }

  void addRangeMove(T * pBegin, T *pEnd)
  {
    insertRangeMove(size(), pBegin, pEnd, move);
  }

  void insertRangeMove(uint32_t index, T * pBegin, T * pEnd)
  {
    uint32_t count = pEnd - pBegin;
    if (count > 0)
    {
      grow(size() + count);

      // Get a pointer to where the items will be inserted
      T *pDest = begin() + index;

      // Get the number of items in the list after 'index'
      uint32_t remaining = (uint32_t)(end() - pDest);

      // Move items, making make space for the new onces
      listMoveConstruct(pDest, pDest + count, remaining, true);
      listMoveConstruct(pDest, pBegin, count); // Move the items into pDest

      m_size += count; // Increase the size
    }
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
  bool erase(uint32_t index, uint32_t count = 1)
  {
    // Check the index is valid
    if (index < 0 || index >= size())
      return false;

    T * pElement = m_pData + index;
    T * pNext = pElement + 1;

    listDestruct(pElement, 1);

    // Shift elements down 1 index
    uint32_t remaining = (uint32_t)(end() - pNext);
    listMoveConstruct(pElement, pNext, remaining);

    --m_size;

    return true;
  }

  /**
   * Find the index of a value in the list.
   **/
  uint32_t find(T const &value, uint32_t fromIndex = 0, uint32_t toIndex = ~uint32_t(0)) const
  {
    uint32_t end = toIndex;
    if (end > size())
      end = size();
    for (uint32_t i = fromIndex; i < end; ++i)
      if (at(i) == value)
        return i;
    return size();
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
        listMoveConstruct(pNewBuffer, m_pData, m_size);
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
    listDestruct(begin(), size());
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

  /**
   * Access an element in the list by index. (const)
   **/
  T const & at(uint32_t index) const
  {
    return m_pData[index];
  }

  /**
   * Array access operator.
   **/
  T &operator[](uint32_t index)
  {
    return at(index);
  }

  /**
   * Array access operator. (const)
   **/
  T const &operator[](uint32_t index) const
  {
    return at(index);
  }

  /**
   * Return a new list containing the elements within the range specified.
   **/
  List<T> getElements(uint32_t start, uint32_t count) const
  {
    start = utilMax<uint32_t>(start, 0);
    uint32_t end = utilMin(start + count, size());

    List<T> elements(end - start);
    elements.addRange(begin() + start, begin() + end);
    return elements;
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

  T& back() {
    return at(size() - 1);
  }

  T& front() {
    return at(0);
  }

  T const & front() const {
    return at(0);
  }

  T const & back() const {
    return at(size() - 1);
  }

protected:
  void grow(uint32_t desiredCapacity)
  {
    if (desiredCapacity > m_capacity)
      reserve(desiredCapacity * 2);
  }

  T *m_pData          = nullptr;
  uint32_t m_size     = 0;
  uint32_t m_capacity = 0;
};

#endif // List_h__
