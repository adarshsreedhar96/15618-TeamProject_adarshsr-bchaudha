#include "benchmark.h"
#include <string.h>
#include "mandelbrot.h"

using namespace std;
class Mandelbrot : Benchmark
{
private:
public:
    // Threading support
    typedef struct
    {
        float x0, x1;
        float y0, y1;
        int width;
        int height;
        int maxIterations;
        int *output;
        int threadId;
        int numThreads;
        // add the rows
        int startRow;
        int totalRows;
    } WorkerArgs;

    void setInput(int viewIndex, int numOfThreads)
    {
        numOfThreads = numOfThreads;
        viewIndex = viewIndex;
        memset(output_thread, 0, width * height * sizeof(int));
    }

    static void workerTask(void *threadArgs)
    {
        WorkerArgs *args = static_cast<WorkerArgs *>(threadArgs);
        int endRow = args->startRow + args->totalRows;
        float dx = (args->x1 - args->x0) / args->width;
        float dy = (args->y1 - args->y0) / args->height;
        for (int j = args->startRow; j < endRow; j++)
        {
            for (int i = 0; i < args->width; ++i)
            {
                float x = args->x0 + i * dx;
                float y = args->y0 + j * dy;
                int index = (j * args->width + i);
                args->output[index] = mandel(x, y, args->maxIterations);
            }
        }
        // return NULL
    }
    template <typename T>
    void getTasks(T **args, int numberOfTasks)
    {
        // int *output_thread = new int[width * height];
        for (int i = 0; i < numberOfTasks; i++)
        {
            args[i] = (WorkerArgs *)malloc(sizeof(WorkerArgs));
            args[i]->threadId = i;
            args[i]->x0 = x0;
            args[i]->y0 = y00;
            args[i]->x1 = x1;
            args[i]->y1 = y11;
            args[i]->height = height;
            args[i]->width = width;
            args[i]->maxIterations = maxIterations;
            args[i]->output = output_thread;
            args[i]->numThreads = numOfThreads;
            args[i]->totalRows = height / numberOfTasks;
            args[i]->startRow = i * args[i]->totalRows;
            if ((i + 1) == numberOfTasks)
            {
                args[i]->totalRows = height - args[i]->startRow;
            }
        }
    }
};