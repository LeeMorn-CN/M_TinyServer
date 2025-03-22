#ifndef M_THREADPOOL_H
#define M_THREADPOOL_H
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <functional>
#include <memory>
#include <vector>

// 线程池类
class ThreadPool {
public:
     // 线程池的数量和任务数
    int thread_pool_size;
    int max_tasks;
    // 构造函数
    ThreadPool(int thread_number = 8, int max_request = 10000): 
    thread_pool_size(thread_number),max_tasks(max_request),ctrl_access(std::make_shared<PoolControl>()){
        //创建线程
        for(int i=0; i<thread_number; ++i){
            m_threads.emplace_back([&ctrl = ctrl_access]{
                while(true){
                    std::unique_lock<std::mutex> m_lock(ctrl->mtx);
                    if(!ctrl->tasks.empty()){
                        auto task = ctrl->tasks.front();
                        ctrl->tasks.pop();
                        m_lock.unlock();
                        task();
                    }
                    else if(ctrl->is_close) break; //关闭线程
                    else ctrl->cv.wait(m_lock);
                }
            }
            );
        }
    }
    ~ThreadPool(){
        {
            std::lock_guard<std::mutex> m_lock(ctrl_access->mtx);
            ctrl_access->is_close = true;
        }
        ctrl_access->cv.notify_all();
        //等待全部线程退出
        for(auto& m_thread : m_threads){
            m_thread.join();
        }
    }

    // 添加任务
    template<typename T>
    bool append_task(T&& task){
        {
            std::lock_guard<std::mutex> m_lock(ctrl_access->mtx);
            ctrl_access->tasks.emplace(std::forward<T>(task));
        }
        ctrl_access->cv.notify_one();
        return true;
    }

private:
    //工作线程
    std::vector<std::thread> m_threads;
    // 线程池控制
    struct PoolControl {
        std::mutex mtx;   //互斥锁
        std::condition_variable cv;     //条件变量
        std::queue<std::function<void()>> tasks;        //任务队列
        bool is_close = false;      //线程池状态
    };
    std::shared_ptr<PoolControl> ctrl_access;//控制入口
};
#endif 