using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.Serialization;
using System.Runtime.Serialization.Json;
using System.Text;

namespace pangolin_ui
{
    [DataContract]
    public class Config
    {
        [DataMember]
        public string server;
        [DataMember]
        public string dns;
        [DataMember]
        public string tun;
        [DataMember]
        public string protocol;
        [DataMember]
        public int mtu;
        [DataMember]
        public ArrayList tokens = new ArrayList();

        public Config()
        {
            server = "0.0.0.0:12345";
            dns = "1.1.1.1";
            tun = "10.0.0.22";
            protocol = "ptcp";
            mtu = 1400;
            tokens.Add("token01");
        }

        public void ReadConfig()
        {
            string cs = System.IO.File.ReadAllText("config.txt");
            var ms = new MemoryStream(Encoding.UTF8.GetBytes(cs));
            var ser = new DataContractJsonSerializer(this.GetType());
            var desConfig = new Config();
            desConfig = ser.ReadObject(ms) as Config;
            ms.Close();
            Assign(desConfig);
        }

        public void SaveConfig()
        {
            var ms = new MemoryStream();
            var ser = new DataContractJsonSerializer(this.GetType());
            ser.WriteObject(ms, this);
            ms.Position = 0;
            var sr = new StreamReader(ms);
            System.IO.File.WriteAllText("config.txt", sr.ReadToEnd());
        }

        public void Assign(Config c)
        {
            server = c.server;
            dns = c.dns;
            tun = c.tun;
            protocol = c.protocol.ToLower();
            mtu = c.mtu;
            tokens = c.tokens;
        }
    }
}
