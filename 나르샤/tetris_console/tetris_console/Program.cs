//using System;
//using System.Collections.Generic;
//using System.Linq;
//using System.Text;
//using System.Threading.Tasks;

//namespace tetris_console // 전체를 묶어주는 범위
//{
//    class Program
//    {
//        static void Main(string[] args) // 메모리 구조 | text data stack heep |    static = data영역에 남아있어서 exe가 끝날때 까지 존재
//        {

//        }
//    }
//}

//// new 키워드 사용시 heep 영역에 올라감


using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using System.Threading;

namespace ConsoleApp2
{
    class Program
    {
        public static int[,,,] block_L = new int[7, 4, 4, 4] {
            {
                { {0,0,0,0},{0,1,0,0},{0,1,1,1},{0,0,0,0} }, // ㄴ
                { {0,0,0,0},{0,1,1,0},{0,1,0,0},{0,1,0,0} },
                { {0,0,0,0},{1,1,1,0},{0,0,1,0},{0,0,0,0} },
                { {0,0,1,0},{0,0,1,0},{0,1,1,0},{0,0,0,0} }
            },
            {
                { {0,0,0,0},{0,0,1,0},{1,1,1,0},{0,0,0,0} }, // 역ㄴ
                { {0,1,0,0},{0,1,0,0},{0,1,1,0},{0,0,0,0} },
                { {0,0,0,0},{0,1,1,1},{0,1,0,0},{0,0,0,0} },
                { {0,0,0,0},{0,1,1,0},{0,0,1,0},{0,0,1,0} }
            },
            {
                { {0,0,0,0},{0,1,1,0},{0,1,1,0},{0,0,0,0} }, // ㅁ
                { {0,0,0,0},{0,1,1,0},{0,1,1,0},{0,0,0,0} },
                { {0,0,0,0},{0,1,1,0},{0,1,1,0},{0,0,0,0} },
                { {0,0,0,0},{0,1,1,0},{0,1,1,0},{0,0,0,0} }
            },
            {
                { {0,0,0,0},{0,0,1,1},{0,1,1,0},{0,0,0,0} }, // 역z
                { {0,0,0,0},{0,1,0,0},{0,1,1,0},{0,0,1,0} },
                { {0,0,0,0},{0,0,1,1},{0,1,1,0},{0,0,0,0} },
                { {0,0,0,0},{0,1,0,0},{0,1,1,0},{0,0,1,0} }
            },
            {
                { {0,0,0,0},{1,1,0,0},{0,1,1,0},{0,0,0,0} }, // z
                { {0,0,1,0},{0,1,1,0},{0,1,0,0},{0,0,0,0} },
                { {0,0,0,0},{1,1,0,0},{0,1,1,0},{0,0,0,0} },
                { {0,0,1,0},{0,1,1,0},{0,1,0,0},{0,0,0,0} }
            },
            {
                { {0,0,0,0},{0,1,0,0},{1,1,1,0},{0,0,0,0} }, // ㅗ
                { {0,1,0,0},{0,1,1,0},{0,1,0,0},{0,0,0,0} },
                { {0,0,0,0},{0,1,1,1},{0,0,1,0},{0,0,0,0} },
                { {0,0,0,0},{0,0,1,0},{0,1,1,0},{0,0,1,0} }
            },
            {
                { {0,0,0,0},{1,1,1,1},{0,0,0,0},{0,0,0,0} }, // |
                { {0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0} },
                { {0,0,0,0},{1,1,1,1},{0,0,0,0},{0,0,0,0} },
                { {0,1,0,0},{0,1,0,0},{0,1,0,0},{0,1,0,0} },
            }
        };


        public static int[,] background = new int[12, 12]
        {
            {1,1,1,1,1,1,1,1,1,1,1,1},
            {1,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,0,0,0,1},
            {1,1,1,1,1,1,1,1,1,1,1,1}
        };


        public static Random rand = new Random();
        public static int overlap_count = 0;
        public static int line_count = 0;
        public static int rotate_num = 0;
        public static char space = ' ';
        public static int now_block = rand.Next(0,6);
        public static int next_block = rand.Next(0,6);


        public static void delete_block(int x, int y, int block)
        {
            for (int j = 0; j < 4; j++) // 지우기
            {
                for (int i = 0; i < 4; i++)
                {
                    if (block_L[block, rotate_num , j, i] == 1)
                    {
                        Console.SetCursorPosition(x + i, y + j);
                        Console.Write(space);
                    }
                }
                Console.WriteLine("");

            }
        }

        public static void make_block(int x, int y, int block)
        {
            for (int j = 0; j < 4; j++) // ㄴ자 블록 그리기 코드
            {
                for (int i = 0; i < 4; i++)
                {
                    if (block_L[block, rotate_num, j, i] == 1)
                    {
                        Console.SetCursorPosition(x + i, y + j);
                        Console.Write("*");
                    }
                }
                Console.WriteLine("");

            }
        }


        public static int overlap_check(int x, int y, int rotate_num)
        {
            int overlap_count = 0;

            for (int j = 0; j < 4; j++)
            {
                for(int i = 0; i < 4; i ++)
                {
                    if (j + y < 12)
                    {
                        if (block_L[now_block, rotate_num, j, i] == 1 && background[j + y, i + x] == 1)
                        {
                            overlap_count++;
                        }
                    }
                }
            }
            return overlap_count;
            
        }

