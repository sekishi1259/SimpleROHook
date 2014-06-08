using System;
using System.ComponentModel;
using System.Windows.Forms;


using System.IO;
using System.Xml;
using System.Xml.Serialization;

namespace SimpleROHookCS
{

    public partial class MainForm : Form,IDisposable
    {
        private SRHSharedData m_SharedData = null;

        public MainForm()
        {
            InitializeComponent();

            m_SharedData = new SRHSharedData();

            string curentdirstr = System.IO.Directory.GetCurrentDirectory() + "\\config.ini";
            m_SharedData.configfilepath = curentdirstr;
            if (File.Exists("config.xml"))
            {
                #region Load Config XML
                using (XmlReader reader = XmlReader.Create("config.xml"))
                {
                    var configration = new Config();
                    var serializer = new XmlSerializer(typeof(Config));

                    configration = (Config)serializer.Deserialize(reader);

                    m_SharedData.write_packetlog
                        = configration.write_packetlog;
                    m_SharedData.freemouse
                        = configration.freemouse;
                    m_SharedData.m2e
                        = configration.m2e;
                    m_SharedData.m2e_zbias
                        = configration.m2e_zbias;
                    m_SharedData.fix_windowmode_vsyncwait
                        = configration.fix_windowmode_vsyncwait;
                    m_SharedData.show_framerate
                        = configration.show_framerate;
                    m_SharedData.objectinformation
                        = configration.objectinformation;
                    m_SharedData._44khz_audiomode
                        = configration._44khz_audiomode;
                    m_SharedData.cpucoolerlevel
                        = configration.cpucoolerlevel;
                }
                #endregion
            }
        }
        public new void Dispose()
        {
            #region Save Config XML
            using (XmlTextWriter writer = new XmlTextWriter("config.xml", System.Text.Encoding.UTF8))
            {
                var configration = new Config();
                var serializer = new XmlSerializer(typeof(Config));

                configration.write_packetlog
                    = m_SharedData.write_packetlog;
                configration.freemouse
                    = m_SharedData.freemouse;
                configration.m2e
                    = m_SharedData.m2e;
                configration.m2e_zbias
                    = m_SharedData.m2e_zbias;
                configration.fix_windowmode_vsyncwait
                    = m_SharedData.fix_windowmode_vsyncwait;
                configration.show_framerate
                    = m_SharedData.show_framerate;
                configration.objectinformation
                    = m_SharedData.objectinformation;
                configration._44khz_audiomode
                    = m_SharedData._44khz_audiomode;
                configration.cpucoolerlevel
                    = m_SharedData.cpucoolerlevel;

                writer.Formatting = Formatting.Indented;
                serializer.Serialize(writer, configration);
            }
            #endregion
            
            m_SharedData.Dispose();
            Dispose(true);
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            TaskTray_notifyIcon.Visible = false;
            Application.Exit();
        }

        private void playMusicOnClientStreamPlayerToolStripMenuItem_Click(object sender, EventArgs e)
        {
            string musicfilename = m_SharedData.musicfilename;
            var openFileDialog = new OpenFileDialog();
            openFileDialog.Title = "Ragnarok Online BGM";
            openFileDialog.InitialDirectory = musicfilename;
            openFileDialog.CheckFileExists = true;
            openFileDialog.Multiselect = false;
            openFileDialog.FileName = musicfilename;
            openFileDialog.Filter = "mp3 file|*.mp3|wave file|*.wav";
            openFileDialog.ShowReadOnly = false;
            if (openFileDialog.ShowDialog() == DialogResult.OK)
            {
                m_SharedData.musicfilename = openFileDialog.FileName;
                m_SharedData.executeorder = true;
            }
        }

        private void UpdateCheckMenu()
        {
            playMusicOnClientStreamPlayerToolStripMenuItem.Enabled
                = (m_SharedData.g_hROWindow == 0)? false : true;
            packetLogToolStripMenuItem.Checked
                = m_SharedData.write_packetlog;
            freeMouseToolStripMenuItem.Checked
                = m_SharedData.freemouse;

            showM2EToolStripMenuItem.Checked
                = m_SharedData.m2e;
            m2e_zbias_ToolStripTrackBar.Value
                = m_SharedData.m2e_zbias;

            fixWindowModeVsyncWaitToolStripMenuItem.Checked
                = m_SharedData.fix_windowmode_vsyncwait;
            showFpsToolStripMenuItem.Checked
                = m_SharedData.show_framerate;
            showObjectInformationToolStripMenuItem.Checked
                = m_SharedData.objectinformation;
            kHzAudioModeonBootToolStripMenuItem.Checked
                = m_SharedData._44khz_audiomode;
        }

