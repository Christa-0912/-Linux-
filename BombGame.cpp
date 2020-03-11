#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <time.h>
#include <termios.h>

#define DDL 10  //游戏计时时间

int map[15][17] = {0};

typedef struct player
{
        int head;
        int body;
        int score;
        int x;
        int y;
}Player;

//*********************************************************

//获取用户输入并不回显
char getch()
{
        char c;

        system("stty -echo");
        system("stty -icanon");

        c = getchar();

        system("stty -icanon");
        system("stty -echo");

        return c;
}

//*********************************************************

//检测键盘的输入状态
int kbhit(void)
{
        struct termios oldt, newt;
        int ch;
        int oldf;
        tcgetattr(STDIN_FILENO, &oldt);
        newt=oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        oldf=fcntl(STDIN_FILENO, F_GETFL, 0);
        fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
        ch=getchar();
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        fcntl(STDIN_FILENO, F_SETFL, oldf);
        if(ch != EOF)
        {
                ungetc(ch, stdin);
                return 1;
        }
        return 0;
}

//*********************************************************

void ReadMap(int level)
{
        int i=0,j=0;

        FILE *fp;

        if(level == 1)
        {
                fp = fopen("map.txt","r");
        }
        else if(level == 2)
        {
                fp = fopen("map2.txt","r");
        }

        //打开文件
        if(!fp)
        {
                printf("地图读取失败\n");
        }

        //将地图数据读到数组map
        for(i=0; i<15; i++)
        {
                for(j=0; j<17; j++)
                {
                        fscanf(fp,"%d",&map[i][j]);
                }
        }

        //关闭文件
        close(fp);
}

//*********************************************************

//地图刷新，做成线程，一直刷新
void* MapChange()
{
        //获取时间
        time_t tPast = -1;
        time_t tNow = -1;

        time(&tPast);  //获取线程开始时的系统时间
        time(&tNow);

        int i=0,j=0,k=0;

        //不停地刷新，直到DDL
        while((tNow-tPast) <= DDL)
        {
                //获取当前系统时间
                time(&tNow);

                //计算时间
                k = DDL+1 - tNow + tPast;

                //清屏
                system("clear");

                printf("\n\n");

                //图形化
                for(i=0; i<15; i++)
                {
                        printf("                                  ");

                        for(j=0; j<17; j++)
                        {
                                switch(map[i][j])
                                {
                                        case 0: printf("   ");break;

                                        case 1: printf(" \033[42m○\033[0m ");break;  //Player1_Body
                                        case 11:printf(" \033[32m●\033[0m ");break;  //Player1_Head

                                        case 2: printf(" \033[44m○\033[0m ");break;  //Player2_Body
                                        case 12:printf(" \033[34m●\033[0m ");break;  //Player2_Head

                                        case 3: printf(" \033[41m \033[0m ");break;  //BombEffect
                                        case 5: printf(" \033[31m★\033[0m ");break;  //Bomb

                                        case 4: printf(" \033[33m□\033[0m ");break;  //Wall can be broken
                                        case 9: printf("\033[47m   \033[0m");break;  //Wall can not be broken
                                }
                        }

                        if(i==0)
						{
                                printf("\033[34m  -------------------\033[0m");
                        }
                        else if(i==1)
                        {
                                printf("\033[34m  |                 |\033[0m");
                        }
                        else if(i==2)
                        {
                                printf("\033[34m  |     倒计时      |\033[0m");
                        }
                        else if(i==3)
                        {

                                if(k > 9)
                                {
                                        printf("\033[34m  |       %d        |\033[0m", k);
                                }
                                else
                                {
                                        printf("\033[34m  |        %d        |\033[0m", k);
                                }
                        }
                        else if(i==4)
                        {
                                printf("\033[34m  -------------------\033[0m");
                        }
                        else if(i==5)
                        {
                                printf("\033[34m  |                 |\033[0m");
                        }
                        else if(i==6)
						{
                                printf("\033[34m  | player1移动:    |\033[0m");
                        }
                        else if(i==7)
                        {
                                printf("\033[34m  |        w        |\033[0m");
                        }
                        else if(i==8)
                        {
                                printf("\033[34m  |      a s d      |\033[0m");
                        }
                        else if(i==9)
                        {
                                printf("\033[34m  | player2移动:    |\033[0m");
                        }
                        else if(i==10)
                        {
                                printf("\033[34m  |        i        |\033[0m");
                        }
                        else if(i==11)
                        {
                                printf("\033[34m  |      j k l      |\033[0m");
                        }
                        else if(i==12)
                        {
                                printf("\033[34m  |  在规定时间内， |\033[0m");
                        }
                        else if(i==13)
						{
                                printf("\033[34m  |占方格数目多者胜 |\033[0m");
                        }
                        else if(i==14)
                        {
                                printf("\033[34m  -------------------\033[0m");
                        }

                        printf("\n");
                }
        }
}

