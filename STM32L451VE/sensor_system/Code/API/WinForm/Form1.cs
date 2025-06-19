using System;
using System.Windows.Forms;
using System.IO.Ports;
using System.Collections.ObjectModel; // �߰��� ����
using System.Threading.Tasks; // �߰��� ����

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
            // ���õ� ��Ʈ ����
            if (cb_serial_port.SelectedItem != null)
            {
                selectedPort = cb_serial_port.SelectedItem.ToString();
                // ���⿡ ���õ� ��Ʈ�� ���� �߰� �۾��� �����ϸ� �˴ϴ�.
                MessageBox.Show("Selected Port: " + selectedPort);
            }
        }
        private void UpdateSerialPortListAndScroll()
        {
            // ���� ���õ� ��Ʈ�� ����
            string selectedPortBeforeUpdate = selectedPort;

            // �޺� �ڽ� �ʱ�ȭ
            this.cb_serial_port.Items.Clear();

            // ��Ʈ ��� �����ͼ� �޺� �ڽ��� �߰�
            string[] serial_list = SerialPort.GetPortNames();
            foreach (string name in serial_list)
            {
                this.cb_serial_port.Items.Add(name);
            }

            // ������ ������ ��Ʈ�� ��Ͽ� ������ �ٽ� ����
            if (!string.IsNullOrEmpty(selectedPortBeforeUpdate) &&
                this.cb_serial_port.Items.Contains(selectedPortBeforeUpdate))
            {
                this.cb_serial_port.SelectedItem = selectedPortBeforeUpdate;
            }

            // ���õ� ��Ʈ�� ���̵��� ��ũ�� ��ġ�� ����
            if (this.cb_serial_port.SelectedItem != null)
            {
                int selectedIndex = this.cb_serial_port.SelectedIndex;
                //this.cb_serial_port.SelectedIndex = -1; // ��ũ�� ������ ���� ���� ����
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