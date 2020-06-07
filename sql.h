#include <string>
#include <regex>

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
    }

    static void execute() {

    }
};
