/* =====================================================================
   迷你洛谷 OJ (Mini OJ) + 内置评测机  —  PATH + data + 描述文件版
   ---------------------------------------------------------------------
   - 编译器：只使用系统 PATH 中的 g++
   - 测试点：从 data\题号\ 目录读取 1.in/1.out/2.in/2.out ...
   - 题目描述：从 data\题号\description.txt (或 desc.txt 等) 读取并显示
   - 提交代码：粘贴源码 -> Ctrl+Z 回车(EOF) -> 自动编译评测
   - 判定：AC / WA / TLE / MLE / RE / CE
   ---------------------------------------------------------------------
   编译:
     g++ mini_oj.cpp -o mini_oj.exe -std=c++11 -lpsapi -static
   Dev-C++:
     工具 -> 编译选项 -> 在"编译时加入以下命令"中填： -std=c++11 -lpsapi
   ===================================================================== */

#include <iostream>
#include <vector>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <conio.h>
#include <windows.h>
#include <psapi.h>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>

using namespace std;

// ===================== 数据结构 =====================
struct User {
    char name[30];
    char pwd[30];
    int ac_count;
    int total_score;
};

struct Problem {
    int id, difficulty;
    int timelimit, memlimit;      // ms / MB
    char title[60], tags[80];
    int solved_count;
};

struct Submission {
    int id, uid, pid, score, time_ms, mem_kb;
    char verdict[5];
    char timestamp[20];
};

// ===================== 全局状态 =====================
vector<User> users;
vector<Problem> problems;
vector<Submission> subs;
int currentUserIdx = -1;
int subIdCounter = 1;

// ===================== 控制台工具 =====================
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
void setColor(int color) { SetConsoleTextAttribute(hConsole, color); }
void clearInput() { cin.clear(); cin.ignore(1000, '\n'); }
void cls() { system("cls"); }
void pauseMenu() { setColor(8); cout << "\n[按任意键返回]"; getch(); setColor(15); cls(); }

const int C_DEF=15, C_GREEN=10, C_RED=12, C_BLUE=9, C_CYAN=11, C_YELLOW=14, C_GRAY=8;

// ===================== 数据持久化 =====================
void saveData() {
    ofstream fout("luogu_users.dat", ios::trunc);
    for (size_t i = 0; i < users.size(); i++) {
        fout << users[i].name << " " << users[i].pwd << " "
             << users[i].ac_count << " " << users[i].total_score << endl;
    }
    fout.close();

    fout.open("luogu_subs.dat", ios::trunc);
    for (size_t i = 0; i < subs.size(); i++) {
        fout << subs[i].id << " " << subs[i].uid << " " << subs[i].pid << " "
             << subs[i].score << " " << subs[i].time_ms << " " << subs[i].mem_kb << " "
             << subs[i].verdict << " " << subs[i].timestamp << endl;
    }
    fout.close();
}

void loadData() {
    ifstream fin("luogu_users.dat");
    if (fin.is_open()) {
        User u;
        while (fin >> u.name >> u.pwd >> u.ac_count >> u.total_score) users.push_back(u);
        fin.close();
    }
    fin.open("luogu_subs.dat");
    if (fin.is_open()) {
        Submission s;
        while (fin >> s.id >> s.uid >> s.pid >> s.score >> s.time_ms >> s.mem_kb
               >> s.verdict >> s.timestamp) subs.push_back(s);
        fin.close();
    }
    for (size_t i = 0; i < subs.size(); i++)
        if (subs[i].id >= subIdCounter) subIdCounter = subs[i].id + 1;
}

// ===================== 评测机: 公共辅助 =====================
enum JudgeResult { AC, WA, TLE, MLE, RE, CE };

string resultName(JudgeResult r) {
    switch (r) {
        case AC:  return "AC";
        case WA:  return "WA";
        case TLE: return "TLE";
        case MLE: return "MLE";
        case RE:  return "RE";
        case CE:  return "CE";
    }
    return "??";
}

