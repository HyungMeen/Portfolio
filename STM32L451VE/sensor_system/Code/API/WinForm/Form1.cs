using System;
using System.Windows.Forms;
using System.IO.Ports;
using System.Collections.ObjectModel; // 추가된 라인
using System.Threading.Tasks; // 추가된 라인

namespace Winform_Test
{
    public partial class Form1 : Form
    {
        private string selectedPort;

        public Form1()
        {
            InitializeComponent();
            this.cb_serial_port.DropDown += new System.EventHandler(comboBox2_deviceDropDownEvent);
            this.cb_serial_port.SelectedIndexChanged += new System.EventHandler(comboBox2_SelectedIndexChanged);
        }

        private void button_start_Click(object sender, EventArgs e)
        {
            textBox_text1.Text = "hello world";
            progressBar_test.Value = 50;
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void textBox_text1_TextChanged(object sender, EventArgs e)
        {

        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void comboBox2_deviceDropDownEvent(object sender, EventArgs e)
        {
            this.cb_serial_port.Items.Clear();

            string[] serial_list = SerialPort.GetPortNames();

            foreach (string name in serial_list)
            {
                this.cb_serial_port.Items.Add(name);
            }
            UpdateSerialPortListAndScroll();
        }
        private void comboBox2_SelectedIndexChanged(object sender, EventArgs e)
        {
            // 선택된 포트 저장
            if (cb_serial_port.SelectedItem != null)
            {
                selectedPort = cb_serial_port.SelectedItem.ToString();
                // 여기에 선택된 포트에 대한 추가 작업을 수행하면 됩니다.
                MessageBox.Show("Selected Port: " + selectedPort);
            }
        }
        private void UpdateSerialPortListAndScroll()
        {
            // 현재 선택된 포트를 저장
            string selectedPortBeforeUpdate = selectedPort;

            // 콤보 박스 초기화
            this.cb_serial_port.Items.Clear();

            // 포트 목록 가져와서 콤보 박스에 추가
            string[] serial_list = SerialPort.GetPortNames();
            foreach (string name in serial_list)
            {
                this.cb_serial_port.Items.Add(name);
            }

            // 이전에 선택한 포트가 목록에 있으면 다시 선택
            if (!string.IsNullOrEmpty(selectedPortBeforeUpdate) &&
                this.cb_serial_port.Items.Contains(selectedPortBeforeUpdate))
            {
                this.cb_serial_port.SelectedItem = selectedPortBeforeUpdate;
            }

            // 선택된 포트가 보이도록 스크롤 위치를 조정
            if (this.cb_serial_port.SelectedItem != null)
            {
                int selectedIndex = this.cb_serial_port.SelectedIndex;
                //this.cb_serial_port.SelectedIndex = -1; // 스크롤 조정을 위해 선택 해제
                this.cb_serial_port.SelectedIndex = selectedIndex;
            }
        }

        private void label1_Click_1(object sender, EventArgs e)
        {

        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {

        }

        private void comboBox2_SelectedIndexChanged_1(object sender, EventArgs e)
        {

        }

        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {

        }

        private void button1_stop_Click(object sender, EventArgs e)
        {

        }

        private void label1_Click_2(object sender, EventArgs e)
        {

        }

        private void radioButton2_CheckedChanged(object sender, EventArgs e)
        {

        }

        private void radioButton1_CheckedChanged(object sender, EventArgs e)
        {

        }

        private void label4_Click(object sender, EventArgs e)
        {

        }

        private void label5_Click(object sender, EventArgs e)
        {

        }

        private void label5_Click_1(object sender, EventArgs e)
        {

        }

        private void panel1_Paint(object sender, PaintEventArgs e)
        {

        }

        private void label2_Click(object sender, EventArgs e)
        {

        }

        private void label_temp_Click(object sender, EventArgs e)
        {
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {
        }

        private void label8_Click(object sender, EventArgs e)
        {

        }

        private void label9_current_temp_Click(object sender, EventArgs e)
        {

        }
    }
}