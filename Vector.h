#include <memory>
#include <algorithm>
#include <iterator>

template <class T, class Allocator = std::allocator<T>>
class Vector {
public:
  typedef T* iterator;
  typedef const T* const_iterator;
  typedef std::reverse_iterator<iterator> reverse_iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
  typedef size_t size_type;
  typedef T value_type;
  typedef Allocator allocator_type;
  typedef std::ptrdiff_t difference_type;
  typedef value_type& reference;
  typedef const value_type& const_reference;
  typedef typename std::allocator_traits<Allocator>::pointer pointer;
  typedef const typename std::allocator_traits<Allocator>::pointer const_pointer;

  Vector() 
  { 
    create();
  }

  explicit Vector(size_type count, const_reference t = T{}) 
  { 
    create(n, t);
  }

  explicit Vector(size_type count, const Allocator& alloc = Allocator())
  {
    create(count);
  }

  explicit Vector(const Allocator& alloc) noexcept
  {
    create();
  }

  Vector(const Vector& other)
  {
    create(other.begin(), other.end());
  }

  Vector(std::initializer_list<T> init, const Allocator& alloc = Allocator())
  {
    create(init.begin(), init.end());
  }
  Vector<T, Allocator>& operator = (const Vector& vector);

  Vector<T, Allocator>::allocator_type get_allocator();

  bool empty();

  Vector<T, Allocator>::size_type max_size();

  ~Vector()
  {
    uncreate();
  }

  Vector& assign(const Vector&);

  T& operator [] (size_type i)
  { 
    return elements[i]; 
  }

  const T& operator [] (size_type i) const
  { 
    return elements[i]; 
  }

  reference at(size_type i)
  {
    if (i >= this->size() || i < 0)
    {
      throw std::out_of_range;
    }
    return elements[i];
  }

  const reference at(size_type i) const
  {
    if (i >= this->size() || i < 0)
    {
      throw std::out_of_range;
    }
    return elements[i];
  }

  reference front()
  {
    return elements;
  }

  const reference front() const
  {
    return const elements;
  }

  reference back()
  {
    if (this->size() = 0)
    {
      return std::prev(this->end());
    }
    return *(available - 1);
  }

  const reference back() const
  {
    if (this->size() = 0)
    {
      return const std::prev(this->end());
    }
    return const *(available - 1);
  }

  void push_back(const_reference& t)
  {
    if (available == limit)
    {
      grow();
    }
    unchecked_append(t);
  }

  template< class... Args >
  reference emplace_back(Args&&... args)
  {
    alloc.construct(std::forward<Args>(args)..., new_element);
    if (available == limit)
    {
      grow();
    }
    unchecked_append(new_element);
  }

  void pop_back()
  {
    delete (this->end() - 1);
    --available;
  }

  size_type size() const
  { 
    return available - elements;
  }

  void resize(size_type count)
  {
    if (count > this->max_size())
    {
      iterator new_elements = alloc.allocate(count);
      iterator new_available = std::uninitialized_copy(elements, available, new_elements);
      uncreate();

      elements = new_elements;
      available = new_available;
      limit = count;
    }
    else if (count < this->max_size)
    {
      int overflow = count - this->max_size();
      for (iterator it = this->end() - 1; it != this->end() - overflow; --it)
      {
        delete it;
      }
    }
  }

  void resize(size_type count, const value_type& value)
  {
    if (count > this->max_size())
    {
      iterator new_elements = alloc.allocate(count);
      iterator new_available = std::uninitialized_copy(elements, available, new_elements);
      uncreate();

      elements = new_elements;
      available = new_available;
      limit = count;
      for (iterator it = available; it != limit; it++)
      {
        it = value;
      }
    }
    else if (count < this->max_size)
    {
      int overflow = count - this->max_size();
      for (iterator it = this->end() - 1; it != this->end() - overflow; --it)
      {
        delete it;
      }
    }
  }

  void swap(Vector& other) noexcept
  {
    std::iter_swap(this->begin(), other.begin());
    std::iter_swap(this->end(), other.end());
  }

  size_type capacity() const noexcept
  { 
    return limit - elements; 
  }

  iterator begin() {
    return elements;
  }

  const_iterator begin() const
  { 
    return elements; 
  }

  const_iterator cbegin() const
  {
    return const elements;
  }

  reverse_iterator rbegin() noexcept
  {
    return elements;
  }

  const_reverse_iterator rbegin() const noexcept
  {
    return const elements;
  }

  const_reverse_iterator crbegin() const noexcept
  {
    return const elements;
  }

  iterator data() noexcept
  {
    return elements;
  }

  iterator end() 
  { 
    return available; 
  }

  const_iterator end() const
  { 
    return available; 
  }

  const_iterator cend() const
  {
    return const available;
  }

  reverse_iterator rend() noexcept
  {
    return available;
  }

  const_reverse_iterator rend() const noexcept
  {
    return const available;
  }

  const_reverse_iterator crend() const noexcept
  {
    return const available;
  }

  allocator_type get_allocator() const;

  bool empty() const noexcept;

