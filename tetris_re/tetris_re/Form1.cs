using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace tetris_re
{
    public partial class Form1 : Form
    {
        int BASE_X = 12;
        int BASE_Y = 7;

        PictureBox[,] boxs = new PictureBox[20, 10];

        int[,] remember_block = new int[20, 10];

        Timer t = new Timer();

        Random rnd = new Random();

        int x = 0;
        int y = 0;

        int currunt_block = 0;
        int next_block = 0;

        //int[,] block_l = new int[4, 4]
        //{
        //    {0,0,0,0 },
        //    {0,1,0,0 },
        //    {0,1,1,1 },
        //    {0,0,0,0 }
        //};

        int[,,] block_n = new int[7, 4, 4]
            {
                {
                    {0, 0, 0, 0},
                    {0, 1, 0, 0},
                    {0, 1, 1, 1},
                    {0, 0, 0, 0},
                },
                {
                    {0, 0, 0, 0},
                    {0, 1, 1, 0},
                    {0, 1, 1, 0},
                    {0, 0, 0, 0},
                },
                {
                    {0, 0, 1, 0},
                    {0, 0, 1, 0},
                    {0, 0, 1, 0},
                    {0, 0, 1, 0},
                },
                {
                    {0, 0, 0, 0},
                    {0, 1, 0, 0},
                    {0, 1, 1, 0},
                    {0, 0, 1, 0},
                },
                {
                    {0, 0, 0, 0},
                    {0, 0, 1, 0},
                    {0, 1, 1, 0},
                    {0, 1, 0, 0},
                },
                {
                    {0, 0, 0, 0},
                    {0, 1, 1, 0},
                    {0, 0, 1, 0},
                    {0, 0, 1, 0},
                },
                {
                    {0, 0, 0, 0},
                    {0, 0, 1, 0},
                    {0, 1, 1, 0},
                    {0, 0, 1, 0},
                }
            };
        Color[] block_color = new Color[8]
        {
            Color.White,
            Color.Red,
            Color.Orange,
            Color.Gold,
            Color.Green,
            Color.Blue,
            Color.Purple,
            Color.Black,
        };
        Color[,] saved_color = new Color[20, 10];

        private void resetBlock()
        {
            block_n = new int[7, 4, 4]
            {
                {
                    {0, 0, 0, 0},
                    {0, 1, 0, 0},
                    {0, 1, 1, 1},
                    {0, 0, 0, 0},
                },
                {
                    {0, 0, 0, 0},
                    {0, 1, 1, 0},
                    {0, 1, 1, 0},
                    {0, 0, 0, 0},
                },
                {
                    {0, 0, 1, 0},
                    {0, 0, 1, 0},
                    {0, 0, 1, 0},
                    {0, 0, 1, 0},
                },
                {
                    {0, 0, 0, 0},
                    {0, 1, 0, 0},
                    {0, 1, 1, 0},
                    {0, 0, 1, 0},
                },
                {
                    {0, 0, 0, 0},
                    {0, 0, 1, 0},
                    {0, 1, 1, 0},
                    {0, 1, 0, 0},
                },
                {
                    {0, 0, 0, 0},
                    {0, 1, 1, 0},
                    {0, 0, 1, 0},
                    {0, 0, 1, 0},
                },
                {
                    {0, 0, 0, 0},
                    {0, 0, 1, 0},
                    {0, 1, 1, 0},
                    {0, 0, 1, 0},
                }
            };
        }

        public Form1()
        {
            InitializeComponent();

            for(int j = 0; j < 20; j++) // 블록을 20 10 크기로 만듦
            {
                for (int i = 0; i < 10; i++)
                {
                    PictureBox box = new PictureBox();
                    box.Location = new Point((BASE_X + 2) + (i * 22), (BASE_Y + 2) + (j * 22));
                    box.Size = new Size(20, 20); // 사이즈를 20,20으로 만듦

                    box.BackColor = Color.White; // 기본 색깔을 흰색으로
                    saved_color[j, i] = Color.White; // 세이브 컬러 안에 초기화 같이함

                    Controls.Add(box); // 블록을 만듦

                    box.BringToFront(); // 가장 앞으로 보냄

                    boxs[j, i] = box; // 객체의 위치에 만든 박스를 넣음
                }
            }
            currunt_block = rnd.Next(7);

            for(int j = 0; j < 4; j++) // 블록 첫 호출
            {
                for(int i = 0; i < 4; i++)
                {
                    if (block_n[currunt_block, j, i] == 0) boxs[y + j, x + i].BackColor = Color.White;
                    else boxs[y + j, x + i].BackColor = block_color[currunt_block+1];
                }
            }

            next_block = rnd.Next(7);
            drawNextBlock();

            // timer
            t.Interval = 1000; // 타이머가 몉ms마다 호출될지 설정
            t.Tick += new EventHandler(timer_event); // 타이머가 호출될 때 마다 실행될 이벤트
            t.Start(); // 타이머 실행
        }

        private void drawNextBlock()
        {
            Bitmap map = new Bitmap(90, 90);
            for(int j = 0; j < 90; j++)
            {
                for(int i = 0; i < 90; i++)
                {
                    map.SetPixel(i, j, Color.Gray);
                }
            }

            int x = 2;
            int y = 2;

            for (int j = 0; j < 4; j++)
            {
                for (int i = 0; i < 4; i++)
                {
                    for(int h = 0; h < 20; h++)
                    {
                        for(int w = 0; w < 20; w++)
                        {
                            if (block_n[next_block, j, i] == 1)
                                map.SetPixel(x + w, y + h, block_color[next_block + 1]);
                            else
                                map.SetPixel(x + w, y + h, block_color[0]);
                        }
                    }
                    x += 22;
                }
                x = 2;
                y += 22;
            }
            pictureBox2.Image = map;    //picturBox2에 우리가 만든 Bitmap을 띄움
        }

        private void timer_event(object sender, EventArgs args)
        {
            if(!move_block(0, 1))
            {
                for (int j = 0; j < 4; j++)
                {
                    for (int i = 0; i < 4; i++)
                    {
                        if (block_n[currunt_block, j, i] == 1)
                        {
                            remember_block[y + j, x + i] = 1;
                            saved_color[y + j, x + i] = block_color[currunt_block + 1];
                        }
                    }
                }

                int block_count = 0; // 줄 비우기 코드
                for(int j = 0; j < 20; j++) // 위에부터 검사
                {
                    for(int i = 0; i < 10; i++) // 카운트 변수를 사용하여 검사중
                    {
                        if (remember_block[j, i] == 1) block_count += 1;
                    }

                    if(block_count == 10) // 만약 줄이 다 차있으면
                    {
                        for (int k = j; k > 0; k--)
                        {
                            for (int i = 0; i < 10; i++)
                            {
                                remember_block[k, i] = remember_block[k - 1, i]; // 위의 줄들을 가지고 내려옴
                                saved_color[k, i] = saved_color[k - 1, i]; // 색 또한 한줄씩 가지고 내려옴
                            }
                        }
                    }
                    redraw_background();
                    block_count = 0;
                }

                x = y = 0;
                resetBlock(); // 블록 돌아간 경우때문에 초기화 
                currunt_block = next_block; // 다음블록 가져옴
                next_block = rnd.Next(7); // 다음블록 랜덤처리
                drawNextBlock();
            } // 한개가 바닥에 닿았을때의 위치

            Console.Clear(); // 디버깅용 코드
            for (int j = 0; j < 20; j++)
            {
                for(int i = 0; i < 10; i++)
                {
                    Console.Write(remember_block[j, i]);
                }
                Console.WriteLine();
            } // 디버깅용 코드
        }

        private void redraw_background()
        {
            for (int j = 0; j < 20; j++)
            {
                for (int i = 0; i < 10; i++)
                {
                    boxs[j, i].BackColor = saved_color[j, i]; // 세이브 컬러에서 바로 가져와서 코드가 간결해짐
                }
            }
        }

        private bool move_block(int x_amount, int y_amount)
        {
            if (overlap_check(x + x_amount, y + y_amount)) return false; // 만약 이동할 위치에 블록을 못옮기면 리턴으로 끝냄

            for (int j = 0; j < 4; j++) // 블록 옮기기 전에 지움
            {
                for (int i = 0; i < 4; i++)
                {
                    if (block_n[currunt_block, j, i] == 1) boxs[y + j, x + i].BackColor = Color.White; // 블록이 있는곳만 지움
                }
            }

            // 위치 옮기기
            x += x_amount;
            y += y_amount;

            for (int j = 0; j < 4; j++) // 다시 출력
            {
                for (int i = 0; i < 4; i++)
                {
                    if (block_n[currunt_block, j, i] == 1) boxs[y + j, x + i].BackColor = block_color[currunt_block+1]; // 블록이 있는곳만 만듦
                }
            }

            return true;
        }

        private int[,] rotate_block() // 블록 돌리는 코드
        {
            int[,] arr = new int[4, 4];
            for(int j = 0; j < 4; j++)
            {
                for(int i = 0; i < 4; i++)
                {
                    arr[j, i] = block_n[currunt_block, 3 - i, j]; // 행 열을 바꿔줌
                }
            }

            return arr;
        }

        private bool overlap_check(int x, int y) // 아래의 오버랩 체크 오버로딩
        {
            return overlap_check(x, y, false);
        }

        private bool overlap_check(int x, int y, bool is_rotate)
        {
            int[,] arr = new int[4,4];
            for (int j = 0; j < 4; j++)
                for (int i = 0; i < 4; i++)
                    arr[j, i] = block_n[currunt_block, j, i];

            if (is_rotate) arr = rotate_block();

            for (int j = 0; j < 4; j++)
            {
                for (int i = 0; i < 4; i++)
                {
                    if (arr[j, i] == 1) // 블록 흰색부분때문에 경계 안밀리는거 방지
                    {
                        if (x + i >= 10 || x + i < 0) return true; // x 부분이 이탈함
                        if (y + j >= 20 || y + j < 0) return true; // y 부분이 이탈함
                        if (remember_block[y + j, x + i] == 1) return true; // 밑에 걸리는지 리맴버에서 확인
                    }
                }
            }
            return false;
        }

        private void Form1_KeyDown(object sender, KeyEventArgs e) // 키가 눌리는 이벤트 관리
        {
            if (e.KeyCode == Keys.A)
            {
                move_block(-1, 0);
            }
            else if (e.KeyCode == Keys.S)
            {
                move_block(0, 1);
            }
            else if (e.KeyCode == Keys.D)
            {
                move_block(1, 0);
            }
            else if (e.KeyCode == Keys.W)
            {
                move_block(0, -1);
            }
            else if (e.KeyCode == Keys.R)
            {
                if (!overlap_check(x, y, true)) // 오버랩 체크가 걸리지 않았을때
                {
                    for (int j = 0; j < 4; j++) // 블록 돌리기 전에 지움
                    {
                        for (int i = 0; i < 4; i++)
                        {
                            if (block_n[currunt_block, j, i] == 1) boxs[y + j, x + i].BackColor = Color.White; // 블록이 있는곳만 지움
                        }
                    }

                    int[,] arr = rotate_block();

                    for (int j = 0; j < 4; j++)
                        for (int i = 0; i < 4; i++)
                            block_n[currunt_block, j, i] = arr[j, i];

                    for (int j = 0; j < 4; j++) // 다시 출력
                    {
                        for (int i = 0; i < 4; i++)
                        {
                            if (block_n[currunt_block, j, i] == 1) boxs[y + j, x + i].BackColor = block_color[currunt_block+1]; // 블록이 있는곳만 만듦
                        }
                    }
                }
            }
        }
    }
}
