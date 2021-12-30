#ifndef TESTERSPEED_H
#define TESTERSPEED_H

#include <vector>
#include <iostream>
#include <algorithm>
#include <string>
#include <chrono>
#include <ctime>

#include "TestSpeed.h"
#include "WriteLineConsole.h"
#include "WriteSerializedDataFile.h"
#include "DateTime.h"


namespace test {
    typedef long double ComparationResultsT;

    // Calculation error in nanoseconds: +- delta value
    constexpr ComparationResultsT kCalcError{ 4000 };

    constexpr char kProjectDir[]{ "./" };
    constexpr char kLogsDir[]{ "./logs/" };
    constexpr char kLogsExtension[]{ ".speedlog" };

    constexpr char kAverageTitle[]{ "Average results:" };
    constexpr char kWinnerListTitle[]{ "Winner List(From Best to Worst):" };
    constexpr char kComparationTitle[]{ "Comparation of Tests Analitic Results(Winner1 - Winner2...; less by Winner1 < Winner2 %):" };
    constexpr char kTotalTimeElapsedTitle[]{ "Total time elapsed:\t" };
    constexpr char kNano[]{ "nanoseconds" };
    constexpr char kMili[]{ "milliseconds" };

    inline double PartPercent(const double part_of_value, const double value) {
        return (part_of_value / value * 100.0);
    }

    /// <summary>
    /// Класс-тестировщик скорости работы функции. Какой код из приведенных сниппетов работает быстрее?
    /// </summary>
    template<typename ReturnT, typename... ArgsT>
    class TesterSpeed {
    public:
        typedef TestSpeed<ReturnT, ArgsT...> TestType;

        struct TestResultsInfo {
            TestResultsInfo() = default;
            TestResultsInfo(const TestType* const test_ptr_p, const std::string& name_p, const ComparationResultsT& result_p)
                : test_ptr{ test_ptr_p },
                    name{ name_p },
                    result{ result_p }
            {};

            const TestType* test_ptr{ nullptr };
            std::string name{};
            ComparationResultsT result{};

            inline bool operator==(const TestResultsInfo& rhs) const {
                return rhs.result == result ? true : false;
            }
            inline bool operator<(const TestResultsInfo& rhs) const{
                return result < rhs.result ? true : false;
            }
            inline bool operator>(const TestResultsInfo& rhs) const {
                return result > rhs.result ? true : false;
            }
            inline bool operator>=(const TestResultsInfo& rhs) const {
                return result >= rhs.result ? true : false;
            }
            inline bool operator<=(const TestResultsInfo& rhs) const {
                return result <= rhs.result ? true : false;
            }
        };

        struct TestComparationInfo : public TestResultsInfo {
            TestComparationInfo() = default;
            TestComparationInfo(const TestType* const test_ptr_p, const std::string& name_p, const ComparationResultsT& result_p,
                                const ComparationResultsT& less_p) 
                : TestResultsInfo{ test_ptr_p, name_p, result_p },
                    less_percent { less_p }
            {};

            ComparationResultsT less_percent{};

            inline void LessCalc(ComparationResultsT best) {
                less_percent = PartPercent(TestResultsInfo::result, best);
            }
        };


        TesterSpeed() = default;

        /// <summary>
        /// Добавляет тест в вектор всех тестов
        /// </summary>
        void AddTest(ReturnT(*const codeForTesting_p)(ArgsT...), const ArgsT... args, 
                     const std::string& fn_name = "",
                     const TestsCountT tests_count_p = tests_count_default,
                     const bool is_multithread_p = true) {
            all_tests_.emplace_back(codeForTesting_p, args..., fn_name, tests_count_p, is_multithread_p);
        }

        /// <summary>
        /// Запуск всех тестов, добавленных в Тестировщик
        /// </summary>
        void RunAllTests() {
            try {
                int imax = all_tests_.size();
                if (imax > 0) {
                    auto start{ std::chrono::steady_clock::now() };
                    for (int i = 0; i < imax; ++i) {
                        std::cout << "Test " << all_tests_[i].GetTestName() << " starts\n";
                        all_tests_[i].RunTest();
                        all_tests_[i].AnalyzeTestResults();
                    }
                    auto end{ std::chrono::steady_clock::now() };
                    total_time_elapsed_ = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
                } else { std::cout << "There is no tests to test.\n"; }
            }
            catch (std::exception e) {
                WriteLnConsole("");
                WriteLnConsole("Error:");
                WriteLnConsole(e.what());
            }
            catch (...) {
                WriteLnConsole("");
                WriteLnConsole("Fatal Error");
            }
        }

        void CreateBestList() {
            for (const TestType& test : all_tests_) {
                best_list_.emplace_back(&test, test.GetTestName(), test.GetTestAnalitics().average);
            }
            std::sort(best_list_.begin(), best_list_.end());
        }

