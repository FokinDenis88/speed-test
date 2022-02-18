#ifndef TESTSPEED_H
#define TESTSPEED_H

#include <vector>
#include <mutex>

#include <iostream>
#include <chrono>
#include <exception>
#include <thread>

#include <algorithm>
#include <execution>
#include <tuple>

#include "TestResultsAnalitics.h"
#include "WriteLineConsole.h"


namespace test {
    inline unsigned long long tests_count_default{ 30000000 };
    typedef unsigned long long TestsCountT;

    template<typename ReturnT, typename... ArgsT>
    class TestSpeed {
    public:
        typedef unsigned long long TestResultType;
        typedef ReturnT(*const FuncForTest)(ArgsT...);

        TestSpeed(FuncForTest code_for_test_p, const ArgsT... args,
                  const std::string& name_p = "",
                  const TestsCountT test_count_p = tests_count_default,
                  const bool is_multithread_p = false)
            : test_name_{ name_p },
              tests_count{ test_count_p },
              code_for_test_{ code_for_test_p },
              args_{ std::tuple<ArgsT...>(args...) },
              tests_results_{ std::vector<TestResultType>(test_count_p) },
              is_multithread_{ is_multithread_p }
        {
            //tests_results_ptr = new std::vector<TestResultType>;
        }

        TestSpeed(const TestSpeed& obj)
            : test_name_{ obj.test_name_ },
            tests_count{ obj.tests_count },
            code_for_test_{ obj.code_for_test_ },
            args_{ obj.args_ },
            tests_results_{ obj.tests_results_ },
            test_analitics_{ obj.test_analitics_ },
            is_multithread_{ obj.is_multithread_ }
        {
        }

        TestSpeed& operator=(const TestSpeed& rhs) {
            test_name_ = rhs.test_name_;
            tests_count= rhs.tests_count;
            code_for_test_= rhs.code_for_test_;
            args_ = rhs.args_;
            tests_results_ = rhs.tests_results_;
            test_analitics_ = rhs.test_analitics_;
            is_multithread_ = rhs.is_multithread_;

            return *this;
        }

        ~TestSpeed() {
        }
        
        /// <summary>
        /// Запуск тестирования, вывод результатов тестирования.
        /// </summary>
        void RunTest() {
            try {
                if (is_multithread_) { // parallel execution
                    std::for_each(std::execution::par_unseq, tests_results_.begin(), tests_results_.end(),
                        [this](TestResultType& result) { this->RunTestThread(result); });
                } else { // sequence
                    std::for_each(std::execution::seq, tests_results_.begin(), tests_results_.end(),
                        [this](TestResultType& result) { this->RunTestThread(result); });
                }
            }
            catch (std::exception& e) {
                WriteLnConsole("");
                WriteLnConsole("Error:");
                WriteLnConsole(e.what());
            }
            catch (...) {
                WriteLnConsole("");
                WriteLnConsole("Fatal Error");
            }
        }

        /// <summary>
        /// Проводится анализ результатов тестирования, результаты хранятся в тестере.
        /// </summary>
        /// <param name="tests_results_p"></param>
        void AnalyzeTestResults() {
            try {
                // Вычисление среднего значения результатов
                long double analyze = 0.0L;
                for (int i = 0; i < tests_count; i++) {
                    analyze += static_cast<long double>((tests_results_)[i]) / tests_count;
                }
                test_analitics_.tests_count = tests_count;
                test_analitics_.average = analyze;
            }
            catch (std::exception& e) {
                WriteLnConsole("");
                WriteLnConsole("Error:");
                WriteLnConsole(e.what());
            }
        }


        /// <summary>
        /// Показывает результаты только в консоле
        /// </summary>
        void ShowTestResultsInConsole() {
            using std::cout;
            using std::cin;

            WriteLnConsole("");
            WriteLnConsole("TestResults:");
            for (int i = 0; i < tests_count; i++) {
                WriteLnConsole(tests_results_);
            }
        }
        /// <summary>
        /// Выводит в консоль результаты анализа тестов
        /// </summary>
        void ShowAnaliticsInConsole() {
            using std::cout;
            using std::cin;

            cout << "\n";
            WriteLnConsole("Analitics");
            WriteLnConsole("Tests count:");
            WriteLnConsole(test_analitics_.tests_count);
            WriteLnConsole("Average run-time of code (nanoseconds):");
            WriteLnConsole(test_analitics_.average);
        }
        /// <summary>
        /// Запись результатов теста в файл
        /// </summary>
        void WriteResultsInFile();
        /// <summary>
        /// Запись анализа результатов теста в файл
        /// </summary>
        void WriteAnaliticsInFile();


        /// <summary>
        /// Последовательно делает все этапы тестирования
        /// </summary>
        void DoEveryTestingStage() {
            RunTest();
            AnalyzeTestResults();

            WriteResultsInFile();
            WriteAnaliticsInFile();

            ShowTestResultsInConsole();
            ShowAnaliticsInConsole();
        }
        /// <summary>
        /// Сокращает действия по тестированию до минимума
        /// </summary>
        void DoShortTestingStage() {
            RunTest();
            AnalyzeTestResults();
            ShowAnaliticsInConsole();
        }


        inline const unsigned long long GetTestsCount() const { return tests_count; };
        inline const std::vector<TestResultType>& GetTestsResults() const { return tests_results_; };
        inline const TestResultsAnalitics& GetTestAnalitics() const { return test_analitics_; };
        inline FuncForTest GetCodeForTest() const { return code_for_test_; };
        inline const std::string& GetTestName() const { return test_name_; };

    private:
        // Поток теста
        void RunTestThread(TestResultType& result) {
            using namespace std::chrono;
            typedef time_point<steady_clock, nanoseconds> TimePointNano;
            
            TimePointNano start_time_point = steady_clock::now();

            // Код, который нужно тестировать
            std::apply(code_for_test_, args_);

            TimePointNano end_time_point = steady_clock::now();

            result = (end_time_point - start_time_point).count();
        }


        std::string test_name_{};

        // Число тестов
        const unsigned long long tests_count;

        FuncForTest code_for_test_{ nullptr };

        std::tuple<ArgsT...> args_;

        // Массив результатов тестирования
        std::vector<TestResultType> tests_results_{};

        // Результаты анализа тестирования
        TestResultsAnalitics test_analitics_{};

        bool is_multithread_{ false };

        // Временной интервал измерения
        //test_time
    };
}

#endif