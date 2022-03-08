#include <queue>
#include <string>
#include <fstream>
#include <map>
#include <typeindex>
#include <memory>
#include <iostream>
#include <stdexcept>
#include <json.hpp>

enum e_TaskType {
    ADD,
    SUB,
    MUL,
    DIV,
    TASK_MAX
};


typedef e_TaskType TaskType;

class TaskHandler {

public:
    TaskHandler(int x, int y) : x(x), y(y) {}

    //TaskType type;
    virtual int execute() = 0;
    //virtual ~TaskHandler() = 0;

protected:
    
    int x, y;

};

class TaskAdd : public TaskHandler {

public:
    TaskAdd(int x, int y) : TaskHandler(x,y) {};

    int execute()
    {
        return x + y;
    }
};

class TaskSub : public TaskHandler {

public:
    TaskSub(int x, int y) : TaskHandler(x,y) {};
    virtual int execute()
    {
        return x - y;
    }
};

class TaskMul : public TaskHandler {

public:
    TaskMul(int x, int y) : TaskHandler(x,y) {};
    virtual int execute()
    {
        return x * y;
    }
};

class TaskDiv : public TaskHandler {

public:
    TaskDiv(int x, int y) : TaskHandler(x,y) {};
    virtual int execute()
    {
        return x / y;
    }
};

std::unique_ptr<TaskHandler> taskHandlerCreate(std::string taskName, int x, int y)
{
    // TODO: While a map from string to type would work better,
    // I cannot currently figure out how to make that work.
    
    if (taskName == "ADD") return std::make_unique<TaskAdd>(x,y);
    if (taskName == "SUB") return std::make_unique<TaskSub>(x,y);
    if (taskName == "MUL") return std::make_unique<TaskMul>(x,y);
    if (taskName == "DIV") return std::make_unique<TaskDiv>(x,y);
    return std::unique_ptr<TaskHandler>(nullptr);
}

std::queue<std::unique_ptr<TaskHandler>> loadTasks(std::string filename)
{

    try
    {
        std::ifstream jsonFile(filename);
        auto json = nlohmann::json::parse(jsonFile);
        std::queue<std::unique_ptr<TaskHandler>> taskQueue;
        for(const auto& taskJson: json["tasks"])
        {
            std::string taskTypeStr = taskJson["type"];
            int taskX = taskJson["num1"];
            int taskY = taskJson["num2"];
            std::unique_ptr<TaskHandler> taskHandler = taskHandlerCreate(taskTypeStr, taskX, taskY);
            if (taskHandler == nullptr)
                throw std::invalid_argument(std::string("Invalid task definition in: ") + std::string(taskJson.dump()));
            taskQueue.push(std::move(taskHandler));
        }    
          
        return taskQueue;
    }
    catch(const std::exception& e)
    {
        std::cerr << "Error processing input JSON file:\n" << e.what() << '\n';
        exit(1);
    }
    
}

int main()
{
    auto taskQueue = loadTasks("tasks.json");
    
    while(!taskQueue.empty())
    {
        std::cout << taskQueue.front()->execute() << ", ";
        taskQueue.pop();
    }
    return 0;
}