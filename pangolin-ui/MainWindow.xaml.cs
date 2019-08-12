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

        public delegate void UpdateTextCallback(string s);
        public delegate void AppendTextCallback(string s);
        public delegate void UpdateUI();

        public Process proc;
        public void RunPangolin()
        {
            ProcessStartInfo psi = new ProcessStartInfo("pangolin-win.exe")
            {
                UseShellExecute = false,
                RedirectStandardInput = false,
                RedirectStandardOutput = true,
                CreateNoWindow = true
            };

            proc = new Process()
            {
                StartInfo = psi
            };

            proc.Start();
            while (!proc.StandardOutput.EndOfStream)
            {
                string line = proc.StandardOutput.ReadLine();
                if(line == "udp client started" || line == "ptcp client started")
                {
                    MessageBox.Show("Connected", "Pangolin");
                }

                outputBox.Dispatcher.Invoke(
                    new AppendTextCallback(this.AppendOutput),
                    line
                    );
            }

            proc.WaitForExit();

            MessageBox.Show("Disconnected", "Pangolin");
            outputBox.Dispatcher.Invoke(
                new UpdateUI(this.UpdateUnconnected)
                );
        }

        private void AppendOutput(string s)
        {
            outputBox.Text += "\n" + s;
            outputBox.ScrollToEnd();
        }

        private void UpdateNormal()
        {
            outputBox.Background = new SolidColorBrush(Color.FromRgb(0xFF, 0xFF, 0xFF));
        }

        private void UpdateUnconnected()
        {
            outputBox.Background = new SolidColorBrush(Color.FromRgb(0xFF, 0x00, 0x00));
        }

        private void UpdateConnected()
        {
            outputBox.Background = new SolidColorBrush(Color.FromRgb(0x00, 0xAA, 0x00));
        }

        private void StartButton_Click(object sender, RoutedEventArgs e)
        {
            startButton.IsEnabled = false;
            outputBox.Text = "";
            UpdateNormal();
            LoadUItoConfig();
            cfg.SaveConfig();
            Thread thr = new Thread(RunPangolin);
            thr.Start();
        }

        private void StopButton_Click(object sender, RoutedEventArgs e)
        {
            startButton.IsEnabled = true;
            try
            {
                proc.Kill();
            }
            catch (Exception ex) { }
        }

        private void On_Closed(object sender, EventArgs e)
        {
            StopButton_Click(sender, null);
        }

        private void AboutButton_Click(object sender, RoutedEventArgs e)
        {
            MessageBox.Show("Pangolin v1.0", "Pangolin");
        }
    }
}
