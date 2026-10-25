@echo off
setlocal
cd /d "%~dp0"

if not exist data (
    echo [ERROR] data folder not found.
    echo Please run make_data.bat first.
    pause
    exit /b 1
)

echo ============================================
echo   Generating problem descriptions
echo ============================================
echo.

REM -------- P1001 --------
(
echo 题目描述
echo 输出 Hello World! 字符串。
echo.
echo 输入格式
echo 无输入。
echo.
echo 输出格式
echo 输出一行 Hello World!
echo.
echo 输入样例
echo 无
echo.
echo 输出样例
echo Hello World!
) > "data\1001\description.txt"
echo [OK] data\1001\description.txt

REM -------- P1002 --------
(
echo 题目描述
echo 输入三个整数, 输出其中的最大值。
echo.
echo 输入格式
echo 一行三个整数 a, b, c, 用空格分隔, 每个数的绝对值不超过 1000。
echo.
echo 输出格式
echo 输出一个整数, 表示三个数中的最大值。
echo.
echo 输入样例
echo 3 5 1
echo.
echo 输出样例
echo 5
echo.
echo 数据范围
echo 每个数的绝对值不超过 1000
) > "data\1002\description.txt"
echo [OK] data\1002\description.txt

REM -------- P1003 --------
(
echo 题目描述
echo 给定正整数 n, 计算 1! + 2! + ... + n! 的值。
echo.
echo 输入格式
echo 一行一个正整数 n, 范围 1 到 10。
echo.
echo 输出格式
echo 输出一个整数, 表示阶乘和。
echo.
echo 输入样例
echo 3
echo.
echo 输出样例
echo 9
echo.
echo 样例解释
echo 1! + 2! + 3! = 1 + 2 + 6 = 9
) > "data\1003\description.txt"
echo [OK] data\1003\description.txt

REM -------- P1004 --------
(
echo 题目描述
echo 给定一个字符串, 判断它是否是回文串。
echo 回文串指正着读和反着读都一样的字符串。
echo.
echo 输入格式
echo 一行一个字符串, 长度不超过 100, 不含空格。
echo.
echo 输出格式
echo 如果是回文串输出 YES, 否则输出 NO。
echo.
echo 输入样例
echo 12321
echo.
echo 输出样例
echo YES
) > "data\1004\description.txt"
echo [OK] data\1004\description.txt

REM -------- P1005 --------
(
echo 题目描述
echo 给定正整数 n, 统计 1 到 n 之间质数的个数。
echo.
echo 输入格式
echo 一行一个正整数 n, 不超过 1000。
echo.
echo 输出格式
echo 输出一个整数, 表示质数的个数。
echo.
echo 输入样例
echo 10
echo.
echo 输出样例
echo 4
echo.
echo 样例解释
echo 10 以内的质数有 2, 3, 5, 7, 共 4 个。
) > "data\1005\description.txt"
echo [OK] data\1005\description.txt

REM -------- P1006 --------
(
echo 题目描述
echo 输入若干个整数, 将它们从小到大排序后输出。
echo.
echo 输入格式
echo 一行若干个整数, 用空格分隔, 表示待排序的序列, 长度不超过 100。
echo.
echo 输出格式
echo 输出排序后的整数, 用空格分隔。
echo.
echo 输入样例
echo 3 1 2
echo.
echo 输出样例
echo 1 2 3
) > "data\1006\description.txt"
echo [OK] data\1006\description.txt

REM -------- P1007 --------
(
echo 题目描述
echo 斐波那契数列定义为 F（1）= 1, F（2）= 1,
echo 当 n 大于 2 时 F（n）= F（n-1）+ F（n-2）。求第 n 项。
echo.
echo 输入格式
echo 一行一个正整数 n, 范围 1 到 30。
echo.
echo 输出格式
echo 输出一个整数, 表示 F（n）。
echo.
echo 输入样例
echo 5
echo.
echo 输出样例
echo 5
echo.
echo 样例解释
echo 数列前 7 项: 1, 1, 2, 3, 5, 8, 13
) > "data\1007\description.txt"
echo [OK] data\1007\description.txt

