#include "benchmark.h"
#include <string.h>
#include "randomsleep.h"

using namespace std;
class RandomSleep : Benchmark
{
private:
public:
    // Threading support
    int numberOfTasks;
    int numOfThreads;

    void setInput(int numberOfTasks, int numOfThreads)
    {
        this->numberOfTasks = numberOfTasks;
        this->numOfThreads = numOfThreads;
    }

    static void workerTask(void *threadArgs)
    {
        TaskNumArgs *args = static_cast<TaskNumArgs *>(threadArgs);
        srand(std::hash<std::thread::id>{}(std::this_thread::get_id()));
        int timeToSleep = args->taskNum % args->numberOfThreads;
        sleep(timeToSleep);
    }
    template <typename T>
    void getTasks(T **args, int numberOfTasks)
    {
        for (int i = 0; i < numberOfTasks; i++)
        {
            args[i] = (TaskNumArgs *)malloc(sizeof(TaskNumArgs));
            args[i]->taskNum = i;
            args[i]->numberOfThreads = this->numOfThreads;
        }
    }
};