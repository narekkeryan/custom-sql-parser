#include <string>
#include <regex>
#include <vector>

class SQL {
  public:
    static void customQuery(std::string query) {
      validate(query);
    }

  private:
    static void validate(std::string query) {
      std::string objectRegex = "\\s+?[^\\s]+?\\s+?";
      std::string conditionRegex = "(\\s([a-zA-z0-9_]+)\\s+([!=]+)\\s(.+)|\\s+[\\*])";

      std::string selectQueryRegex = "(SELECT" + objectRegex + "WHERE" + conditionRegex + ")";
      std::string deleteQueryRegex = "(DELETE" + objectRegex + "WHERE" + conditionRegex + ")";
      std::string validQueryRegex = selectQueryRegex + "|" + deleteQueryRegex;

      std::regex queryRegex(validQueryRegex, std::regex_constants::ECMAScript | std::regex_constants::icase);

      if (!std::regex_search(query, queryRegex)) {
        throw "SYNTAX_ERROR";
      }

      execute(query);
    }

    static void execute(std::string query) {
      std::vector<std::string> data = split(query, std::regex("select|delete|where"));

      std::string tableName = data[1];
      std::string condition = data[2];

      std::cout << "tableName" << tableName << std::endl;
      std::cout << "condition" << condition << std::endl;
    }

    static std::vector<std::string> split(std::string s, std::regex r) {
      std::vector<std::string> splits;
      std::smatch m;

      while (regex_search(s, m, r)) {
        int split_on = m.position();
        splits.push_back(s.substr(0, split_on));
        s = s.substr(split_on + m.length());
      }

      if(!s.empty()) {
        splits.push_back(s);
      }

      return splits;
    }
};
