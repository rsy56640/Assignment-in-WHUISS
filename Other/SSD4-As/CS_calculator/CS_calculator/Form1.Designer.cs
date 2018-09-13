using System.Drawing;
using System.Drawing.Text;
using System.Windows.Forms;
using System;

namespace CS_calculator
{
    partial class Form1
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要修改
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.button1 = new ButtonEx();
            //new System.Windows.Forms.Button();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.button2 = new ButtonEx();
            //new System.Windows.Forms.Button();
            this.button3 = new ButtonEx();
            //new System.Windows.Forms.Button();
            this.button4 = new ButtonEx();
            //new System.Windows.Forms.Button();
            this.button5 = new ButtonEx();
            //new System.Windows.Forms.Button();
            this.button6 = new ButtonEx();
            //new System.Windows.Forms.Button();
            this.button7 = new ButtonEx();
            //new System.Windows.Forms.Button();
            this.button8 = new ButtonEx();
            //new System.Windows.Forms.Button();
            this.button9 = new ButtonEx();
            //new System.Windows.Forms.Button();
            this.button10 = new ButtonEx();
            //new System.Windows.Forms.Button();
            this.button11 = new ButtonEx();
            //new System.Windows.Forms.Button();
            this.button12 = new ButtonEx();
            //new System.Windows.Forms.Button();
            this.button13 = new ButtonEx();
            //new System.Windows.Forms.Button();
            this.button14 = new ButtonEx();
            //new System.Windows.Forms.Button();
            this.button15 = new ButtonEx();
            //new System.Windows.Forms.Button();
            this.button16 = new ButtonEx();
            //new System.Windows.Forms.Button();
            this.button17 = new ButtonEx();
            //new System.Windows.Forms.Button();
            this.button18 = new ButtonEx();
            //new System.Windows.Forms.Button();
            this.button19 = new ButtonEx();
            //new System.Windows.Forms.Button();
            this.button20 = new ButtonEx();
            //new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // button1
            // 
            this.button1.BackColor = System.Drawing.SystemColors.Control;
            this.button1.FlatAppearance.BorderSize = 0;
            this.button1.Location = new System.Drawing.Point(28, 309);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(75, 40);
            this.button1.TabIndex = 0;
            this.button1.Text = "1";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // textBox1
            // 
            this.textBox1.Font = new System.Drawing.Font("楷体", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.textBox1.Location = new System.Drawing.Point(28, 37);
            this.textBox1.Multiline = true;
            this.textBox1.Name = "textBox1";
            this.textBox1.ReadOnly = true;
            this.textBox1.Size = new System.Drawing.Size(522, 56);
            this.textBox1.TabIndex = 2;
            this.textBox1.TextChanged += new System.EventHandler(this.textBox1_TextChanged);
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(141, 309);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(75, 40);
            this.button2.TabIndex = 3;
            this.button2.Text = "2";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.button2_Click);
            // 
            // button3
            // 
            this.button3.Location = new System.Drawing.Point(258, 309);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(75, 40);
            this.button3.TabIndex = 4;
            this.button3.Text = "3";
            this.button3.UseVisualStyleBackColor = true;
            this.button3.Click += new System.EventHandler(this.button3_Click);
            // 
            // button4
            // 
            this.button4.Location = new System.Drawing.Point(28, 228);
            this.button4.Name = "button4";
            this.button4.Size = new System.Drawing.Size(75, 40);
            this.button4.TabIndex = 5;
            this.button4.Text = "4";
            this.button4.UseVisualStyleBackColor = true;
            this.button4.Click += new System.EventHandler(this.button4_Click);
            // 
            // button5
            // 
            this.button5.Location = new System.Drawing.Point(141, 228);
            this.button5.Name = "button5";
            this.button5.Size = new System.Drawing.Size(75, 40);
            this.button5.TabIndex = 6;
            this.button5.Text = "5";
            this.button5.UseVisualStyleBackColor = true;
            this.button5.Click += new System.EventHandler(this.button5_Click);
            // 
            // button6
            // 
            this.button6.Location = new System.Drawing.Point(258, 228);
            this.button6.Name = "button6";
            this.button6.Size = new System.Drawing.Size(75, 40);
            this.button6.TabIndex = 7;
            this.button6.Text = "6";
            this.button6.UseVisualStyleBackColor = true;
            this.button6.Click += new System.EventHandler(this.button6_Click);
            // 
            // button7
            // 
            this.button7.Location = new System.Drawing.Point(28, 147);
            this.button7.Name = "button7";
            this.button7.Size = new System.Drawing.Size(75, 40);
            this.button7.TabIndex = 8;
            this.button7.Text = "7";
            this.button7.UseVisualStyleBackColor = true;
            this.button7.Click += new System.EventHandler(this.button7_Click);
            // 
            // button8
            // 
            this.button8.Location = new System.Drawing.Point(141, 147);
            this.button8.Name = "button8";
            this.button8.Size = new System.Drawing.Size(75, 40);
            this.button8.TabIndex = 9;
            this.button8.Text = "8";
            this.button8.UseVisualStyleBackColor = true;
            this.button8.Click += new System.EventHandler(this.button8_Click);
            // 
            // button9
            // 
            this.button9.Location = new System.Drawing.Point(258, 147);
            this.button9.Name = "button9";
            this.button9.Size = new System.Drawing.Size(75, 40);
            this.button9.TabIndex = 10;
            this.button9.Text = "9";
            this.button9.UseVisualStyleBackColor = true;
            this.button9.Click += new System.EventHandler(this.button9_Click);
            // 
            // button10
            // 
            this.button10.Location = new System.Drawing.Point(28, 388);
            this.button10.Name = "button10";
            this.button10.Size = new System.Drawing.Size(75, 40);
            this.button10.TabIndex = 11;
            this.button10.Text = "0";
            this.button10.UseVisualStyleBackColor = true;
            this.button10.Click += new System.EventHandler(this.button10_Click);
            // 
            // button11
            // 
            this.button11.Location = new System.Drawing.Point(258, 388);
            this.button11.Name = "button11";
            this.button11.Size = new System.Drawing.Size(75, 40);
            this.button11.TabIndex = 12;
            this.button11.Text = "CE";
            this.button11.UseVisualStyleBackColor = true;
            this.button11.Click += new System.EventHandler(this.button11_Click);
            // 
            // button12
            // 
            this.button12.Location = new System.Drawing.Point(372, 147);
            this.button12.Name = "button12";
            this.button12.Size = new System.Drawing.Size(75, 40);
            this.button12.TabIndex = 13;
            this.button12.Text = "+";
            this.button12.UseVisualStyleBackColor = true;
            this.button12.Click += new System.EventHandler(this.button12_Click);
            // 
            // button13
            // 
            this.button13.Location = new System.Drawing.Point(475, 147);
            this.button13.Name = "button13";
            this.button13.Size = new System.Drawing.Size(75, 40);
            this.button13.TabIndex = 14;
            this.button13.Text = "-";
            this.button13.UseVisualStyleBackColor = true;
            this.button13.Click += new System.EventHandler(this.button13_Click);
            // 
            // button14
            // 
            this.button14.Location = new System.Drawing.Point(372, 228);
            this.button14.Name = "button14";
            this.button14.Size = new System.Drawing.Size(75, 40);
            this.button14.TabIndex = 15;
            this.button14.Text = "*";
            this.button14.UseVisualStyleBackColor = true;
            this.button14.Click += new System.EventHandler(this.button14_Click);
            // 
            // button15
            // 
            this.button15.Location = new System.Drawing.Point(475, 228);
            this.button15.Name = "button15";
            this.button15.Size = new System.Drawing.Size(75, 40);
            this.button15.TabIndex = 16;
            this.button15.Text = "/";
            this.button15.UseVisualStyleBackColor = true;
            this.button15.Click += new System.EventHandler(this.button15_Click);
            // 
            // button16
            // 
            this.button16.Location = new System.Drawing.Point(372, 309);
            this.button16.Name = "button16";
            this.button16.Size = new System.Drawing.Size(75, 40);
            this.button16.TabIndex = 17;
            this.button16.Text = "(";
            this.button16.UseVisualStyleBackColor = true;
            this.button16.Click += new System.EventHandler(this.button16_Click);
            // 
            // button17
            // 
            this.button17.Location = new System.Drawing.Point(475, 309);
            this.button17.Name = "button17";
            this.button17.Size = new System.Drawing.Size(75, 40);
            this.button17.TabIndex = 18;
            this.button17.Text = ")";
            this.button17.UseVisualStyleBackColor = true;
            this.button17.Click += new System.EventHandler(this.button17_Click);
            // 
            // button18
            // 
            this.button18.Location = new System.Drawing.Point(141, 388);
            this.button18.Name = "button18";
            this.button18.Size = new System.Drawing.Size(75, 40);
            this.button18.TabIndex = 19;
            this.button18.Text = ".";
            this.button18.UseVisualStyleBackColor = true;
            this.button18.Click += new System.EventHandler(this.button18_Click);
            // 
            // button19
            // 
            this.button19.Location = new System.Drawing.Point(475, 388);
            this.button19.Name = "button19";
            this.button19.Size = new System.Drawing.Size(75, 40);
            this.button19.TabIndex = 20;
            this.button19.Text = "=";
            this.button19.UseVisualStyleBackColor = true;
            this.button19.Click += new System.EventHandler(this.button19_Click);
            // 
            // button20
            // 
            this.button20.Location = new System.Drawing.Point(372, 388);
            this.button20.Name = "button20";
            this.button20.Size = new System.Drawing.Size(75, 40);
            this.button20.TabIndex = 21;
            this.button20.Text = "C";
            this.button20.UseVisualStyleBackColor = true;
            this.button20.Click += new System.EventHandler(this.button20_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.Control;
            this.ClientSize = new System.Drawing.Size(575, 460);
            this.Controls.Add(this.button20);
            this.Controls.Add(this.button19);
            this.Controls.Add(this.button18);
            this.Controls.Add(this.button17);
            this.Controls.Add(this.button16);
            this.Controls.Add(this.button15);
            this.Controls.Add(this.button14);
            this.Controls.Add(this.button13);
            this.Controls.Add(this.button12);
            this.Controls.Add(this.button11);
            this.Controls.Add(this.button10);
            this.Controls.Add(this.button9);
            this.Controls.Add(this.button8);
            this.Controls.Add(this.button7);
            this.Controls.Add(this.button6);
            this.Controls.Add(this.button5);
            this.Controls.Add(this.button4);
            this.Controls.Add(this.button3);
            this.Controls.Add(this.button2);
            this.Controls.Add(this.textBox1);
            this.Controls.Add(this.button1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.Name = "Form1";
            this.Text = "Calculator";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.Button button3;
        private System.Windows.Forms.Button button4;
        private System.Windows.Forms.Button button5;
        private System.Windows.Forms.Button button6;
        private System.Windows.Forms.Button button7;
        private System.Windows.Forms.Button button8;
        private System.Windows.Forms.Button button9;
        private System.Windows.Forms.Button button10;
        private System.Windows.Forms.Button button11;
        private System.Windows.Forms.Button button12;
        private System.Windows.Forms.Button button13;
        private System.Windows.Forms.Button button14;
        private System.Windows.Forms.Button button15;
        private System.Windows.Forms.Button button16;
        private System.Windows.Forms.Button button17;
        private System.Windows.Forms.Button button18;
        private System.Windows.Forms.Button button19;
        private System.Windows.Forms.Button button20;
    }


    public partial class ButtonEx : Button
    {
        public ButtonEx()
        {
            //双缓冲的一大堆设置 具体参数含义参照msdn的ControlStyles枚举值
            this.SetStyle(ControlStyles.UserPaint, true);
            this.SetStyle(ControlStyles.ResizeRedraw, true);
            this.SetStyle(ControlStyles.AllPaintingInWmPaint, true);
            this.SetStyle(ControlStyles.OptimizedDoubleBuffer, true);
            this.SetStyle(ControlStyles.SupportsTransparentBackColor, true);
            this.BackColor = Color.Transparent;
        }

        private bool m_bMouseHover;
        private bool m_bMouseDown;

        private static void RenderBackground(Graphics g, Image img, Rectangle rect)
        {
            //填充四个角
            g.DrawImage(img, new Rectangle(rect.X, rect.Y, 5, 5),
                new Rectangle(0, 0, 5, 5), GraphicsUnit.Pixel);
            g.DrawImage(img, new Rectangle(rect.Right - 5, rect.Y, 5, 5),
                new Rectangle(img.Width - 5, 0, 5, 5), GraphicsUnit.Pixel);
            g.DrawImage(img, new Rectangle(rect.X, rect.Bottom - 5, 5, 5),
                new Rectangle(0, img.Height - 5, 5, 5), GraphicsUnit.Pixel);
            g.DrawImage(img, new Rectangle(rect.Right - 5, rect.Bottom - 5, 5, 5),
                new Rectangle(img.Width - 5, img.Height - 5, 5, 5), GraphicsUnit.Pixel);
            //四边
            g.DrawImage(img, new Rectangle(rect.X, rect.Y + 5, 5, rect.Height - 10),
                new Rectangle(0, 5, 5, img.Height - 10), GraphicsUnit.Pixel);
            g.DrawImage(img, new Rectangle(rect.X + 5, rect.Y, rect.Width - 10, 5),
                new Rectangle(5, 0, img.Width - 10, 5), GraphicsUnit.Pixel);
            g.DrawImage(img, new Rectangle(rect.Right - 5, rect.Y + 5, 5, rect.Height - 10),
                new Rectangle(img.Width - 5, 5, 5, img.Height - 10), GraphicsUnit.Pixel);
            g.DrawImage(img, new Rectangle(rect.X + 5, rect.Bottom - 5, rect.Width - 10, 5),
                new Rectangle(5, img.Height - 5, img.Width - 10, 5), GraphicsUnit.Pixel);
            //中间
            g.DrawImage(img,
                new Rectangle(rect.X + 5, rect.Y + 5, rect.Width - 10, rect.Height - 10),
                new Rectangle(5, 5, img.Width - 10, img.Height - 10), GraphicsUnit.Pixel);

        }

        protected override void OnMouseEnter(System.EventArgs e)
        {
            m_bMouseHover = true;
            this.Invalidate();
            base.OnMouseEnter(e);
        }
        protected override void OnMouseLeave(EventArgs e)
        {
            m_bMouseHover = false;
            this.Invalidate();
            base.OnMouseLeave(e);
        }

        protected override void OnMouseDown(MouseEventArgs mevent)
        {
            m_bMouseDown = true;
            this.Invalidate();
            base.OnMouseDown(mevent);
        }

        protected override void OnMouseUp(MouseEventArgs mevent)
        {
            m_bMouseDown = false;
            this.Invalidate();
            base.OnMouseUp(mevent);
        }

        protected override void OnPaint(PaintEventArgs pevent)
        {
            base.OnPaint(pevent);
            //因为上面调用了base会绘制原生控件 重刷一下背景清掉原生绘制 不然自己绘制的是重叠在原生绘制上
            base.OnPaintBackground(pevent);
            Graphics g = pevent.Graphics;

            StringFormat sf = new StringFormat();
            sf.Alignment = StringAlignment.Center;
            sf.LineAlignment = StringAlignment.Center;
            //处理热键 当Alt点下时
            sf.HotkeyPrefix = this.ShowKeyboardCues ? HotkeyPrefix.Show : HotkeyPrefix.Hide;
            //判断使用什么资源图
            //Bitmap 
            System.Drawing.Bitmap bmpDraw = Properties.Resources.Qbtn_Normal;
            if (!this.Enabled) bmpDraw = Properties.Resources.Qbtn_Gray;
            else if (m_bMouseDown) bmpDraw = Properties.Resources.Qbtn_Down;
            else if (m_bMouseHover) bmpDraw = Properties.Resources.Qbtn_High;
            else if (this.Focused) bmpDraw = Properties.Resources.Qbtn_Focus;
            //绘制背景(若不知道这句啥意思 参照九宫切图里面的代码)
            RenderBackground(g, bmpDraw, this.ClientRectangle);
            if (!this.Enabled)
            {
                //绘制双重阴影文字
                g.DrawString(this.Text, this.Font, Brushes.White, this.ClientRectangle, sf);
                g.TranslateTransform(-1, -1);//左上移动一个单位坐标系
                g.DrawString(this.Text, this.Font, Brushes.DarkGray, this.ClientRectangle, sf);
                g.ResetTransform();
                return;
            }
            using (SolidBrush sb = new SolidBrush(this.ForeColor))
            {
                g.DrawString(this.Text, this.Font, sb, this.ClientRectangle, sf);
            }
        }
    }

}

