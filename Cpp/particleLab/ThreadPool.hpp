#include <thread>
#include <vector>

template <typename T>
class Worker
{
    std::shared_ptr<std::thread> threadPtr_;
    bool running = false;

    Worker(T task)
    {
        running = true;
        threadPtr_ =  std::make_shared<std::thread>(
            thread([this,task](){
                while(!running){}; // wait for start
                while(this->running)
                {
                    task();
                }
            })
        );
        threadPtr_->detach()
    }

    void start()
    {
        this->running = true;
    }

    void stop()
    {
        this->running = false;
    }

    ~Worker()
    {
        stop();
    }
};

template <typename T>
class Pool
{
    std::vector<Worker> workers;
    public:
        Pool(T task, int nWorkers)
        {
            for(int n = 0 ; n < nWorkers; n++)
            {
                workers.append(Worker(task));
            }
        }

        stop()
        {
            for(auto worker : workers)
            {
                worker.stop();
            }
        }
};