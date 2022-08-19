//
// Created by wyl on 2022/8/9.
//

#ifndef GSAM_TEST_THREADSAFEQUEUE_H
#define GSAM_TEST_THREADSAFEQUEUE_H
#include <queue>
#include <boost/thread.hpp>
#include <boost/lambda/lambda.hpp>
template<typename Data>
class concurrent_queue
{
private:
    std::queue<Data> the_queue;
    mutable boost::mutex the_mutex;
    boost::condition_variable the_condition_variable;

public:
    void push(Data const& data)
    {
#if 0
        boost::mutex::scoped_lock lock(the_mutex);
        the_queue.push(data);
        lock.unlock();
#else   //利用作用域自动释放锁，防止异常崩溃场景
        {
            boost::mutex::scoped_lock lock(the_mutex);
            the_queue.push(data);
        }
#endif
        the_condition_variable.notify_one();
    }

    bool empty() const
    {
        boost::mutex::scoped_lock lock(the_mutex);
        return the_queue.empty();
    }

    //获取队列头部的值，并移除队列头部
    bool try_pop(Data& popped_value)
    {
        boost::mutex::scoped_lock lock(the_mutex);
        if(the_queue.empty())
        {
            return false;
        }

        popped_value=the_queue.front();
        the_queue.pop();
        return true;
    }

    //等待队列有值，再弹出队列的头部
    void wait_and_pop(Data& popped_value)
    {
        boost::mutex::scoped_lock lock(the_mutex);
        while(the_queue.empty())
        {
            the_condition_variable.wait(lock);
        }

        popped_value=the_queue.front();
        the_queue.pop();
    }

};
#endif //GSAM_TEST_THREADSAFEQUEUE_H
