#include <cstddef>
#include <iostream>

namespace alisov
{
  struct Person
  {
    size_t id;
    std::string info;
  };

  template < class T >
  struct Vector
  {
    T *data;
    size_t size;
    size_t cap;

    ~Vector()
    {
      delete[] data;
    }
  };

  template < class T >
  void init(Vector< T > &vec)
  {
    vec.data = nullptr;
    vec.size = 0;
    vec.cap = 0;
  }

  template < class T >
  void reserve(Vector< T > &vec, size_t new_cap)
  {
    if (new_cap <= vec.cap) {
      return;
    }
    T *new_data = new T[new_cap];
    for (size_t i = 0; i < vec.size; ++i) {
      new_data[i] = vec.data[i];
    }
    delete[] vec.data;
    vec.data = new_data;
    vec.cap = new_cap;
  }

  template < class T >
  void push_back(Vector< T > &vec, const T &val)
  {
    if (vec.size >= vec.cap) {
      size_t new_cap = vec.cap == 0 ? 4 : vec.cap * 2;
      reserve(vec, new_cap);
    }
    vec.data[vec.size] = val;
    ++vec.size;
  }

}

int main(int argc, char *argv[])
{
  alisov::Vector< alisov::Person > people;
  alisov::init(people);

  if (argc < 1) {
    return 1;
  }
  if (argc > 3) {
    return 1;
  }
}