using System;
using System.Windows.Forms;
using System.Runtime.InteropServices;

namespace SimpleROHookCS
{
    static class Program
    {
        /// <summary>
        /// アプリケーションのメイン エントリ ポイントです。
        /// </summary>
        [DllImport("kernel32.dll", EntryPoint = "LoadLibrary")]
        static extern int LoadLibrary(
            [MarshalAs(UnmanagedType.LPStr)] string lpLibFileName);
        [DllImport("kernel32.dll", EntryPoint = "GetProcAddress")]
        static extern IntPtr GetProcAddress(int hModule,
            [MarshalAs(UnmanagedType.LPStr)] string lpProcName);
        [DllImport("kernel32.dll", EntryPoint = "FreeLibrary")]
        static extern bool FreeLibrary(int hModule);

        delegate void HOOKFUNC();

        [STAThread]
        static void Main()
        {
            using (System.Threading.Mutex mutex = new System.Threading.Mutex(false, Application.ProductName))
            {
                if (mutex.WaitOne(0, false))
                {
                    int hModule = LoadLibrary(@"Core.dll");

                    if (hModule == 0)
                    {
                        MessageBox.Show("error:LoadLibrary failed.");
                        return;
                    }
                    IntPtr intPtrInstall = GetProcAddress(hModule, @"InstallHook");
                    IntPtr intPtrRemove = GetProcAddress(hModule, @"RemoveHook");

                    HOOKFUNC InstallHook =
                        (HOOKFUNC)Marshal.GetDelegateForFunctionPointer
                                        (intPtrInstall, typeof(HOOKFUNC));
                    HOOKFUNC RemoveHook =
                        (HOOKFUNC)Marshal.GetDelegateForFunctionPointer
                                        (intPtrRemove, typeof(HOOKFUNC));

                    Application.EnableVisualStyles();
                    Application.SetCompatibleTextRenderingDefault(false);

                    InstallHook();

                    using (new MainForm())
                    {
                        Application.Run();
                    }
                    //MessageBox.Show("end of app");

                    RemoveHook();
                    FreeLibrary(hModule);

                    //Properties.Settings.Default.Save();
                    //MessageBox.Show("save default setting.");
                }
            }
        }
    }
}
