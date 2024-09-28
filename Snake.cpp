//
// Created by cp on 2024/9/28.
//

#include "Snake.h"

//设置光标位置
void SetPos(short x,short y)
{
    COORD pos = { x, y };
    HANDLE hOutput = NULL;
    //获取标准输出的句柄(用来标识不同设备的数值)
    hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    //设置标准输出上光标的位置为pos
    SetConsoleCursorPosition(hOutput, pos);
}

void WelcomToGame()
{
    SetPos(40,13);
    wprintf(L"%ls",L"欢迎来到贪吃蛇小游戏");
    SetPos(40,25);
    system("pause");
    system("cls");
    SetPos(25,12);
    wprintf(L"%ls",L"用 ↑ . ↓ . ← . → 分别控制蛇的移动， F3为加速，F4为减速\n");
    SetPos(25,13);
    wprintf(L"%ls",L"加速将得到更高的分数\n");
    SetPos(40,25);
    system("pause");
    system("cls");
}

void CreatMap()
{
    //地图大小为56*26
    int i=0;
    SetPos(0,0);
    for(i=0;i<=56;i+=2)
    {
        wprintf(L"%lc",WALL);
    }
    SetPos(0,26);
    for(i=0;i<=56;i+=2)
    {
        wprintf(L"%lc",WALL);
    }
    for(i=1;i<26;i++)
    {
        SetPos(0,i);
        wprintf(L"%lc",WALL);
    }

    for(i=1;i<26;i++)
    {
        SetPos(56,i);
        wprintf(L"%lc",WALL);
    }
}

void InitSnake(pSnake ps)
{
    pSnakeNode cur=NULL;
    int i=0;
    for(i=0;i<5;i++)
    {
        cur=(pSnakeNode)malloc(sizeof(SnakeNode));
        if(cur==NULL)
        {
            perror("fail SnakeNode malloc!");
            return;
        }
        else
        {
            cur->next=NULL;
            cur->x=POS_X+i*2;
            cur->y=POS_Y;
            //头插
            if(ps->_pSnake==NULL)
            {
                ps->_pSnake=cur;
            }
            else
            {
                cur->next=ps->_pSnake;
                ps->_pSnake=cur;
            }
        }
    }
    //打印蛇的身体
    cur=ps->_pSnake;
    while(cur)
    {
        SetPos(cur->x,cur->y);
        wprintf(L"%lc",BODY);
        cur=cur->next;
    }
    //初始化蛇的相关系数
    ps->_SleepTime=400;
    ps->_Dir=RIGHT;
    ps->_Status=OK;
    ps->_Socre=0;
    ps->_FoodWeigh=10;
}

void CreatFood(pSnake ps)
{
    int x=0;
    int y=0;
    //x必须为2的倍数
    again:
    do{
        x=rand()%53+2;
        y=rand()%25+1;
    } while (x%2!=0);
    //食物坐标不能和蛇身冲突
    pSnakeNode cur=ps->_pSnake;
    while(cur)
    {
        if(cur->x==x && cur->y==y)
        {
            goto again;
        }
        cur=cur->next;
    }
    //创建食物
    pSnakeNode pFood= (pSnakeNode)malloc(sizeof(SnakeNode));
    if(pFood==NULL)
    {
        perror("fail pFood malloc!");
        return;
    }
    else
    {
        pFood->x=x;
        pFood->y=y;
        SetPos(pFood->x,pFood->y);
        wprintf(L"%lc",FOOD);
        ps->_pFood=pFood;
    }
}

void GameStart(pSnake ps)
{
    system("mode con cols=100 lines=30");//用mode设置窗口台大小为 100列，30行
    system("title Snake-Game");//设置cmd窗口名称
    //获取标准输出的句柄(用来标识不同设备的数值)
    HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);

    //影藏光标操作
    CONSOLE_CURSOR_INFO CursorInfo;
    GetConsoleCursorInfo(hOutput, &CursorInfo);//获取控制台光标信息
    CursorInfo.bVisible = false; //隐藏控制台光标
    SetConsoleCursorInfo(hOutput, &CursorInfo);//设置控制台光标状态

    //打印进入游戏界面
    WelcomToGame();
    //打印地图
    CreatMap();
    //初始化蛇
    InitSnake(ps);
    //初始化第一个食物
    CreatFood(ps);
}

void PrintHelpInfo()
{
    SetPos(64, 15);
    wprintf(L"%ls",L"不能穿墙，不能咬到自己\n");
    SetPos(64, 16);
    wprintf(L"%ls",L"用↑.↓.←.→分别控制蛇的移动.");
    SetPos(64, 17);
    wprintf(L"%ls",L"F1 为加速，F2 为减速\n");
    SetPos(64, 18);
    wprintf(L"%ls",L"ESC ：退出游戏.space：暂停游戏.");
}

void pause()
{
    while(1)
    {
        Sleep(300);
        if(KEY_PRESS(VK_ESCAPE))
        {
            break;
        }
    }
}

int NextIsFood(pSnakeNode pNext,pSnake ps)
{
    return ((pNext->x==ps->_pFood->x) && (pNext->y==ps->_pFood->y));
}

