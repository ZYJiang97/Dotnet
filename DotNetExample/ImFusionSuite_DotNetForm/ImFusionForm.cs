using System;
using System.Drawing;
using System.Windows.Forms;
using System.Collections;
using ImFusion.CLI;

namespace ImFusionSuite_DotNetForm
{
    public partial class ImFusionForm : Form
    {
        ImFusionApp m_app;
        ImFusionRenderer m_renderer;
        public class MissingLicenseException : Exception
        {
            public MissingLicenseException(string message) : base(message)
            { }
        }

        public ImFusionForm()
        {
            InitializeComponent();
            m_app = new ImFusionApp();
            bool ok = m_app.Init();
            if (!ok)
            {
                throw new MissingLicenseException("Did not find license. Exiting application. To activate software please use ImFusion Suite.");
            }

            m_renderer = new ImFusionRenderer(m_app);
        }

        private void ImFusionForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            // order is important, GL Context must still exist
            m_renderer.Close();
            m_app.Close();
        }

        private void ImFusionForm_Load(object sender, EventArgs e)
        {
            Application.Idle += new EventHandler(Application_Idle);

            Controls.Add(m_renderer);
            m_renderer.BringToFront();
            m_renderer.Dock = DockStyle.Fill;
        }

        // We need to update the ImFusionApp to process events
        private void Application_Idle(object sender, EventArgs e)
        {
            m_app.Update();
        }

        private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenFileDialog dialog = new OpenFileDialog();
            if (dialog.ShowDialog() == DialogResult.OK)
            {
                bool ok = m_app.Open(dialog.FileName);
                if (!ok)
                {
                    MessageBox.Show("Could not load file " + dialog.FileName);
                }

            }
        }
    }
}
