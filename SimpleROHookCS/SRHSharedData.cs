using System;

using System.Windows.Forms;
using System.IO.MemoryMappedFiles;
using System.Runtime.InteropServices;

namespace SimpleROHookCS
{
    unsafe class SRHSharedData : IDisposable
    {
        private const int MAX_PATH = 260;

        [StructLayout(LayoutKind.Sequential)]
        unsafe struct StSHAREDMEMORY
        {
            public int g_hROWindow;

            public int executeorder;

            public int write_packetlog;
            public int freemouse;
            public int m2e;
            public int m2e_zbias;
            public int fix_windowmode_vsyncwait;
            public int show_framerate;
            public int objectinformation;
            public int _44khz_audiomode;
            public int cpucoolerlevel;
            public fixed char configfilepath[MAX_PATH];
            public fixed char musicfilename[MAX_PATH];
        }

        private MemoryMappedFile m_Mmf = null;
        private MemoryMappedViewAccessor m_Accessor = null;
        private StSHAREDMEMORY* m_pSharedMemory;

        public SRHSharedData()
        {
            m_Mmf = MemoryMappedFile.CreateNew(@"SimpleROHook1009",
                Marshal.SizeOf(typeof(StSHAREDMEMORY)),
                MemoryMappedFileAccess.ReadWrite);
            if (m_Mmf == null)
                MessageBox.Show("CreateOrOpen MemoryMappedFile Failed.");
            m_Accessor = m_Mmf.CreateViewAccessor();

            byte* p = null;
            m_Accessor.SafeMemoryMappedViewHandle.AcquirePointer(ref p);
            m_pSharedMemory = (StSHAREDMEMORY*)p;

            write_packetlog = false;
            freemouse = false;
            m2e = false;
            m2e_zbias = 0;
            fix_windowmode_vsyncwait = false;
            show_framerate = false;
            objectinformation = false;
            _44khz_audiomode = false;
            cpucoolerlevel = 0;
            configfilepath = "";
            musicfilename = "";
            executeorder = false;
            g_hROWindow = 0;
        }
        public void Dispose()
        {
            m_Accessor.Dispose();
            m_Mmf.Dispose();
        }

        public bool write_packetlog
        {
            get
            {
                return (m_pSharedMemory->write_packetlog == 0) ? false : true;
            }
            set
            {
                m_pSharedMemory->write_packetlog = (value == false) ? 0 : 1;
            }
        }
        public bool freemouse
        {
            get
            {
                return (m_pSharedMemory->freemouse == 0)? false : true;
            }
            set
            {
                m_pSharedMemory->freemouse = (value == false)? 0 : 1;
            }
        }
        public bool m2e
        {
            get
            {
                return (m_pSharedMemory->m2e == 0) ? false : true;
            }
            set
            {
                m_pSharedMemory->m2e = (value == false) ? 0 : 1;
            }
        }
        public int m2e_zbias
        {
            get
            {
                return m_pSharedMemory->m2e_zbias;
            }
            set
            {
                m_pSharedMemory->m2e_zbias = value;
            }
        }

        public bool fix_windowmode_vsyncwait
        {
            get
            {
                return (m_pSharedMemory->fix_windowmode_vsyncwait == 0) ? false : true;
            }
            set
            {
                m_pSharedMemory->fix_windowmode_vsyncwait = (value == false) ? 0 : 1;
            }
        }
        public bool show_framerate 
        {
            get
            {
                return (m_pSharedMemory->show_framerate == 0) ? false : true;
            }
            set
            {
                m_pSharedMemory->show_framerate = (value == false) ? 0 : 1;
            }
        }
        public bool objectinformation
        {
            get
            {
                return (m_pSharedMemory->objectinformation == 0) ? false : true;
            }
            set
            {
                m_pSharedMemory->objectinformation = (value == false) ? 0 : 1;
            }
        }
        public bool _44khz_audiomode
        {
            get
            {
                return (m_pSharedMemory->_44khz_audiomode == 0) ? false : true;
            }
            set
            {
                m_pSharedMemory->_44khz_audiomode = (value == false) ? 0 : 1;
            }
        }
        public int cpucoolerlevel
        {
            get
            {
                return m_pSharedMemory->cpucoolerlevel;
            }
            set
            {
                m_pSharedMemory->cpucoolerlevel = value;
            }
        }

        public bool executeorder
        {
            get
            {
                return (m_pSharedMemory->executeorder == 0)? false : true;
            }
            set
            {
                m_pSharedMemory->executeorder = (value == false)? 0 : 1;
            }
        }
        public int g_hROWindow
        {
            get
            {
                return m_pSharedMemory->g_hROWindow;
            }
            set
            {
                m_pSharedMemory->g_hROWindow = value;
            }
        }

        public string configfilepath
        {
            get
            {
                string result = new string(m_pSharedMemory->configfilepath);
                return result;
            }
            set
            {
                char[] cstr = value.ToCharArray();
                Marshal.Copy(cstr, 0, (IntPtr)m_pSharedMemory->configfilepath, cstr.Length);
                m_pSharedMemory->configfilepath[cstr.Length] = '\0';
            }
        }
        public string musicfilename
        {
            get
            {
                string result = new string(m_pSharedMemory->musicfilename);
                return result;
            }
            set
            {
                char[] cstr = value.ToCharArray();
                Marshal.Copy(cstr, 0, (IntPtr)m_pSharedMemory->musicfilename, cstr.Length);
                m_pSharedMemory->musicfilename[cstr.Length] = '\0';
            }
        }
    }
}
