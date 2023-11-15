#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <Windows.h>

mutex m;
queue<int32> q;
HANDLE handle;

void Producer() {
	while (true) {
		{
			unique_lock<mutex> lock(m);
			q.push(100);
		}

		::SetEvent(handle);
		this_thread::sleep_for(100ms);
	}
}

void Consumer() {
	while (true) {
		::WaitForSingleObject(handle, INFINITE);

		unique_lock<mutex> lock(m);
		if (q.empty() == false)
		{
			int32 data = q.front();
			q.pop();
			cout << data << endl;
		}
	}
}

int main() {
	// 커널 오브젝트 = 커널에서 사용하는 오브젝트
	// Usage Count
	// Signal / Non-Signal << bool
	// 이벤트의 경우 << Auto / Manual << bool

	// TRUE가 ManualReset
	// Event는 커널쪽에서 만들어준다.
	handle = ::CreateEvent(NULL, FALSE, FALSE, NULL);

	thread t1(Producer);
	thread t2(Consumer);

	t1.join();
	t2.join();

	::CloseHandle(handle);

	return 0;
}