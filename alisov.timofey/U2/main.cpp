#include <cstddef>
#include <fstream>
#include <iostream>
#include <string>

namespace alisov
{
  struct Person
  {
    size_t id;
    std::string info;
  };

  struct Meet
  {
    size_t id1;
    size_t id2;
    size_t duration;
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

  bool id_unique(const Vector< Person > &vec, const size_t id)
  {
    for (size_t i = 0; i < vec.size; ++i) {
      if (vec.data[i].id == id) {
        return false;
      }
    }
    return true;
  }

  bool parse_person(const std::string &line, Person &out_person, bool &is_empty)
  {
    is_empty = false;
    if (line.empty()) {
      is_empty = true;
      return false;
    }
    const size_t first_non_space = line.find_first_not_of(" \t\r\n");
    if (first_non_space == std::string::npos) {
      is_empty = true;
      return false;
    }
    if (line[first_non_space] < '0' || line[first_non_space] > '9') {
      return false;
    }
    const size_t id_end = line.find_first_of(" \t\r\n", first_non_space);
    std::string id_str = "";
    if (id_end == std::string::npos) {
      id_str = line.substr(first_non_space);
    } else {
      id_str = line.substr(first_non_space, id_end - first_non_space);
    }
    size_t parsed_bytes = 0;
    const unsigned long long temp_id = std::stoull(id_str, &parsed_bytes);
    if (parsed_bytes != id_str.length()) {
      return false;
    }
    if (id_end == std::string::npos) {
      return false;
    }
    const size_t info_start = line.find_first_not_of(" \t\r\n", id_end);
    if (info_start == std::string::npos) {
      return false;
    }
    const size_t info_end = line.find_last_not_of(" \t\r\n");
    out_person.id = static_cast< size_t >(temp_id);
    out_person.info = line.substr(info_start, info_end - info_start + 1);
    return true;
  }
}

int main()
{}
