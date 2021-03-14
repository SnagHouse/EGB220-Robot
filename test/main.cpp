#include "Framework.h"

int main(int argc, char **argv)
{
  if (Tests_Run())
    return 0;

  system("pause");
  return 1;
}