        /// <summary>
        /// Сравнивает результаты проведенных тестов
        /// </summary>
        void CompareTestsAnaliticResults() {
            try {
                if (best_list_.empty()) { CreateBestList(); }
                for (int i = 1, imax = best_list_.size(); i < imax; ++i) {
                    TestComparationInfo t = TestComparationInfo();
                    copmaration_tests_results_.emplace_back(best_list_[i].test_ptr, best_list_[i].name,
                                                            best_list_[i].result - best_list_[0].result, 0.0);
                    copmaration_tests_results_[i - 1].LessCalc(best_list_[0].result);
                }
                SaveResultsToLog();
            }
            catch (std::exception e) {
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
        /// Показывает результаты сравнения тестов в консоле
        /// </summary>
        void ShowComparationTestsAnaliticResults() {
            if (all_tests_.size() > 0) {
                using std::cout;
                using std::cin;

                cout << '\n';
                for (const TestType& test : all_tests_) {
                    cout << "Test " << test.GetTestName() << " count: " << test.GetTestsCount() << '\n';
                }
                cout << '\n';

                WriteLnConsole(kAverageTitle);
                for (int i = 0, imax = all_tests_.size(); i < imax; ++i) {
                    cout << i + 1 << ") " << all_tests_[i].GetTestName() << ' ' << all_tests_[i].GetTestAnalitics().average;
                    cout << " nanoseconds\n";
                }
                cout << '\n';

                WriteLnConsole(kWinnerListTitle);
                for (int i = 0, imax = best_list_.size(); i < imax; ++i) {
                    cout << i + 1 << ") " << best_list_[i].name << ' ' << best_list_[i].result;
                    cout << " nanoseconds\n";
                }
                cout << '\n';

                WriteLnConsole(kComparationTitle);
                for (const TestComparationInfo& info : copmaration_tests_results_) {
                    cout << info.name << ' ' << info.result;
                    cout << " nanoseconds; less by < " << info.less_percent << "%\n";
                }
                cout << '\n';

                cout << kTotalTimeElapsedTitle << total_time_elapsed_ << " " << kMili << "\n\n";

                cout << "Calculation Error: +-" << kCalcError << " nanoseconds\n";
            }
        }

        void SaveResultsToLog();

        const std::vector<TestType>& GetAllTests() const { return all_tests_; };
        inline const std::string& GetLogFileName() const { return log_file_name_; };

        inline void SetLogFileName(const std::string& file_name) {  log_file_name_ = file_name; };

    private:
        /// <summary>
        /// Все тесты, которые провел класс Тестировщик
        /// </summary>
        std::vector<TestType> all_tests_{};
        // List of best algorithms & functions
        std::vector<TestResultsInfo> best_list_{};
        // Difference between average best & average others results
        std::vector<TestComparationInfo> copmaration_tests_results_{};

        // Time of elapsing of all tests
        long long total_time_elapsed_{};

        std::string log_file_name_{};
    };

    template<typename ReturnT, typename ...ArgsT>
    inline void TesterSpeed<ReturnT, ArgsT...>::SaveResultsToLog() {
        if (!log_file_name_.empty()) {
            std::string file_path{kLogsDir + log_file_name_ + kLogsExtension};
            std::string log{};
            log.append("\n\n");
            log.append(date_time::CurrentDateTimeLocal());  log.append("\n");
            if (all_tests_.size() > 0) {
                log.append("Tests count: ");
                log.append(std::to_string(all_tests_[0].GetTestsCount()));  log.append("\n\n");
            }
            log.append(kWinnerListTitle);    log.append("\n");
            for (const TestResultsInfo& best : best_list_) {
                log.append(best.name + '\t' + std::to_string(best.result) + " nanoseconds\n");
            }
            log.append("\n");   log.append(kComparationTitle);  log.append("\n");
            for (const TestComparationInfo& compare_result : copmaration_tests_results_) {
                log.append(compare_result.name + '\t' + std::to_string(compare_result.result) + " nanoseconds; less by < ");
                log.append(std::to_string(compare_result.less_percent) + "%\n");
            }
            log.append("\n");   log.append(kTotalTimeElapsedTitle); log.append(std::to_string(total_time_elapsed_)); log.append(" ");
            log.append(kMili);
            log.append("\n");
            file::WriteSerializedDataFile(file_path, &log[0], log.size());
        }
    }

    template<typename ReturnT, typename... ArgsT>
    inline void FullTesting(TesterSpeed<ReturnT, ArgsT...>& tester) {
        tester.RunAllTests();
        tester.CompareTestsAnaliticResults();
        tester.ShowComparationTestsAnaliticResults();
    }

}

#endif