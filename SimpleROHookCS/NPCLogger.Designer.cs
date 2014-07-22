namespace SimpleROHookCS
{
    partial class NPCLogger
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.richTextBox_LogText = new System.Windows.Forms.RichTextBox();
            this.SuspendLayout();
            // 
            // richTextBox_LogText
            // 
            this.richTextBox_LogText.AutoWordSelection = true;
            this.richTextBox_LogText.Dock = System.Windows.Forms.DockStyle.Fill;
            this.richTextBox_LogText.Location = new System.Drawing.Point(0, 0);
            this.richTextBox_LogText.Name = "richTextBox_LogText";
            this.richTextBox_LogText.ReadOnly = true;
            this.richTextBox_LogText.Size = new System.Drawing.Size(284, 262);
            this.richTextBox_LogText.TabIndex = 0;
            this.richTextBox_LogText.Text = "";
            // 
            // NPCLogger
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(284, 262);
            this.Controls.Add(this.richTextBox_LogText);
            this.Name = "NPCLogger";
            this.Text = "NPCLogger";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Window_FormClosing);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.RichTextBox richTextBox_LogText;
    }
}