typedef struct
{
    int taskNum;
    int numberOfThreads;
} TaskNumArgs;

void printTaskNum(void *task_num)
{
    TaskNumArgs *args = static_cast<TaskNumArgs *>(task_num);
    printf("helloworld called by task_num: %d\n", args->taskNum);
    srand(std::hash<std::thread::id>{}(std::this_thread::get_id()));
    int timeToSleep = args->taskNum % args->numberOfThreads;
    sleep(timeToSleep);
}

void printEmpty()
{
    srand(std::hash<std::thread::id>{}(std::this_thread::get_id()));
    int timeToSleep = rand() % 10;
    sleep(timeToSleep);
}