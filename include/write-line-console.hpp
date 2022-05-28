#ifndef WRITELINECONSOLE_H
#define WRITELINECONSOLE_H

#include <iostream>

/// <summary>
/// Пишет на новой строке информацию и переводит снова на новую строку
/// </summary>
template<typename T> inline void WriteLnConsole(T value_p) { std::cout << value_p << '\n'; }

#endif // !WRITELINECONSOLE_H
