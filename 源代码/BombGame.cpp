#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <time.h>
#include <termios.h>

#define DDL 10  //��Ϸ��ʱʱ��

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

//��ȡ�û����벢������
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

//�����̵�����״̬
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

        //���ļ�
        if(!fp)
        {
                printf("��ͼ��ȡʧ��\n");
        }

        //����ͼ���ݶ�������map
        for(i=0; i<15; i++)
        {
                for(j=0; j<17; j++)
                {
                        fscanf(fp,"%d",&map[i][j]);
                }
        }

        //�ر��ļ�
        close(fp);
}

//*********************************************************

//��ͼˢ�£������̣߳�һֱˢ��
void* MapChange()
{
        //��ȡʱ��
        time_t tPast = -1;
        time_t tNow = -1;

        time(&tPast);  //��ȡ�߳̿�ʼʱ��ϵͳʱ��
        time(&tNow);

        int i=0,j=0,k=0;

        //��ͣ��ˢ�£�ֱ��DDL
        while((tNow-tPast) <= DDL)
        {
                //��ȡ��ǰϵͳʱ��
                time(&tNow);

                //����ʱ��
                k = DDL+1 - tNow + tPast;

                //����
                system("clear");

                printf("\n\n");

                //ͼ�λ�
                for(i=0; i<15; i++)
                {
                        printf("                                  ");

                        for(j=0; j<17; j++)
                        {
                                switch(map[i][j])
                                {
                                        case 0: printf("   ");break;

                                        case 1: printf(" \033[42m��\033[0m ");break;  //Player1_Body
                                        case 11:printf(" \033[32m��\033[0m ");break;  //Player1_Head

                                        case 2: printf(" \033[44m��\033[0m ");break;  //Player2_Body
                                        case 12:printf(" \033[34m��\033[0m ");break;  //Player2_Head

                                        case 3: printf(" \033[41m \033[0m ");break;  //BombEffect
                                        case 5: printf(" \033[31m��\033[0m ");break;  //Bomb

                                        case 4: printf(" \033[33m��\033[0m ");break;  //Wall can be broken
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
                                printf("\033[34m  |     ����ʱ      |\033[0m");
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
                                printf("\033[34m  | player1�ƶ�:    |\033[0m");
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
                                printf("\033[34m  | player2�ƶ�:    |\033[0m");
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
                                printf("\033[34m  |  �ڹ涨ʱ���ڣ� |\033[0m");
                        }
                        else if(i==13)
						{
                                printf("\033[34m  |ռ������Ŀ����ʤ |\033[0m");
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

//���ǽ��
void BreakWall(int x, int y)
{
        //��
        if(map[x-1][y] == 4)
        {
                map[x-1][y] = 0;
        }

        //��
        if(map[x+1][y] == 4)
        {
                map[x+1][y] = 0;
        }

        //��
        if(map[x][y-1] == 4)
        {
                map[x][y-1] = 0;
        }

        //��
        if(map[x][y+1] == 4)
        {
                map[x][y+1] = 0;
        }
}

//*********************************************************

//ը��ըǽ
void Bomb(int x, int y)
{
        //����ը��
        map[x][y] = 5;

        //ը����Ч

        //��ǽ
        BreakWall(x, y);
}

//*********************************************************

//����ͳ�ƣ����ص÷�
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

//���˵�

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
                        printf("                                                 \033[35m|\033[33m                    ��Ϸ����ռ��Ϊ��                       \033[35m|\033[0m\n");
                        printf("                                                 \033[35m|                                                           |\033[0m\n");
                        printf("                                                 \033[35m|-----------------------------------------------------------|\033[0m\n");
                        printf("                                                 \033[35m|\033[33m       1.��ʼ��Ϸ      2.��Ϸ˵��      3.�˳���Ϸ          \033[35m|\033[0m\n");
                        printf("                                                 \033[35m-------------------------------------------------------------\033[0m\n");
                        printf("\n                                                                       ����������ѡ��                       \n");
                }
        }
}

//*********************************************************

//��һ�ʤ����

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

//��Ϸ�ȼ����桢����ʱʱ�����

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

//��Ϸ˵������

void GameExplain()
{
        system("clear");

        printf("\n\n");

        printf("                                             **********************************************************\n");
        printf("                                             *                        ��Ϸ˵��                        *\n");
        printf("                                             **********************************************************\n");
        printf("                                             *                                                        *\n");
        printf("                                             *      ����Ϸʱ�����֮����ռ��ɫ���������һ�ʤ      *\n");
        printf("                                             *                                                        *\n");
        printf("                                             **********************************************************\n");
}

//*********************************************************

//ը����ɫ��Ϸ

void* BombGame()
{
        //�����ͼ
        ReadMap(1);

        Player p1,p2;

        //player1����������
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

        //���ͷ����ʼλ��
        map[p1.x][p1.y] = p1.head;
        map[p2.x][p2.y] = p2.head;

        //�趨��Ϸʱ��
        time_t tPast = -1;
        time_t tNow = -1;

        //��ȡ�̵߳���ʱ��ϵͳʱ��
 		time(&tPast);
        time(&tNow);

        //����ƶ�
        int i=0,j=0;

        char x = 'a';

        while(x != '0' && (tNow-tPast) <= DDL)
        {
                //��ȡ�û�����
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

                //��ȡ����ϵͳʱ��
                time(&tNow);
        }

        //�������
        p1.score = Score(p1.body);
        p2.score = Score(p2.body);

        sleep(2);

        Result(p1.score, p2.score);
}

//*********************************************************

//������ɫ��Ϸ

void* SlideGame()
{
        //�����ͼ
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

        //���ͷ����ʼλ��
        map[p1.x][p1.y] = p1.head;
        map[p2.x][p2.y] = p2.head;

        //�趨��Ϸʱ��
        time_t tPast = -1;
        time_t tNow = -1;

        //��ȡ�̵߳���ʱ��ϵͳʱ��
		time(&tPast);
        time(&tNow);

        //����ƶ�
        int i=0, j=0;

        char x = 'a';

        while(x != '0' && (tNow-tPast) <= DDL)
        {
                //��ȡ�û�����
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

                //��ȡ����ϵͳʱ��
                time(&tNow);
        }

        //�������
        p1.score = Score(p1.body);
        p2.score = Score(p2.body);

        sleep(2);

        Result(p1.score, p2.score);
}

//*********************************************************

void Game(int x)
{
        //���������̺߳�
        pthread_t update;
        pthread_t gm1;
        pthread_t gm2;

        //�����߳�
        pthread_create(&update, NULL, MapChange, NULL);  //��ͼˢ��

        if(x == 1)
        {
                //ը����ɫ��Ϸ
                pthread_create(&gm1, NULL, BombGame, NULL);
                pthread_join(update, NULL);
                pthread_join(gm1, NULL);
        }
        else if(x == 2)
        {
                //������ɫ��Ϸ
                pthread_create(&gm2, NULL, SlideGame, NULL);
                pthread_join(update, NULL);
                pthread_join(gm2, NULL);
        }
}

//*********************************************************

int main()
{
        char p = '1';

        //��ʼ���˵�
        GameMenu();

        //�û�����ѡ��
        p = getch();

        //��Ϸ�߼�
        while(p != '3')
        {
                //ѡ����Ϸ�׶�
                switch(p)
                {
                        case '1':
                                //��һ�ؽ��桢����ʱ
                                LV1();
                                sleep(2);
                                Three();
                                sleep(1);
                                Two();
                                sleep(1);
                                One();
                                sleep(1);

                                Game(1);  //����Ϸ1��ʼ

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
                                //ֱ�ӽ�����Ϸ
                                p = '3';
                                break;

                        default:
                                GameMenu();
                                p = getch();
                                break;
                }
        }

        printf("\n\n                                                                        ��л����ʹ�ã�\n");

        return 0;
}
