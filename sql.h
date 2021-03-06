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
    static void validate(std::string query) {
      std::string objectRegex = "\\s+?[^\\s]+?\\s+?";
      std::string conditionRegex = "(\\s([a-zA-z0-9_]+)\\s+([!=]+)\\s(.+)|\\s+[\\*])";

      std::string selectQueryRegex = "(SELECT" + objectRegex + "WHERE" + conditionRegex + ")";
      std::string deleteQueryRegex = "(DELETE" + objectRegex + "WHERE" + conditionRegex + ")";
      std::string addQueryRegex = "(ADD\\s+?[^\\s]+)";
      std::string validQueryRegex = selectQueryRegex + "|" + deleteQueryRegex + "|" + addQueryRegex;

      std::regex queryRegex(validQueryRegex, std::regex_constants::ECMAScript | std::regex_constants::icase);

      if (!std::regex_search(query, queryRegex)) {
        throw "SYNTAX_ERROR";
      }

      execute(query);
    }

    static void execute(std::string query) {
      std::vector<std::string> data = split(query, std::regex("select|delete|add|where", std::regex_constants::icase));

      std::string tableName = data[1];
      std::string condition = data.size() >= 3 ? data[2] : "";

      std::fstream fileStream;

      fileStream.open("database/" + tableName);

      if (fileStream.fail()) {
        if (errno == 2) {
          throw "TABLE_DOES_NOT_EXISTS";
        }

        throw strerror(errno);
      } else {
        fileStream.close();

        if (std::regex_search(query, std::regex("select", std::regex_constants::icase))) {
          executeSelect(tableName, condition);
        } else if (std::regex_search(query, std::regex("delete", std::regex_constants::icase))) {
          executeDelete(tableName, condition);
        } else if (std::regex_search(query, std::regex("add", std::regex_constants::icase))) {
          executeAdd(tableName);
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
          if (condition == "*" || matchCondition(str, condition)) {
            std::cout << GREEN << str << RESET << std::endl;
            c++;
          }
        }

        std::cout << YELLOW << c << " RESULT" << RESET << std::endl;

        fileStream.close();
      }
    }

    static void executeDelete(std::string tableName, std::string condition) {
      std::string path = "database/" + tableName;

      std::fstream fileStream;
      std::ofstream temp;

      fileStream.open(path);
      temp.open("database/temp");

      if (fileStream.fail()) {
        throw strerror(errno);
      } else {
        unsigned int c = 0;
        std::string str;
        while (std::getline(fileStream, str)) {
          if (condition == "*" || matchCondition(str, condition)) {
            c++;
            continue;
          }

          temp << str << std::endl;
        }

        fileStream.close();
        temp.close();

        const char * p = path.c_str();
        remove(p);
        rename("database/temp", p);

        std::cout << YELLOW << c << " RESULT" << RESET << std::endl;
      }
    }

    static void executeAdd(std::string tableName) {
      std::fstream tables;

      tables.open("database/tables");

      if (tables.fail()) {
        throw strerror(errno);
      } else {
        std::string line;
        while (std::getline(tables, line)) {
          if (line.find(tableName) != std::string::npos) {
            std::ofstream table;
            table.open("database/" + tableName, std::fstream::app);

            if (table.fail()) {
              throw strerror(errno);
            } else {
              replace(line, tableName + ":", "");
              std::string newLine = "";
              std::string fieldValue;
              std::vector<std::string> data = split(line, std::regex(","));
              for(std::vector<std::string>::iterator it = data.begin(); it != data.end(); ++it) {
                std::cout << *it << ": ";
                std::cin >> fieldValue;

                newLine += *it + " = " + fieldValue + " ";
              }

              table << newLine << std::endl;
              table.close();

              std::cout << YELLOW << "1 RESULT" << RESET << std::endl;
            }
          }
        }

        tables.close();
      }
    }

    static bool matchCondition(std::string line, std::string condition) {
      if (condition.find("!=") != std::string::npos) {
        replace(condition, "!=", "=");
        return line.find(condition) == std::string::npos;
      } else if (condition.find("=") != std::string::npos) {
        return line.find(condition) != std::string::npos;
      } else {
        return false;
      }
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

    static void replace(std::string& str, const std::string& from, const std::string& to) {
      size_t start_pos = str.find(from);
      if(start_pos != std::string::npos)
        str.replace(start_pos, from.length(), to);
    }
};
