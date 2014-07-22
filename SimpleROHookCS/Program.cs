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
                    int hModule = LoadLibrary(@"Injection.dll");

                    if (hModule == 0)
                    {
                        MessageBox.Show("error:LoadLibrary failed.");
                        return;
                    }
                    IntPtr intPtrInstall = GetProcAddress(hModule, @"InstallHook");
                    IntPtr intPtrRemove = GetProcAddress(hModule, @"RemoveHook");

                    if (intPtrInstall == null)
                    {
                        MessageBox.Show("error:InstallHook function is not included in Core.dll.");
                        FreeLibrary(hModule);
                        return;
                    }
                    if (intPtrRemove == null)
                    {
                        MessageBox.Show("error:intPtrRemove function is not included in Core.dll.");
                        FreeLibrary(hModule);
                        return;
                    }

                    HOOKFUNC InstallHook =
                        (HOOKFUNC)Marshal.GetDelegateForFunctionPointer
                                        (intPtrInstall, typeof(HOOKFUNC));
                    HOOKFUNC RemoveHook =
                        (HOOKFUNC)Marshal.GetDelegateForFunctionPointer
                                        (intPtrRemove, typeof(HOOKFUNC));

                    Application.EnableVisualStyles();
                    Application.SetCompatibleTextRenderingDefault(false);

                    InstallHook();

                    Application.Run(new MainForm());

                    RemoveHook();
                    FreeLibrary(hModule);
                }
            }
        }
    }
}
