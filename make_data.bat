@echo off
setlocal
cd /d "%~dp0"

echo ============================================
echo   MiniOJ Data Generator
echo ============================================
echo.

if exist data (
    echo [WARN] "data" folder already exists.
    choice /c YN /m "Overwrite it?"
    if errorlevel 2 (
        echo Cancelled.
        pause
        exit /b
    )
    rmdir /s /q data
)

echo [1/12] P1000 A+B ...
mkdir data\1000 2>nul
(echo 1 2)> data\1000\1.in
(echo 3)> data\1000\1.out
(echo -5 10)> data\1000\2.in
(echo 5)> data\1000\2.out

echo [2/12] P1001 Hello World ...
mkdir data\1001 2>nul
type nul> data\1001\1.in
(echo Hello World!)> data\1001\1.out

echo [3/12] P1002 Max ...
mkdir data\1002 2>nul
(echo 3 5 1)> data\1002\1.in
(echo 5)> data\1002\1.out
(echo -2 -8 -1)> data\1002\2.in
(echo -1)> data\1002\2.out

echo [4/12] P1003 Factorial Sum ...
mkdir data\1003 2>nul
(echo 3)> data\1003\1.in
(echo 9)> data\1003\1.out
(echo 4)> data\1003\2.in
(echo 33)> data\1003\2.out

echo [5/12] P1004 Palindrome ...
mkdir data\1004 2>nul
(echo 12321)> data\1004\1.in
(echo YES)> data\1004\1.out
(echo abcba)> data\1004\2.in
(echo YES)> data\1004\2.out

echo [6/12] P1005 Prime Count ...
mkdir data\1005 2>nul
(echo 10)> data\1005\1.in
(echo 4)> data\1005\1.out
(echo 20)> data\1005\2.in
(echo 8)> data\1005\2.out

echo [7/12] P1006 Sort ...
mkdir data\1006 2>nul
(echo 3 1 2)> data\1006\1.in
(echo 1 2 3)> data\1006\1.out
(echo 5 4 3 2 1)> data\1006\2.in
(echo 1 2 3 4 5)> data\1006\2.out

echo [8/12] P1007 Fibonacci ...
mkdir data\1007 2>nul
(echo 5)> data\1007\1.in
(echo 5)> data\1007\1.out
(echo 7)> data\1007\2.in
(echo 13)> data\1007\2.out

echo [9/12] P1008 Transpose ...
mkdir data\1008 2>nul
(echo 1 2 3 4)> data\1008\1.in
(echo 1 3 2 4)> data\1008\1.out
(echo 1 2 3 4 5 6)> data\1008\2.in
(echo 1 4 2 5 3 6)> data\1008\2.out

echo [10/12] P1009 Max Subarray ...
mkdir data\1009 2>nul
(echo 3 -2 4 -1 2)> data\1009\1.in
(echo 5)> data\1009\1.out
(echo -1 -2 -3)> data\1009\2.in
(echo -1)> data\1009\2.out

echo [11/12] P1010 String Match ...
mkdir data\1010 2>nul
(echo hello world world)> data\1010\1.in
(echo 2)> data\1010\1.out
(echo abc abc abc)> data\1010\2.in
(echo 3)> data\1010\2.out

echo [12/12] P1011 0-1 Knapsack ...
mkdir data\1011 2>nul
(echo 10 3 5 10 6 8 4 5)> data\1011\1.in
(echo 15)> data\1011\1.out
(echo 5 2 2 3 3 4)> data\1011\2.in
(echo 7)> data\1011\2.out

echo.
echo ============================================
echo   All data generated!
echo ============================================
echo.
echo Folders under data\ :
dir /b /ad data
echo.
echo Test file check:
echo   data\1000\1.in  = 
type data\1000\1.in
echo   data\1000\1.out = 
type data\1000\1.out
echo.
echo Now you can run mini_oj.exe and submit code.
pause