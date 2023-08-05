#pragma once

#include <thread>
#include <vector>

#include <mutex>
#include <queue>

#include <functional>

// Thread-safe queue
template <typename T>
class TSQueue {
    private:
        // Underlying queue
        std::queue<T> m_queue;
        // mutex for thread synchronization
        std::mutex m_mutex;
        // Condition variable for signaling
        std::condition_variable m_cond;
    public:
        // Pushes an element to the queue
        void push(T item)
        {
            // Acquire lock
            std::unique_lock<std::mutex> lock(m_mutex);
            // Add item
            m_queue.push(item);
            // Notify one thread that
            // is waiting
            m_cond.notify_one();
        }
        // Pops an element off the queue
        T pop()
        {
            // acquire lock
            std::unique_lock<std::mutex> lock(m_mutex);
            // wait until queue is not empty
            m_cond.wait(lock,
                        [this]() { return !m_queue.empty(); });
            // retrieve item
            T item = m_queue.front();
            m_queue.pop();
            // return item
            return item;
        }

        // Get size of queue
        size_t size()
        {
            // acquire lock
            std::unique_lock<std::mutex> lock(m_mutex);
            // return item
            return m_queue.size();
        }
};

class Worker
{
    private:
        std::shared_ptr<std::thread> threadPtr_;
        bool running = false;

    public:

        template <typename InType,typename OutType>
        void execute(std::function<void(TSQueue<InType>&, OutType&)> task,TSQueue<InType> &workload,OutType& outputContainer)
        {
            if(running) return;

            running = true;
            threadPtr_ =  std::make_shared<std::thread>(
                [this,task,&workload,&outputContainer](){
                    while(!this->running){}; // wait for start
                    while(workload.size() > 0)
                    {
                        task(workload,outputContainer);
                    }
                }
            );
            threadPtr_->detach();
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

class Pool
{
    std::vector<Worker> workers;
    public:
        Pool(int nWorkers)
        {
            for(int n = 0 ; n < nWorkers; n++)
            {
                workers.push_back(Worker());
            }
        }

        template <typename InType,typename OutType>
        void start(std::function<void(TSQueue<InType>&, OutType&)> task,TSQueue<InType> &workload,OutType& outputContainer)
        {
            for(auto worker : workers)
            {
                worker.execute(task,workload,outputContainer);
            }
        };

        void  stop()
        {
            for(auto worker : workers)
            {
                worker.stop();
            }
        }
};