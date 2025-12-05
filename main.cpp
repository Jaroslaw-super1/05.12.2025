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

  
}


int main()
{

  using topit::p_t;
  p_t a{1, 0}, b{1, 0};

  topit::IDraw * p = new topit::Dot(10, 10);

  std::cout << (a != b) << '\n';

  return 0;
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


