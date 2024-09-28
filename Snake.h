//
// Created by cp on 2024/9/28.
//

#ifndef SNAKEGAME_SNAKE_H
#define SNAKEGAME_SNAKE_H

#endif //SNAKEGAME_SNAKE_H

#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <stdbool.h>
#include <locale.h>
#include <time.h>

#define WALL L'□'
#define BODY L'●'
#define FOOD L'★'
#define POS_X 24
#define POS_Y 5

#define KEY_PRESS(VK) ((GetAsyncKeyState(VK)&0x1)?1:0)   //0x1表示十六进制的1，键盘上获得的值按位与十六进制的1，计算出是否按下此键，按下为1，没按为0

enum DIRECTION
{
    UP=1,
    DOWN,
    RIGHT,
    LEFT
};

enum GAME_STATUS
{
    OK,             //正常
    KILL_BY_WALL,   //撞墙
    KILL_BY_SELF,   //撞上蛇身
    NORMAL_END      //正常结束
};

typedef struct SnakeNode
{
    int x;
    int y;
    struct SnakeNode*next;
}SnakeNode,*pSnakeNode;

typedef struct Snake
{
    pSnakeNode _pSnake;//整条蛇的指针
    pSnakeNode _pFood;//食物的指针
    enum DIRECTION _Dir;//枚举蛇的方向
    enum GAME_STATUS _Status;//枚举蛇的状态
    int _Socre;//当前获得的分数
    int _FoodWeigh;//食物的分数，默认10分
    int _SleepTime;//每一步的休眠时间
}Snake,*pSnake;

//游戏的初始化
void GameStart(pSnake ps);

//打印进入游戏界面
void WelcomToGame();

//设置光标位置
void SetPos(short x,short y);

//打印地图
void CreatMap();

//初始化蛇
void InitSnake(pSnake ps);

//初始化第一个食物
void CreatFood(pSnake ps);

void GameRun(pSnake ps);

//打印帮助信息
void PrintHelpInfo();

//蛇的移动
void SnakeMove(pSnake ps);

//如果下一个位置是食物
int NextIsFood(pSnakeNode pNext,pSnake ps);

//吃掉食物
void EatFood(pSnakeNode pNext,pSnake ps);

//下一个位置没有食物
void NoFood(pSnakeNode pNext,pSnake ps);

//撞墙
int KillByWall(pSnake ps);

//撞到自己
int KillBySelf(pSnake ps);

//游戏结束
void GameEnd(pSnake ps);