string trimStr(const string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, last - first + 1);
}

string getExeDir() {
    char buf[MAX_PATH];
    GetModuleFileNameA(NULL, buf, MAX_PATH);
    string path = buf;
    size_t pos = path.find_last_of("\\/");
    if (pos == string::npos) return ".";
    return path.substr(0, pos);
}

// ===================== 评测机: 编译 (只用 PATH 中的 g++) =====================
JudgeResult compileUserCode(const string& sourceFile, const string& exeFile) {
    string compiler = "g++";
    string command = "\"" + compiler + "\" \"" + sourceFile + "\" -o \"" + exeFile
                   + "\" -O2 -std=c++11 -static-libgcc -static-libstdc++"
                   + " > compile_error.log 2>&1";
    DeleteFileA(exeFile.c_str());
    int ret = system(("\"" + command + "\"").c_str());
    return (ret == 0) ? AC : CE;
}

// ===================== 评测机: 单测试点 =====================
JudgeResult runTestCase(const string& exeFile, const string& inputFile,
                        const string& outputFile, int timeLimit,
                        int memoryLimit, int& usedTime, int& usedMem)
{
    usedTime = 0;
    usedMem  = 0;

    SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES), NULL, TRUE };

    HANDLE hInput = CreateFileA(inputFile.c_str(), GENERIC_READ, FILE_SHARE_READ,
                                &sa, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hInput == INVALID_HANDLE_VALUE) return RE;

    HANDLE hOutput = CreateFileA(outputFile.c_str(), GENERIC_WRITE, FILE_SHARE_READ,
                                 &sa, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hOutput == INVALID_HANDLE_VALUE) { CloseHandle(hInput); return RE; }

    STARTUPINFOA si;
    ZeroMemory(&si, sizeof(si));
    si.cb         = sizeof(si);
    si.dwFlags    = STARTF_USESTDHANDLES;
    si.hStdInput  = hInput;
    si.hStdOutput = hOutput;
    si.hStdError  = hOutput;

    PROCESS_INFORMATION pi;
    ZeroMemory(&pi, sizeof(pi));

    string cmdLine = "\"" + exeFile + "\"";
    vector<char> cmdBuf(cmdLine.begin(), cmdLine.end());
    cmdBuf.push_back('\0');

    BOOL ok = CreateProcessA(NULL, cmdBuf.data(), NULL, NULL, TRUE,
                             CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
    CloseHandle(hInput);
    CloseHandle(hOutput);
    if (!ok) return RE;

    DWORD startTime = GetTickCount();
    bool isTLE = false, isMLE = false;
    int  maxMem = 0;

    while (true) {
        DWORD code;
        GetExitCodeProcess(pi.hProcess, &code);
        if (code != STILL_ACTIVE) break;

        if (GetTickCount() - startTime > (DWORD)timeLimit) {
            isTLE = true;
            TerminateProcess(pi.hProcess, 1);
            break;
        }
        PROCESS_MEMORY_COUNTERS pmc;
        if (GetProcessMemoryInfo(pi.hProcess, &pmc, sizeof(pmc))) {
            if ((int)pmc.WorkingSetSize > maxMem) maxMem = (int)pmc.WorkingSetSize;
            if (pmc.WorkingSetSize > (SIZE_T)memoryLimit) {
                isMLE = true;
                TerminateProcess(pi.hProcess, 1);
                break;
            }
        }
        Sleep(10);
    }

    usedTime = (int)(GetTickCount() - startTime);
    usedMem  = maxMem / 1024;

    DWORD finalCode = 0;
    GetExitCodeProcess(pi.hProcess, &finalCode);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    if (isTLE) return TLE;
    if (isMLE) return MLE;
    if (finalCode != 0) return RE;
    return AC;
}

