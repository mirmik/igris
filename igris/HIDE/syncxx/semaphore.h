#ifndef IGRIS_SYNC_SEMAPHORE_H
#define IGRIS_SYNC_SEMAPHORE_H

#include <mutex>
#include <condition_variable>
#include <chrono>

// https://elweitzel.de/drupal/?q=node/6

namespace igris
{
	class semaphore
	{
	private:
		unsigned int m_uiCount;
		std::mutex m_mutex;
		std::condition_variable m_condition;

	public:
		inline semaphore(unsigned int uiCount)
			: m_uiCount(uiCount) { }

		inline void wait()
		{
			std::unique_lock< std::mutex > lock(m_mutex);
			m_condition.wait(lock, [&]()->bool{ return m_uiCount > 0; });
			--m_uiCount;
		}

		template< typename R, typename P >
		bool wait(const std::chrono::duration<R, P>& crRelTime)
		{
			std::unique_lock< std::mutex > lock(m_mutex);
			if (!m_condition.wait_for(lock, crRelTime, [&]()->bool{ return m_uiCount > 0; }))
				return false;
			--m_uiCount;
			return true;
		}

		inline void signal()
		{
			std::unique_lock< std::mutex > lock(m_mutex);
			++m_uiCount;
			m_condition.notify_one();
		}
	};
}

#endif