//*********************************************************

//拆毁墙壁
void BreakWall(int x, int y)
{
        //上
        if(map[x-1][y] == 4)
        {
                map[x-1][y] = 0;
        }

        //下
        if(map[x+1][y] == 4)
        {
                map[x+1][y] = 0;
        }

        //左
        if(map[x][y-1] == 4)
        {
                map[x][y-1] = 0;
        }

        //右
        if(map[x][y+1] == 4)
        {
                map[x][y+1] = 0;
        }
}

//*********************************************************

//炸弹炸墙
void Bomb(int x, int y)
{
        //放置炸弹
        map[x][y] = 5;

        //炸弹特效

        //拆墙
        BreakWall(x, y);
}

//*********************************************************

//分数统计，返回得分
int Score(int id)
{
        int score = 0;

        int i=0, j=0;

        for(i=0; i<15; i++)
        {
                for(j=0; j<17; j++)
                {
                        if(map[i][j] == id)
                        {
                                score++;
                        }
                }
        }

        return score;
}

//*********************************************************

void Result(int a, int b)
{
        if(a>b)
        {
                P1_Win();
        }
        else
        {
                P2_Win();
        }
}

//*********************************************************

//主菜单

void GameMenu()
{
        int i;
        while(!kbhit())
        {
                for(i=0; i<=7; i++)
                {
                        system("clear");
                        printf("\n\n");
						printf("                                                 \033[35m-------------------------------------------------------------\033[0m\n");
                        printf("                                                 \033[35m|                                                           |\033[0m\n");
                        printf("                                                 \033[35m|\033[3%dm * * *           * * *       * *           * *   * * *     \033[35m|\033[0m\n",i);
                        printf("                                                 \033[35m|\033[3%dm *     *       *       *     *   *       *   *   *     *   \033[35m|\033[0m\n",i);
                        printf("                                                 \033[35m|\033[3%dm *      *    *          *    *     *   *     *   *      *  \033[35m|\033[0m\n",i);
                        printf("                                                 \033[35m|\033[3%dm *     *    *            *   *       *       *   *     *   \033[35m|\033[0m\n",i);
                        printf("                                                 \033[35m|\033[3%dm * * *      *            *   *       *       *   * * *     \033[35m|\033[0m\n",i);
                        printf("                                                 \033[35m|\033[3%dm *     *    *            *   *       *       *   *     *   \033[35m|\033[0m\n",i);
                        printf("                                                 \033[35m|\033[3%dm *      *    *          *    *               *   *      *  \033[35m|\033[0m\n",i);
                        printf("                                                 \033[35m|\033[3%dm *     *      *        *     *               *   *     *   \033[35m|\033[0m\n",i);
                        printf("                                                 \033[35m|\033[3%dm * * *           * * *       *               *   * * *     \033[35m|\033[0m\n",i);
                        printf("                                                 \033[35m|                                                           |\033[0m\n");
						printf("                                                 \033[35m|                                                           |\033[0m\n");
                        printf("                                                 \033[35m|\033[33m                    游戏名：占地为王                       \033[35m|\033[0m\n");
                        printf("                                                 \033[35m|                                                           |\033[0m\n");
                        printf("                                                 \033[35m|-----------------------------------------------------------|\033[0m\n");
                        printf("                                                 \033[35m|\033[33m       1.开始游戏      2.游戏说明      3.退出游戏          \033[35m|\033[0m\n");
                        printf("                                                 \033[35m-------------------------------------------------------------\033[0m\n");
                        printf("\n                                                                       请输入您的选择！                       \n");
                }
        }
}

