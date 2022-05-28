/**
 * @file NPDMessageQueue.hpp
 * @author BlueRabbit (515704389@protonmail.com)
 * @brief 线程安全队列
 * @version 0.1
 * @date 2022-03-04 10:52:12 
 * 
 * @copyright Copyright (c) 2022 BlueRabbit
 * 
 */

#ifndef _NPD_MESSAGE_QUEUE_H_
#define _NPD_MESSAGE_QUEUE_H_

#include <queue>
#include <mutex>
#include <NPDError.h>
#include "NPDDataType.h"

namespace NPD
{
    namespace NPDBASE
    {
        template <typename EleType_T>
        class NPDMessageQueue
        {
        public:
            /**
             * @brief 添加消息队列任务
             * 
             * @param _task 消息队列任务
             */
            void push(EleType_T _task)
            {
                std::lock_guard<std::mutex> LOCK(_QUEUE_LOCK);
                _NPD_MESSAGE_QUEUE.push(_task);
            }
            /**
             * @brief 弹出消息队列任务
             * 
             * @return EleType_T 消息队列任务
             */
            EleType_T pop()
            {
                if (_NPD_MESSAGE_QUEUE.empty())
                {
                    NPDTHROW("MessageQueue::pop()队列为空");
                }
                std::lock_guard<std::mutex> LOCK(_QUEUE_LOCK);
                EleType_T _NPD_MESSAGE_QUEUE_FRONT = _NPD_MESSAGE_QUEUE.front();
                _NPD_MESSAGE_QUEUE.pop();
                return std::move(_NPD_MESSAGE_QUEUE_FRONT);
            }
            /**
             * @brief 获取消息队列长度
             * 
             * @return uint32_t 消息队列长度
             */
            const uint32_t size()
            {
                std::lock_guard<std::mutex> LOCK(_QUEUE_LOCK);
                return _NPD_MESSAGE_QUEUE.size();
            }
            /**
             * @brief 清理消息队列
             * 
             */
            void clear()
            {
                std::lock_guard<std::mutex> LOCK(_QUEUE_LOCK);
                _NPD_MESSAGE_QUEUE.clear();
            }

        private:
            /** 内部消息队列 */
            std::queue<EleType_T> _NPD_MESSAGE_QUEUE;

        private:
            /** 内部线程锁 */
            std::mutex _QUEUE_LOCK;
        };
    }
}

#endif /* _NPD_MESSAGE_QUEUE_H_ */