        private void UpdateCoolerMenu()
        {
            int level = m_SharedData.cpucoolerlevel;
            offToolStripMenuItem.Checked = (level == 0) ? true : false;
            level1ToolStripMenuItem.Checked = (level == 1) ? true : false;
            level2ToolStripMenuItem.Checked = (level == 2) ? true : false;
            level3ToolStripMenuItem.Checked = (level == 3) ? true : false;
        }

        private void offToolStripMenuItem_Click(object sender, EventArgs e)
        {
            m_SharedData.cpucoolerlevel = 0;
            UpdateCoolerMenu();
        }

        private void level1ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            m_SharedData.cpucoolerlevel = 1;
            UpdateCoolerMenu();
        }

        private void level2ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            m_SharedData.cpucoolerlevel = 2;
            UpdateCoolerMenu();
        }

        private void level3ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            m_SharedData.cpucoolerlevel = 3;
            UpdateCoolerMenu();
        }

        private void kHzAudioModeonBootToolStripMenuItem_Click(object sender, EventArgs e)
        {
            var tsm = (ToolStripMenuItem)sender;
            m_SharedData._44khz_audiomode = tsm.Checked;
        }

        private void packetLogToolStripMenuItem_Click(object sender, EventArgs e)
        {
            var tsm = (ToolStripMenuItem)sender;
            m_SharedData.write_packetlog = tsm.Checked;
        }

        private void freeMouseToolStripMenuItem_Click(object sender, EventArgs e)
        {
            var tsm = (ToolStripMenuItem)sender;
            m_SharedData.freemouse = tsm.Checked;
        }

        private void showM2EToolStripMenuItem_Click(object sender, EventArgs e)
        {
            var tsm = (ToolStripMenuItem)sender;
            m_SharedData.m2e = tsm.Checked;
        }

        private void showFpsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            var tsm = (ToolStripMenuItem)sender;
            m_SharedData.show_framerate = tsm.Checked;
        }

        private void showObjectInformationToolStripMenuItem_Click(object sender, EventArgs e)
        {
            var tsm = (ToolStripMenuItem)sender;
            m_SharedData.objectinformation = tsm.Checked;
        }

        private void fixWindowModeVsyncWaitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            var tsm = (ToolStripMenuItem)sender;
            m_SharedData.fix_windowmode_vsyncwait = tsm.Checked;
        }

        private void aboutSimpleROHookToolStripMenuItem_Click(object sender, EventArgs e)
        {
            var aboutbox = new SRHAboutBox();
            aboutbox.ShowDialog();
        }

        private void TaskTray_contextMenuStrip_Opening(object sender, CancelEventArgs e)
        {
            m2e_zbias_ToolStripTrackBar.SetMinMax(0, 16);
            m2e_zbias_ToolStripTrackBar.SetTickFrequency(1);
            m2e_zbias_ToolStripTrackBar.SetChangeValue(1, 4);
            UpdateCheckMenu();
            UpdateCoolerMenu();
        }

        private void m2e_zbias_trackBarMenuItem_Click(object sender, EventArgs e)
        {

        }


    }

    public class Config
    {
        public Config()
        {
            write_packetlog = false;
            freemouse = false;
            m2e = true;
            m2e_zbias = 0;
            fix_windowmode_vsyncwait = true;
            show_framerate = true;
            objectinformation = false;
            _44khz_audiomode = false;
            cpucoolerlevel = 0;
        }

        public bool write_packetlog { get; set; }
        public bool freemouse { get; set; }
        public bool m2e { get; set; }
        public int  m2e_zbias { get; set; }
        public bool fix_windowmode_vsyncwait { get; set; }
        public bool show_framerate { get; set; }
        public bool objectinformation { get; set; }
        public bool _44khz_audiomode { get; set; }
        public int cpucoolerlevel { get; set; }

        // without serialize
        [System.Xml.Serialization.XmlIgnoreAttribute]
        public string configfilepath { get; set; }
        [System.Xml.Serialization.XmlIgnoreAttribute]
        public string musicfilename { get; set; }
        [System.Xml.Serialization.XmlIgnoreAttribute]
        public bool executeorder { get; set; }
    }

}