//*********************************************************

//玩家获胜界面

void P1_Win()
{
        system("clear");

        int i;

        while(!kbhit())
        {
                for(i=0;i<=7;i++)
                {
                        system("clear");

                        printf("\n\n");

                        printf("                             \033[3%dm ------------------------------------------------------------------------ \n\033[0m",i);
                        printf("\n");
                        printf("                             \033[3%dm | * * * *         *        *               *     * * *     *         * |\n\033[0m",i);
                        printf("                             \033[3%dm | *       *     * *        *               *       *       * *       * |\n\033[0m",i);
                        printf("                             \033[3%dm | *       *   *   *        *               *       *       *         * |\n\033[0m",i);
                        printf("                             \033[3%dm | *       *       *        *       *       *       *       *   *     * |\n\033[0m",i);
                        printf("                             \033[3%dm | * * * *         *        *       *       *       *       *         * |\n\033[0m",i);
                        printf("                             \033[3%dm | *               *        *       *       *       *       *     *   * |\n\033[0m",i);
                        printf("                             \033[3%dm | *               *        *     *   *     *       *       *         * |\n\033[0m",i);
                        printf("                             \033[3%dm | *               *        *   *       *   *       *       *       * * |\n\033[0m",i);
                        printf("                             \033[3%dm | *           * * * * *     * *         * *    * * * * *   *         * |\n\033[0m",i);
                        printf("\n");
                        printf("                             \033[3%dm ------------------------------------------------------------------------ \n\033[0m",i);

                }
        }
}

void P2_Win()
{
        system("clear");

        int i;

        while(!kbhit())
        {
                for(i=0;i<=7;i++)
                {
                        system("clear");

                        printf("\n\n");

                        printf("                             \033[3%dm ----------------------------------------------------------------------- \n\033[0m",i);
                        printf("\n");
                        printf("                             \033[3%dm | * * * *       * * *     *               *     * * *     *         * |\n\033[0m",i);
                        printf("                             \033[3%dm | *       *   *       *   *               *       *       * *       * |\n\033[0m",i);
                        printf("                             \033[3%dm | *       *           *   *               *       *       *         * |\n\033[0m",i);
                        printf("                             \033[3%dm | *       *           *   *       *       *       *       *   *     * |\n\033[0m",i);
                        printf("                             \033[3%dm | * * * *           *     *       *       *       *       *         * |\n\033[0m",i);
                        printf("                             \033[3%dm | *               *       *       *       *       *       *     *   * |\n\033[0m",i);
                        printf("                             \033[3%dm | *             *         *     *   *     *       *       *         * |\n\033[0m",i);
                        printf("                             \033[3%dm | *           *       *   *   *       *   *       *       *       * * |\n\033[0m",i);
                        printf("                             \033[3%dm | *           * * * * *    * *         * *    * * * * *   *         * |\n\033[0m",i);
                        printf("\n");
                        printf("                             \033[3%dm ----------------------------------------------------------------------- \n\033[0m",i);
                }
        }
}
//*********************************************************

//游戏等级界面、倒计时时间界面

void LV1()
{
        system("clear");

        printf("\n\n\n");

        printf("                                             \033[33m *           *               *       *       \n\033[0m");
        printf("                                             \033[33m *                                 * *       \n\033[0m");
        printf("                                             \033[33m *             *           *     *   *       \n\033[0m");
        printf("                                             \033[33m *                                   *       \n\033[0m");
        printf("                                             \033[33m *               *       *           *       \n\033[0m");
        printf("                                             \033[33m *                                   *       \n\033[0m");
        printf("                                             \033[33m *                 *   *             *       \n\033[0m");
        printf("                                             \033[33m *                                   *       \n\033[0m");
        printf("                                             \033[33m * * * * *           *           * * * * *   \n\033[0m");
}

