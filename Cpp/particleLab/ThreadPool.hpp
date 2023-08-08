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

        void workerThread(  std::shared_ptr<ThreadControl> &ptrControl_, 
                            std::shared_ptr<TSQueue<InType>> &ptrWorkloadStream_, 
                            std::shared_ptr<TSQueue<OutType>> &ptrOutputStream_,
                            std::function<OutType&(InType&)> &task)
            {
            while(ptrControl_->running){
                while(!ptrWorkloadStream_->empty() && ptrControl_->running)
                    {
                        auto tmp = task(
                            ptrWorkloadStream_->pop()
                        );
                        ptrOutputStream_->push(tmp);
                }
            };
        }

        Worker(std::function<OutType&(InType&)> &task,std::shared_ptr<TSQueue<InType>> workloadStream,std::shared_ptr<TSQueue<OutType>> outputStream)
        {
            // std::cout << "Creating: " << this << std::endl;
            control = std::make_shared<ThreadControl>();
            control->running = true;

            workloadStream = workloadStream;
            outputStream = outputStream;

            threadPtr_ = std::make_shared<std::thread>(&Worker<InType,OutType>::workerThread,this,control,workloadStream,outputStream,task);
            // threadPtr_ =  std::make_shared<std::thread>(Worker::workerThread,this);
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
    int channel__ = 0;
    int nWorkers__ = 0;
    std::function<OutType&(InType&)> task;
    std::vector<Worker<InType,OutType>> workers;
    std::vector<std::shared_ptr<TSQueue<InType>>> inputStreams;
    std::vector<std::shared_ptr<TSQueue<OutType>>> outputStreams;
    // std::shared_ptr<TSQueue<InType>> workloadStream = std::make_shared<TSQueue<InType>>();
    // std::shared_ptr<TSQueue<OutType>> outputStream  = std::make_shared<TSQueue<OutType>>();

    public:
        Pool(){};
        Pool(Pool &other){
            task = other.task;
            nWorkers__ = other.nWorkers;
            for(int n = 0 ; n < nWorkers__ ; n++)
            {
                inputStreams.push_back(std::make_shared<TSQueue<InType>>());
                outputStreams.push_back(std::make_shared<TSQueue<OutType>>());
                workers.push_back(Worker<InType,OutType>(task,inputStreams[n],outputStreams[n]));
            }
        };

        Pool(int nWorkers,std::function<OutType&(InType&)> task)
        {
            task = task;
            nWorkers__ = nWorkers;
            for(int n = 0 ; n < nWorkers__ ; n++)
            {
                inputStreams.push_back(std::make_shared<TSQueue<InType>>());
                outputStreams.push_back(std::make_shared<TSQueue<OutType>>());
                workers.push_back(Worker<InType,OutType>(task,inputStreams[n],outputStreams[n]));
            }
        }

        void push(InType job)
        {
            inputStreams[channel__]->push(job);
            channel__++;
            channel__ %= nWorkers__;
            nTasks++;
        }

        std::vector<OutType> getResults()
        {
            std::vector<OutType> tmp_;
            for(int j = 0 ; j < nWorkers__ ; j++){
                auto nResults = outputStreams[j]->size();
                for(int n = 0 ; n < nResults ; n++)
                {
                    tmp_.push_back(outputStreams[j]->pop());
                }
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
            size_t size = 1;
            while(size > 0){
                size = 0;
                for(int j = 0 ; j < nWorkers__ ; j++){
                    size += inputStreams[j]->size();
                }
            };

            size = 0;
            while(nTasks != size){
                size = 0;
                for(int j = 0 ; j < nWorkers__ ; j++){
                    size += outputStreams[j]->size();
                }
            };
        }

        void  stop()
        {
            for(auto worker : workers)
            {
                worker.stop();
            }
        }
};