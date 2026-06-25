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

int main(int argc, char *argv[])
{
  alisov::Vector< alisov::Person > people;
  if (argc < 1) {
    return 1;
  }
  if (argc > 3) {
    return 1;
  }
}