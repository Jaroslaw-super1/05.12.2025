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

  size_t rows(f_t);
  size_t cols(f_t);


  struct IDraw
  {
    // = 0 - Значит метод чисто виртуальный и мы не предоставим реализацию
    virtual ~IDraw() = default;

    virtual p_t begin() const = 0;
    virtual p_t next(p_t) const = 0;
  };

  struct Dot : IDraw
  {
    Dot(int x, int y);
    // explicit - не должен использовать в неявных преобразованиях
    explicit Dot(p_t dd);
    p_t begin() const override;
    p_t next(p_t) const override;

    p_t d;
  };

  // ДЗ добавить 2-3 фигуры вертикальный отрезок и горизонтальный отрезок
  // диагональ под 45 заданной длинны

  // Горизонтальная линия
  struct HorizontalSegment : IDraw
  {
    HorizontalSegment(int y, int x1, int x2);
    explicit HorizontalSegment(p_t start, p_t end);
    
    p_t begin() const override;
    p_t next(p_t) const override;

    p_t start, end;
  };

  // Вертикальная линия
  struct VerticalSegment : IDraw
  {
    VerticalSegment(int x, int y1, int y2);
    explicit VerticalSegment(p_t start, p_t end);
    
    p_t begin() const override;
    p_t next(p_t) const override;

    p_t start, end;
  };

  void extend(p_t ** pts, size_t s, p_t p);

  // Должен расширять заданный массив точками из очередной фигуры
  size_t points(const IDraw & d, p_t ** pts, size_t & s);

  // Найти минимум и максимум по каждой координате и сформировать фрейм
  f_t frame(const p_t * pts, size_t s);

  // Построить полотно (из фрейма получить количество столбцов и колонок)
  char * canvas(f_t fr, const char fill);

  // Координаты точки перевести в координаты в массиве
  void paint(char * cnv, f_t fr, p_t p, const char fill);

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

  size_t lenght_shps = 5;
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
    shps[1] = new Dot(40, 20);

    shps[2] = new Dot(11, 0);
    shps[3] = new HorizontalSegment(5, 3, 10);
    shps[4] = new VerticalSegment(11, 6, 13);

    for (size_t i = 0; i < lenght_shps; i++)
    {
      s += points(*(shps[i]), & pts, s);
    }

    f_t fr = frame(pts, s);

    char * cnv = canvas(fr, '.');
    for (size_t i = 0; i < s; i++)
    {
      paint(cnv, fr, pts[i], '#');
    }

    flush(std::cout, cnv, fr);
    delete[] cnv;
  } catch (...)
  {
    err = 2;
    std::cerr << "Bad drawing\n";
  }

  delete[] pts;

  for (size_t i = 0; i < lenght_shps; i++)
  {
    delete shps[i];
  }
  
  



/*
  using topit::p_t;
  p_t a{1, 0}, b{1, 0};

  topit::IDraw * p = new topit::Dot(10, 10);

  std::cout << (a != b) << '\n';
*/
  return err;
}


// Координаты точки перевести в координаты в массиве
void topit::paint(char * cnv, f_t fr, p_t p, const char fill)
{
  size_t dy = fr.bb.y - p.y;
  size_t dx = p.x - fr.aa.x;
  cnv[dy * cols(fr) + dx] = fill;
}

// Вывод двумерного массива на основе размеров определяемых фреймом
void topit::flush(std::ostream & os, const char * cnv, f_t fr)
{
  for (size_t i = 0; i < rows(fr); i++)
  {
    for (size_t j = 0; j < cols(fr); j++)
    {
      os << cnv[i * cols(fr) + j];
    }
    os << '\n';
  }
}

size_t topit::rows(f_t fr)
{
  return fr.bb.y - fr.aa.y + 1;
}
size_t topit::cols(f_t fr)
{
  return fr.bb.x - fr.aa.x + 1;
}

// Построить полотно (из фрейма получить количество столбцов и колонок)
char * topit::canvas(f_t fr, const char fill)
{
  char * cnv = new char[rows(fr) * cols(fr)];
  for (size_t i = 0; i < rows(fr) * cols(fr); i++)
  {
    cnv[i] = fill;
  }
  return cnv;
}

topit::f_t topit::frame(const p_t * pts, size_t s)
{
  if (!s)
  {
    throw std::logic_error("Nu tochi nada");
  }
  int minx = pts[0].x, maxx = minx;
  int miny = pts[0].y, maxy = miny;
  for (size_t i = 0; i < s; i++)
  {
    minx = std::min(minx, pts[i].x);
    maxx = std::max(maxx, pts[i].x);
    miny = std::min(miny, pts[i].y);
    maxy = std::max(maxy, pts[i].y);
  }
  p_t aa = {minx, miny};
  p_t bb = {maxx, maxy};
  return {aa, bb};
  
}

void topit::extend(p_t ** pts, size_t s, p_t p)
{
  p_t * e = new p_t[s + 1];
  for (size_t i = 0; i < s; i++)
  {
    e[i] = (*pts)[i];
  }
  e[s] = p;
  delete [] *pts;
  *pts = e;
}

size_t topit::points(const IDraw & d, p_t ** pts, size_t & s)
{
  size_t r = 1;
  p_t p = d.begin();
  extend(pts, s, p);
  while (d.next(p) != d.begin())
  {
    p = d.next(p);
    extend(pts, s + r, p);
    ++r;
  }
  return r;
}

bool topit::operator==(p_t a, p_t b)
{
  return a.x == b.x && a.y == b.y;
}
bool topit::operator!=(p_t a, p_t b)
{
  return !(a == b);
}

// Точка ::Dot - имя класса второй ::Dot имя конструктора
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

// Линия
topit::HorizontalSegment::HorizontalSegment(p_t a, p_t b):
  IDraw(),
  start{a},
  end{b}
{}
topit::HorizontalSegment::HorizontalSegment(int y, int x1, int x2):
  IDraw(),
  start{x1, y},
  end{x2, y}
{}
topit::p_t topit::HorizontalSegment::begin() const
{
  return start;
}
topit::p_t topit::HorizontalSegment::next(p_t prev) const
{
  if (prev == end)
  {
    return begin();
  }
  return p_t{prev.x + 1, prev.y};
  
  /*else
  {
    throw std::logic_error("bad impl");
  }*/
}

// Вертикальная линия
topit::VerticalSegment::VerticalSegment(p_t a, p_t b):
  IDraw(),
  start{a},
  end{b}
{}
topit::VerticalSegment::VerticalSegment(int x, int y1, int y2):
  IDraw(),
  start{x, y1},
  end{x, y2}
{}
topit::p_t topit::VerticalSegment::begin() const
{
  return start;
}
topit::p_t topit::VerticalSegment::next(p_t prev) const
{
  if (prev == end)
  {
    return begin();
  }
  return p_t{prev.x, prev.y + 1};
}
