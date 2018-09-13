using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace CS_calculator
{
    public partial class Form1 : Form
    {
        private List<string> op = new List<string> { };
        private List<string> type = new List<string> { };
        //helper for CE(back), length of each string
        private List<int> index = new List<int> { };
        private HashSet<string> ops =
            new HashSet<string> { "+", "-", "*", "/" };
        private Dictionary<string, int> prio = new Dictionary<string, int>
            { { "+", 1 }, { "-", 1 }, { "*", 2 }, { "/", 2 }, { "(", 3 }, { ")", 0 } };

        private bool prev_ans = false;

        private double num_prev = 0;
        private bool is_num = false;
        //in case that the input is 000.1, so record the previous '0'
        //and the zero_num is 3, but!!!!
        //it should be -1 if the num is parsed.
        //(because the num is "0.1", so zero_num - 1 ).
        private int zero_num = 0;

        private int zero_after_dot = 0;

        private bool dot = false;
        private int digit_after_dot = 0;

        private bool next_id = true;  //num, left_parenthesis 
        private int left_right = 0;   //left_p - right_p

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {
            InputNum("1");
        }
        private void button2_Click(object sender, EventArgs e)
        {
            InputNum("2");
        }
        private void button3_Click(object sender, EventArgs e)
        {
            InputNum("3");
        }
        private void button4_Click(object sender, EventArgs e)
        {
            InputNum("4");
        }
        private void button5_Click(object sender, EventArgs e)
        {
            InputNum("5");
        }
        private void button6_Click(object sender, EventArgs e)
        {
            InputNum("6");
        }
        private void button7_Click(object sender, EventArgs e)
        {
            InputNum("7");
        }
        private void button8_Click(object sender, EventArgs e)
        {
            InputNum("8");
        }
        private void button9_Click(object sender, EventArgs e)
        {
            InputNum("9");
        }
        private void button10_Click(object sender, EventArgs e)
        {
            InputNum("0");
        }


        private string zero_generator(int zero_num)
        {
            string str = "";
            for (int i = 0; i < zero_num; i++)
                str += "0";
            return str;
        }

        private void reduce_num()
        {
            if (is_num)
            {
                if (Math.Truncate(num_prev) == 0) zero_num--;
                op.Add(zero_generator(zero_num) + num_prev.ToString()); index.Add(num_prev.ToString().Length + zero_num); type.Add("num");
                if (zero_after_dot > 0)
                {
                    string tmp = op.ElementAt(op.Count() - 1);
                    op.RemoveAt(op.Count() - 1);
                    int i_tmp = index.ElementAt(index.Count() - 1);
                    index.RemoveAt(index.Count() - 1);
                    if (Math.Truncate(num_prev) == num_prev)
                    {
                        op.Add(tmp + "." + zero_generator(zero_after_dot));
                        index.Add(i_tmp + 1 + zero_after_dot);
                    }
                    else
                    {
                        op.Add(tmp + zero_generator(zero_after_dot));
                        index.Add(i_tmp + zero_after_dot);
                    }
                }
                is_num = false;
                dot = false;
                zero_num = 0;
                zero_after_dot = 0;
                digit_after_dot = 0;
                num_prev = 0.0;
            }
        }

        //_ty == "num" back track the num
        //pop the num from the op, etc.
        //and enable it to accept more digit
        private void back_track_num(string _ty)
        {
            if (_ty == "num")
            {
                is_num = true;
                string num = op.ElementAt(op.Count() - 1);
                int _index = index.ElementAt(index.Count() - 1);
                num_prev = Double.Parse(num);

                int len = num.Length;
                //reset the 'dot' and 'digit_after_dot'
                int i = 0;
                for (; i < len; i++)
                {
                    if (num[i] == '.') break;
                }
                if (i == len)
                {
                    dot = false;
                    digit_after_dot = 0;
                }
                else
                {
                    dot = true;
                    digit_after_dot = len - i - 1;
                }

                zero_num = 0;
                for (i = 0; i < len; i++)
                {
                    if (num[i] != '0') break;
                    zero_num++;
                }

                zero_after_dot = 0;
                if (dot)
                {
                    /*
                    if (num_prev == Math.Truncate(num_prev))
                        zero_after_dot = digit_after_dot;
                        */
                    for (i = len - 1; i >= 0; i--)
                    {
                        if (num[i] != '0') break;
                        zero_after_dot++;
                    }

                }

                op.RemoveAt(op.Count() - 1); type.RemoveAt(type.Count() - 1); index.RemoveAt(index.Count() - 1);
            }
        }

        //"CE" button, track back the status
        private void button11_Click(object sender, EventArgs e)
        {
            if (op.Count() == 0)
            {
                clear();
                return;
            }
            //if is num,
            //stop the num counting, and push it into op, etc.
            //then delete it, and reset the status
            reduce_num();

            //remove the last in the textBox
            textBox1.Text = textBox1.Text.Substring(0, textBox1.Text.Length - index.Last());

            string ty = type.ElementAt(type.Count() - 1);

            //remove the last element
            op.RemoveAt(op.Count() - 1);
            type.RemoveAt(type.Count() - 1);
            index.RemoveAt(index.Count() - 1);

            //extra work
            if (op.Count() == 0)
            {
                clear();
                return;
            }


            //status back track

            if (ty == "num")
            {
                next_id = true;
                return;
            }
            if (ty == "(")
            {
                next_id = true;  //accept (, num
                left_right--;
                return;
            }
            if (ty == ")")
            {
                //next_id = false;   //no need because is 'false' already
                left_right++;
                string _ty = type.ElementAt(type.Count() - 1);
                //if (_ty == ")") //do nothing

                //_ty == "num" back track the num
                //pop the num from the op, etc.
                //and enable it to accept more digit
                back_track_num(_ty);

                return;
            }
            if (ty == ".")
            {
                dot = false;
                return;
            }

            //ty == +-*/    ..........................................
            next_id = false;
            string _ty2 = type.ElementAt(type.Count() - 1);
            //if (_ty == ")") //do nothing

            //_ty == "num" back track the num
            back_track_num(_ty2);

        }

        //main function to control the whole logic
        private void InputNum(string value)
        {

            //"(" and nums
            if (next_id)
            {
                //"("
                if (value == "(")
                {
                    left_right++;
                    op.Add(value); index.Add(1); type.Add("(");
                    textBox1.Text += value;
                }
                //dot and nums
                else if (!ops.Contains(value))
                {
                    if (value == ")") return;
                    next_id = false;
                    if (value == ".")
                    {
                        dot = true;
                        is_num = true;
                        digit_after_dot = 0;
                        zero_num = 0;
                        textBox1.Text += value;
                        return;
                    }
                    if (value == "0") zero_num++;
                    num_prev = num_prev * 10.0 + Double.Parse(value);
                    is_num = true;
                    textBox1.Text += value;
                }
            }
            //accept ")" and ops and !!! succeed nums !!!
            else
            {
                if (value == "(") return;
                //")"
                if (value == ")")
                {
                    if (left_right <= 0) return;

                    reduce_num();

                    left_right--;
                    op.Add(value); index.Add(1); type.Add(")");
                    textBox1.Text += value;
                    return;
                }

                //+-*/
                if (ops.Contains(value))
                {

                    reduce_num();

                    next_id = true;
                    op.Add(value); index.Add(1); type.Add(value);
                    prev_ans = false;
                    textBox1.Text += value;
                    return;
                }

                //succeed nums
                if (op.Count() > 0 && op.Last() == ")") return;
                if (prev_ans) return;
                if (dot)
                {
                    if (value == ".") return;
                    digit_after_dot++;
                    if (value == "0") zero_after_dot++;
                    else zero_after_dot = 0;
                    num_prev += Math.Pow(10, -digit_after_dot) * Double.Parse(value);
                    textBox1.Text += value;
                }
                else
                {
                    if (value == ".")
                    {
                        dot = true;
                        digit_after_dot = 0;
                    }
                    else
                    {
                        if (value == "0" && num_prev == 0) zero_num++;
                        num_prev = num_prev * 10.0 + Double.Parse(value);
                    }
                    textBox1.Text += value;
                }
            }


        }

        private double calc(double num1, double num2, string op)
        {
            switch (op)
            {
                case "+":
                    return num1 + num2;
                case "-":
                    return num1 - num2;
                case "*":
                    return num1 * num2;
                case "/":
                    if (num2 == 0.0)
                    {
                        DialogResult dr;
                        dr = MessageBox.Show("divided by 0", "error", MessageBoxButtons.OK,
                MessageBoxIcon.Warning, MessageBoxDefaultButton.Button1);
                        return 0;
                    }
                    return num1 / num2;
                default:
                    return 0.0;
            }

        }

        private void clear()
        {
            textBox1.Text = "";

            op.Clear();
            type.Clear();
            index.Clear();

            prev_ans = false;

            num_prev = 0;
            is_num = false;
            zero_num = 0;
            zero_after_dot = 0;

            dot = false;
            digit_after_dot = 0;

            next_id = true;
            left_right = 0;
        }

        private void button20_Click(object sender, EventArgs e)
        {
            clear();
        }
        private void button12_Click(object sender, EventArgs e)
        {
            InputNum("+");
        }
        private void button13_Click(object sender, EventArgs e)
        {
            InputNum("-");
        }
        private void button14_Click(object sender, EventArgs e)
        {
            InputNum("*");
        }
        private void button15_Click(object sender, EventArgs e)
        {
            InputNum("/");
        }
        private void button16_Click(object sender, EventArgs e)
        {
            InputNum("(");
        }
        private void button17_Click(object sender, EventArgs e)
        {
            InputNum(")");
        }
        private void button18_Click(object sender, EventArgs e)
        {
            InputNum(".");
        }

        //button "=" do calculation
        private void button19_Click(object sender, EventArgs e)
        {
            if (left_right > 0 || next_id) return;

            if (is_num)
            {
                op.Add(num_prev.ToString()); index.Add(num_prev.ToString().Length); type.Add("num");
                is_num = false;
                zero_num = 0;
                dot = false;
                digit_after_dot = 0;
                num_prev = 0.0;
            }

            int size = op.Count();

            //infix 2 postfix
            List<string> stk = new List<string> { };//postfix
            List<string> _type = new List<string> { };
            Stack<string> tmp = new Stack<string> { };//temp operand
            for (int i = 0; i < size; ++i)
            {
                string str = op.ElementAt(i);
                string ty = type.ElementAt(i);
                if (ty == "num")
                {
                    stk.Add(str);
                    _type.Add(ty);
                }
                else //str == ty
                {
                    if (tmp.Count() == 0)
                    {
                        tmp.Push(str);
                        continue;
                    }
                    if (prio[str] > prio[tmp.Peek()])
                    {
                        tmp.Push(str);
                        continue;
                    }
                    if (ty == ")")
                    {
                        while (tmp.Peek() != "(")
                        {
                            string sstr = tmp.Pop();
                            //except (, )
                            stk.Add(sstr);
                            _type.Add(sstr);
                        }
                        tmp.Pop();
                        continue;
                    }
                    while (tmp.Count() != 0 && tmp.Peek() != "(" && prio[str] <= prio[tmp.Peek()])
                    {
                        string sstr = tmp.Pop();
                        stk.Add(sstr);
                        _type.Add(sstr);
                    }
                    tmp.Push(str);
                }
            }
            while (tmp.Count() > 0)
            {
                string sstr = tmp.Pop();
                stk.Add(sstr);
                _type.Add(sstr);
            }

            //calculate postfix
            Stack<double> sequence = new Stack<double> { };
            int len = stk.Count();
            for (int i = 0; i < len; i++)
            {
                string str = stk.ElementAt(i);
                string ty = _type.ElementAt(i);
                if (ty == "num")
                    sequence.Push(Double.Parse(str));
                else
                {
                    double num2 = sequence.Pop();
                    double num1 = sequence.Pop();
                    sequence.Push(calc(num1, num2, ty));
                }
            }
            double ans = sequence.Pop();
            clear();
            textBox1.Text = ans.ToString();
            next_id = false;
            prev_ans = true;
            op.Add(ans.ToString()); index.Add(ans.ToString().Length); type.Add("num");
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {


        }

    }
}
