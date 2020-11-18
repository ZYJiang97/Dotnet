using System;
using System.Collections.Generic;
using System.Windows.Forms;

namespace ImFusionSuite_DotNetForm
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            try
            {
                Application.EnableVisualStyles();
                Application.SetCompatibleTextRenderingDefault(false);
                Application.Run(new ImFusionForm());
            }
            catch (ImFusionForm.MissingLicenseException ex)
            {
                MessageBox.Show(ex.Message);
            }
        }
    }
}
