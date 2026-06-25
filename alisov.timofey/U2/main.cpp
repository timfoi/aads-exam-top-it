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

  template < class T, class Compare >
  void bubble_sort(Vector< T > &vec, Compare comp)
  {
    if (vec.size == 0) {
      return;
    }
    for (size_t i = 0; i < vec.size - 1; ++i) {
      for (size_t j = 0; j < vec.size - i - 1; ++j) {
        if (comp(vec.data[j + 1], vec.data[j])) {
          T temp = vec.data[j];
          vec.data[j] = vec.data[j + 1];
          vec.data[j + 1] = temp;
        }
      }
    }
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

  bool parse_meet(const std::string &line, Meet &out_meet)
  {
    if (line.empty()) {
      return false;
    }
    const size_t f_space = line.find_first_not_of(" \t\r\n");
    if (f_space == std::string::npos || line[f_space] < '0' || line[f_space] > '9') {
      return false;
    }
    const size_t id1_end = line.find_first_of(" \t\r\n", f_space);
    if (id1_end == std::string::npos) {
      return false;
    }

    const size_t id2_start = line.find_first_not_of(" \t\r\n", id1_end);
    if (id2_start == std::string::npos || line[id2_start] < '0' || line[id2_start] > '9') {
      return false;
    }
    const size_t id2_end = line.find_first_of(" \t\r\n", id2_start);
    if (id2_end == std::string::npos) {
      return false;
    }

    const size_t dur_start = line.find_first_not_of(" \t\r\n", id2_end);
    if (dur_start == std::string::npos || line[dur_start] < '0' || line[dur_start] > '9') {
      return false;
    }
    const size_t dur_end = line.find_first_of(" \t\r\n", dur_start);
    const std::string dur_str =
        (dur_end == std::string::npos) ? line.substr(dur_start) : line.substr(dur_start, dur_end - dur_start);

    size_t pb = 0;
    out_meet.id1 = static_cast< size_t >(std::stoull(line.substr(f_space, id1_end - f_space)));
    out_meet.id2 = static_cast< size_t >(std::stoull(line.substr(id2_start, id2_end - id2_start)));
    out_meet.duration = static_cast< size_t >(std::stoull(dur_str, &pb));

    if (pb != dur_str.length()) {
      return false;
    }
    return true;
  }
}

int main(int argc, char *argv[])
{
  if (argc > 4) {
    return 0;
  }
  std::string in_file = "";
  std::string out_file = "";
  std::string data_file = "";
  bool has_in = false;
  bool has_out = false;
  bool has_data = false;

  for (int i = 1; i < argc; ++i) {
    const std::string arg = argv[i];
    if (arg.compare(0, 3, "in:") == 0) {
      if (has_in)
        return 1;
      in_file = arg.substr(3);
      has_in = true;
    } else if (arg.compare(0, 4, "out:") == 0) {
      if (has_out)
        return 1;
      out_file = arg.substr(4);
      has_out = true;
    } else if (arg.compare(0, 5, "data:") == 0) {
      if (has_data)
        return 1;
      data_file = arg.substr(5);
      has_data = true;
    } else {
      return 1;
    }
  }

  if (!has_data) {
    return 1;
  }

  std::ifstream f_data(data_file);
  if (!f_data.is_open()) {
    return 2;
  }

  alisov::Vector< alisov::Meet > meets;
  alisov::init(meets);
  std::string line = "";
  while (std::getline(f_data, line)) {
    alisov::Meet m;
    if (alisov::parse_meet(line, m)) {
      alisov::push_back(meets, m);
    } else if (!line.empty() && line.find_first_not_of(" \t\r\n") != std::string::npos) {
      return 3;
    }
  }
  f_data.close();
}