  size_type max_size() const noexcept;

  void reserve(size_type new_cap);

  void shrink_to_fit();

  void clear() noexcept;

  iterator insert(const_iterator pos, const_reference value);
  iterator insert(const_iterator pos, size_type count, const_reference value);
  template< class InputIt >
  iterator insert(const_iterator pos, InputIt first, InputIt last);
  iterator insert(const_iterator pos, std::initializer_list<T> ilist);

  template< class... Args >
  iterator emplace(const_iterator pos, Args&&... args);

  iterator erase(const_iterator pos);
  iterator erase(const_iterator first, const_iterator last);

  friend void swap(Vector& lhs,
    Vector& rhs) noexcept;

  template< class T, class Alloc >
  friend bool operator == (const Vector<T, Alloc>& lhs,
    const Vector<T, Alloc>& rhs);

  template< class T, class Alloc >
  friend bool operator != (const Vector<T, Alloc>& lhs,
    const Vector<T, Alloc>& rhs);

  template< class T, class Alloc >
  friend bool operator < (const Vector<T, Alloc>& lhs,
    const Vector<T, Alloc>& rhs);

  template< class T, class Alloc >
  friend bool operator <= (const Vector<T, Alloc>& lhs,
    const Vector<T, Alloc>& rhs);

  template< class T, class Alloc >
  friend bool operator > (const Vector<T, Alloc>& lhs,
    const Vector<T, Alloc>& rhs);

  template< class T, class Alloc >
  friend bool operator >= (const Vector<T, Alloc>& lhs,
    const Vector<T, Alloc>& rhs);
private:
  iterator elements;
  iterator available;
  iterator limit; 

  // atminties išskyrimo valdymui
  std::allocator<T> alloc;
  // išskirti atmintį (array) ir ją inicializuoti
  void create();
  void create(size_type, const T&);
  void create(const_iterator, const_iterator);
  // sunaikinti elementus array ir atlaisvinti atmintį
  void uncreate();
  // pagalbinės funkcijos push_back() realizacijai
  void grow();
  void unchecked_append(const T&);
};

template <class T, class Alloc>
void Vector<T, Alloc>::create() 
{
  elements = available = limit = nullptr;
}
template <class T, class Alloc>
void Vector<T, Alloc>::create(Vector<T, Alloc>::size_type n, const T& val)
{
  elements = alloc.allocate(n);
  limit = available = elements + n;
  std::uninitialized_fill(elements, limit, val);
}
template <class T, class Alloc>
void Vector<T, Alloc>::create(Vector<T, Alloc>::const_iterator i, Vector<T, Alloc>::const_iterator j)
{
  elements = alloc.allocate(j - i);
  limit = available = std::uninitialized_copy(i, j, elements);
}

template <class T, class Alloc>
void Vector<T, Alloc>::uncreate() 
{
  if (elements) {
    // sunaikinti (atbuline tvarka) sukonstruotus elementus
    iterator it = available;
    while (it != elements)
    {
      alloc.destroy(--it);
    }
      
    // atlaisvinti išskirtą atmintį. Turi būti elements != nullptr
    alloc.deallocate(elements, limit - elements);
  }
  // reset'inam pointer'iuss - Vector'ius tuščias
  elements = limit = available = nullptr;
}

template <class T, class Alloc>
void Vector<T, Alloc>::grow()
{
  size_type new_size = std::max(2 * (limit - elements), ptrdiff_t(1));
  // išskiriama nauja vieta ir perkopijuojami egzistuojantys elementai
  iterator new_elements = alloc.allocate(new_size);
  iterator new_available = std::uninitialized_copy(elements, available, new_elements);
  // atlaisvinama sena vieta
  uncreate();
  // rodyklės perkeliamos į naujai priskirtą vietą
  elements = new_elements;
  available = new_available;
  limit = elements + new_size;
}

template <class T, class Alloc>
void Vector<T, Alloc>::unchecked_append(const T& val)
{
  alloc.construct(available++, val);
}

template <class T, class Alloc>
Vector<T, Alloc>& Vector<T, Alloc>::operator = (const Vector& vector)
{
  if (&vector != this) {
    uncreate();
    create(vector.begin(), vector.end());
  }
  return *this;
}

template <class T, class Alloc>
typename Vector<T, Alloc>::allocator_type Vector<T, Alloc>::get_allocator()
{
  return alloc;
}

template <class T, class Alloc>
bool Vector<T, Alloc>::empty()
{
  return begin() == end();
}

template <class T, class Alloc>
typename Vector<T, Alloc>::size_type Vector<T, Alloc>::max_size()
{
  return limit;
}

template <class T, class Alloc>
void Vector<T, Alloc>::reserve(size_type new_cap)
{
  alloc.allocate(new_cap);
  iterator new_available = std::uninitialized_copy(elements, available, new_elements);
  uncreate();
  elements = new_elements;
  available = new_available;
  limit = elements + new_size;
}