// ===================== 扫描 data\题号\ 下的测试点 =====================
vector<int> getTestCases(const string& problemDir) {
    vector<int> cases;
    string pattern = problemDir + "\\*.in";

    WIN32_FIND_DATAA fd;
    HANDLE hFind = FindFirstFileA(pattern.c_str(), &fd);
    if (hFind == INVALID_HANDLE_VALUE) return cases;

    do {
        if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) continue;
        string name = fd.cFileName;
        size_t dot = name.find('.');
        if (dot == string::npos) continue;
        string num = name.substr(0, dot);
        if (num.empty() || num.find_first_not_of("0123456789") != string::npos) continue;
        cases.push_back(atoi(num.c_str()));
    } while (FindNextFileA(hFind, &fd));

    FindClose(hFind);
    sort(cases.begin(), cases.end());
    return cases;
}

// ===================== 读取并显示文本文件（兼容 UTF-8 BOM / ANSI） =====================
void printTextFile(const string& path) {
    ifstream fin(path.c_str(), ios::binary);
    if (!fin) {
        setColor(C_GRAY);
        cout << "（无法打开描述文件）\n";
        setColor(C_DEF);
        return;
    }
    string content((istreambuf_iterator<char>(fin)), istreambuf_iterator<char>());
    fin.close();

    bool isUtf8 = false;
    if (content.size() >= 3 &&
        (unsigned char)content[0] == 0xEF &&
        (unsigned char)content[1] == 0xBB &&
        (unsigned char)content[2] == 0xBF) {
        isUtf8 = true;
        content = content.substr(3);
    }

    if (isUtf8) {
        int wlen = MultiByteToWideChar(CP_UTF8, 0, content.c_str(), (int)content.size(), NULL, 0);
        if (wlen > 0) {
            vector<wchar_t> wbuf(wlen + 1);
            MultiByteToWideChar(CP_UTF8, 0, content.c_str(), (int)content.size(), wbuf.data(), wlen);
            wbuf[wlen] = 0;
            int alen = WideCharToMultiByte(CP_ACP, 0, wbuf.data(), wlen, NULL, 0, NULL, NULL);
            if (alen > 0) {
                vector<char> abuf(alen + 1);
                WideCharToMultiByte(CP_ACP, 0, wbuf.data(), wlen, abuf.data(), alen, NULL, NULL);
                abuf[alen] = 0;
                cout << abuf.data();
                return;
            }
        }
    }
    cout << content;
}

// ===================== 在 data\题号\ 下找描述文件 =====================
string findDescriptionFile(const string& problemDir) {
    const char* names[] = {
        "description.txt",
        "desc.txt",
        "problem.txt",
        "题目描述.txt",
        "题面.txt"
    };
    for (int i = 0; i < 5; i++) {
        string p = problemDir + "\\" + names[i];
        if (GetFileAttributesA(p.c_str()) != INVALID_FILE_ATTRIBUTES) return p;
    }
    return "";
}

