namespace ImFusionSuite_DotNetForm
{
    partial class ImFusionForm
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
			this.menuStrip2 = new System.Windows.Forms.MenuStrip();
			this.openToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.menuStrip2.SuspendLayout();
			this.SuspendLayout();
			// 
			// menuStrip2
			// 
			this.menuStrip2.ImageScalingSize = new System.Drawing.Size(20, 20);
			this.menuStrip2.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.openToolStripMenuItem});
			this.menuStrip2.Location = new System.Drawing.Point(0, 0);
			this.menuStrip2.Name = "menuStrip2";
			this.menuStrip2.Size = new System.Drawing.Size(1037, 24);
			this.menuStrip2.TabIndex = 1;
			this.menuStrip2.Text = "menuStrip2";
			// 
			// openToolStripMenuItem
			// 
			this.openToolStripMenuItem.Name = "openToolStripMenuItem";
			this.openToolStripMenuItem.Size = new System.Drawing.Size(48, 20);
			this.openToolStripMenuItem.Text = "Open";
			this.openToolStripMenuItem.Click += new System.EventHandler(this.openToolStripMenuItem_Click);
			// 
			// ImFusionForm
			// 
			this.ClientSize = new System.Drawing.Size(1037, 670);
			this.Controls.Add(this.menuStrip2);
			this.Name = "ImFusionForm";
			this.Text = "ImFusion .NET Demo";
			this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.ImFusionForm_FormClosing);
			this.Load += new System.EventHandler(this.ImFusionForm_Load);
			this.menuStrip2.ResumeLayout(false);
			this.menuStrip2.PerformLayout();
			this.ResumeLayout(false);
			this.PerformLayout();

        }

        #endregion
        private System.Windows.Forms.MenuStrip menuStrip2;
        private System.Windows.Forms.ToolStripMenuItem openToolStripMenuItem;
    }
}

