#include <iostream>
#include <vector>
#include <map>

int maxArea(std::vector<int>& height) {
  if (height.size() < 2) {
    return 0;
  }

  std::map<int, int> map_arpe, map_height;
  std::map<int, int>::iterator it_map;
  std::vector<int>::iterator it = height.begin();
  int height_size = height.size();
  int res_array[3] = {0};

  for (int i = 0; it != height.end(); ++it, ++i)
  {
    map_arpe.insert(std::pair<int, int>(*it, i));
    map_height.insert(std::pair<int, int>(i, *it));
  }

  for (it_map = map_arpe.begin(); it_map != map_arpe.end(); ++it_map)
  {
    int left_length = it_map->second - (map_height.begin())->first;
    int right_length = (map_height.rbegin())->first - it_map->second;
    int area_tmp = 0;
    left_length > right_length
      ? (area_tmp = left_length * it_map->first)
      : (area_tmp = right_length * it_map->first);

    if (area_tmp > res_array[2]) {
      res_array[0] = it_map->second;
      res_array[1] = left_length > right_length 
        ? (map_height.begin())->first
        : (map_height.rbegin())->first;
      res_array[2] = area_tmp;
    }
    left_length > right_length
      ? map_height.erase((map_height.begin())->first)
      : map_height.erase((map_height.rbegin())->first);
  }

  return res_array[2];
}

int main()
{
  int a[] = {3, 7, 5, 4};
  std::vector<int> height(a, a + sizeof(a)/sizeof(int));
  maxArea(height);

  int i = 0;
  return 0;
}