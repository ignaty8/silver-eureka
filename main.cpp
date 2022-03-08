enum e_TaskType {
    ADD,
    SUB,
    MUL,
    DIV,
    TASK_MAX
};


typedef e_TaskType TaskType;

static const std::map<TaskType, std::type_index> taskTypeMap = 
{
    {TaskType::ADD, typeid(TaskAdd)},
    {TaskType::SUB, typeid(TaskSub)},
    {TaskType::MUL, typeid(TaskMul)},
    {TaskType::DIV, typeid(TaskDiv)}
};

class TaskHandler {

public:
    TaskHandler(int x, int y) : x(x), y(y) {}

    //TaskType type;
    virtual int execute() = 0;

protected:
    
    int x, y;

};

class TaskAdd : TaskHandler {

public:
    virtual int execute()
    {
        return x + y;
    }
};

class TaskSub : TaskHandler {

public:
    virtual int execute()
    {
        return x - y;
    }
};

class TaskMul : TaskHandler {

public:
    virtual int execute()
    {
        return x * y;
    }
};

class TaskDiv : TaskHandler {

public:
    virtual int execute()
    {
        return x / y;
    }
};
