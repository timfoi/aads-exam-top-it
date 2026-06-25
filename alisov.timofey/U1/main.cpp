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
}

int main()
{}