void EatFood(pSnakeNode pNext,pSnake ps)
{
    //头插
    pNext->next=ps->_pSnake;
    ps->_pSnake=pNext;

    pSnakeNode cur=ps->_pSnake;
    while(cur)
    {
        SetPos(cur->x,cur->y);
        wprintf(L"%lc",BODY);
        cur=cur->next;
    }
    ps->_Socre+=ps->_FoodWeigh;
    free(ps->_pFood);
    CreatFood(ps);
}

void NoFood(pSnakeNode pNext,pSnake ps)
{
    //头插
    pNext->next=ps->_pSnake;
    ps->_pSnake=pNext;

    pSnakeNode cur=ps->_pSnake;
    while(cur->next->next)
    {
        SetPos(cur->x,cur->y);
        wprintf(L"%lc",BODY);
        cur=cur->next;
    }
    SetPos(cur->next->x,cur->next->y);
    printf("  ");
    free(cur->next);
    cur->next=NULL;
}

int KillByWall(pSnake ps)
{
    if((ps->_pSnake->x==0)
       ||(ps->_pSnake->x==56)
       ||(ps->_pSnake->y==0)
       ||(ps->_pSnake->y==26))
    {
        ps->_Status=KILL_BY_WALL;
        return 1;
    }
    return 0;
}

int KillBySelf(pSnake ps)
{
    pSnakeNode cur=ps->_pSnake->next;
    while(cur)
    {
        if((ps->_pSnake->x==cur->x)
        &&(ps->_pSnake->y==cur->y))
        {
            ps->_Status=KILL_BY_SELF;
            return 1;
        }
        cur=cur->next;
    }
    return 0;
}

void SnakeMove(pSnake ps)
{
    pSnakeNode pNextNode=NULL;
    pNextNode=(pSnakeNode) malloc(sizeof(SnakeNode));
    if(pNextNode==NULL)
    {
        perror("fail pNextNode malloc!");
        return;
    }
    else
    {
        switch (ps->_Dir)
        {
            case UP:
            {
                pNextNode->x=ps->_pSnake->x;
                pNextNode->y=ps->_pSnake->y-1;
            }
            break;

            case DOWN:
            {
                pNextNode->x=ps->_pSnake->x;
                pNextNode->y=ps->_pSnake->y+1;
            }
            break;

            case LEFT:
            {
                pNextNode->x=ps->_pSnake->x-2;
                pNextNode->y=ps->_pSnake->y;
            }
            break;

            case RIGHT:
            {
                pNextNode->x=ps->_pSnake->x+2;
                pNextNode->y=ps->_pSnake->y;
            }
            break;
        }
        //如果下一个位置是食物
        if(NextIsFood(pNextNode,ps))
        {
            EatFood(pNextNode,ps);
        }
        else   //如果下一个位置不是食物
        {
            NoFood(pNextNode,ps);
        }
        KillByWall(ps);
        KillBySelf(ps);
    }
}

void GameRun(pSnake ps)
{
    PrintHelpInfo();
    do {
        SetPos(64, 10);
        wprintf(L"得分：%d   ", ps->_Socre);
        wprintf(L"每个食物得分：%d分", ps->_FoodWeigh);
        //上： VK_UP
        //• 下： VK_DOWN
        //• 左： VK_LEFT
        //• 右： VK_RIGHT
        //• 空格： VK_SPACE
        //• ESC： VK_ESCAPE
        //• F3： VK_F3
        //• F4： VK_F4
        if(KEY_PRESS(VK_UP) && ps->_Dir!=DOWN)
        {
            ps->_Dir=UP;
        }
        else if(KEY_PRESS(VK_DOWN) && ps->_Dir!=UP)
        {
            ps->_Dir=DOWN;
        }
        else if(KEY_PRESS(VK_LEFT) && ps->_Dir!=RIGHT)
        {
            ps->_Dir=LEFT;
        }
        else if(KEY_PRESS(VK_RIGHT) && ps->_Dir!=LEFT)
        {
            ps->_Dir=RIGHT;
        }
        else if(KEY_PRESS(VK_SPACE))
        {
            pause();
        }
        else if(KEY_PRESS(VK_ESCAPE))
        {
            ps->_Status=NORMAL_END;
            break;
        }
        else if(KEY_PRESS(VK_F1))//加速
        {
            if(ps->_SleepTime>=50)
            {
                ps->_SleepTime-=30;
                ps->_FoodWeigh+=2;
            }
        }
        else if(KEY_PRESS(VK_F2))//减速
        {
            if(ps->_FoodWeigh>0)
            {
                ps->_SleepTime+=30;
                ps->_FoodWeigh-=2;
            }
        }
        Sleep(ps->_SleepTime);
        SnakeMove(ps);
    }while(ps->_Status==OK);
}

void GameEnd(pSnake ps)
{
    pSnakeNode cur=ps->_pSnake;
    SetPos(24,12);
    switch (ps->_Status)
    {
        case NORMAL_END:
            wprintf(L"您主动退出游戏，游戏结束\n");
            break;
        case KILL_BY_SELF:
            wprintf(L"您撞到自己了，游戏结束\n");
            break;
        case KILL_BY_WALL:
            wprintf(L"您撞墙了，游戏结束\n");
            break;
    }
    while (cur)
    {
        pSnakeNode del=cur;
        cur=cur->next;
        free(del);
    }
}