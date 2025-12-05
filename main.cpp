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
    p_t begin() const override;
    p_t next(p_t) const override;
  };
  
}


int main()
{

  using topit::p_t;
  p_t a{1, 0}, b{1, 0};

  std::cout << (a != b) << '\n';

  return 0;
}

bool topit::operator==(p_t a, p_t b)
{
  return a.x == b.x && a.y == b.y;
}
bool topit::operator!=(p_t a, p_t b)
{
  return !(a == b);
}