        public static void draw_background(int x, int y)
        {
            for (int j = 0; j < 12; j++) // 백그라운드 출력
            {
                for (int i = 0; i < 12; i++)
                {
                    if (background[j, i] == 1)
                    {
                        Console.SetCursorPosition(i, j);
                        Console.Write("*");
                    }
                    else
                    {
                        Console.SetCursorPosition(i, j);
                        Console.Write(space);
                    }
                }
                Console.WriteLine("");
            }

            for (int j = 0; j < 12; j++) // 백그라운드 디버깅용 출력
            {
                for (int i = 0; i < 12; i++)
                {
                    if (background[j, i] == 1)
                    {

                        Console.SetCursorPosition(i + 25, j);
                        Console.Write(background[j, i]);
                    }
                    else
                    {
                        Console.SetCursorPosition(i + 25, j);
                        Console.Write(background[j, i]);
                    }
                }
                Console.WriteLine("");
            }
            
        }
        
        public static void insert_block(int x, int y)
        {
            for (int j = 0; j < 4; j++) // 백그라운드에 블록 넣어버리기
            {
                for (int i = 0; i < 4; i++)
                {
                    if (block_L[now_block, rotate_num, j, i] == 1)
                    {
                        background[j + y, x + i] = 1;
                    }
                }
                draw_background(x, y);
            }
        }

        public static void delete_line(int x, int y)
        {
            for (int k = 1; k <= 10; k++)
            {
                line_count = 0;

                for (int i = 1; i <= 10; i++)
                {
                    if (background[k, i] == 1)
                    {
                        line_count++;
                    }
                }

                if (line_count == 10)
                {
                    for (int j = k; j > 1; j--)
                    {
                        for (int i = 1; i <= 10; i++)
                        {
                            background[j, i] = background[j - 1, i];
                        }
                    }
                    draw_background(x, y);
                }
            }

        }


        static void Main(string[] args)
        {
            ConsoleKeyInfo key_value;
            String ch;
            int count = 0;
            int x = 0;
            int y = 0;
            
            draw_background(x,y);

            x = 4; // 블록이 생성되면서 백그라운드 파괴 방지용
            y = 1;

            Console.SetCursorPosition(x, y);
            make_block(x,y, now_block);
            
            make_block(15, 1, next_block); // 옆에 다음블록 출력만

            while (true)
            {
                
                if (count == 10)
                {
                    
                    overlap_count = overlap_check(x , y + 1, rotate_num);
                    count = 0;

                    if (overlap_count == 0)
                    {
                        delete_block(x, y, now_block);
                        y++;
                        make_block(x, y, now_block);
                        count = 0;
                    }
                    else
                    {
                        insert_block(x, y); // 블록을 백그라운드에 넣음
                        delete_line(x, y); // 만약 줄을 다 채우면 줄비우기
                        
                        rotate_num = 0; // 로테이트 초기화를 먼저해줘서 딜리트 오류방지

                        delete_block(15, 1, next_block); // 옆에 블록 지우기
                        now_block = next_block; // 다음블록을 현재블록으로 바꿔줌
                        next_block = rand.Next(0, 6); // 랜덤함수로 다음블록 만들기
                        make_block(15, 1, next_block); // 다음블록 출력
                        


                        x = 4;
                        y = 0;
                        count = 0;
                    }


                }


                if (Console.KeyAvailable)
                {

                    key_value = Console.ReadKey(true);
                    ch = key_value.Key.ToString();

                    if (ch == "A")
                    {
                        overlap_count = overlap_check(x-1, y, rotate_num);
                        if(overlap_count == 0)
                        {
                            delete_block(x, y, now_block);
                            x--;
                            make_block(x, y, now_block);
                        }
                    }
                    else if (ch == "D")
                    {
                        overlap_count = overlap_check(x+1, y, rotate_num);
                        if (overlap_count == 0)
                        {
                            delete_block(x, y, now_block);
                            x++;
                            make_block(x, y, now_block);
                        }
                    }
                    else if (ch == "W")
                    {
                        overlap_count = overlap_check(x, y-1, rotate_num);
                        if (overlap_count == 0)
                        {
                            //delete_block(x, y);
                            //y--;
                            //make_block(x, y);
                        }
                    }
                    else if (ch == "S")
                    {
                        overlap_count = overlap_check(x, y+1, rotate_num);
                        if (overlap_count == 0)
                        {
                            delete_block(x, y, now_block);
                            y++;
                            make_block(x, y, now_block);
                        }
                    }
                    else if (ch == "R")
                    {
                        int tmp_rotate_num = rotate_num + 1;
                        if (tmp_rotate_num == 4)
                            tmp_rotate_num = 0;

                        overlap_count = overlap_check(x, y, tmp_rotate_num);
                        if (overlap_count == 0)
                        {
                            delete_block(x, y, now_block);
                            rotate_num++;
                            if (rotate_num == 4)
                                rotate_num = 0;
                            make_block(x, y, now_block);
                        }
                    }
                }

                count++;
                Thread.Sleep(100);
                
            }


        }
    }
}
