#ifndef WRITELINECONSOLE_H
#define WRITELINECONSOLE_H

#include <iostream>

/// <summary>
/// ����� �� ����� ������ ���������� � ��������� ����� �� ����� ������
/// </summary>
template<typename T> inline void WriteLnConsole(T value_p) { std::cout << value_p << '\n'; }

#endif // !WRITELINECONSOLE_H
