#ifndef TESTRESULTSANALITICS_H
#define TESTRESULTSANALITICS_H

namespace test {
	// Анализ результатов тестирования
	struct TestResultsAnalitics {
		// Общее количество тестов
		unsigned long long tests_count;
		long double average;
	};
}

#endif