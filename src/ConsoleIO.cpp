#include "ConsoleIO.h"
using namespace std;
using namespace real_yellow;

map<char,function<string(const UnlimitedInteger &,const UnlimitedInteger &)> > ConsoleIO::prepareOperators(){
    function<string(const UnlimitedInteger &,const UnlimitedInteger &)> add = 
    [](const UnlimitedInteger &u1, const UnlimitedInteger &u2) {
        return (u1 + u2).toString();
    };

    function<string(const UnlimitedInteger &,const UnlimitedInteger &)> minus = 
    [](const UnlimitedInteger &u1, const UnlimitedInteger &u2) {
        return (u1 - u2).toString();
    };

    function<string(const UnlimitedInteger &,const UnlimitedInteger &)> multiply = 
    [](const UnlimitedInteger &u1, const UnlimitedInteger &u2) {
        return (u1 * u2).toString();
    };

    function<string(const UnlimitedInteger &,const UnlimitedInteger &)> divide = 
    [](const UnlimitedInteger &u1, const UnlimitedInteger &u2) {
        return (u1 / u2).toString();
    };

    function<string(const UnlimitedInteger &,const UnlimitedInteger &)> power = [](const UnlimitedInteger &u1, const UnlimitedInteger &u2) {
        stringstream stream;
        unsigned index;
        stream << u2.toString();
        stream >> index;
        return UnlimitedInteger::power(u1, index).toString();
    };

    map<char, function<string(const UnlimitedInteger &,const UnlimitedInteger &)> > funcMap;
    funcMap['+'] = add;
    funcMap['-'] = minus;
    funcMap['*'] = multiply;
    funcMap['/'] = divide;
    funcMap['^'] = power;
    return funcMap;
}

string ConsoleIO::compute(string expression)
{
    UnlimitedInteger u1, u2;
    for (int i = 1; i < expression.size(); i++)
    {
        if (funcMap.find(expression[i]) != funcMap.end())
        {
            u1 = expression.substr(0, i);
            u2 = expression.substr(i + 1, expression.size());
            auto operate = funcMap[expression[i]];
            return operate(u1, u2);
        }
    }
}

void ConsoleIO::start()
{
    //存放表达式的文件
    fstream expressionFile;
    //存放计算结果的文件
    fstream resultFile;
    string expression;
    string result;
    string resultPath = "result.txt";
    try
    {
        resultFile.open(resultPath, ios::out | ios::in | ios::app);
    }
    catch (const std::exception &e)
    {
        cerr << "错误:无法访问result.txt" << endl;
    }
    while (true)
    {
        if (!resultFile)
        {
            cout << "警告:计算结果不能被保存" << endl;
        }
        cout << "大整数计算器--请选择模式" << endl;
        cout << "1.输入表达式" << endl;
        cout << "2.从文件导入表达式" << endl;
        cout << "ESC键退出" << endl;
        char input = getch();
        switch (input)
        {
        case '1':
            try
            {
                cout << "表达式示例:1145141919810+1926817 (不支持两个数字以上的运算)" << endl;
                cout << "若计算负数，请这样输入:-1145141919810+-1926817" << endl;
                cout << "请输入表达式:";
                cin >> expression;
                result = compute(expression);
                cout << "计算结果:" << result << endl;
                if (resultFile)
                {
                    resultFile << expression << "=" << result << endl;
                    cout << "计算结果已经保存到result.txt中" << endl;
                }
            }
            catch (const std::exception &e)
            {
                cerr << e.what() <<endl;
                cerr << "程序无法识别您的输入，请重新输入" << endl;
            }
            break;
        case '2':
            try
            {
                string path;
                cout << "请输入文件路径:";
                cin >> path;
                expressionFile.open(path, ios::in);
                while (getline(expressionFile, expression))
                {
                    result = compute(expression);
                    cout << "计算结果:" << result << endl;
                    if (resultFile)
                    {
                        resultFile << expression << "=" << result << endl;
                        cout << "计算结果已经保存到result.txt中" << endl;
                    }
                }
            }
            catch (const std::exception &e)
            {
                cerr << "程序无法识别文件内容，原因可能是这些" << endl;
                cerr << "1.用户指定的文件路径错误" << endl;
                cerr << "2.程序没有读取该文件的权限" << endl;
                cerr << "3.文件里的表达式无法被程序识别" << endl;
            }
            if (expressionFile)
            {
                expressionFile.close();
            }
            break;
        case 27:
            cout << "退出程序中" << endl;
            if (resultFile)
            {
                resultFile.close();
            }
            return;
        default:
            break;
        }
        std::system("pause");
        std::system("cls");
    }
}