// ===================== 题库初始化 (只有元信息) =====================
void initProblems() {
    if (!problems.empty()) return;
    Problem p;

    memset(&p, 0, sizeof(p));
    p.id=1000; p.difficulty=1; p.timelimit=1000; p.memlimit=128;
    strcpy(p.title,"A+B Problem"); strcpy(p.tags,"入门 模拟");
    problems.push_back(p);

    memset(&p, 0, sizeof(p));
    p.id=1001; p.difficulty=1; p.timelimit=1000; p.memlimit=128;
    strcpy(p.title,"Hello World"); strcpy(p.tags,"入门 字符串");
    problems.push_back(p);

    memset(&p, 0, sizeof(p));
    p.id=1002; p.difficulty=2; p.timelimit=500; p.memlimit=64;
    strcpy(p.title,"求最大值"); strcpy(p.tags,"分支 排序");
    problems.push_back(p);

    memset(&p, 0, sizeof(p));
    p.id=1003; p.difficulty=1; p.timelimit=1000; p.memlimit=64;
    strcpy(p.title,"阶乘计算"); strcpy(p.tags,"入门 循环");
    problems.push_back(p);

    memset(&p, 0, sizeof(p));
    p.id=1004; p.difficulty=2; p.timelimit=1000; p.memlimit=64;
    strcpy(p.title,"回文判断"); strcpy(p.tags,"入门 字符串");
    problems.push_back(p);

    memset(&p, 0, sizeof(p));
    p.id=1005; p.difficulty=2; p.timelimit=1000; p.memlimit=128;
    strcpy(p.title,"质数计数"); strcpy(p.tags,"普及- 数论");
    problems.push_back(p);

    memset(&p, 0, sizeof(p));
    p.id=1006; p.difficulty=2; p.timelimit=1000; p.memlimit=128;
    strcpy(p.title,"数组排序"); strcpy(p.tags,"普及- 排序");
    problems.push_back(p);

    memset(&p, 0, sizeof(p));
    p.id=1007; p.difficulty=3; p.timelimit=500; p.memlimit=64;
    strcpy(p.title,"斐波那契"); strcpy(p.tags,"普及 递推");
    problems.push_back(p);

    memset(&p, 0, sizeof(p));
    p.id=1008; p.difficulty=3; p.timelimit=1000; p.memlimit=128;
    strcpy(p.title,"矩阵转置"); strcpy(p.tags,"普及/提高 模拟");
    problems.push_back(p);

    memset(&p, 0, sizeof(p));
    p.id=1009; p.difficulty=4; p.timelimit=500; p.memlimit=256;
    strcpy(p.title,"最大子段和"); strcpy(p.tags,"提高 动态规划");
    problems.push_back(p);

    memset(&p, 0, sizeof(p));
    p.id=1010; p.difficulty=4; p.timelimit=1000; p.memlimit=128;
    strcpy(p.title,"字符串匹配"); strcpy(p.tags,"提高 KMP");
    problems.push_back(p);

    memset(&p, 0, sizeof(p));
    p.id=1011; p.difficulty=5; p.timelimit=2000; p.memlimit=512;
    strcpy(p.title,"01背包问题"); strcpy(p.tags,"省选 动态规划");
    problems.push_back(p);
}

// ===================== 用户 =====================
void registerUser() {
    cls(); setColor(C_CYAN); cout << "=== 注册新账号 ===\n"; setColor(C_DEF);
    User newUser;
    cout << "用户名: "; cin >> newUser.name;
    for (size_t i = 0; i < users.size(); i++) {
        if (strcmp(newUser.name, users[i].name) == 0) {
            setColor(C_RED); cout << "该用户名已被注册！\n"; setColor(C_DEF);
            pauseMenu(); return;
        }
    }
    cout << "密  码: "; cin >> newUser.pwd;
    clearInput();
    newUser.ac_count = 0; newUser.total_score = 0;
    users.push_back(newUser);
    currentUserIdx = (int)users.size() - 1;
    setColor(C_GREEN); cout << "注册成功并已自动登录！\n"; setColor(C_DEF);
    saveData(); pauseMenu();
}

void login() {
    cls(); setColor(C_BLUE); cout << "=== 用户登录 ===\n"; setColor(C_DEF);
    char uName[30], uPwd[30];
    cout << "用户名: "; cin >> uName;
    cout << "密  码: "; cin >> uPwd;
    clearInput();
    currentUserIdx = -1;
    for (size_t i = 0; i < users.size(); i++) {
        if (strcmp(users[i].name, uName) == 0 && strcmp(users[i].pwd, uPwd) == 0) {
            currentUserIdx = (int)i; break;
        }
    }
    if (currentUserIdx != -1) {
        setColor(C_GREEN);
        cout << "登录成功！欢迎回来, " << users[currentUserIdx].name << "\n";
    } else {
        setColor(C_RED); cout << "账号或密码错误。\n";
    }
    setColor(C_DEF); pauseMenu();
}

