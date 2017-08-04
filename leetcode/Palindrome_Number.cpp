#include <iostream>

bool isPalindrome1(int x) 
{
  int x_copy = x;
  int reversion_num = 0;
  while ( x_copy > 0)
  {
    reversion_num = reversion_num * 10 + x_copy%10;
    x_copy = x_copy/10;
  };

  if ( x == reversion_num) {
    return true;
  }

  return false;
}

bool isPalindrome2(int x) 
{
  int x_copy = x;
  int x_size = 0;
  int x_array[10] = {-1};
  int array_pos = 0;

  while(x_copy > 0)
  {
    x_array[array_pos++] = x_copy%10;
    x_size ++;
    x_copy /= 10;
  }

  int middle_pos_left = x_size/2 - 1;
  int middle_pos_right = x_size/2 + x_size%2;

  while(middle_pos_left >=0 && middle_pos_right < x_size -1) {
    if (x_array[middle_pos_left--] == x_array[middle_pos_right++]) {
      continue;
    } else {
      break;
    }
  }

  return false;
}
  

int main()
{
  int i = -2147483648;
  bool isPalindrome_num = false;
  isPalindrome_num = isPalindrome2(i);

  return 0;
}