#include <string>
#include <regex>
#include <vector>
#include <fstream>

#define RESET "\033[0m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"

class SQL {
  public:
    static void customQuery(std::string query) {
      validate(query);
    }

  private:
    // std::vector<std::string> student = { "name", "lastName", "age" };
    // std::vector<std::string> lecturer = { "name", "lastName", "dep" };

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

      std::fstream fileStream;

      fileStream.open("database/" + tableName);

      if (fileStream.fail()) {
        if (errno == 2) {
          throw "TABLE_DOES_NOT_EXISTS";
        }

        throw strerror(errno);
      } else {
        fileStream.close();

        if (std::regex_search(query, std::regex("select"))) {
          executeSelect(tableName, condition);
        } else if (std::regex_search(query, std::regex("delete"))) {
          executeDelete(tableName, condition);
        }
      }
    }

    static void executeSelect(std::string tableName, std::string condition) {
      std::fstream fileStream;

      fileStream.open("database/" + tableName);

      if (fileStream.fail()) {
        throw strerror(errno);
      } else {
        unsigned int c = 0;
        std::string str;
        while (std::getline(fileStream, str)) {
          if (condition == "*" || str.find(condition) != std::string::npos) {
            std::cout << GREEN << str << RESET << std::endl;
            c++;
          }
        }

        if (c == 0) {
          std::cout << YELLOW << "NO_RESULT" << RESET << std::endl;
        }

        fileStream.close();
      }
    }

    static void executeDelete(std::string tableName, std::string condition) {

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

      for(std::vector<std::string>::iterator it = splits.begin(); it != splits.end(); ++it) {
        trim(*it);
      }

      return splits;
    }

    static inline void ltrim(std::string &s) {
      s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        return !std::isspace(ch);
      }));
    }

    static inline void rtrim(std::string &s) {
      s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
      }).base(), s.end());
    }

    static inline void trim(std::string &s) {
      ltrim(s);
      rtrim(s);
    }
};