void LV2()
{
        system("clear");

        printf("\n\n\n");

        printf("                                             \033[35m *           *               *     * * *     \n\033[0m");
        printf("                                             \033[35m *                               *       *   \n\033[0m");
        printf("                                             \033[35m *             *           *             *   \n\033[0m");
        printf("                                             \033[35m *                                       *   \n\033[0m");
        printf("                                             \033[35m *               *       *             *     \n\033[0m");
        printf("                                             \033[35m *                                   *       \n\033[0m");
        printf("                                             \033[35m *                 *   *           *         \n\033[0m");
        printf("                                             \033[35m *                               *           \n\033[0m");
        printf("                                             \033[35m * * * * *           *           * * * * *   \n\033[0m");
}

void One()
{
        system("clear");

        printf("\n\n");

        printf("                                                           --------------- \n");
        printf("                                                          |               |\n");
        printf("                                                          |               |\n");
        printf("                                                          |      **       |\n");
        printf("                                                          |     * *       |\n");
        printf("                                                          |       *       |\n");
        printf("                                                          |       *       |\n");
        printf("                                                          |       *       |\n");
        printf("                                                          |       *       |\n");
        printf("                                                          |       *       |\n");
        printf("                                                          |       *       |\n");
        printf("                                                          |       *       |\n");
        printf("                                                          |    *******    |\n");
        printf("                                                          |               |\n");
        printf("                                                           --------------- \n");
}

void Two()
{
        system("clear");

        printf("\n\n");

        printf("                                                           --------------- \n");
        printf("                                                          |               |\n");
        printf("                                                          |               |\n");
        printf("                                                          |      ***      |\n");
        printf("                                                          |     *   *     |\n");
        printf("                                                          |    *    *     |\n");
        printf("                                                          |         *     |\n");
        printf("                                                          |        *      |\n");
        printf("                                                          |       *       |\n");
        printf("                                                          |      *        |\n");
        printf("                                                          |     *         |\n");
        printf("                                                          |    *******    |\n");
        printf("                                                          |               |\n");
        printf("                                                          |               |\n");
        printf("                                                           --------------- \n");
}

void    Three()
{
        system("clear");

        printf("\n\n");

        printf("                                                           --------------- \n");
        printf("                                                          |               |\n");
        printf("                                                          |     ****      |\n");
        printf("                                                          |   *     *     |\n");
        printf("                                                          |         *     |\n");
        printf("                                                          |         *     |\n");
        printf("                                                          |         *     |\n");
        printf("                                                          |     *****     |\n");
        printf("                                                          |         *     |\n");
        printf("                                                          |         *     |\n");
        printf("                                                          |         *     |\n");
        printf("                                                          |    *    *     |\n");
        printf("                                                          |     ****      |\n");
        printf("                                                          |               |\n");
        printf("                                                           --------------- \n");
}

//*********************************************************

//游戏说明界面

void GameExplain()
{
        system("clear");

        printf("\n\n");

        printf("                                             **********************************************************\n");
        printf("                                             *                        游戏说明                        *\n");
        printf("                                             **********************************************************\n");
        printf("                                             *                                                        *\n");
        printf("                                             *      在游戏时间结束之后，所占颜色格数多的玩家获胜      *\n");
        printf("                                             *                                                        *\n");
        printf("                                             **********************************************************\n");
}

//*********************************************************

//炸弹填色游戏

