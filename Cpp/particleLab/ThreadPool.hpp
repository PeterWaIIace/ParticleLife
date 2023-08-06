#pragma once

#include <thread>
#include <vector>
#include <array>

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

        size_t size__ = 0;
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

        bool empty()
        {
            return !m_queue.size();
        }

        // Get size of queue
        size_t size()
        {
            return m_queue.size();
        }
};


template <class  InType,class  OutType>
class Worker
{
    private:
        struct ThreadControl{
            bool running = false;
        };

        std::shared_ptr<ThreadControl> control;
        std::shared_ptr<std::thread> threadPtr_;
        std::shared_ptr<TSQueue<InType>> workloadStream;
        std::shared_ptr<TSQueue<OutType>> outputStream;

    public:

        Worker(Worker& other)
        {
            // std::cout << "Creating copy: " << this << std::endl;
            control = other.control;
            threadPtr_ = other.threadPtr_;
            workloadStream = other.workloadStream;
            outputStream = other.outputStream;
        }

        Worker(std::function<OutType&(InType&)> task,std::shared_ptr<TSQueue<InType>> workloadStream,std::shared_ptr<TSQueue<OutType>> outputStream)
        {
            // std::cout << "Creating: " << this << std::endl;
            control = std::make_shared<ThreadControl>();
            control->running = true;

            workloadStream = workloadStream;
            outputStream = outputStream;

            auto ptrControl_ = control;
            auto ptrWorkloadStream_ = workloadStream;
            auto ptrOutputStream_ = outputStream;

            threadPtr_ =  std::make_shared<std::thread>(
                [ptrControl_,ptrWorkloadStream_,ptrOutputStream_,task](){
                    while(ptrControl_->running){
                        while(!ptrWorkloadStream_->empty() && ptrControl_->running)
                        {
                            ptrOutputStream_->push(
                                task(
                                    ptrWorkloadStream_->pop()
                                )
                            );
                        }
                    }; // wait for start
                    ptrControl_->running = false;
                }
            );
            threadPtr_->detach();
        }

        void execute()
        {
            if(control->running) return;

            control->running = true;
        }

        void connectWorkload()
        {

        }

        void stop()
        {
            control->running = false;
        }

        ~Worker(){
        //     std::cout << "Deleting: " << this << std::endl;
        }
};

//TODO: add diagnostics

template <class InType,class OutType>
class Pool
{
    int nTasks = 0;
    std::function<OutType&(InType&)> task;
    std::vector<Worker<InType,OutType>> workers;
    std::shared_ptr<TSQueue<InType>> workloadStream = std::make_shared<TSQueue<InType>>();
    std::shared_ptr<TSQueue<OutType>> outputStream  = std::make_shared<TSQueue<OutType>>();

    public:
        Pool(){};
        Pool(Pool &other){
            task = other.task;
            for(int n = 0 ; n < nWorkers ; n++)
            {
                workers.push_back(Worker<InType,OutType>(task,workloadStream,outputStream));
            }
        };

        Pool(int nWorkers,std::function<OutType&(InType&)> task)
        {
            task = task;
            for(int n = 0 ; n < nWorkers ; n++)
            {
                workers.push_back(Worker<InType,OutType>(task,workloadStream,outputStream));
            }
        }

        void push(InType job)
        {
            workloadStream->push(job);
            nTasks++;
        }

        std::vector<OutType> getResults()
        {
            std::vector<OutType> tmp_;
            auto nResults = outputStream->size();
            for(int n = 0 ; n < nResults ; n++)
            {
                tmp_.push_back(outputStream->pop());
            }
            return tmp_;
        }

        void start()
        {
            for(auto worker : workers)
            {
                worker.execute();
            }
        };

        void await()
        {
            while(workloadStream->size() > 0){};
            while(nTasks != outputStream->size()){};
        }

        void  stop()
        {
            for(auto worker : workers)
            {
                worker.stop();
            }
        }
};