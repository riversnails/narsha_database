using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
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

        String[] block_name = new string[] {"block_white", "block_red", "block_blue", "block_yellow",
                                            "block_green", "block_orange","block_brown", "block_purple"};

        Color[] block_color = new Color[8]
        {
            Color.White,
            Color.Red,
            Color.Orange,
            Color.Yellow,
            Color.Green,
            Color.Blue,
            Color.Purple,
            Color.Black
        };

        int[,] block_L = new int[4, 4]
        {
            {0,0,0,0},
            {0,1,0,0},
            {0,1,1,1},
            {0,0,0,0}
        };

        PictureBox[,] pictureBox = new PictureBox[20, 10];  //Background
        int[,] block_state = new int[20, 10];   //Current block

        public Form1()
        {
            InitializeComponent();


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
                    this.Controls.Add(pictureBox[j, i]);
                }
            }
        }

        private void draw_block_L(int x, int y)
        {
            for (int j = 0; j < 4; j++)
            {
                for (int i = 0; i < 4; i++)
                {
                    if (block_L[j, i] == 1)
                    {
                        pictureBox[j + this.y, i + this.x].BackColor = block_color[0];
                    }
                }
            }
            this.y += y;
            this.x += x;

            for (int j = 0; j < 4; j++)
            {
                for (int i = 0; i < 4; i++)
                {
                    if (block_L[j, i] == 1)
                    {
                        pictureBox[j + this.y, i + this.x].BackColor = block_color[1];
                    }
                }
            }
        }

        private bool overlap_check(int x, int y)
        {
            for (int j = 0; j < 4; j++)
            {
                for (int i = 0; i < 4; i++)
                {
                    if (block_L[j, i] == 1)
                    {
                        if (x + i >= 10 || x + i < 0) return true;
                        if (y + j >= 20 || y + i < 0) return true;

                        if (block_state[j + y, i + x] != 0) return true;
                    }
                }
            }
            return false;
        }

        private void reset_block()
        {
            for (int j = 0; j < 20; j++)
                for (int i = 0; i < 10; i++)
                {
                    if (block_state[j, i] == 1) pictureBox[j, i].BackColor = block_color[1];
                    else pictureBox[j, i].BackColor = block_color[0];
                }
        }

        //private int rotateblockL()
        //{
        //    int[,] temp = new int[4, 4];

        //    for (int j = 0; j < 4; j++)
        //    {
        //        for (int i = 0; i < 4; i++)
        //        {
        //            temp[j, i] = block_L[i, 3 - j];
        //        }
        //    }
        //}

        private void event_1sec(Object myObject, EventArgs myEventArgs)
        {
            if (!overlap_check(x, y + 1))
            {
                draw_block_L(0, 1);
            }
            else  //바닥에 닿았을 때
            {
                for (int j = 0; j < 4; j++)
                {
                    for (int i = 0; i < 4; i++)
                    {
                        if (block_L[j, i] == 1)
                        {
                            block_state[j + y, i + x] = 1;
                        }
                    }
                }
                // ----------------------------
                reset_block();
                x = 4;
                y = 1;
            }
        }

        private void Form1_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar == 'a')
            {
                if (!overlap_check(x - 1, y))
                    draw_block_L(-1, 0);
            }
            else if (e.KeyChar == 'd')
            {
                if (!overlap_check(x + 1, y))
                    draw_block_L(1, 0);
            }
            else if (e.KeyChar == 'w')
            {
                if (!overlap_check(x, y - 1))
                    draw_block_L(0, -1);
            }
            else if (e.KeyChar == 's')
            {
                if (!overlap_check(x, y + 1))
                    draw_block_L(0, 1);
            }
            else if (e.KeyChar == 'R')
            {

            }
        }
    }
}