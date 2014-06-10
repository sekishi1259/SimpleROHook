namespace SimpleROHookCS
{
    partial class MainForm
    {
        /// <summary>
        /// 必要なデザイナー変数です。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 使用中のリソースをすべてクリーンアップします。
        /// </summary>
        /// <param name="disposing">マネージ リソースが破棄される場合 true、破棄されない場合は false です。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows フォーム デザイナーで生成されたコード

        /// <summary>
        /// デザイナー サポートに必要なメソッドです。このメソッドの内容を
        /// コード エディターで変更しないでください。
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
            this.TaskTray_notifyIcon = new System.Windows.Forms.NotifyIcon(this.components);
            this.TaskTray_contextMenuStrip = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.kHzAudioModeonBootToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.playMusicOnClientStreamPlayerToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.packetLogToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.freeMouseToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.showM2EToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.m2EZBiasToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.m2e_zbias_ToolStripTrackBar = new SimpleROHookCS.ToolStripTrackBar();
            this.showFpsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.showObjectInformationToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.fixWindowModeVsyncWaitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.CPUCoolerText_toolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.CPUCooler_toolStripTrackBar = new SimpleROHookCS.ToolStripTrackBar();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.aboutSimpleROHookToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.TaskTray_contextMenuStrip.SuspendLayout();
            this.SuspendLayout();
            // 
            // TaskTray_notifyIcon
            // 
            this.TaskTray_notifyIcon.ContextMenuStrip = this.TaskTray_contextMenuStrip;
            this.TaskTray_notifyIcon.Icon = ((System.Drawing.Icon)(resources.GetObject("TaskTray_notifyIcon.Icon")));
            this.TaskTray_notifyIcon.Text = "SimpleROHook";
            this.TaskTray_notifyIcon.Visible = true;
            // 
            // TaskTray_contextMenuStrip
            // 
            this.TaskTray_contextMenuStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.kHzAudioModeonBootToolStripMenuItem,
            this.playMusicOnClientStreamPlayerToolStripMenuItem,
            this.toolStripSeparator1,
            this.packetLogToolStripMenuItem,
            this.freeMouseToolStripMenuItem,
            this.showM2EToolStripMenuItem,
            this.m2EZBiasToolStripMenuItem,
            this.m2e_zbias_ToolStripTrackBar,
            this.showFpsToolStripMenuItem,
            this.showObjectInformationToolStripMenuItem,
            this.fixWindowModeVsyncWaitToolStripMenuItem,
            this.CPUCoolerText_toolStripMenuItem,
            this.CPUCooler_toolStripTrackBar,
            this.toolStripSeparator2,
            this.aboutSimpleROHookToolStripMenuItem,
            this.exitToolStripMenuItem});
            this.TaskTray_contextMenuStrip.Name = "contextMenuStrip1";
            this.TaskTray_contextMenuStrip.Size = new System.Drawing.Size(276, 398);
            this.TaskTray_contextMenuStrip.Opening += new System.ComponentModel.CancelEventHandler(this.TaskTray_contextMenuStrip_Opening);
            // 
            // kHzAudioModeonBootToolStripMenuItem
            // 
            this.kHzAudioModeonBootToolStripMenuItem.CheckOnClick = true;
            this.kHzAudioModeonBootToolStripMenuItem.Name = "kHzAudioModeonBootToolStripMenuItem";
            this.kHzAudioModeonBootToolStripMenuItem.Size = new System.Drawing.Size(275, 22);
            this.kHzAudioModeonBootToolStripMenuItem.Text = "44kHz Audio Mode(on boot)";
            this.kHzAudioModeonBootToolStripMenuItem.Click += new System.EventHandler(this.kHzAudioModeonBootToolStripMenuItem_Click);
            // 
            // playMusicOnClientStreamPlayerToolStripMenuItem
            // 
            this.playMusicOnClientStreamPlayerToolStripMenuItem.Name = "playMusicOnClientStreamPlayerToolStripMenuItem";
            this.playMusicOnClientStreamPlayerToolStripMenuItem.Size = new System.Drawing.Size(275, 22);
            this.playMusicOnClientStreamPlayerToolStripMenuItem.Text = "PlayMusic on ClientStreamPlayer";
            this.playMusicOnClientStreamPlayerToolStripMenuItem.Click += new System.EventHandler(this.playMusicOnClientStreamPlayerToolStripMenuItem_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(272, 6);
            // 
            // packetLogToolStripMenuItem
            // 
            this.packetLogToolStripMenuItem.CheckOnClick = true;
            this.packetLogToolStripMenuItem.Name = "packetLogToolStripMenuItem";
            this.packetLogToolStripMenuItem.Size = new System.Drawing.Size(275, 22);
            this.packetLogToolStripMenuItem.Text = "Packet Log";
            this.packetLogToolStripMenuItem.Click += new System.EventHandler(this.packetLogToolStripMenuItem_Click);
            // 
            // freeMouseToolStripMenuItem
            // 
            this.freeMouseToolStripMenuItem.CheckOnClick = true;
            this.freeMouseToolStripMenuItem.Name = "freeMouseToolStripMenuItem";
            this.freeMouseToolStripMenuItem.Size = new System.Drawing.Size(275, 22);
            this.freeMouseToolStripMenuItem.Text = "FreeMouse(on boot)";
            this.freeMouseToolStripMenuItem.Click += new System.EventHandler(this.freeMouseToolStripMenuItem_Click);
            // 
            // showM2EToolStripMenuItem
            // 
            this.showM2EToolStripMenuItem.CheckOnClick = true;
            this.showM2EToolStripMenuItem.Name = "showM2EToolStripMenuItem";
            this.showM2EToolStripMenuItem.Size = new System.Drawing.Size(275, 22);
            this.showM2EToolStripMenuItem.Text = "Show M2E";
            this.showM2EToolStripMenuItem.Click += new System.EventHandler(this.showM2EToolStripMenuItem_Click);
            // 
            // m2EZBiasToolStripMenuItem
            // 
            this.m2EZBiasToolStripMenuItem.Enabled = false;
            this.m2EZBiasToolStripMenuItem.Name = "m2EZBiasToolStripMenuItem";
            this.m2EZBiasToolStripMenuItem.Size = new System.Drawing.Size(275, 22);
            this.m2EZBiasToolStripMenuItem.Text = "M2E Z Bias";
            // 
            // m2e_zbias_ToolStripTrackBar
            // 
            this.m2e_zbias_ToolStripTrackBar.Name = "m2e_zbias_ToolStripTrackBar";
            this.m2e_zbias_ToolStripTrackBar.Size = new System.Drawing.Size(215, 45);
            this.m2e_zbias_ToolStripTrackBar.Text = "ToolStripTrackBar1";
            this.m2e_zbias_ToolStripTrackBar.Value = 0;
            this.m2e_zbias_ToolStripTrackBar.ValueChanged += new System.EventHandler(this.m2e_zbias_ToolStripTrackBar_Update);
            // 
            // showFpsToolStripMenuItem
            // 
            this.showFpsToolStripMenuItem.CheckOnClick = true;
            this.showFpsToolStripMenuItem.Name = "showFpsToolStripMenuItem";
            this.showFpsToolStripMenuItem.Size = new System.Drawing.Size(275, 22);
            this.showFpsToolStripMenuItem.Text = "Show Fps";
            this.showFpsToolStripMenuItem.Click += new System.EventHandler(this.showFpsToolStripMenuItem_Click);
            // 
            // showObjectInformationToolStripMenuItem
            // 
            this.showObjectInformationToolStripMenuItem.CheckOnClick = true;
            this.showObjectInformationToolStripMenuItem.Name = "showObjectInformationToolStripMenuItem";
            this.showObjectInformationToolStripMenuItem.Size = new System.Drawing.Size(275, 22);
            this.showObjectInformationToolStripMenuItem.Text = "Show Object Information";
            this.showObjectInformationToolStripMenuItem.Click += new System.EventHandler(this.showObjectInformationToolStripMenuItem_Click);
            // 
            // fixWindowModeVsyncWaitToolStripMenuItem
            // 
            this.fixWindowModeVsyncWaitToolStripMenuItem.CheckOnClick = true;
            this.fixWindowModeVsyncWaitToolStripMenuItem.Name = "fixWindowModeVsyncWaitToolStripMenuItem";
            this.fixWindowModeVsyncWaitToolStripMenuItem.Size = new System.Drawing.Size(275, 22);
            this.fixWindowModeVsyncWaitToolStripMenuItem.Text = "Fix WindowMode VsyncWait";
            this.fixWindowModeVsyncWaitToolStripMenuItem.Click += new System.EventHandler(this.fixWindowModeVsyncWaitToolStripMenuItem_Click);
            // 
            // CPUCoolerText_toolStripMenuItem
            // 
            this.CPUCoolerText_toolStripMenuItem.Enabled = false;
            this.CPUCoolerText_toolStripMenuItem.Name = "CPUCoolerText_toolStripMenuItem";
            this.CPUCoolerText_toolStripMenuItem.Size = new System.Drawing.Size(275, 22);
            this.CPUCoolerText_toolStripMenuItem.Text = "CPU Cooler";
            // 
            // CPUCooler_toolStripTrackBar
            // 
            this.CPUCooler_toolStripTrackBar.Name = "CPUCooler_toolStripTrackBar";
            this.CPUCooler_toolStripTrackBar.Size = new System.Drawing.Size(104, 45);
            this.CPUCooler_toolStripTrackBar.Text = "toolStripTrackBar1";
            this.CPUCooler_toolStripTrackBar.Value = 0;
            this.CPUCooler_toolStripTrackBar.ValueChanged += new System.EventHandler(this.CPUCooler_toolStripTrackBar_Update);
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(272, 6);
            // 
            // aboutSimpleROHookToolStripMenuItem
            // 
            this.aboutSimpleROHookToolStripMenuItem.Name = "aboutSimpleROHookToolStripMenuItem";
            this.aboutSimpleROHookToolStripMenuItem.Size = new System.Drawing.Size(275, 22);
            this.aboutSimpleROHookToolStripMenuItem.Text = "About SimpleROHook";
            this.aboutSimpleROHookToolStripMenuItem.Click += new System.EventHandler(this.aboutSimpleROHookToolStripMenuItem_Click);
            // 
            // exitToolStripMenuItem
            // 
            this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
            this.exitToolStripMenuItem.Size = new System.Drawing.Size(275, 22);
            this.exitToolStripMenuItem.Text = "Exit";
            this.exitToolStripMenuItem.Click += new System.EventHandler(this.exitToolStripMenuItem_Click);
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(284, 262);
            this.Name = "MainForm";
            this.Text = "SimpleROHook";
            this.TaskTray_contextMenuStrip.ResumeLayout(false);
            this.TaskTray_contextMenuStrip.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.NotifyIcon TaskTray_notifyIcon;
        private System.Windows.Forms.ContextMenuStrip TaskTray_contextMenuStrip;
        private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem kHzAudioModeonBootToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem playMusicOnClientStreamPlayerToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripMenuItem showM2EToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem showFpsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem showObjectInformationToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem fixWindowModeVsyncWaitToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
        private System.Windows.Forms.ToolStripMenuItem aboutSimpleROHookToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem freeMouseToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem packetLogToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem m2EZBiasToolStripMenuItem;
        private ToolStripTrackBar m2e_zbias_ToolStripTrackBar;
        private System.Windows.Forms.ToolStripMenuItem CPUCoolerText_toolStripMenuItem;
        private ToolStripTrackBar CPUCooler_toolStripTrackBar;
    }
}

