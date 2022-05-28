/**
 * @file NPDBuffer.h
 * @author BlueRabbit (515704389@protonmail.com)
 * @brief 自动扩容的数据存放类
 * @version 0.1
 * @date 2022-03-04 09:33:45
 *
 * @copyright Copyright (c) 2022 BlueRabbit
 *
 */

#ifndef _NPD_BUFFER_H_
#define _NPD_BUFFER_H_

#include <vector>
#include <NPDError.h>
#include <NPDDataType.h>

namespace NPD
{
    namespace NPDBASE
    {
        class NPDbuffer
        {
        public:
            NPDbuffer();
            NPDbuffer(const uint32_t size);
            NPDbuffer(const char *src, size_t size);
            NPDbuffer(const NPDbuffer &src);
            NPDbuffer(NPDbuffer &&src);
            ~NPDbuffer();

        public:
            /* 运算符重载 */

            /**
             * @brief (=)运算符重载
             * 
             * @param src 数据
             * @return NPDbuffer& 计算后数据
             */
            NPDbuffer &operator=(const NPDbuffer &src);
            /**
             * @brief (+)运算符重载
             * 
             * @param src 数据
             * @return NPDbuffer& 计算后数据
             */
            NPDbuffer &operator+(const NPDbuffer &src);
            /**
             * @brief (<<)运算符重载
             * 
             * @param src 数据
             * @return NPDbuffer& 计算后数据
             */
            NPDbuffer &operator<<(const NPDbuffer &src);

        public:
            /**
             * @brief 获取NPDbuffer长度
             * 
             * @return uint32_t NPDbuffer长度
             */
            uint32_t size() const;
            /**
             * @brief 重新为NPDbuffer分配空间
             * 
             * @param size 分配空间长度
             */
            void resize(uint32_t size);
            /**
             * @brief 获取NPDbuffer所有内容
             * 
             * @return char* NPDbuffer内容指针(数据可修改)
             */
            char *data();
            /**
             * @brief 获取NPDbuffer所有内容
             * 
             * @return const char* NPDbuffer内容指针(数据不可修改)
             */
            const char *const_data() const;
            /**
             * @brief 按位置从NPDbuffer中获取数据
             * 
             * @param pos 获取数据的启始位置
             * @param size 获取数据的长度
             * @return NPDbuffer 新对象
             */
            NPDbuffer get(uint32_t pos, uint32_t size);
            /**
             * @brief 按位置查询NPDbuffer中的数据
             * 
             * @param pos 查询数据的启始位置
             * @param size 获查询数据的长度
             * @return const char* NPDbuffer内容指针(数据不可修改)
             */
            const char *query(uint32_t pos, uint32_t size) const;
            /**
             * @brief 按位置向NPDbuffer中放置数据
             * 
             * @param pos 放置数据的位置(const char*类型)
             * @param src 要放置的数据
             * @param srcsize 放置数据的长度
             */
            void set(uint32_t pos, const char *src, uint32_t srcsize);
            /**
             * @brief 按位置放置数据
             * 
             * @param pos 放置数据的位置(NPDbuffer类型)
             * @param src 要放置的数据
             */
            void set(uint32_t pos, const NPDbuffer &src);
            /**
             * @brief 在NPDbuffer尾部追加数据
             * 
             * @param src 要追加的数据(const char*类型)
             * @param size 追加数据的长度
             * @return NPDbuffer& 被追加数据的对象
             */
            NPDbuffer &append(const char *src, uint32_t size);
            /**
             * @brief 在NPDbuffer尾部追加数据
             * 
             * @param src 要追加的数据(NPDbuffer类型)
             * @return NPDbuffer& 被追加数据的对象
             */
            NPDbuffer &append(const NPDbuffer &src);
            /**
             * @brief 按位置在NPDbuffer插入数据
             * 
             * @param pos 插入数据的位置
             * @param src 要插入的数据(const char*类型)
             * @param size 插入数据的长度
             */
            void insert(uint32_t pos, const char *src, uint32_t size);
            /**
             *  @brief  按位置在NPDbuffer插入数据.
             *  @param  pos  插入数据的位置.
             *  @param  src  要插入的数据(NPDbuffer类型).
             *  @return  无返回.
             */
            void insert(uint32_t pos, const NPDbuffer &src);
            /**
             * @brief 查询NPDbuffer是否为空
             * 
             * @return true NPDbuffer为空
             * @return false NPDbuffer不为空
             */
            bool empty();
            /**
             * @brief 遍历显示NPDbuffer中所有数据
             * 
             */
            void dump() const;
            /**
             * @brief 按位置遍历显示NPDbuffer中的数据
             * 
             * @param pos 遍历启始位置
             * @param size 要遍历数据的长度
             */
            void dump(uint32_t pos, uint32_t size) const;
            /**
             * @brief 移除NPDbuffer中所有数据
             * 
             */
            void remove();
            /**
             * @brief 按位置移除移除NPDbuffer中的数据
             * 
             * @param pos 移除数据的起始位置
             * @param size 要移除数据的长度
             */
            void remove(uint32_t pos, uint32_t size);

        public:
            /**
             * @brief 从NPDbuffer中按所需类型取出数据
             * 
             * @tparam RetType_T 所需类型数据
             * @return RetType_T 返回指定类型的数据
             */
            template <typename RetType_T, typename = std::enable_if<!std::is_same<RetType_T, std::string>::value>>
            RetType_T value()
            {
                if (sizeof(RetType_T) > _buffer.size() || _buffer.empty())
                {
                    NPDTHROW("NPDbuffer::value()获取值错误,当前类型长度[%u]NPDbuffer长度[%u]", sizeof(RetType_T), _buffer.size());
                }
                RetType_T v;
                memcpy(&v, _buffer.data(), sizeof(RetType_T));
                return std::move(v);
            }

        private:
            std::vector<char> _buffer;
        };
    }
}

#endif /* _NPD_BUFFER_H_ */