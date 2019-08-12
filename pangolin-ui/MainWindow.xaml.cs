using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace pangolin_ui
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public Config cfg;
        public MainWindow()
        {
            InitializeComponent();
            cfg = new Config();
            try
            {
                cfg.ReadConfig();
            }
            catch(Exception e)
            {
                MessageBox.Show("Can't open config.txt: " + e.ToString());

            }

            LoadConfigToUI();
        }

        public void LoadConfigToUI()
        {
            serverEdit.Text = cfg.server;
            dnsEdit.Text = cfg.dns;
            tunEdit.Text = cfg.tun;
            mtuEdit.Text = cfg.mtu.ToString();
            if(cfg.protocol.ToUpper() == "PTCP")
            {
                protoComboBox.SelectedItem = 0;
            }
            else
            {
                protoComboBox.SelectedItem = 1;
            }
            if(cfg.tokens.Count > 0)
            {
                tokenEdit.Text = cfg.tokens[0].ToString();
            }
        }

        public void LoadUItoConfig()
        {
            cfg.server = serverEdit.Text;
            cfg.dns = dnsEdit.Text;
            cfg.tun = tunEdit.Text;
            cfg.mtu = Int32.Parse(mtuEdit.Text);
            cfg.tokens.Clear();
            cfg.tokens.Add(tokenEdit.Text);
            cfg.protocol = ((ComboBoxItem)protoComboBox.SelectedItem).Content.ToString();
        }

        [DllImport(@"pangolin-win.dll")]
        public static extern void main();

        private void StartButton_Click(object sender, RoutedEventArgs e)
        {
           // MessageBox.Show(Directory.GetCurrentDirectory());
            LoadUItoConfig();
            cfg.SaveConfig();
            try
            {
                main();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.ToString());
            }
        }
    }
}
