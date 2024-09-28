//
// Created by cp on 2024/9/28.
//

#include "Snake.h"
void test()
{
    int ch=0;
    srand((unsigned int)time(NULL));
    do {
        Snake snake={0};
        GameStart(&snake);
        GameRun(&snake);
        GameEnd(&snake);
        SetPos(20, 15);
        wprintf(L"再来一局吗？(Y/N):");
        ch=getchar();
        getchar();

    } while (ch=='Y');
    SetPos(0,27);
}

int main()
{
    setlocale(LC_ALL,""); //修改当前地区为本地模式，为了支持中文宽字符的打印
    test();
    return 0;
}