#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <future>

int64 Calculate() {
	int64 sum = 0;

	for (int32 i = 0; i < 100'000; i++)
		sum += i;

	return sum;
}

void PromiseWorker(std::promise<string>&& promise) {
	promise.set_value("Secret Message");
}

void TaskWorker(std::packaged_task<int64(void)>&& task) {
	task();
}

int main() {
	{
		// 1) deferred
		// 2) async
		// 3) deferred + async
		// 미래에 결과물을 반환해줄꺼라 약속
		std::future<int64> future = std::async(std::launch::async, Calculate);
		int64 sum = future.get();
	}

	// std::promise
	{
		// 미래에 결과물을 반환해줄것이라 약속
		std::promise<string> promise;
		std::future<string> future = promise.get_future();

		thread t(PromiseWorker, std::move(promise));

		string message = future.get();
		cout << message << endl;

		t.join();
	}

	// std::packaged_task
	{
		std::packaged_task<int64(void)> task(Calculate);
		std::future<int64> future = task.get_future();

		std::thread t(TaskWorker, std::move(task));

		int64 sum = future.get();
		cout << sum << endl;
	}
	// 1) async: 원하는 함수를 비동기적으로 실행
	// 2) promise: 결과물을 promise를 통해 future로 받아줌
	// 3) packaged_task: 원하는 함수의 결과를 packaged_task를 통해 future로 받아줌

	return 0;
}