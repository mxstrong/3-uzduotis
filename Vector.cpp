#include <iostream>
#include "Vector.h"

int main()
{
  Vector<int> vector;
  vector.push_back(1);
  std::cout << vector[0] << std::endl;
  system("pause");
  return 0;
}