#include <iostream>
#include <string>

#include "./sql.h"

using namespace std;

#define RESET "\033[0m"
#define RED "\033[31m"

int main() {  
  // input
  string query;

  cout << "SQL Query: ";
  getline(cin, query);

  // execute
  try {
    SQL::customQuery(query);
  } catch (char const* msg) {
    cout << RED << msg << RESET << endl;
  }
}
