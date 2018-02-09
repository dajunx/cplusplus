// boost multi_index 测试

#include <algorithm>
#include <boost/foreach.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index_container.hpp>
#include <iostream>
#include <iterator>
#include <string>

using boost::multi_index_container;
using namespace boost::multi_index;

/* an employee record holds its ID, name and age */

struct employee {
  int id;
  std::string name;
  int age;

  employee(int id_, std::string name_, int age_)
      : id(id_), name(name_), age(age_) {}
};

/* tags for accessing the corresponding indices of employee_set */

struct id {};
struct name {};
struct age {};

/* see Compiler specifics: Use of member_offset for info on
 * BOOST_MULTI_INDEX_MEMBER
 */

/* Define a multi_index_container of employees with following indices:
 *   - a unique index sorted by employee::int,
 *   - a non-unique index sorted by employee::name,
 *   - a non-unique index sorted by employee::age.
 */

typedef multi_index_container<
    employee,
    indexed_by<
        ordered_unique<tag<id>, BOOST_MULTI_INDEX_MEMBER(employee, int, id)>,
        ordered_non_unique<tag<name>, BOOST_MULTI_INDEX_MEMBER(
                                          employee, std::string, name)>,
        ordered_non_unique<tag<age>,
                           BOOST_MULTI_INDEX_MEMBER(employee, int, age)>>>
    employee_set;

int main() {
  employee_set es;

  es.insert(employee(0, "Joe", 31));
  es.insert(employee(1, "Robert", 27));
  es.insert(employee(2, "John", 40));

  /* next insertion will fail, as there is an employee with
   * the same ID
   */

  es.insert(employee(2, "Aristotle", 2387));

  es.insert(employee(3, "Albert", 20));
  es.insert(employee(4, "John", 40));

  /* list the employees sorted by ID, name and age */

  std::cout << "by ID" << std::endl;
  typedef boost::multi_index::index<employee_set, age>::type age_map0;
  age_map0 &i = get<age>(es);
  // age_map& i= es.get<age>();
  std::pair<age_map0::iterator, age_map0::iterator> it_range =
      i.equal_range(40);
  if (it_range.first != i.end()) {
    age_map0::iterator it_child = it_range.first;
    for (; it_child != it_range.second; it_child++) {
      it_child->age;
      std::cout << "id:" << it_child->id << ", name:" << it_child->name
                << ", age:" << it_child->age << std::endl;
    }
  }
  // print_out_by<id>(es);
  std::cout << std::endl;

  //按照 id索引，对容易进行遍历
  typedef boost::multi_index::index<employee_set, id>::type id_map;
  id_map &id_ = get<id>(es);
  id_map::iterator id_map_begin = id_.begin();
  for (; id_map_begin != id_.end(); id_map_begin++) {
    std::cout << "id:" << id_map_begin->id << ", name:" << id_map_begin->name
              << ", age:" << id_map_begin->age << std::endl;
  }
  std::cout << std::endl;

  //按照 age索引，对容易进行遍历
  typedef boost::multi_index::index<employee_set, age>::type age_map1;
  age_map1 &age_ = get<age>(es);
  age_map1::iterator age_map_begin = age_.begin();
  for (; age_map_begin != age_.end(); age_map_begin++) {
    std::cout << "id:" << age_map_begin->id << ", name:" << age_map_begin->name
              << ", age:" << age_map_begin->age << std::endl;
  }
  std::cout << std::endl;

  BOOST_FOREACH(employee const &ee, es) {
    std::cout << "id:" << ee.id << ", name:" << ee.name << ", age:" << ee.age
              << std::endl;
  }

  return 0;
}