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
            this.freeMouseToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.showM2EToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.showFpsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.showObjectInformationToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.fixWindowModeVsyncWaitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripTextBox1 = new System.Windows.Forms.ToolStripTextBox();
            this.offToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.level1ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.level2ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.level3ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
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
            this.freeMouseToolStripMenuItem,
            this.showM2EToolStripMenuItem,
            this.showFpsToolStripMenuItem,
            this.showObjectInformationToolStripMenuItem,
            this.fixWindowModeVsyncWaitToolStripMenuItem,
            this.toolStripTextBox1,
            this.offToolStripMenuItem,
            this.level1ToolStripMenuItem,
            this.level2ToolStripMenuItem,
            this.level3ToolStripMenuItem,
            this.toolStripSeparator2,
            this.aboutSimpleROHookToolStripMenuItem,
            this.exitToolStripMenuItem});
            this.TaskTray_contextMenuStrip.Name = "contextMenuStrip1";
            this.TaskTray_contextMenuStrip.Size = new System.Drawing.Size(268, 351);
            this.TaskTray_contextMenuStrip.Opening += new System.ComponentModel.CancelEventHandler(this.TaskTray_contextMenuStrip_Opening);
            // 
            // kHzAudioModeonBootToolStripMenuItem
            // 
            this.kHzAudioModeonBootToolStripMenuItem.CheckOnClick = true;
            this.kHzAudioModeonBootToolStripMenuItem.Name = "kHzAudioModeonBootToolStripMenuItem";
            this.kHzAudioModeonBootToolStripMenuItem.Size = new System.Drawing.Size(267, 22);
            this.kHzAudioModeonBootToolStripMenuItem.Text = "44kHz Audio Mode(on boot)";
            this.kHzAudioModeonBootToolStripMenuItem.Click += new System.EventHandler(this.kHzAudioModeonBootToolStripMenuItem_Click);
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
            // freeMouseToolStripMenuItem
            // 
            this.freeMouseToolStripMenuItem.CheckOnClick = true;
            this.freeMouseToolStripMenuItem.Name = "freeMouseToolStripMenuItem";
            this.freeMouseToolStripMenuItem.Size = new System.Drawing.Size(267, 22);
            this.freeMouseToolStripMenuItem.Text = "FreeMouse(on boot)";
            this.freeMouseToolStripMenuItem.Click += new System.EventHandler(this.freeMouseToolStripMenuItem_Click);
            // 
            // showM2EToolStripMenuItem
            // 
            this.showM2EToolStripMenuItem.CheckOnClick = true;
            this.showM2EToolStripMenuItem.Name = "showM2EToolStripMenuItem";
            this.showM2EToolStripMenuItem.Size = new System.Drawing.Size(267, 22);
            this.showM2EToolStripMenuItem.Text = "Show M2E";
            this.showM2EToolStripMenuItem.Click += new System.EventHandler(this.showM2EToolStripMenuItem_Click);
            // 
            // showFpsToolStripMenuItem
            // 
            this.showFpsToolStripMenuItem.CheckOnClick = true;
            this.showFpsToolStripMenuItem.Name = "showFpsToolStripMenuItem";
            this.showFpsToolStripMenuItem.Size = new System.Drawing.Size(267, 22);
            this.showFpsToolStripMenuItem.Text = "Show Fps";
            this.showFpsToolStripMenuItem.Click += new System.EventHandler(this.showFpsToolStripMenuItem_Click);
            // 
            // showObjectInformationToolStripMenuItem
            // 
            this.showObjectInformationToolStripMenuItem.CheckOnClick = true;
            this.showObjectInformationToolStripMenuItem.Name = "showObjectInformationToolStripMenuItem";
            this.showObjectInformationToolStripMenuItem.Size = new System.Drawing.Size(267, 22);
            this.showObjectInformationToolStripMenuItem.Text = "Show Object Information";
            this.showObjectInformationToolStripMenuItem.Click += new System.EventHandler(this.showObjectInformationToolStripMenuItem_Click);
            // 
            // fixWindowModeVsyncWaitToolStripMenuItem
            // 
            this.fixWindowModeVsyncWaitToolStripMenuItem.CheckOnClick = true;
            this.fixWindowModeVsyncWaitToolStripMenuItem.Name = "fixWindowModeVsyncWaitToolStripMenuItem";
            this.fixWindowModeVsyncWaitToolStripMenuItem.Size = new System.Drawing.Size(267, 22);
            this.fixWindowModeVsyncWaitToolStripMenuItem.Text = "Fix WindowMode VsyncWait";
            this.fixWindowModeVsyncWaitToolStripMenuItem.Click += new System.EventHandler(this.fixWindowModeVsyncWaitToolStripMenuItem_Click);
            // 
            // toolStripTextBox1
            // 
            this.toolStripTextBox1.Enabled = false;
            this.toolStripTextBox1.Name = "toolStripTextBox1";
            this.toolStripTextBox1.Size = new System.Drawing.Size(100, 25);
            this.toolStripTextBox1.Text = "-- CPU Cooler --";
            // 
            // offToolStripMenuItem
            // 
            this.offToolStripMenuItem.Name = "offToolStripMenuItem";
            this.offToolStripMenuItem.Size = new System.Drawing.Size(267, 22);
            this.offToolStripMenuItem.Text = "Off";
            this.offToolStripMenuItem.Click += new System.EventHandler(this.offToolStripMenuItem_Click);
            // 
            // level1ToolStripMenuItem
            // 
            this.level1ToolStripMenuItem.Name = "level1ToolStripMenuItem";
            this.level1ToolStripMenuItem.Size = new System.Drawing.Size(267, 22);
            this.level1ToolStripMenuItem.Text = "Level1";
            this.level1ToolStripMenuItem.Click += new System.EventHandler(this.level1ToolStripMenuItem_Click);
            // 
            // level2ToolStripMenuItem
            // 
            this.level2ToolStripMenuItem.Name = "level2ToolStripMenuItem";
            this.level2ToolStripMenuItem.Size = new System.Drawing.Size(267, 22);
            this.level2ToolStripMenuItem.Text = "Level2";
            this.level2ToolStripMenuItem.Click += new System.EventHandler(this.level2ToolStripMenuItem_Click);
            // 
            // level3ToolStripMenuItem
            // 
            this.level3ToolStripMenuItem.Name = "level3ToolStripMenuItem";
            this.level3ToolStripMenuItem.Size = new System.Drawing.Size(267, 22);
            this.level3ToolStripMenuItem.Text = "Level3";
            this.level3ToolStripMenuItem.Click += new System.EventHandler(this.level3ToolStripMenuItem_Click);
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
        private System.Windows.Forms.ToolStripTextBox toolStripTextBox1;
        private System.Windows.Forms.ToolStripMenuItem offToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem level1ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem level2ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem level3ToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
        private System.Windows.Forms.ToolStripMenuItem aboutSimpleROHookToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem freeMouseToolStripMenuItem;
    }
}

