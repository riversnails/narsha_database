using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace tetris3_keyInput
{
    public partial class Form1 : Form
    {
        static System.Windows.Forms.Timer timer_1sec = new System.Windows.Forms.Timer();

        int x = 5, y = 0;
        int offset_x = 10, offset_y = 10;

        Random rnd = new Random();

        int current_block = 0; // 블록종류
        int next_block;

        Color[] block_color = new Color[8]
        {
            Color.White,
            Color.Red,
            Color.Orange,
            Color.Gold,
            Color.Green,
            Color.Blue,
            Color.Purple,
            Color.Black
        }; // 색갈 배열

        int[,,] block_n; // 블록7개 선언

        PictureBox nextBlockBox = new PictureBox(); // 다음블록
        PictureBox[,] pictureBox = new PictureBox[20, 10]; // Background
        int[,] block_state = new int[20, 10]; // Current block
        Color[,] saved_color = new Color[20, 10]; //Block color save

        private void initBlock() // 블록 로테이트된거 초기화 && 블록 초기화 값 저장해놓은거
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
            next_block = rnd.Next(7);

            initBlock();

            timer_1sec.Interval = 1000;
            timer_1sec.Tick += new EventHandler(event_1sec);
            timer_1sec.Start();

            this.KeyPreview = true;
        }



        private void Form1_Load(object sender, EventArgs e)
        {
            int i, j;

            for (j = 0; j < 20; j++)
            {
                for (i = 0; i < 10; i++)
                {
                    pictureBox[j, i] = new PictureBox();
                    pictureBox[j, i].BackColor = block_color[0];
                    pictureBox[j, i].Location = new Point(offset_x + (i * 22), offset_y + (j * 22));
                    pictureBox[j, i].Size = new Size(20, 20);
                    saved_color[j, i] = Color.White;
                    this.Controls.Add(pictureBox[j, i]);
                }
            }

            offset_x = 250;
            offset_y = 10;

            nextBlockBox.BackColor = Color.Gray;
            nextBlockBox.Location = new Point(offset_x, offset_y);
            nextBlockBox.Size = new Size(90, 90);
            Controls.Add(nextBlockBox);
            drawNextBlock();
        }

        private void drawNextBlock()
        {
            Bitmap map = new Bitmap(90, 90);
            int x = 2, y = 2;

            for(int j = 0; j < 4; j++)
            {
                for(int i = 0; i < 4; i++)
                {
                    if(block_n[next_block,j,i] == 1)
                    {
                        drawBlock(x, y, 20, map, block_color[next_block + 1]);
                        
                    }
                    else
                    {
                        drawBlock(x, y, 20, map, Color.White);
                    }
                    x += 22;
                }
                x = 2;
                y += 22;
            }
            nextBlockBox.Image = map;
        }

        private void drawBlock(int x, int y, int size, Bitmap map, Color color)
        {
            for (int j = 0; j < size; j++)
            {
                for (int i = 0; i < size; i++)
                {
                    map.SetPixel(x + i, y + j, color);
                }
            }
        }

        private bool overlap_check(int x, int y)
        {
            return overlap_check(x, y, false);
        }
        private bool overlap_check(int x, int y, bool is_rotate)
        {
            int[,] arr = new int[4, 4];
            for (int j = 0; j < 4; j++) for (int i = 0; i < 4; i++) arr[j, i] = block_n[current_block, j, i];

            if (is_rotate) arr = rotateBlockL();

            for (int j = 0; j < 4; j++)
            {
                for (int i = 0; i < 4; i++)
                {
                    if (arr[j, i] == 1)
                    {
                        if (x + i >= 10 || x + i < 0) return true;
                        if (y + j >= 20 || y + j < 0) return true;

                        if (block_state[j + y, i + x] != 0) return true;

                    }
                }
            }
            return false;
        }

        private void draw_block_l(int x, int y)
        {
            for (int j = 0; j < 4; j++)
            {
                for (int i = 0; i < 4; i++)
                {
                    if (block_n[current_block, j, i] == 1)
                    {
                        pictureBox[j + this.y, i + this.x].BackColor = block_color[0]; // Clear block
                    }
                }
            }
            this.y += y;
            this.x += x;

            for (int j = 0; j < 4; j++)
            {
                for (int i = 0; i < 4; i++)
                {
                    if (block_n[current_block, j, i] == 1)
                    {
                        pictureBox[j + this.y, i + this.x].BackColor = block_color[current_block + 1]; // Draw block
                    }
                }
            }

        }

        private void resetBlock()
        {
            for (int j = 0; j < 20; j++)
            {
                for (int i = 0; i < 10; i++)
                {
                    pictureBox[j, i].BackColor = saved_color[j, i];
                }
            }
        }

        private int[,] rotateBlockL()
        {
            int[,] temp = new int[4, 4];
            for (int j = 0; j < 4; j++)
            {
                for (int i = 0; i < 4; i++)
                {
                    temp[j, i] = block_n[current_block, 3 - i, j];
                }
            }
            return temp;
        }

        private void event_1sec(Object myObject, EventArgs myEventArgs)
        {
            if (!overlap_check(x, y + 1))
                draw_block_l(0, 1);
            else // 땅바닥에 쿵
            {
                for (int j = 0; j < 4; j++)
                {
                    for (int i = 0; i < 4; i++)
                    {
                        if (block_n[current_block, j, i] == 1)
                        {
                            block_state[j + y, i + x] = 1;
                            saved_color[j + y, i + x] = block_color[current_block + 1];
                        }
                    }
                }

                //------------------------------------
                for (int j = 0; j < 20; j++)
                {
                    int count = 0;
                    for (int i = 0; i < 10; i++)
                    {
                        if (block_state[j, i] == 1) count++;
                    }

                    if (count == 10)
                    {
                        for (int level = j; level > 0; level--)
                        {
                            for (int i = 0; i < 10; i++)
                            {
                                block_state[level, i] = block_state[level - 1, i];
                                saved_color[level, i] = saved_color[level - 1, i];
                            }
                        }
                        j--;
                    }
                }

                //------------------------------------
                current_block = next_block; // 다음블록을 현재 블록으로
                next_block = rnd.Next(7); // 다음블록을 랜덤으로 지정
                drawNextBlock(); // 

                initBlock();
                resetBlock();
                x = 4;
                y = 1;
            }
        }

        private void Form1_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == 'a')
            {
                if (!overlap_check(x - 1, y))
                    draw_block_l(-1, 0);
            }
            else if (e.KeyChar == 'd')
            {
                if (!overlap_check(x + 1, y))
                    draw_block_l(1, 0);
            }
            else if (e.KeyChar == 'w')
            {
                if (!overlap_check(x, y - 1))
                    draw_block_l(0, -1);
            }
            else if (e.KeyChar == 's')
            {
                if (!overlap_check(x, y + 1))
                    draw_block_l(0, 1);
            }
            else if (e.KeyChar == 'r')
            {
                if (!overlap_check(x, y, true))
                {
                    for (int j = 0; j < 4; j++)
                        for (int i = 0; i < 4; i++)
                            if (block_n[current_block, j, i] == 1)
                                pictureBox[y + j, x + i].BackColor = Color.White;

                    int[,] arr = rotateBlockL();
                    for (int j = 0; j < 4; j++) for (int i = 0; i < 4; i++) block_n[current_block, j, i] = arr[j, i];

                    for (int j = 0; j < 4; j++)
                        for (int i = 0; i < 4; i++)
                            if (block_n[current_block, j, i] == 1)
                                pictureBox[y + j, x + i].BackColor = block_color[current_block + 1];

                }
            }
        }
    }
}