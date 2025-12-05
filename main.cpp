#include <iostream>

namespace topit
{
  struct p_t
  {
    int x, y;
  };
  bool operator == (p_t, p_t);
  bool operator != (p_t, p_t);

  struct f_t
  {
    p_t aa, bb;
  };

  struct IDraw
  {
    // = 0 - Значит метод чисто виртуальный и ма не предоставим реализацию
    virtual ~IDraw() = default;

    virtual p_t begin() const = 0;
    virtual p_t next(p_t) const = 0;
  };

  struct Dot: IDraw
  {
    Dot(int x, int y);
    // explicit - не должен использовать в неявных преобразованиях
    explicit Dot(p_t dd);
    p_t begin() const override;
    p_t next(p_t) const override;

    p_t d;
  };

  // Должен расширять заданный массив точками из очередной фигуры
  size_t points(const IDraw & d, p_t ** pts, size_t & s);

  // Найти минимум и максимум по каждой координате и сформировать фрейм
  f_t frame(const p_t * pts, size_t s);

  // Построить полотно (из фрейма получить количество столбцов и колонок)
  char * canvas(f_t fr, const char * fill);

  // Координаты точки перевести в координаты в массиве
  void paint(char * cnv, f_t fr, p_t p, const char * fill);
  
  // Вывод двумерного массива на основе размеров определяемых фреймом
  void flush(std::ostream & os, const char * cnv, f_t fr);

}


int main()
{
  using namespace topit;
  /*
  using topit::p_t;
  using topit::f_t;
  using topit::Dot;
  using topit::IDraw;
  */

  size_t lenght_shps = 3;
  // ошибка в shps
  IDraw * shps[lenght_shps] = {};
  for (size_t i = 0; i < lenght_shps; i++)
  {
    shps[i] = nullptr;
  }
  int err = 0;
  p_t * pts = nullptr;
  size_t s = 0;
  try
  {
    shps[0] = new Dot(0, 0);
    shps[1] = new Dot(5, 2);
    shps[2] = new Dot(1, 6);

    for (size_t i = 0; i < lenght_shps; i++)
    {
      s += points(*(shps[i]), & pts, s);
    }

    f_t fr = frame(pts, s);

    char * cnv = canvas(fr, ".");
    for (size_t i = 0; i < s; i++)
    {
      paint(cnv, fr, pts[i], "#");
    }

    flush(std::cout, cnv, fr);
    



  } catch (...)
  {
    err = 2;
    std::cerr << "Bad drawing\n";
  }

  delete[] pts;

  for (size_t i = 0; i < lenght_shps; i++)
  {
    delete[] shps[i];
  }
  
  



/*
  using topit::p_t;
  p_t a{1, 0}, b{1, 0};

  topit::IDraw * p = new topit::Dot(10, 10);

  std::cout << (a != b) << '\n';
*/
  return err;
}

// ::Dot - имя класса второй ::Dot имя конструктора
topit::Dot::Dot(p_t dd):
  IDraw(),
  d{dd}
{}
topit::Dot::Dot(int x, int y):
  IDraw(),
  d{x, y}
{}

topit::p_t topit::Dot::begin() const
{
  return d;
}

topit::p_t topit::Dot::next(p_t prev) const
{
  if (prev != begin())
  {
    throw std::logic_error("bad impl");
  }
  return d;
}

bool topit::operator==(p_t a, p_t b)
{
  return a.x == b.x && a.y == b.y;
}
bool topit::operator!=(p_t a, p_t b)
{
  return !(a == b);
}