template <class T, class Alloc>
void Vector<T, Alloc>::shrink_to_fit()
{
  
  iterator new_elements = alloc.allocate(this->size());
  iterator new_available = std::uninitialized_copy(elements, available, new_elements);
  uncreate();
  elements = new_elements;
  available = new_available;
  limit = available;
}

template <class T, class Alloc>
void Vector<T, Alloc>::clear() noexcept
{
  uncreate();
}

template <class T, class Alloc>
typename Vector<T, Alloc>::iterator Vector<T, Alloc>::insert(const_iterator pos, const_reference value)
{
  
  if (capacity >= 1)
  {
    *pos = value;
    return pos;
  }
  else
  {
    this->grow();
    *pos = value;
    return pos;
  }
}

template <class T, class Alloc>
typename Vector<T, Alloc>::iterator Vector<T, Alloc>::insert(const_iterator pos, Vector<T, Alloc>::size_type count, const_reference value)
{
  if (count <= this->capacity)
  {
    --pos;
    for (int i = 0; i < count; i++)
    {
      std::swap(pos, available + i);
      *pos = value;
      ++pos
      ++current;
    }
  }
  else {
    while (count > this->capacity())
    {
      this->grow();
    }
    --pos;
    for (int i = 0; i < count; i++)
    {
      std::swap(pos, available + i);
      *pos = value;
      ++pos
        ++current;
    }
  }
}

template <class T, class Alloc>
typename Vector<T, Alloc>::iterator Vector<T, Alloc>::insert(const_iterator pos, std::initializer_list<T> ilist)
{
  if (ilist.size() <= this->capacity())
  {
    int current = 0;
    --pos;
    for (for elem in ilist)
    {
      std::swap(pos, available + current);
      *pos = elem;
      ++pos
        ++current;
    }
  }
  else {
    while (count > this->capacity())
    {
      this->grow();
    }

    int current = 0;
    --pos;
    for (for elem in ilist)
    {
      std::swap(pos, available + current);
      *pos = elem;
      ++pos;
      ++current;
    }
  }
}

template <class T, class Alloc>
typename Vector<T, Alloc>::iterator Vector<T, Alloc>::erase(const_iterator pos)
{
  for (it = pos; it != available; ++it)
  {
    it = it + 1;
  }
  (available - 1) = nullptr;
  --available;
  return pos;
}

template <class T, class Alloc>
typename Vector<T, Alloc>::iterator Vector<T, Alloc>::erase(const_iterator first, const_iterator last)
{
  for (iterator it first; it != last; ++it)
  {
    delete it;
  }
  std::move(last, this->end(), first);
  return first;
}

template <class T, class Alloc>
Vector<T, Alloc>& Vector<T, Alloc>::assign(const Vector& vector) {
  if (&vector != this) {
    uncreate();
    create(vector.begin(), vector.end());
  }
  return *this;
}

template <class T, class Alloc>
template<class InputIt>
typename Vector<T, Alloc>::iterator Vector<T, Alloc>::insert(const_iterator pos, InputIt first, InputIt last)
{
  if (last - first <= this->capacity)
  {
    int current = 0;
    --pos;
    for (iterator it = first; it != last; ++it)
    {
      std::swap(pos, available + current);
      *pos = *it;
      ++pos;
      ++current;
    }
  }
  else {
    while (count > this->capacity())
    {
      this->grow();
    }

    int current = 0;
    --pos;
    for (iterator it = first; it != last; ++it)
    {
      std::swap(pos, available + current);
      *pos = *it;
      ++pos;
      ++current;
    }
  }
}

template <class T, class Alloc>
template<class ...Args>
typename Vector<T, Alloc>::iterator Vector<T, Alloc>::emplace(const_iterator pos, Args&& ...args)
{
  const_reference new_elements;
  alloc.construct(std::forward<Args>(args)..., new_elements);
  if (capacity < sizeof(new_elements)) {
    this->grow();
  }
   --pos;
  int current = 0;
  for (element in new_elements)
  {
    std::swap(pos, available + current);
    pos = element;
    ++pos;
    ++current;
  }
}

template< class T, class Alloc >
void swap(Vector<T, Alloc>& lhs,
  Vector<T, Alloc>& rhs) noexcept
{
  lhs.swap(rhs);
}

template<class T, class Alloc>
bool operator == (const Vector<T, Alloc>& lhs, const Vector<T, Alloc>& rhs)
{
  return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template<class T, class Alloc>
bool operator != (const Vector<T, Alloc>& lhs, const Vector<T, Alloc>& rhs)
{
  return !std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template<class T, class Alloc>
bool operator < (const Vector<T, Alloc>& lhs, const Vector<T, Alloc>& rhs)
{
  return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template<class T, class Alloc>
bool operator <= (const Vector<T, Alloc>& lhs, const Vector<T, Alloc>& rhs)
{
  return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()) || std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template<class T, class Alloc>
bool operator > (const Vector<T, Alloc>& lhs, const Vector<T, Alloc>& rhs)
{
  return !std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template<class T, class Alloc>
bool operator >= (const Vector<T, Alloc>& lhs, const Vector<T, Alloc>& rhs)
{
  return !std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()) || std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}
