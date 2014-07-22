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
            this.playMusicOnClientStreamPlayerToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.extraToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.kHzAudioModeonBootToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.freeMouseToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.fixWindowModeVsyncWaitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.windowToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.nPCLoggerToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.debugInfomationToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.showFpsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.showObjectInformationToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.packetLogToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.dMapMarkerToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.showM2EToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.showBBEtoolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.showDeadCelltoolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.showChatScopetoolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.groundZBiasToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.ground_zbias_ToolStripTrackBar = new SimpleROHookCS.ToolStripTrackBar();
            this.alphaLeveltoolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.alphaLeveltoolStripTrackBar = new SimpleROHookCS.ToolStripTrackBar();
            this.toolStripSeparator3 = new System.Windows.Forms.ToolStripSeparator();
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
            this.TaskTray_notifyIcon.MouseClick += new System.Windows.Forms.MouseEventHandler(this.notifyIcon_MouseClick);
            // 
            // TaskTray_contextMenuStrip
            // 
            this.TaskTray_contextMenuStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.playMusicOnClientStreamPlayerToolStripMenuItem,
            this.toolStripSeparator1,
            this.extraToolStripMenuItem,
            this.windowToolStripMenuItem,
            this.debugInfomationToolStripMenuItem,
            this.dMapMarkerToolStripMenuItem,
            this.toolStripSeparator3,
            this.CPUCoolerText_toolStripMenuItem,
            this.CPUCooler_toolStripTrackBar,
            this.toolStripSeparator2,
            this.aboutSimpleROHookToolStripMenuItem,
            this.exitToolStripMenuItem});
            this.TaskTray_contextMenuStrip.Name = "contextMenuStrip1";
            this.TaskTray_contextMenuStrip.Size = new System.Drawing.Size(268, 246);
            this.TaskTray_contextMenuStrip.Opening += new System.ComponentModel.CancelEventHandler(this.TaskTray_contextMenuStrip_Opening);
            // 
            // playMusicOnClientStreamPlayerToolStripMenuItem
            // 
            this.playMusicOnClientStreamPlayerToolStripMenuItem.Name = "playMusicOnClientStreamPlayerToolStripMenuItem";
            this.playMusicOnClientStreamPlayerToolStripMenuItem.Size = new System.Drawing.Size(267, 22);
            this.playMusicOnClientStreamPlayerToolStripMenuItem.Text = "PlayMusic on ClientStreamPlayer";
            this.playMusicOnClientStreamPlayerToolStripMenuItem.Click += new System.EventHandler(this.playMusicOnClientStreamPlayerToolStripMenuItem_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(264, 6);
            // 
            // extraToolStripMenuItem
            // 
            this.extraToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.kHzAudioModeonBootToolStripMenuItem,
            this.freeMouseToolStripMenuItem,
            this.fixWindowModeVsyncWaitToolStripMenuItem});
            this.extraToolStripMenuItem.Name = "extraToolStripMenuItem";
            this.extraToolStripMenuItem.Size = new System.Drawing.Size(267, 22);
            this.extraToolStripMenuItem.Text = "Extra";
            // 
            // kHzAudioModeonBootToolStripMenuItem
            // 
            this.kHzAudioModeonBootToolStripMenuItem.CheckOnClick = true;
            this.kHzAudioModeonBootToolStripMenuItem.Name = "kHzAudioModeonBootToolStripMenuItem";
            this.kHzAudioModeonBootToolStripMenuItem.Size = new System.Drawing.Size(238, 22);
            this.kHzAudioModeonBootToolStripMenuItem.Text = "44kHz Audio Mode(on boot)";
            this.kHzAudioModeonBootToolStripMenuItem.Click += new System.EventHandler(this.kHzAudioModeonBootToolStripMenuItem_Click);
            // 
            // freeMouseToolStripMenuItem
            // 
            this.freeMouseToolStripMenuItem.CheckOnClick = true;
            this.freeMouseToolStripMenuItem.Name = "freeMouseToolStripMenuItem";
            this.freeMouseToolStripMenuItem.Size = new System.Drawing.Size(238, 22);
            this.freeMouseToolStripMenuItem.Text = "FreeMouse(on boot)";
            this.freeMouseToolStripMenuItem.Click += new System.EventHandler(this.freeMouseToolStripMenuItem_Click);
            // 
            // fixWindowModeVsyncWaitToolStripMenuItem
            // 
            this.fixWindowModeVsyncWaitToolStripMenuItem.CheckOnClick = true;
            this.fixWindowModeVsyncWaitToolStripMenuItem.Name = "fixWindowModeVsyncWaitToolStripMenuItem";
            this.fixWindowModeVsyncWaitToolStripMenuItem.Size = new System.Drawing.Size(238, 22);
            this.fixWindowModeVsyncWaitToolStripMenuItem.Text = "Fix WindowMode VsyncWait";
            this.fixWindowModeVsyncWaitToolStripMenuItem.Click += new System.EventHandler(this.fixWindowModeVsyncWaitToolStripMenuItem_Click);
            // 
            // windowToolStripMenuItem
            // 
            this.windowToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.nPCLoggerToolStripMenuItem});
            this.windowToolStripMenuItem.Name = "windowToolStripMenuItem";
            this.windowToolStripMenuItem.Size = new System.Drawing.Size(267, 22);
            this.windowToolStripMenuItem.Text = "Window";
            // 
            // nPCLoggerToolStripMenuItem
            // 
            this.nPCLoggerToolStripMenuItem.CheckOnClick = true;
            this.nPCLoggerToolStripMenuItem.Name = "nPCLoggerToolStripMenuItem";
            this.nPCLoggerToolStripMenuItem.Size = new System.Drawing.Size(144, 22);
            this.nPCLoggerToolStripMenuItem.Text = "NPC Logger";
            this.nPCLoggerToolStripMenuItem.Click += new System.EventHandler(this.nPCLoggerToolStripMenuItem_Click);
            // 
            // debugInfomationToolStripMenuItem
            // 
            this.debugInfomationToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.showFpsToolStripMenuItem,
            this.showObjectInformationToolStripMenuItem,
            this.packetLogToolStripMenuItem});
            this.debugInfomationToolStripMenuItem.Name = "debugInfomationToolStripMenuItem";
            this.debugInfomationToolStripMenuItem.Size = new System.Drawing.Size(267, 22);
            this.debugInfomationToolStripMenuItem.Text = "Debug Infomation";
            // 
            // showFpsToolStripMenuItem
            // 
            this.showFpsToolStripMenuItem.CheckOnClick = true;
            this.showFpsToolStripMenuItem.Name = "showFpsToolStripMenuItem";
            this.showFpsToolStripMenuItem.Size = new System.Drawing.Size(223, 22);
            this.showFpsToolStripMenuItem.Text = "Show Fps";
            this.showFpsToolStripMenuItem.Click += new System.EventHandler(this.showFpsToolStripMenuItem_Click);
            // 
            // showObjectInformationToolStripMenuItem
            // 
            this.showObjectInformationToolStripMenuItem.CheckOnClick = true;
            this.showObjectInformationToolStripMenuItem.Name = "showObjectInformationToolStripMenuItem";
            this.showObjectInformationToolStripMenuItem.Size = new System.Drawing.Size(223, 22);
            this.showObjectInformationToolStripMenuItem.Text = "Show Object Information";
            this.showObjectInformationToolStripMenuItem.Click += new System.EventHandler(this.showObjectInformationToolStripMenuItem_Click);
            // 
            // packetLogToolStripMenuItem
            // 
            this.packetLogToolStripMenuItem.CheckOnClick = true;
            this.packetLogToolStripMenuItem.Name = "packetLogToolStripMenuItem";
            this.packetLogToolStripMenuItem.Size = new System.Drawing.Size(223, 22);
            this.packetLogToolStripMenuItem.Text = "Packet Log";
            this.packetLogToolStripMenuItem.Click += new System.EventHandler(this.packetLogToolStripMenuItem_Click);
            // 
            // dMapMarkerToolStripMenuItem
            // 
            this.dMapMarkerToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.showM2EToolStripMenuItem,
            this.showBBEtoolStripMenuItem,
            this.showDeadCelltoolStripMenuItem,
            this.showChatScopetoolStripMenuItem,
            this.groundZBiasToolStripMenuItem,
            this.ground_zbias_ToolStripTrackBar,
            this.alphaLeveltoolStripMenuItem,
            this.alphaLeveltoolStripTrackBar});
            this.dMapMarkerToolStripMenuItem.Name = "dMapMarkerToolStripMenuItem";
            this.dMapMarkerToolStripMenuItem.Size = new System.Drawing.Size(267, 22);
            this.dMapMarkerToolStripMenuItem.Text = "3D Map Grid";
            // 
            // showM2EToolStripMenuItem
            // 
            this.showM2EToolStripMenuItem.CheckOnClick = true;
            this.showM2EToolStripMenuItem.Name = "showM2EToolStripMenuItem";
            this.showM2EToolStripMenuItem.Size = new System.Drawing.Size(275, 22);
            this.showM2EToolStripMenuItem.Text = "Show M2E";
            this.showM2EToolStripMenuItem.Click += new System.EventHandler(this.showM2EToolStripMenuItem_Click);
            // 
            // showBBEtoolStripMenuItem
            // 
            this.showBBEtoolStripMenuItem.CheckOnClick = true;
            this.showBBEtoolStripMenuItem.Name = "showBBEtoolStripMenuItem";
            this.showBBEtoolStripMenuItem.Size = new System.Drawing.Size(275, 22);
            this.showBBEtoolStripMenuItem.Text = "Show BBE";
            this.showBBEtoolStripMenuItem.Click += new System.EventHandler(this.showBBEtoolStripMenuItem_Click);
            // 
            // showDeadCelltoolStripMenuItem
            // 
            this.showDeadCelltoolStripMenuItem.CheckOnClick = true;
            this.showDeadCelltoolStripMenuItem.Name = "showDeadCelltoolStripMenuItem";
            this.showDeadCelltoolStripMenuItem.Size = new System.Drawing.Size(275, 22);
            this.showDeadCelltoolStripMenuItem.Text = "Show Dead Cell";
            this.showDeadCelltoolStripMenuItem.Click += new System.EventHandler(this.showDeadCelltoolStripMenuItem_Click);
            // 
            // showChatScopetoolStripMenuItem
            // 
            this.showChatScopetoolStripMenuItem.CheckOnClick = true;
            this.showChatScopetoolStripMenuItem.Name = "showChatScopetoolStripMenuItem";
            this.showChatScopetoolStripMenuItem.Size = new System.Drawing.Size(275, 22);
            this.showChatScopetoolStripMenuItem.Text = "Show Chat Scope";
            this.showChatScopetoolStripMenuItem.Click += new System.EventHandler(this.showChatScopetoolStripMenuItem_Click);
            // 
            // groundZBiasToolStripMenuItem
            // 
            this.groundZBiasToolStripMenuItem.Enabled = false;
            this.groundZBiasToolStripMenuItem.Name = "groundZBiasToolStripMenuItem";
            this.groundZBiasToolStripMenuItem.Size = new System.Drawing.Size(275, 22);
            this.groundZBiasToolStripMenuItem.Text = "Ground Z Bias";
            // 
            // ground_zbias_ToolStripTrackBar
            // 
            this.ground_zbias_ToolStripTrackBar.Name = "ground_zbias_ToolStripTrackBar";
            this.ground_zbias_ToolStripTrackBar.Size = new System.Drawing.Size(215, 45);
            this.ground_zbias_ToolStripTrackBar.Text = "ToolStripTrackBar1";
            this.ground_zbias_ToolStripTrackBar.Value = 0;
            this.ground_zbias_ToolStripTrackBar.ValueChanged += new System.EventHandler(this.ground_zbias_ToolStripTrackBar_Update);
            // 
            // alphaLeveltoolStripMenuItem
            // 
            this.alphaLeveltoolStripMenuItem.Enabled = false;
            this.alphaLeveltoolStripMenuItem.Name = "alphaLeveltoolStripMenuItem";
            this.alphaLeveltoolStripMenuItem.Size = new System.Drawing.Size(275, 22);
            this.alphaLeveltoolStripMenuItem.Text = "Alpha Level";
            // 
            // alphaLeveltoolStripTrackBar
            // 
            this.alphaLeveltoolStripTrackBar.Name = "alphaLeveltoolStripTrackBar";
            this.alphaLeveltoolStripTrackBar.Size = new System.Drawing.Size(215, 45);
            this.alphaLeveltoolStripTrackBar.Text = "ToolStripTrackBar1";
            this.alphaLeveltoolStripTrackBar.Value = 0;
            this.alphaLeveltoolStripTrackBar.ValueChanged += new System.EventHandler(this.alphaLeveltoolStripTrackBar_Update);
            // 
            // toolStripSeparator3
            // 
            this.toolStripSeparator3.Name = "toolStripSeparator3";
            this.toolStripSeparator3.Size = new System.Drawing.Size(264, 6);
            // 
            // CPUCoolerText_toolStripMenuItem
            // 
            this.CPUCoolerText_toolStripMenuItem.Enabled = false;
            this.CPUCoolerText_toolStripMenuItem.Name = "CPUCoolerText_toolStripMenuItem";
            this.CPUCoolerText_toolStripMenuItem.Size = new System.Drawing.Size(267, 22);
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
            this.toolStripSeparator2.Size = new System.Drawing.Size(264, 6);
            // 
            // aboutSimpleROHookToolStripMenuItem
            // 
            this.aboutSimpleROHookToolStripMenuItem.Name = "aboutSimpleROHookToolStripMenuItem";
            this.aboutSimpleROHookToolStripMenuItem.Size = new System.Drawing.Size(267, 22);
            this.aboutSimpleROHookToolStripMenuItem.Text = "About SimpleROHook";
            this.aboutSimpleROHookToolStripMenuItem.Click += new System.EventHandler(this.aboutSimpleROHookToolStripMenuItem_Click);
            // 
            // exitToolStripMenuItem
            // 
            this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
            this.exitToolStripMenuItem.Size = new System.Drawing.Size(267, 22);
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
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Window_FormClosing);
            this.Load += new System.EventHandler(this.Window_Load);
            this.TaskTray_contextMenuStrip.ResumeLayout(false);
            this.TaskTray_contextMenuStrip.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.NotifyIcon TaskTray_notifyIcon;
        private System.Windows.Forms.ContextMenuStrip TaskTray_contextMenuStrip;
        private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem playMusicOnClientStreamPlayerToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
        private System.Windows.Forms.ToolStripMenuItem aboutSimpleROHookToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem CPUCoolerText_toolStripMenuItem;
        private ToolStripTrackBar CPUCooler_toolStripTrackBar;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator3;
        private System.Windows.Forms.ToolStripMenuItem extraToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem kHzAudioModeonBootToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem freeMouseToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem fixWindowModeVsyncWaitToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem windowToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem nPCLoggerToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem debugInfomationToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem showFpsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem showObjectInformationToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem packetLogToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem dMapMarkerToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem showM2EToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem showBBEtoolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem showDeadCelltoolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem showChatScopetoolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem groundZBiasToolStripMenuItem;
        private ToolStripTrackBar ground_zbias_ToolStripTrackBar;
        private System.Windows.Forms.ToolStripMenuItem alphaLeveltoolStripMenuItem;
        private ToolStripTrackBar alphaLeveltoolStripTrackBar;
    }
}