// ===================== 题库 =====================
void showProblems() {
    cls(); setColor(C_YELLOW);
    cout << "================ 题 目 库 (共" << problems.size() << "题) ================\n";
    setColor(C_DEF);
    cout << " 编号 | 难度 | 题目名称          | 标签        | 通过人数\n";
    cout << "------|------|-------------------|-------------|--------\n";
    for (size_t i = 0; i < problems.size(); i++) {
        setColor(C_BLUE); cout << " P" << problems[i].id;
        setColor(C_YELLOW);
        for (int d = 0; d < problems[i].difficulty; d++) cout << "★";
        setColor(C_DEF); cout << "  ";
        setColor(C_GREEN); cout << problems[i].title;
        setColor(C_CYAN);
        int pad = 20 - (int)strlen(problems[i].title);
        for (int k = 0; k < pad; k++) cout << " ";
        setColor(C_GRAY); cout << problems[i].tags << "      ";
        setColor(C_DEF); cout << problems[i].solved_count << endl;
    }
    cout << "========================================================\n";
    pauseMenu();
}

// ===================== 提交代码 (描述页 + EOF 输入 + 自动评测) =====================
void submitProblem() {
    if (currentUserIdx == -1) {
        setColor(C_RED); cout << "请先登录！"; setColor(C_DEF); pauseMenu(); return;
    }
    cls(); setColor(C_CYAN);
    cout << "请输入题目编号 (例: 1000): "; setColor(C_DEF);
    int pid;
    if (!(cin >> pid)) {
        cin.clear(); clearInput();
        setColor(C_RED); cout << "输入无效\n"; setColor(C_DEF); pauseMenu(); return;
    }
    clearInput();

    int pIdx = -1;
    for (size_t i = 0; i < problems.size(); i++)
        if (problems[i].id == pid) { pIdx = (int)i; break; }
    if (pIdx == -1) {
        setColor(C_RED); cout << "题目不存在！"; setColor(C_DEF); pauseMenu(); return;
    }

    Problem &p = problems[pIdx];
    string exeDir     = getExeDir();
    string problemDir = exeDir + "\\data\\" + to_string(p.id);
    vector<int> cases = getTestCases(problemDir);
    if (cases.empty()) {
        setColor(C_RED);
        cout << "\n[错误] 在 " << problemDir << " 下没有找到任何 .in 文件。\n";
        cout << "请确认目录结构为 data\\" << p.id << "\\1.in, 1.out, 2.in, 2.out ...\n";
        setColor(C_DEF); pauseMenu(); return;
    }

    // ================= 题目描述页 =================
    cls();
    setColor(C_CYAN);
    cout << "============================================================\n";
    cout << "  P" << p.id << " - " << p.title << "\n";
    cout << "  难度: ";
    setColor(C_YELLOW);
    for (int d = 0; d < p.difficulty; d++) cout << "★";
    setColor(C_CYAN);
    cout << "    标签: " << p.tags << "\n";
    cout << "  限时: " << p.timelimit << "ms    限内存: " << p.memlimit << "MB"
         << "    测试点: " << cases.size() << " 个\n";
    cout << "============================================================\n";
    setColor(C_DEF);
    cout << "\n";

    string descPath = findDescriptionFile(problemDir);
    if (!descPath.empty()) {
        printTextFile(descPath);
    } else {
        setColor(C_GRAY);
        cout << "（暂无题目描述）\n";
        cout << "可在 " << problemDir << "\\description.txt 创建描述文件。\n";
        setColor(C_DEF);
    }

    setColor(C_GRAY);
    cout << "\n------------------------------------------------------------\n";
    cout << "[按任意键进入代码提交页面]";
    setColor(C_DEF);
    getch();

    // ================= 代码输入页 =================
    cls();
    setColor(C_BLUE);
    cout << "========== 提交代码 ==========\n";
    cout << "题目: P" << p.id << " - " << p.title << "\n";
    cout << "限时: " << p.timelimit << "ms    限内存: " << p.memlimit << "MB\n";
    cout << "测试点: " << cases.size() << " 个 (" << problemDir << ")\n";
    setColor(C_YELLOW);
    cout << "\n请粘贴你的 C++ 代码, 完成后按 Ctrl+Z 再按回车 (EOF) 提交:\n";
    setColor(C_GRAY); cout << "----------------------------------------\n";
    setColor(C_DEF);

    string code, line;
    while (getline(cin, line)) {
        code += line;
        code += "\n";
    }
    cin.clear();

    setColor(C_GRAY); cout << "----------------------------------------\n";
    cout << "共读取 " << code.size() << " 字节代码\n";
    setColor(C_DEF);

    if (code.empty()) {
        setColor(C_RED); cout << "代码为空, 提交已取消。\n"; setColor(C_DEF);
        pauseMenu(); return;
    }

    string codePath = exeDir + "\\code.cpp";
    string exePath  = exeDir + "\\code.exe";

    {
        ofstream fout(codePath.c_str(), ios::binary);
        if (!fout) {
            setColor(C_RED); cout << "无法写入 code.cpp, 请检查目录权限。\n";
            setColor(C_DEF); pauseMenu(); return;
        }
        fout << code;
    }

    setColor(C_CYAN); cout << "\n正在编译...\n"; setColor(C_DEF);
    JudgeResult comp = compileUserCode(codePath, exePath);

    time_t now = time(NULL);
    struct tm* tt = localtime(&now);
    char ts[20];
    sprintf(ts, "%02d:%02d", tt->tm_hour, tt->tm_min);

    if (comp == CE) {
        setColor(C_RED); cout << "\n编译错误 (CE):\n"; setColor(C_DEF);
        ifstream log("compile_error.log");
        string l; int cnt = 0;
        while (getline(log, l) && cnt < 30) { cout << l << "\n"; cnt++; }
        log.close();

        Submission s;
        s.id = subIdCounter++; s.uid = currentUserIdx; s.pid = pid;
        s.score = 0; s.time_ms = 0; s.mem_kb = 0;
        strcpy(s.verdict, "CE");
        strcpy(s.timestamp, ts);
        subs.push_back(s); saveData();

        setColor(C_CYAN);
        cout << "\n================ 评测结果 =================\n";
        setColor(C_RED);  cout << " 状态: CE (编译错误)\n";
        setColor(C_DEF);  cout << " 得分: 0/100\n";
        setColor(C_CYAN); cout << "==========================================\n";
        setColor(C_DEF); pauseMenu(); return;
    }

    cout << "编译成功!\n\n";

    int totalTime = 0, maxMemOverall = 0, acCount = 0;
    bool anyTLE = false, anyMLE = false, anyRE = false, anyWA = false;

    for (size_t ci = 0; ci < cases.size(); ci++) {
        int tc = cases[ci];
        string inFile  = problemDir + "\\" + to_string(tc) + ".in";
        string stdFile = problemDir + "\\" + to_string(tc) + ".out";
        string outFile = problemDir + "\\" + to_string(tc) + ".myout";

        int usedTime = 0, usedMem = 0;
        JudgeResult r = runTestCase(exePath, inFile, outFile,
                                    p.timelimit, p.memlimit * 1024 * 1024,
                                    usedTime, usedMem);

        totalTime += usedTime;
        if (usedMem > maxMemOverall) maxMemOverall = usedMem;

        string userOut;
        {
            ifstream uf(outFile.c_str(), ios::binary);
            stringstream ss; ss << uf.rdbuf();
            userOut = ss.str();
        }

        string stdOut;
        {
            ifstream sf(stdFile.c_str(), ios::binary);
            stringstream ss; ss << sf.rdbuf();
            stdOut = ss.str();
        }

        bool ok = false;
        if (r == AC) {
            ok = (trimStr(userOut) == trimStr(stdOut));
            if (!ok) r = WA;
        }

        cout << "  测试点 #" << tc << ": ";
        if (r == AC && ok) {
            setColor(C_GREEN); cout << "AC"; acCount++;
        } else {
            setColor(C_RED); cout << resultName(r);
            if (r == TLE) anyTLE = true;
            if (r == MLE) anyMLE = true;
            if (r == RE)  anyRE  = true;
            if (r == WA) {
                anyWA = true;
                cout << "\n    期望 : " << trimStr(stdOut);
                cout << "\n    你的 : " << trimStr(userOut);
            }
        }
        setColor(C_DEF);
        cout << "  (" << usedTime << "ms, " << usedMem << "KB)\n";

        DeleteFileA(outFile.c_str());
    }

    int score = (cases.empty()) ? 0 : (100 * acCount / (int)cases.size());

    const char* verdict;
    if (acCount == (int)cases.size()) verdict = "AC";
    else if (anyTLE) verdict = "TLE";
    else if (anyMLE) verdict = "MLE";
    else if (anyRE)  verdict = "RE";
    else             verdict = "WA";

    Submission s;
    s.id = subIdCounter++; s.uid = currentUserIdx; s.pid = pid;
    s.score = score; s.time_ms = totalTime; s.mem_kb = maxMemOverall;
    strcpy(s.verdict, verdict);
    strcpy(s.timestamp, ts);
    subs.push_back(s);

    if (acCount == (int)cases.size()) {
        problems[pIdx].solved_count++;
        users[currentUserIdx].ac_count++;
        users[currentUserIdx].total_score += 100;
    }
    saveData();

    cls(); setColor(C_CYAN);
    cout << "================ 评测结果 =================\n";
    setColor(C_DEF);
    cout << " 题目: P" << p.id << " - " << p.title << "\n";
    setColor((acCount == (int)cases.size()) ? C_GREEN : C_RED);
    cout << " 状态: " << verdict << "\n";
    setColor(C_YELLOW);
    cout << " 通过: " << acCount << "/" << cases.size() << "\n";
    cout << " 得分: " << score << "/100\n";
    setColor(C_DEF);
    cout << " 总用时: " << totalTime << "ms  峰值内存: " << maxMemOverall << "KB\n";
    setColor(C_CYAN);
    cout << "==========================================\n";
    setColor(C_DEF);
    pauseMenu();
}

