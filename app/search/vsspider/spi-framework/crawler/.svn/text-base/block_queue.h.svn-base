#ifndef __BLOCK_QUEUE_H__
#define __BLOCK_QUEUE_H__

#include "bsl/containers/deque/bsl_deque.h"
#include "init.h"

// 初始化的block_queue的大小
#define INIT_BLOCK_QUEUE_SIZE 30000

// 对mutex封装的类, 程序更安全，更简洁
class lock_wrapper_t
{
	private:                      
		pthread_mutex_t *_lock;
	public:
		lock_wrapper_t(pthread_mutex_t *lock) : _lock(lock) {
			pthread_mutex_lock(_lock);     
		}  
		~lock_wrapper_t() {       
			pthread_mutex_unlock(_lock);   
		}  
};

// 用bsl::deque包装的线程安全的阻塞队列
template<typename DataType>
class block_queue_t{

	private:
		pthread_cond_t  _cond;
		pthread_mutex_t _mutex;
		int _size;
		int _limit;

		typedef bsl::deque<DataType> queue_t;
		queue_t q;
	public:
		// 阻塞队列的大小
		block_queue_t(int len = INIT_BLOCK_QUEUE_SIZE);
		
		// 析构函数
		virtual ~block_queue_t();

		// 设置堵塞条件大小
		void set_limit(int limit);

		// 出队列
		int pop(DataType& data);

		// 入队列
		int push(const DataType& data);

		// 队列元素个数
		int size();

		// 清空
		int clear();

		void info(){ _LOG_NOTICE("size:%d,limit:%d",_size,_limit); }
};

template<class DataType>
block_queue_t<DataType>::block_queue_t(int cap )
{
	q.create(cap) ;
	_limit =  cap * 3 / 4;
	_size = 0;
	pthread_mutex_init(&_mutex, NULL);
	pthread_cond_init(&_cond,NULL);
}

template<class DataType>
block_queue_t<DataType>::~block_queue_t()
{
	pthread_mutex_destroy(&_mutex);
	pthread_cond_destroy(&_cond);
}

template<class DataType>
void block_queue_t<DataType>::set_limit(int limit)
{
	pthread_mutex_lock(&_mutex);
	_limit = limit;
	pthread_mutex_unlock(&_mutex);
}

template<class DataType>
int block_queue_t<DataType>::pop(DataType& data)
{
	pthread_mutex_lock(&_mutex);
	data = q.front();
	if( q.pop_front() == 0){
		_size--;
		pthread_cond_broadcast(&_cond);
		pthread_mutex_unlock(&_mutex);
		return 0;
	}
	pthread_mutex_unlock(&_mutex);
	return -1;
}

template<class DataType>
int block_queue_t<DataType>::push(const DataType& data)
{
	pthread_mutex_lock(&_mutex);
	while( _size == _limit){
		_LOG_WARNING("Block queue block at size[%d]",_size);
		pthread_cond_wait(&_cond,&_mutex);
	}
	if( q.push_back(data) == 0 ){
		_size++;
		pthread_mutex_unlock(&_mutex);
		return 0;
	}
	pthread_mutex_unlock(&_mutex);
	return -1;
}

template<class DataType>
int block_queue_t<DataType>::size()
{
	lock_wrapper_t lock(&_mutex);
	return _size;
}

template<class DataType>
int block_queue_t<DataType>::clear()
{
	lock_wrapper_t lock(&_mutex);
	_size = 0;
	return q.clear();
}

#endif
