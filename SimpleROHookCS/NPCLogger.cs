using System;
using System.Drawing;
using System.Runtime.InteropServices;
using System.Security.Permissions;
using System.Windows.Forms;

namespace SimpleROHookCS
{
    public partial class NPCLogger : Form
    {
        protected override CreateParams CreateParams
        {
            [SecurityPermission(SecurityAction.Demand, Flags = SecurityPermissionFlag.UnmanagedCode)]
            get
            {
                const int WS_EX_TOOLWINDOW = 0x80;
                const int CS_NOCLOSE = 0x0;//0x200;
                CreateParams cp = base.CreateParams;
                cp.ClassStyle = cp.ClassStyle | CS_NOCLOSE;
                cp.ExStyle = WS_EX_TOOLWINDOW;

                return cp;
            }
        }

        [StructLayout(LayoutKind.Sequential)]
        private struct COPYDATASTRUCT
        {
            public IntPtr dwData;
            public int cbData;
            public IntPtr lpData;
        }
        private const int WM_COPYDATA = 0x4A;

        private const int NPCLOGLINE_MAX = 30001;
        private int m_textcolor;

        public NPCLogger()
        {
            InitializeComponent();
            m_textcolor = 0;
        }

        private void AppendNPCMessage(string message)
        {
            int indextop = 0;

            richTextBox_LogText.Enabled = false;

            for (int ii = 0; ii < message.Length; ii++)
            {
                if (message[ii] == '^')
                {
                    if (indextop != ii)
                    {
                        richTextBox_LogText.SelectionColor = Color.FromArgb(m_textcolor);
                        richTextBox_LogText.AppendText(message.Substring(indextop, ii - indextop));
                    }
                    string colorhex = message.Substring(ii + 1, 6);
                    m_textcolor = Convert.ToInt32(colorhex, 16);
                    ii += 7;
                    indextop = ii;
                }
            }
            richTextBox_LogText.SelectionColor = Color.FromArgb(m_textcolor);
            richTextBox_LogText.AppendText(message.Substring(indextop));
            richTextBox_LogText.AppendText(Environment.NewLine);
            if (richTextBox_LogText.Lines.Length > NPCLOGLINE_MAX)
            {
                richTextBox_LogText.ReadOnly = false;
                richTextBox_LogText.Select(0, richTextBox_LogText.Lines[0].Length + 1);
                richTextBox_LogText.SelectedText = String.Empty;
                richTextBox_LogText.ReadOnly = true;
            }

            richTextBox_LogText.Enabled = true;
            richTextBox_LogText.Focus();
            richTextBox_LogText.ScrollToCaret();
        }

        protected override void WndProc(ref Message m)
        {
            if (m.Msg == WM_COPYDATA)
            {
                COPYDATASTRUCT data = new COPYDATASTRUCT();
                data = (COPYDATASTRUCT)Marshal.PtrToStructure(m.LParam, typeof(COPYDATASTRUCT));
                if (data.dwData == (IntPtr)COPYDATAENTRY.COPYDATA_NPCLogger && data.cbData > 0)
                {
                    char[] buffer = new char[data.cbData / 2];
                    Marshal.Copy(data.lpData, buffer, 0, data.cbData / 2);

                    string npcmessage = new string(buffer);
                    AppendNPCMessage(npcmessage);

                }
            }
            base.WndProc(ref m);
        }

        private void Window_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (Visible)
            {
                e.Cancel = true;
                this.Hide();
            }
        }
    }
}
