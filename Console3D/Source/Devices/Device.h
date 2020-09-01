#pragma once

#include "Utilities/Design Patterns/Observer.h"

class SyncDevice : public ObserverBase<SyncDevice>
{
private:
	std::thread m_MainThread;

	struct
	{
		bool Run      : 1;
		bool Notified : 1;
	} m_State;

	std::mutex              m_NotifMut;
	std::condition_variable m_NotifCond;

	virtual void MainThread() = 0;

protected:
	bool Continue();

public:
	virtual ~SyncDevice();

	void Start();

	void Update(bool run);
};
