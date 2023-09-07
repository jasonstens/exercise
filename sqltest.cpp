#include <iostream>
#include "/root/sql_test/lib/sqlite3pp/src/sqlite3pp.h"
extern "C" {
    #include <cjson/cJSON.h>
}

bool getNpmInfoFromJsonFileByCJson(const std::string& json_file_path)
{
    int n_result = 0;
    bool ret = false;
    std::string str_json;
    cJSON* json_root = NULL;

    do {
        n_result = FileToString(json_file_path, str_json, RULES_FILE_MAX_SIZE);
        if (n_result != BASE_API_CODE_SUCCESS) {
            break;
        }

        json_root = cJSON_Parse(str_json.c_str());
        if (!json_root) {
            LOG_E("pase jsonstring err.");
            break;
        }

        cJSON* json_name = cJSON_GetObjectItem(json_root, NPM_PACKAGE_PARSE_JSON_NAME);
        if (!json_name) {
            LOG_E("chk fmt err, jsonRoot.isMember(\"name\")");
            break;
        }

        cJSON* json_version = cJSON_GetObjectItem(json_root, NPM_PACKAGE_PARSE_JSON_VERSION);
        if (!json_version) {
            LOG_E("chk fmt err, jsonRoot.isMember(\"version\")");
            break;
        }

        cJSON* json_scripts = cJSON_GetObjectItem(json_root, NPM_PACKAGE_PARSE_JSON_SCRIPTS);
        if (!json_scripts) {
        } else {
            char *tmp = cJSON_Print(json_scripts);  // 分配内存
            if (tmp) {  // 检查是否分配成功
                str_json = tmp;
                free(tmp);  // 释放内存
            }
        }
        ret = true;

    } while (0);

    if (json_root) {
        cJSON_Delete(json_root);
        json_root = NULL;
    }

    return ret;
}

int main() {
  // try {
  //   // 创建或打开数据库
  //   sqlite3pp::database* db = new sqlite3pp::database("test.db");

  //   // 创建表
  //   db->execute("DROP TABLE IF EXISTS users;");
  //   db->execute("CREATE TABLE users (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT, age INTEGER);");

  //   {
  //     // 插入数据
  //     sqlite3pp::command cmd(*db, "INSERT INTO users (name, age) VALUES (?, ?)");

  //     cmd.bind(1, "Alice", sqlite3pp::nocopy);
  //     cmd.bind(2, 30);
  //     cmd.execute();
      
  //     // 第二次插入
  //     sqlite3pp::command cmd2(*db, "INSERT INTO users (name, age) VALUES (?, ?)");
  //     cmd2.bind(1, "Bob", sqlite3pp::nocopy);
  //     cmd2.bind(2, 40);
  //     cmd2.execute();
  //   }

  //   // 查询数据
  //   sqlite3pp::query qry(*db, "SELECT id, name, age FROM users");
    
  //   for (sqlite3pp::query::iterator i = qry.begin(); i != qry.end(); ++i) {
  //     int id;
  //     char const* name;
  //     int age;
  //     std::tie(id, name, age) = (*i).get_columns<int, char const*, int>(0, 1, 2);
  //     std::cout << "ID: " << id << " Name: " << name << " Age: " << age << std::endl;
  //   }

  //   // 释放数据库资源
  //   delete db;

  // } catch (std::exception& ex) {
  //   std::cerr << ex.what() << std::endl;
  //   return -1;
  // }
  getNpmInfoFromJsonFileByCJson("/root/sql_test/package.json");
  return 0;
}