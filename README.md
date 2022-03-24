# Dynamic Scheduling with Work Stealing - Framework and Analysis of Different Strategies

### Adarsh Sreedhar (adarshsr)
### Bhakti Chaudhari (bchaudha)

# Summary
We are going to implement a dynamic scheduling framework with work stealing using different strategies, such as child-first, child-continuation, priority-based, randomly-chosen etc., and analyze their performance on various workloads to determine which strategy is best suited for a type of workload. 

# Background
In Multithreaded or Multiprocessor Computations, Work Stealing is a way of balancing the workload across different execution units. After the initial assignment of tasks to threads, due to scheduling, nature of task split or computation, it may happen that certain threads complete faster than others. In such cases, they can steal tasks originally intended for other threads – thereby not only keeping itself busy, but also reducing the workload on the other thread.
There are many ways in which we can maintain the list of tasks – a centralized doubly-ended queue, a per-processor or per-thread queue with preassigned tasks, or a priority queue based on certain heuristics. As with a queue, we can choose to remove tasks from the top or bottom. A right set of choices of the implementation details for a workload can greatly improve parallelism, and a wrong set of choices may hinder performance than without any work stealing itself. Moreover, for problems that need a first/single-result logic, the importance of the scheduler becomes even more underlined.
We want to implement a workload sensitive scheduler with select work stealing strategy.

# The Challenge
There are several factors determining the best choice for a workload. Some of them are:
-	The Scheduler
o	The initial distribution of the tasks ensures that all processors/execution units are busy. A basic rule of thumb is to create at least as many tasks as there as number of threads, which should be at least as many as the execution units available.
o	If we use a centralized queue, then threads can deque as and when they become idle, however the threads cause race conditions in dequeuing a task. Using individual task queues involve more work at the initial scheduling, as well as possibility of uneven work distribution.
o	Task granularity is an important factor as well – a very large task may be shelved frequently due to context switching causing unnecessary I/O and increase in latency.
##### -	The Work Stealing algorithm 
Since there is a communication penalty in stealing a task, the algorithm must factor whether the currently executing thread would still be occupied by the time the tasks have been brought to the new unit’s space completely and begins execution.
Depending on the task granularity and the number of tasks available for stealing, we must also consider the number of tasks to steal – since it is faster to steal multiple tasks at once than at different intervals. It also depends on whether we steal multiple tasks from the same thread or individual tasks from various threads.
##### -	Inter-Processor Communication Cost
As mentioned, transferring a task requires use of a bus, and perhaps even invalidation of cache in the original processor. 
In cases where Non-Uniform Memory Access is used, it adds a greater penalty to shift the tasks as memory access becomes longer.
##### -	Data Locality
If the computation operates on sequential data structures, then allocating contiguous ranges of data to tasks helps in improving cache hit performance. The work stealing algorithm must also take into factor the loss of locality as well as the communication costs before attempting to steal a task.
##### -	Work Thread Pool
Creating a pool of threads before we scheduler the tasks help avoid expensive runtime costs in creating and deleting address-space/ signal-set and other resources for each task. This also helps reuse of the threads.
##### -	Nature of the Problem Definition
As in the case of the Mandelbrot computation, we know that every pixel has a different convergence factor. This implies that each task may finish at different intervals, requiring a stealing logic. However, if the computation performed is around the same for any data, then all tasks are mostly completed on time, and work stealing is not required.
##### -	Size Complexity of the Result
If the order of runtime for the logic is high such as O(n2) number of tasks are very high, the data structure size may increase and consume significant memory. The OS scheduler would also attempt to pause this process and run other processes, in which there is cache invalidations, and switching between primary and secondary memory as well, all of which contribute to significant increase in runtime. 
As mentioned above, we need to able to make educated guesses on the right set of parameters to choose based on our dataset or processing logic – and adding this intelligence makes it a hard problem t resolve.