// ===================== 提交记录 =====================
void showHistory() {
    if (currentUserIdx == -1) { setColor(C_RED); cout << "请先登录！"; setColor(C_DEF); pauseMenu(); return; }
    cls(); setColor(C_YELLOW);
    cout << "================ 我的提交记录 ==============\n";
    setColor(C_DEF);
    cout << " ID   | 题号 | 状态 | 得分 | 用时(ms) | 内存(KB) | 时间\n";
    cout << "------|------|------|------|----------|----------|--------\n";
    bool hasData = false;
    for (int i = (int)subs.size() - 1; i >= 0; i--) if (subs[i].uid == currentUserIdx) {
        hasData = true;
        setColor(C_BLUE); cout << " " << subs[i].id;
        setColor(C_DEF);  cout << " | P" << subs[i].pid << " ";
        int color = (strcmp(subs[i].verdict, "AC") == 0) ? C_GREEN : C_RED;
        setColor(color);  cout << subs[i].verdict << "  ";
        setColor(C_YELLOW); cout << subs[i].score << "   | ";
        setColor(C_DEF);
        cout << subs[i].time_ms << "       | " << subs[i].mem_kb << "      | "
             << subs[i].timestamp << endl;
    }
    if (!hasData) { setColor(C_GRAY); cout << "暂无提交记录。\n"; setColor(C_DEF); }
    cout << "============================================\n";
    pauseMenu();
}