REM -------- P1008 --------
(
echo 题目描述
echo 给定一个矩阵, 输出它的转置矩阵。
echo 输入是一行若干整数, 按行优先顺序表示矩阵的所有元素。
echo.
echo 输入格式
echo 一行若干个整数, 用空格分隔。
echo 测试数据中矩阵为 2 行 2 列, 或 2 行 3 列。
echo.
echo 输出格式
echo 输出转置后的矩阵, 按行优先顺序排列, 用空格分隔。
echo.
echo 输入样例
echo 1 2 3 4
echo.
echo 输出样例
echo 1 3 2 4
echo.
echo 样例解释
echo 原矩阵 2 行 2 列: 第一行 1 2, 第二行 3 4。
echo 转置后: 第一行 1 3, 第二行 2 4。
echo 按行优先输出为 1 3 2 4。
) > "data\1008\description.txt"
echo [OK] data\1008\description.txt

REM -------- P1009 --------
(
echo 题目描述
echo 给定一个整数序列, 求它的最大子段和。
echo 子段是序列中连续的一段, 至少包含一个元素。
echo.
echo 输入格式
echo 一行若干个整数, 用空格分隔, 表示整数序列。
echo 长度不超过 1000, 每个元素的绝对值不超过 10000。
echo.
echo 输出格式
echo 输出一个整数, 表示最大子段和。
echo.
echo 输入样例
echo 3 -2 4 -1 2
echo.
echo 输出样例
echo 5
echo.
echo 样例解释
echo 序列为 3, -2, 4, -1, 2。
echo 最大子段为 4 + （-1）+ 2 = 5。
) > "data\1009\description.txt"
echo [OK] data\1009\description.txt

REM -------- P1010 --------
(
echo 题目描述
echo 给定一行字符串, 统计该字符串中最后一个单词出现的次数。
echo.
echo 输入格式
echo 一行字符串, 由若干个单词组成, 单词之间用一个空格分隔,
echo 总长度不超过 100。
echo.
echo 输出格式
echo 输出一个整数, 表示最后一个单词在整行中出现的次数。
echo.
echo 输入样例
echo hello world world
echo.
echo 输出样例
echo 2
echo.
echo 样例解释
echo 最后一个单词是 world, 在整行中出现了 2 次。
) > "data\1010\description.txt"
echo [OK] data\1010\description.txt

REM -------- P1011 --------
(
echo 题目描述
echo 有 N 件物品和一个容量为 V 的背包。
echo 第 i 件物品的重量是 wi, 价值是 vi。每件物品最多选一次,
echo 求在总重量不超过 V 的前提下能获得的最大总价值。
echo.
echo 输入格式
echo 一行共 2N+2 个整数, 依次为:
echo 背包容量 V, 物品数 N, 然后 N 组 重量和值。
echo.
echo 输出格式
echo 输出一个整数, 表示最大总价值。
echo.
echo 输入样例
echo 10 3 5 10 6 8 4 5
echo.
echo 输出样例
echo 15
echo.
echo 样例解释
echo 容量 10, 3 件物品:
echo 物品1 重量5 价值10
echo 物品2 重量6 价值8
echo 物品3 重量4 价值5
echo 最优选物品1和物品3, 总重量9, 总价值15。
) > "data\1011\description.txt"
echo [OK] data\1011\description.txt

echo.
echo ============================================
echo   All descriptions generated.
echo ============================================
echo.
echo Check list:
dir /b data\1001\description.txt
dir /b data\1002\description.txt
dir /b data\1003\description.txt
dir /b data\1004\description.txt
dir /b data\1005\description.txt
dir /b data\1006\description.txt
dir /b data\1007\description.txt
dir /b data\1008\description.txt
dir /b data\1009\description.txt
dir /b data\1010\description.txt
dir /b data\1011\description.txt
echo.
echo Done.
pause