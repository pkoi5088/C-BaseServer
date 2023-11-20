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

// CV는 User-Level Object
condition_variable cv;

void Producer() {
	while (true) {
		// 1) Lock을 잡고
		// 2) 공유 변수 값을 수정
		// 3) Lock을 풀고
		// 4) 조건변수 통해 다른 쓰레드에게 통지

		{
			unique_lock<mutex> lock(m);
			q.push(100);
		}

		cv.notify_one();
	}
}

void Consumer() {
	while (true) {
		unique_lock<mutex> lock(m);
		cv.wait(lock, []() { return q.empty() == false; });
		// 1) Lock을 잡고
		// 2) 조건 확인
		// - 만족 O => 빠져 나와서 코드를 진행
		// - 만족 X => Lock을 풀어주고 대기 상태

		//if (q.empty() == false)
		{
			int32 data = q.front();
			q.pop();
			cout << q.size() << endl;
		}
	}
}

int main() {
	thread t1(Producer);
	thread t2(Consumer);

	t1.join();
	t2.join();

	return 0;
}