// ===================== 排行榜 =====================
void showRanklist() {
    cls(); setColor(C_YELLOW);
    cout << "================ 排行榜 ==================\n";
    setColor(C_DEF);
    cout << " 排名 | 用户名        | AC题数 | 总得分\n";
    cout << "------|--------------|--------|-------\n";
    vector<int> idx;
    for (size_t i = 0; i < users.size(); i++) idx.push_back((int)i);
    for (size_t i = 0; i < idx.size(); i++)
        for (size_t j = i + 1; j < idx.size(); j++) {
            if (users[idx[j]].ac_count > users[idx[i]].ac_count ||
                (users[idx[j]].ac_count == users[idx[i]].ac_count &&
                 users[idx[j]].total_score > users[idx[i]].total_score)) {
                int t = idx[i]; idx[i] = idx[j]; idx[j] = t;
            }
        }
    for (size_t i = 0; i < idx.size(); i++) {
        User &u = users[idx[i]];
        setColor(C_BLUE); cout << " #" << (i + 1);
        setColor(C_GREEN); cout << " | " << u.name;
        int pad = 14 - (int)strlen(u.name);
        for (int k = 0; k < pad; k++) cout << " ";
        setColor(C_YELLOW);
        cout << " | " << u.ac_count << "     | " << u.total_score << "\n";
        setColor(C_DEF);
    }
    cout << "==========================================\n";
    pauseMenu();
}

