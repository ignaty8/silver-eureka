#include <queue>
#include <string>
#include <fstream>
#include <memory>
#include <iostream>
#include <stdexcept>
#include <json.hpp>

/*!
    \class TaskHandler
    \brief Abstract class for processing all tasks.
*/
class TaskHandler {

public:
    TaskHandler(int x, int y, std::string name) : x(x), y(y), name(name) {}

    virtual int execute() = 0; // Performs task function
    
    //! \brief Overload << for pretty-printing
    friend std::ostream& operator<<(std::ostream& os, const TaskHandler& taskHandler)
    {
        os << taskHandler.name << ": " << taskHandler.x << ", " << taskHandler.y;
        return os;
    }

protected:
    
    int x, y;
    const std::string name;

};

/*!
    \class TaskAdd
    \brief Adds 2 numbers together
*/
class TaskAdd : public TaskHandler {

public:
    TaskAdd(int x, int y, std::string name) : TaskHandler(x,y,name) {};
    virtual int execute()
    {
        return x + y;
    }
};

/*!
    \class TaskSub
    \brief Substracts one number from another
*/
class TaskSub : public TaskHandler {

public:
    TaskSub(int x, int y, std::string name) : TaskHandler(x,y,name) {};
    virtual int execute()
    {
        return x - y;
    }
};

/*!
    \class TaskMul
    \brief Multiplies 2 numbers together
*/
class TaskMul : public TaskHandler {

public:
    TaskMul(int x, int y, std::string name) : TaskHandler(x,y,name) {};
    virtual int execute()
    {
        return x * y;
    }
};

/*!
    \class TaskDiv
    \brief Divides one number by another. Throws exception if threatened.
*/
class TaskDiv : public TaskHandler {

public:
    TaskDiv(int x, int y, std::string name) : TaskHandler(x,y,name) {};
    virtual int execute()
    {
        if (!y)
            throw std::invalid_argument("Division by zero!");
        return x / y;
    }
};

//! \brief Creates TaskHandler instance based on task name. Returns a nullptr if task type unkown.
std::unique_ptr<TaskHandler> taskHandlerCreate(std::string taskName, int x, int y)
{
    // TODO: While a map from string to type would work better,
    // I cannot currently figure out how to make that work.
    
    if (taskName == "ADD") return std::make_unique<TaskAdd>(x,y,taskName);
    if (taskName == "SUB") return std::make_unique<TaskSub>(x,y,taskName);
    if (taskName == "MUL") return std::make_unique<TaskMul>(x,y,taskName);
    if (taskName == "DIV") return std::make_unique<TaskDiv>(x,y,taskName);
    return std::unique_ptr<TaskHandler>(nullptr);
}

//! \brief Loads all tasks from JSON file. Returns queue of TaskHandler instances.
std::queue<std::unique_ptr<TaskHandler>> loadTasks(std::string filename)
{
    try
    {
        // Parse JSON file
        std::ifstream jsonFile(filename);
        auto json = nlohmann::json::parse(jsonFile);
        std::queue<std::unique_ptr<TaskHandler>> taskQueue; // Queue for task handlers

        for(const auto& taskJson: json["tasks"])
        {
            // Parse every entry
            std::string taskTypeStr = taskJson["type"];
            int taskX = taskJson["num1"];
            int taskY = taskJson["num2"];
            std::unique_ptr<TaskHandler> taskHandler = taskHandlerCreate(taskTypeStr, taskX, taskY);
            if (taskHandler == nullptr) // Check if task is valid
                throw std::invalid_argument(std::string("Invalid task definition in: ") + std::string(taskJson.dump()));
            taskQueue.push(std::move(taskHandler)); // Push to queue, move on
        }    
          
        return taskQueue;
    }
    catch(const std::exception& e) // Catch JSON Parsing and other errors
    {
        std::cerr << "Error processing input JSON file: \"" << filename << "\"\n" << e.what() << '\n';
        exit(1);
    }
}

int main(int argc, char* argv[])
{
    // Go over every input file
    for(int k = 1; k < argc; k++)
    {
        auto taskQueue = loadTasks(argv[k]);
        
        // Process task queue
        while(!taskQueue.empty())
        {
            int result;
            try
            {
                // Process task
                result = taskQueue.front()->execute();
                std::cout << result;
            }
            catch(const std::exception& e) // Catch task execution errors
            {
                std::cerr << "Exception while processing task \"" << *(taskQueue.front().get()) << "\"\n" << e.what();
            }
            
            taskQueue.pop();
            
            // Check to avoid trailing coma
            if(!taskQueue.empty())
                std::cout << ", ";
        }
        std::cout << '\n';
    }
    return 0;
}