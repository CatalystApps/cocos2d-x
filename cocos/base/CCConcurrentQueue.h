////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  CCConcurrentQueue.h
//  cocos2d_libs
//
//  Created by Dmitry Bushtets on 4/5/16.
//
//  Using idea https://juanchopanzacpp.wordpress.com/2013/02/26/concurrent-queue-c11/
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <queue>
#include "platform/CCPlatformMacros.h"

NS_CC_BEGIN

template<typename T>
class CCConcurrentQueue
{
public:
    // TODO: maybe popAll() ?
    
    T pop()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        while (m_queue.empty())
        {
            m_cond.wait(lock);
        }
        auto item = m_queue.front();
        m_queue.pop();
        return item;
    }
    
    void pop(T& item)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        while (m_queue.empty())
        {
            m_cond.wait(lock);
        }
        item = m_queue.front();
        m_queue.pop();
    }
    
    void push(const T& item)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_queue.push(item);
        lock.unlock();
        m_cond.notify_one();
    }
    
    void push(T&& item)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_queue.push(std::move(item));
        lock.unlock();
        m_cond.notify_one();
    }
    
    // method for read checks only
    bool isEmpty() const
    {
        // in fact, this will be faster without any mutex
        // if we miss read some events - we will read it in next loop
        return m_queue.empty();
    }
    
    CCConcurrentQueue()=default;
    CCConcurrentQueue(const CCConcurrentQueue&)=delete;
    CCConcurrentQueue& operator=(const CCConcurrentQueue&)=delete;

private:
    std::queue<T> m_queue;
    std::mutex m_mutex;
    std::condition_variable m_cond;
    
};

NS_CC_END
