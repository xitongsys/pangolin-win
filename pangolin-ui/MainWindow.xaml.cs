using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
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
            UpdateStatusUnconnected();
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
            cfg.protocol = ((ComboBoxItem)protoComboBox.SelectedItem).Content.ToString().ToLower();
        }

        public delegate void UpdateTextCallback();

        public Process proc;
        public void RunPangolin()
        {
            ProcessStartInfo psi = new ProcessStartInfo("pangolin-win.exe")
            {
                UseShellExecute = false,
                RedirectStandardInput = false,
                CreateNoWindow = true
            };

            proc = new Process()
            {
                StartInfo = psi
            };

            proc.Start();
            statusLabel.Dispatcher.Invoke(
                new UpdateTextCallback(this.UpdateStatusConnected)
                );
            
            proc.WaitForExit();

            statusLabel.Dispatcher.Invoke(
                new UpdateTextCallback(this.UpdateStatusUnconnected)
                );
        }

        private void UpdateStatusConnecting()
        {
            statusLabel.Content = "Status: Connecting";
            statusLabel.Background = new SolidColorBrush((Color)ColorConverter.ConvertFromString("#FFFF00"));
        }

        private void UpdateStatusUnconnected()
        {
            statusLabel.Content = "Status: Unconnected";
            statusLabel.Background = new SolidColorBrush((Color)ColorConverter.ConvertFromString("#AAAAAA"));
        }

        private void UpdateStatusConnected()
        {
            statusLabel.Content = "Status: Connected";
            statusLabel.Background = new SolidColorBrush((Color)ColorConverter.ConvertFromString("#00FF00"));
        }

        private void StartButton_Click(object sender, RoutedEventArgs e)
        {
            UpdateStatusConnecting();
            LoadUItoConfig();
            cfg.SaveConfig();
            Thread thr = new Thread(RunPangolin);
            thr.Start();
        }

        private void StopButton_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                proc.Kill();
            }
            catch (Exception ex) { }
            UpdateStatusUnconnected();
        }
    }
}
