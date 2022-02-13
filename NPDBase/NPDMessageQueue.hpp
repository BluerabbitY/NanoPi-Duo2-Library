#ifndef _NPD_MESSAGE_QUEUE_H_
#define _NPD_MESSAGE_QUEUE_H_

#include <queue>
#include <mutex>
#include "NPDError.h"
#include "NPDDataType.h"

namespace NPD
{

    template <typename T>
    class NPDMessageQueue
    {
    public:
        /**
         *  @brief  添加消息队列任务.
         *  @param  _task  消息队列任务.
         *  @return  无返回.
         */
        void push(T _task)
        {
            std::lock_guard<std::mutex> LOCK(_QUEUE_LOCK);
            _NPD_MESSAGE_QUEUE.push(_task);
        }
        /**
         *  @brief  弹出消息队列任务.
         *  @return  消息队列任务.
         */
        T pop()
        {
            if (_NPD_MESSAGE_QUEUE.empty())
            {
                NPD_THROW("MessageQueue::pop()队列为空");
            }
            std::lock_guard<std::mutex> LOCK(_QUEUE_LOCK);
            T _NPD_MESSAGE_QUEUE_FRONT = _NPD_MESSAGE_QUEUE.front();
            _NPD_MESSAGE_QUEUE.pop();
            return std::move(_NPD_MESSAGE_QUEUE_FRONT);
        }
        /**
         *  @brief  消息队列长度.
         *  @return  消息队列长度.
         */
        uint32_t size() const
        {
            std::lock_guard<std::mutex> LOCK(_QUEUE_LOCK);
            return _NPD_MESSAGE_QUEUE.size();
        }
        /**
         *  @brief  清理消息队列.
         *  @return  无返回.
         */
        void clear()
        {
            _NPD_MESSAGE_QUEUE.clear();
        }

    private:
        /** 内部消息队列 */
        std::queue<T> _NPD_MESSAGE_QUEUE;

    private:
        /** 内部线程锁 */
        std::mutex _QUEUE_LOCK;
    };

}

#endif /* _NPD_MESSAGE_QUEUE_H_ */