# Resources
We have selected few papers for our literature survey. 
[1] attempts to study the problem of efficiently scheduling multithreaded computations on multiprocessors, and have come up with a set of theorems that explain the expected speedup in a mathematical form.
[2] presents an interesting idea of confidence-based work stealing, wherein they deal with a problem of search i.e. an O(1) result by parallelly searching an O(n) space. We would like to implement our priority queue-based implementation based on this idea, i.e., to use a set of heuristics to change the priority and schedule work so as to find the result as quickly as possible.
[3] attempts to explore randomized work stealing on large scale systems through use of Partitioned Global Address Space (PGAS). While the implementation is out of scope for our survey, they explore various benchmarks and make use of split task queues and lockless release operations, which we believe can prove to be an important resource for perusal.
Lastly, [4] is a survey paper that reviews work stealing scheduling from the perspective of scheduling algorithms, optimization of algorithm implementation and processor architecture-oriented optimization. 
Beyond Literature Survey, we would heavily reference the Cilk Library for splitting the workload into independent tasks and then scheduling them as required. The original paper [5] would also help us in understanding the problems for which this library was designed to alleviate.
[6] introduces Wool, a work-stealing library that is very similar to Cilk, but allows the user to make these calls in C itself, without having to write in a different language or construct (Cilk-5 or Intel TBB). Since we are also planning to provide a simple interface abstracting the multiple implementations of work stealing algorithms, this paper may provide valuable information.

# Goals and Deliverables
#### Should Achieve (75%):
We will implement a task scheduling library that provides APIs for various scheduling algorithms, each of which employ different data-structures to store the tasks, as well as different strategies in stealing work during cases of uneven load distribution. We will employ two data structures – a centralized doubly ended queue as well as a per-task queue. We will then run these algorithms for various datasets and problem definitions, on different execution unit combinations, and collect and analyze the speedup obtained.

#### Plan to Achieve (100%):
We will attempt more data-structures to store the tasks such as priority queues based on custom ranking logic as well as randomized task selection.We will perform a deeper analysis of the results obtained on all the algorithms used - results in terms of the speedup obtained, bottleneck observed, cache hit rate, load distribution after work stealing, and then deduce the set of parameters required to be able to choose a specific scheduler for maximum performance.

#### Hope to Achieve (125%):
We want to develop a heuristic which will automate the selection of the most optimal work stealing algorithm for a given problem set. Basically, the analysis we manually perform above could then be fed as a set of rules to the heuristic, thereby enabling the user to get maximum performance even without knowing the internals.

# Platform Choice:
We will test our algorithms on the GHC, PSC and Latedays cluster. We will implement this in C++11.
The difference in the number of cores, as well as the size of L3 cache and RAM, would also help us in robust testing of our framework

# Schedule
Week	Deliverables
Week 1	Literature Survey Complete, and rough outline of the APIs to expose to the user, and the selection of work stealing algorithms + data structures to use to maintain the tasks
Week 2	Implement a centralized deque from which all threads can steal tasks – and finalize the set of benchmarks to run on
Week 3	Implement two other strategies – using per-processor queues with both child-first and child-continuation stealing
Week 4	Run the different strategies on the benchmarks on various machines – GHC, PSC and the Latedays Cluster. Gather and filter the data
Week 5	Finalize the Library API endpoints, and internally call the relevant algorithms (Integration Phase)
Week 6	Finalize the Report, Deduce the Set of Parameters to select a logic and possibly add a simple heuristic with rules to select the same.

# References:
[1] Blumofe, R.D. and Leiserson, C.E., 1999. Scheduling multithreaded computations by work stealing. Journal of the ACM (JACM), 46(5), pp.720-748.
[2] Chu G., Schulte C., Stuckey P.J. (2009) Confidence-Based Work Stealing in Parallel Constraint Programming. In: Gent I.P. (eds) Principles and Practice of Constraint Programming - CP 2009. CP 2009. Lecture Notes in Computer Science, vol 5732. Springer, Berlin, Heidelberg. https://doi.org/10.1007/978-3-642-04244-7_20
[3] SC '09: Proceedings of the Conference on High Performance Computing Networking, Storage and Analysis November 2009 Article No.: 53Pages 1–11. https://doi.org/10.1145/1654059.1654113
[4] Yang, J., He, Q. Scheduling Parallel Computations by Work Stealing: A Survey. Int J Parallel Prog 46, 173–197 (2018). https://doi.org/10.1007/s10766-016-0484-8
[5] PPOPP '95: Proceedings of the fifth ACM SIGPLAN symposium on Principles and practice of parallel programming August 1995 Pages 207–216. https://doi.org/10.1145/209936.209958
[6] Faxen, Karl-Filip. (2008). Wool-A work stealing library. SIGARCH Computer Architecture News. 36. 93-100. 10.1145/1556444.1556457.
