#ifndef MPE_THREADPOOL_HPP
#define MPE_THREADPOOL_HPP

#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <functional>
#include <future>
#include <memory>
#include <utility>
#include <condition_variable>

namespace mpe
{
	//Singleton thread pool to execute parallel tasks
	class ThreadPool
	{
	public:
		static ThreadPool& Get();
		~ThreadPool();
		template <typename F>
		auto Execute(F&& func) -> std::future<decltype(func())>
		{
			using R = decltype(func());
			auto func_wrap = std::make_shared<std::packaged_task<R()>>(std::move(func));
			{
				std::lock_guard<std::mutex> lg(mtx);
				tasks.emplace([=]() { (*func_wrap)(); });
			}
			cv.notify_one();
			return func_wrap->get_future();
		}
	private:
		ThreadPool(unsigned num_threads);
		std::vector<std::thread> pool;
		std::queue<std::function<void()>> tasks;
		std::mutex mtx;
		std::condition_variable cv;
		unsigned char flags;
		void Start(unsigned num_threads);
		void End();
	};
}

#endif