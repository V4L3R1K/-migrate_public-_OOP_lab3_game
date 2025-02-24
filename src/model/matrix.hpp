#ifndef MATRIX_HPP
#define MATRIX_HPP
// #include <vector>
#include <initializer_list>
#include <iterator>
#include <stdexcept>

/// @brief Пространство имен лабораторной работы
namespace oopLab3 {

/// @brief Строка матрицы
/// @tparam T Тип элемента, хранимого в строке
template <typename T> class MatrixRow {
  /// @brief Данные, хранимые в строке матрицы
  T *data;
  /// @brief Вместимость (выделенная память) под элементы в штуках
  size_t capacity_;
  /// @brief Занятое пространство под элементы в штуках
  size_t size_;

  /// @brief Увеличить вместимость вдвое
  void resize() {
    capacity_ = (capacity_ == 0) ? 1 : capacity_ * 2;
    T *newData = new T[capacity_];
    for (size_t i = 0; i < size_; ++i)
      newData[i] = data[i];
    delete[] data;
    data = newData;
  }

public:
  /// @brief Итератор по строке матрицы
  class iterator {
    T *ptr;

  public:
    /// @brief Категория итератора из стандартных
    using iterator_category = std::random_access_iterator_tag;
    /// @brief Тип данных в строке матрицы
    using value_type = T;
    /// @brief Тип данных различия между двумя элементами строки матрицы
    using difference_type = std::ptrdiff_t;
    /// @brief Тип данных указателя на элемент строки матрицы
    using pointer = T *;
    /// @brief Тип данных ссылки на элемент строки матрицы
    using reference = T &;

    /// @brief Конструктор итератора
    /// @param p Указатель на элемент
    iterator(T *p) : ptr(p) {}

    /// @brief Оператор разыменования
    /// @return Ссылка на элемент
    reference operator*() const { return *ptr; }
    /// @brief Оператор обращения
    /// @return Указатель на элемент
    pointer operator->() const { return ptr; }
    /// @brief Оператор инкремента (префиксный)
    /// @return Итератор на 1 позицию вперед
    iterator &operator++() {
      ++ptr;
      return *this;
    }
    /// @brief Оператор инкремента (постфиксный)
    /// @return Итератор на текущую позицию
    iterator operator++(int) {
      iterator temp = *this;
      ++ptr;
      return temp;
    }
    /// @brief Оператор декремента (префиксный)
    /// @return Итератор на 1 позицию назад
    iterator &operator--() {
      --ptr;
      return *this;
    }
    /// @brief Оператор декремента (постфиксный)
    /// @return Итератор на текущую позицию
    iterator operator--(int) {
      iterator temp = *this;
      --ptr;
      return temp;
    }
    /// @brief Оператор сложения
    /// @param n сдвиг
    /// @return Итератор на n позиций вперед
    iterator operator+(difference_type n) const { return iterator(ptr + n); }
    /// @brief Оператор вычетания
    /// @param n сдвиг
    /// @return Итератор на n позиций назад
    iterator operator-(difference_type n) const { return iterator(ptr - n); }
    /// @brief Оператор вычетания
    /// @param other итератор сдвига
    /// @return Сдвиг назад
    difference_type operator-(const iterator &other) const {
      return ptr - other.ptr;
    }
    /// @brief Оператор сравнения (равенство)
    /// @param other С чем сравниваем
    /// @return true - если итераторы равны, false - иначе
    bool operator==(const iterator &other) const { return ptr == other.ptr; }
    /// @brief Оператор сравнения (неравенство)
    /// @param other С чем сравниваем
    /// @return true - если итераторы не равны, false - иначе
    bool operator!=(const iterator &other) const { return ptr != other.ptr; }
    /// @brief Оператор сравнения (меньше)
    /// @param other С чем сравниваем
    /// @return true - если левый итератор меньше правого, false - иначе
    bool operator<(const iterator &other) const { return ptr < other.ptr; }
    /// @brief Оператор сравнения (больше)
    /// @param other С чем сравниваем
    /// @return true - если левый итератор больше правого, false - иначе
    bool operator>(const iterator &other) const { return ptr > other.ptr; }
    /// @brief Оператор сравнения (меньше или равно)
    /// @param other С чем сравниваем
    /// @return true - если левый итератор меньше или равен правого, false -
    /// иначе
    bool operator<=(const iterator &other) const { return ptr <= other.ptr; }
    /// @brief Оператор сравнения (больше или равно)
    /// @param other С чем сравниваем
    /// @return true - если левый итератор больше или равен правого, false -
    /// иначе
    bool operator>=(const iterator &other) const { return ptr >= other.ptr; }
    /// @brief Оператор сложения с присваиванием
    /// @param n Сдвиг
    /// @return Итератор со сдвигом вперед
    iterator &operator+=(difference_type n) {
      ptr += n;
      return *this;
    }
    /// @brief Оператор вычетания с присваиванием
    /// @param n Сдвиг
    /// @return Итератор со сдвигом назад
    iterator &operator-=(difference_type n) {
      ptr -= n;
      return *this;
    }
    /// @brief Оператор образения по индексу
    /// @param n индекс
    /// @return Ссылка на элемент по индексу
    reference operator[](difference_type n) const { return *(ptr + n); }
  };

  /// @brief Пустой Конструктор
  MatrixRow() : data(nullptr), capacity_(0), size_(0) {}

  /// @brief Конструктор через initializer list
  /// @param init Initializer list (то что в фигурных скобочках после =)
  MatrixRow(std::initializer_list<T> init)
      : data(nullptr), capacity_(init.size()), size_(init.size()) {
    data = new T[capacity_];
    size_t i = 0;
    for (const auto &elem : init) {
      data[i++] = elem;
    }
  }

  /// @brief Конструктор копирования
  /// @param other Откуда копируем
  MatrixRow(const MatrixRow &other)
      : data(nullptr), capacity_(0), size_(other.size_) {
    if (size_ > 0) {
      capacity_ = size_;
      data = new T[capacity_];
      for (size_t i = 0; i < size_; ++i) {
        data[i] = other.data[i];
      }
    }
  }

  /// @brief Конструктор перемещения
  /// @param other Откуда перемещаем
  MatrixRow(MatrixRow &&other) noexcept
      : data(other.data), capacity_(other.capacity_), size_(other.size_) {
    other.data = nullptr;
    other.capacity_ = 0;
    other.size_ = 0;
  }

  /// @brief Деструктор
  ~MatrixRow() { delete[] data; }

  /// @brief Копирующее присваивание
  /// @param other Откуда копируем
  /// @return Ссылка на скопированную строку матрицы
  MatrixRow &operator=(const MatrixRow &other) {
    if (this != &other) {
      delete[] data;
      size_ = other.size_;
      capacity_ = other.capacity_;
      data = new T[capacity_];
      for (size_t i = 0; i < size_; ++i) {
        data[i] = other.data[i];
      }
    }
    return *this;
  }

  /// @brief Перемещающее присваивание
  /// @param other Откуда перемещаем
  /// @return Ссылка на скопированную строку матрицы
  MatrixRow &operator=(MatrixRow &&other) noexcept {
    if (this != &other) {
      delete[] data;
      data = other.data;
      capacity_ = other.capacity_;
      size_ = other.size_;
      other.data = nullptr;
      other.capacity_ = 0;
      other.size_ = 0;
    }
    return *this;
  }

  /// @brief Вставка в конец
  /// @param value Ссылка на вставляемый элемент
  void push_back(const T &value) {
    if (size_ == capacity_)
      resize();

    data[size_++] = value;
  }

  /// @brief Удаление с конца
  void pop_back() {
    if (size_ == 0)
      throw std::out_of_range("Empty");
    --size_;
  }

  /// @brief Удаление по индексу
  /// @param index Индекс, по которому удаляем
  void erase(iterator index) {
    if (index < begin() || index >= end())
      throw std::out_of_range("Index out of range");

    for (iterator i = index; i < end() - 1; ++i) {
      *i = *(i + 1);
    }
    --size_;
  }

  /// @brief Оператор индексирования (неконстантный)
  /// @param index Индекс
  /// @return Ссылка на элемент по индексу
  T &operator[](size_t index) {
    if (index >= size_)
      throw std::out_of_range("Index out of range");

    return data[index];
  }

  /// @brief Оператор индексирования (константный)
  /// @param index Индекс
  /// @return Ссылка на элемент по индексу
  const T &operator[](size_t index) const {
    if (index >= size_)
      throw std::out_of_range("Index out of range");

    return data[index];
  }

  /// @brief Индексирование (неконстантное)
  /// @param index Индекс
  /// @return Ссылка на элемент по индексу
  T &at(size_t index) {
    if (index >= size_)
      throw std::out_of_range("Index out of range");
    return data[index];
  }

  /// @brief Индексирование (константное)
  /// @param index Индекс
  /// @return Ссылка на элемент по индексу
  const T &at(size_t index) const {
    if (index >= size_)
      throw std::out_of_range("Index out of range");
    return data[index];
  }

  /// @brief Получить размер строки матрицы
  /// @return Размер в штуках
  size_t size() const { return size_; }

  /// @brief Получить итератор начала (неконстантный)
  /// @return Итератор начала
  iterator begin() { return iterator(data); }
  /// @brief Получить итератор конца (неконстантный)
  /// @return Итератор конца
  iterator end() { return iterator(data + size_); }
  /// @brief Получить итератор начала (константный)
  /// @return Итератор начала
  iterator begin() const { return iterator(data); }
  /// @brief Получить итератор начала (константный)
  /// @return Итератор начала
  iterator end() const { return iterator(data + size_); }
};

/// @brief Матрица
/// @tparam T Тип элементов в матрице
template <typename T> using Matrix = MatrixRow<MatrixRow<T>>;

// временная затычка вместо нормальной матрицы
// template <typename T> using MatrixRow = std::vector<T>;
// template <typename T> using Matrix = MatrixRow<MatrixRow<T>>;

} // namespace oopLab3
#endif // MATRIX_HPP