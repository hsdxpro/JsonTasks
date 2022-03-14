#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "../externals/json11.hpp"

using json11::Json;
using namespace std;

class Task
{
 public:
  virtual string Solve() = 0;
};

class TaskAdd : public Task
{
 public:
  TaskAdd(Json json) : a(json["num1"].int_value()), b(json["num2"].int_value()) {}

  string Solve() override { return to_string(a + b); }

 private:
  int a;
  int b;
};

class TaskSub : public Task
{
 public:
  TaskSub(Json json) : a(json["num1"].int_value()), b(json["num2"].int_value()) {}

  string Solve() override { return to_string(a - b); }

 private:
  int a;
  int b;
};

class TaskDiv : public Task
{
 public:
  TaskDiv(Json json) : a(json["num1"].int_value()), b(json["num2"].int_value()) {}

  string Solve() override
  {
    if (b == 0)
    {
      cout << "[TaskDiv] division by zero not allowed, task returning zero as a result";
      return 0;
    }

    return to_string(a / b);
  }

 private:
  int a;
  int b;
};

class TaskMul : public Task
{
 public:
  TaskMul(Json json) : a(json["num1"].int_value()), b(json["num2"].int_value()) {}

  string Solve() override { return to_string(a * b); }

 private:
  int a;
  int b;
};

int main()
{
  ifstream file_stream("tasks.json");
  if (!file_stream.is_open())
  {
    cout << "Missing tasks.json file" << endl;
    return 1;
  }

  stringstream string_stream;
  string_stream << file_stream.rdbuf();

  string input = string_stream.str();

  string err;
  Json json = Json::parse(string_stream.str(), err);

  if (!err.empty())
  {
    cout << "Error parsing tasks.json file: " << err << endl;
    return 1;
  }

  Json::array taskArray = json["tasks"].array_items();

  vector<unique_ptr<Task>> tasks;
  for (Json &taskJson : taskArray)
  {
    string type = taskJson["type"].string_value();
    if (type == "ADD")
      tasks.push_back(make_unique<TaskAdd>(taskJson));
    else if (type == "SUB")
      tasks.push_back(make_unique<TaskSub>(taskJson));
    else if (type == "DIV")
      tasks.push_back(make_unique<TaskDiv>(taskJson));
    else if (type == "MUL")
      tasks.push_back(make_unique<TaskMul>(taskJson));
    // else if (type == "CUSTOM") // Example of handling new task
    //  tasks.push_back(make_unique<TaskCustom>(taskJson));
    else
    {
      cout << "Invalid task type, check tasks.json for errors: " << type << endl;
    }
  }

  string result;
  for (auto &task : tasks)
    result += task->Solve() + ',';

  if (result.size() > 0)
    result.pop_back();

  cout << result << endl;
  return 0;
}