void* BombGame()
{
        //读入地图
        ReadMap(1);

        Player p1,p2;

        //player1，属性设置
        p1.head = 11;
        p1.body = 1;
        p1.score = 0;
        p1.x = 1;
        p1.y = 1;

        //player2
        p2.head = 12;
        p2.body = 2;
        p2.score = 0;
        p2.x = 13;
        p2.y = 15;

        //玩家头部初始位置
        map[p1.x][p1.y] = p1.head;
        map[p2.x][p2.y] = p2.head;

        //设定游戏时间
        time_t tPast = -1;
        time_t tNow = -1;

        //获取线程调用时的系统时间
 		time(&tPast);
        time(&tNow);

        //玩家移动
        int i=0,j=0;

        char x = 'a';

        while(x != '0' && (tNow-tPast) <= DDL)
        {
                //获取用户输入
                x = getch();

                switch(x)
                {
                        //player1
                        case 'w':
                                if(map[p1.x-1][p1.y]<4)
                                {
                                        map[p1.x][p1.y] = p1.body;
                                        p1.x = p1.x-1;
                                        map[p1.x][p1.y] = p1.head;
                                }
                                break;
                        case 's':
                                if(map[p1.x+1][p1.y]<4)
                                {
                                        map[p1.x][p1.y] = p1.body;
                                        p1.x = p1.x+1;
                                        map[p1.x][p1.y] = p1.head;
                                }
                                break;
						case 'a':
                                if(map[p1.x][p1.y-1]<4)
                                {
                                        map[p1.x][p1.y] = p1.body;
                                        p1.y = p1.y-1;
                                        map[p1.x][p1.y] = p1.head;
                                }
                                break;
                        case 'd':
                                if(map[p1.x][p1.y+1]<4)
                                {
                                        map[p1.x][p1.y] = p1.body;
                                        p1.y = p1.y+1;
                                        map[p1.x][p1.y] = p1.head;
                                }
                                break;
                        case 'x':
                                Bomb(p1.x, p1.y);
                                break;
                                
						//player2
                        case 'i':
                                if(map[p2.x-1][p2.y]<4)
                                {
                                        map[p2.x][p2.y] = p2.body;
                                        p2.x = p2.x-1;
                                        map[p2.x][p2.y] = p2.head;
                                }
                                break;
                        case 'k':
                                if(map[p2.x+1][p2.y]<4)
                                {
                                        map[p2.x][p2.y] = p2.body;
                                        p2.x = p2.x+1;
                                        map[p2.x][p2.y] = p2.head;
                                }
                                break;
                        case 'j':
                                if(map[p2.x][p2.y-1]<4)
                                {
                                        map[p2.x][p2.y] = p2.body;
                                        p2.y = p2.y-1;
                                        map[p2.x][p2.y] = p2.head;
                                }
                                break;
                        case 'l':
                                 if(map[p2.x][p2.y+1]<4)
                                {
                                        map[p2.x][p2.y] = p2.body;
                                        p2.y = p2.y+1;
                                        map[p2.x][p2.y] = p2.head;
                                }
                                break;
						case 'm':
                                Bomb(p2.x, p2.y);
                                break;
                }

                //获取最新系统时间
                time(&tNow);
        }

        //结果结算
        p1.score = Score(p1.body);
        p2.score = Score(p2.body);

        sleep(2);

        Result(p1.score, p2.score);
}

//*********************************************************

//滑动填色游戏