// ===================== 主菜单 =====================
int main() {
    string exeDir = getExeDir();
    SetCurrentDirectoryA(exeDir.c_str());

    // ---- 检查 PATH 中是否有 g++ ----
    {
        FILE* fp = _popen("where g++ 2>nul", "r");
        char buf[512] = {0};
        bool found = false;
        if (fp) {
            if (fgets(buf, sizeof(buf), fp)) found = true;
            _pclose(fp);
        }
        if (!found) {
            cout << "\n[错误] 在 PATH 中找不到 g++。" << endl;
            cout << "请先安装 MinGW-w64 / MSYS2，并把其 bin 目录加入系统 PATH。" << endl;
            cout << "验证方法：打开 cmd，执行 where g++ 应能看到路径。" << endl;
            system("pause");
            return 1;
        }
    }

    system("title 迷你洛谷 OJ 控制台版 v3.0");
    initProblems();
    loadData();
    srand((unsigned)time(NULL));

    while (true) {
        cls();
        setColor(C_CYAN);
        cout << "╔══════════════════════════════════╗\n";
        cout << "║        迷你洛谷 (Mini OJ)        ║\n";
        cout << "╠══════════════════════════════════╣\n";
        if (currentUserIdx == -1) {
            setColor(C_YELLOW);
            cout << "║  [1] 注册账号                    ║\n";
            cout << "║  [2] 登录账号                    ║\n";
        } else {
            setColor(C_GREEN);
            cout << "║  [已登录: " << users[currentUserIdx].name << "]\n";
            setColor(C_YELLOW);
            cout << "║  [0] 退出登录                    ║\n";
        }
        setColor(C_YELLOW);
        cout << "║  [3] 查看题库                    ║\n";
        cout << "║  [4] 提交代码 (EOF 结束)         ║\n";
        cout << "║  [5] 提交记录                    ║\n";
        cout << "║  [6] 排行榜                      ║\n";
        cout << "║  [7] 退出系统                    ║\n";
        cout << "╚══════════════════════════════════╝\n";
        setColor(C_DEF);
        cout << "请选择: ";

        char ch = getch();
        switch (ch) {
            case '0':
                if (currentUserIdx != -1) {
                    currentUserIdx = -1;
                    setColor(C_GREEN); cout << "已退出登录。";
                    pauseMenu();
                }
                break;
            case '1':
                if (users.size() < 10) registerUser();
                else { setColor(C_RED); cout << " 演示版限制: 最多10个账号"; pauseMenu(); }
                break;
            case '2': login();         break;
            case '3': showProblems();  break;
            case '4': submitProblem(); break;
            case '5': showHistory();   break;
            case '6': showRanklist();  break;
            case '7':
                saveData(); cls();
                setColor(C_GREEN);
                cout << " 感谢使用！数据已保存。\n";
                setColor(C_DEF);
                return 0;
            default: break;
        }
    }
    return 0;
}
