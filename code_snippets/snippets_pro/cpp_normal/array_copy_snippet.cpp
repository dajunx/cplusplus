//数组拷贝

// 拷贝数组
int random_numb[max_tmp];
//   for (int i=0;i<max_tmp;++i)
//   {
//     random_numb[i] = random_numb_tmp[i];
//   }
memcpy(reinterpret_cast<char *>(random_numb),
       reinterpret_cast<char *>(random_numb_tmp), max_tmp);