void* SlideGame()
{
        //读入地图
        ReadMap(2);

        Player p1,p2;

        //player1
        p1.head = 11;
        p1.body = 1;
        p1.score = 0;
        p1.x = 1;
        p1.y = 1;

        //player2
        p2.head = 12;
        p2.body = 2;
        p2.score = 0;
        p2.x = 13;
        p2.y = 15;

        //玩家头部初始位置
        map[p1.x][p1.y] = p1.head;
        map[p2.x][p2.y] = p2.head;

        //设定游戏时间
        time_t tPast = -1;
        time_t tNow = -1;

        //获取线程调用时的系统时间
		time(&tPast);
        time(&tNow);

        //玩家移动
        int i=0, j=0;

        char x = 'a';

        while(x != '0' && (tNow-tPast) <= DDL)
        {
                //获取用户输入
                x = getch();

                switch(x)
                {
                        //player1
                        case 'w':
                                while(map[p1.x-1][p1.y]<4)
                                {
                                        map[p1.x][p1.y] = p1.body;
                                        p1.x = p1.x-1;
                                        map[p1.x][p1.y] = p1.head;
                                }
                                break;
                        case 's':
                                while(map[p1.x+1][p1.y]<4)
                                {
                                        map[p1.x][p1.y] = p1.body;
                                        p1.x = p1.x+1;
                                        map[p1.x][p1.y] = p1.head;
                                }
                                break;
						case 'a':
                                while(map[p1.x][p1.y-1]<4)
                                {
                                        map[p1.x][p1.y] = p1.body;
                                        p1.y = p1.y-1;
                                        map[p1.x][p1.y] = p1.head;
                                }
                                break;
                        case 'd':
                                while(map[p1.x][p1.y+1]<4)
                                {
                                        map[p1.x][p1.y] = p1.body;
                                        p1.y = p1.y+1;
                                        map[p1.x][p1.y] = p1.head;
                                }
                                break;

                        //player2
                        case 'i':
                                while(map[p2.x-1][p2.y]<4)
                                {
                                        map[p2.x][p2.y] = p2.body;
                                        p2.x = p2.x-1;
                                        map[p2.x][p2.y] = p2.head;
                                }
                                break;
                        case 'k':
                                while(map[p2.x+1][p2.y]<4)
                                {
                                        map[p2.x][p2.y] = p2.body;
                                        p2.x = p2.x+1;
                                        map[p2.x][p2.y] = p2.head;
                                }
                                break;
						case 'j':
                                while(map[p2.x][p2.y-1]<4)
                                {
                                        map[p2.x][p2.y] = p2.body;
                                        p2.y = p2.y-1;
                                        map[p2.x][p2.y] = p2.head;
                                }
                                break;
                        case 'l':
                                while(map[p2.x][p2.y+1]<4)
                                {
                                        map[p2.x][p2.y] = p2.body;
                                        p2.y = p2.y+1;
                                        map[p2.x][p2.y] = p2.head;
                                }
                                break;
                }

                //获取最新系统时间
                time(&tNow);
        }

        //结果结算
        p1.score = Score(p1.body);
        p2.score = Score(p2.body);

        sleep(2);

        Result(p1.score, p2.score);
}

//*********************************************************

void Game(int x)
{
        //定义两个线程号
        pthread_t update;
        pthread_t gm1;
        pthread_t gm2;

        //创建线程
        pthread_create(&update, NULL, MapChange, NULL);  //地图刷新

        if(x == 1)
        {
                //炸弹填色游戏
                pthread_create(&gm1, NULL, BombGame, NULL);
                pthread_join(update, NULL);
                pthread_join(gm1, NULL);
        }
        else if(x == 2)
        {
                //滑动填色游戏
                pthread_create(&gm2, NULL, SlideGame, NULL);
                pthread_join(update, NULL);
                pthread_join(gm2, NULL);
        }
}

//*********************************************************

int main()
{
        char p = '1';

        //初始主菜单
        GameMenu();

        //用户输入选择
        p = getch();

        //游戏逻辑
        while(p != '3')
        {
                //选择游戏阶段
                switch(p)
                {
                        case '1':
                                //第一关界面、倒计时
                                LV1();
                                sleep(2);
                                Three();
                                sleep(1);
                                Two();
                                sleep(1);
                                One();
                                sleep(1);

                                Game(1);  //从游戏1开始

                                getch();

                                LV2();
                                sleep(2);
                                Three();
                                sleep(1);
                                Two();
                                sleep(1);
                                One();
                                sleep(1);

                                Game(2);

                                p = '0';

                                break;
                        case '2':
                                GameExplain();
                                p = getch();
                                break;

                        case '3':
                                system("clear");
                                //直接结束游戏
                                p = '3';
                                break;

                        default:
                                GameMenu();
                                p = getch();
                                break;
                }
        }

        printf("\n\n                                                                        感谢您的使用！\n");

        return 0;
}
