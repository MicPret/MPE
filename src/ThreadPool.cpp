#include <MPE/Core/ThreadPool.hpp>

static void thread_func(
	std::condition_variable& cv,
	std::mutex& mtx,
	std::queue<std::function<void()>>& tasks,
	const unsigned char& flags);

constexpr char DONE = 1;
constexpr char RUNNING = 2;

namespace mpe
{
	ThreadPool::ThreadPool(unsigned num_threads) :
		flags(RUNNING)
	{
		Start(num_threads);
	}

	ThreadPool& ThreadPool::Get()
	{
		static ThreadPool t(std::thread::hardware_concurrency());
		return t;
	}

	ThreadPool::~ThreadPool()
	{
		End();
	}

	void ThreadPool::Start(unsigned num_threads)
	{
		pool.reserve(num_threads);
		for (unsigned i = 0; i < num_threads; i++)
			pool.emplace_back([&]() { thread_func(cv, mtx, tasks, flags); });
	}

	void ThreadPool::End()
	{
		{
			std::lock_guard<std::mutex> lg(mtx);
			flags = DONE;
		}
		cv.notify_all();
		for (auto& t : pool)
			t.join();
	}
}

void thread_func(
	std::condition_variable& cv,
	std::mutex& mtx,
	std::queue<std::function<void()>>& tasks,
	const unsigned char& flags)
{
	std::function<void()> task;

	while (true)
	{
		{
			std::unique_lock lk(mtx);
			cv.wait(lk, [&]() { return (flags == DONE) || !tasks.empty(); });

			if ((flags == DONE) && tasks.empty())
				break;

			task = std::move(tasks.front());
			tasks.pop();
		}
		task();
	}
}