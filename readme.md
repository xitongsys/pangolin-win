# ![logo](imgs/logo.png) Pangolin-win
This is a windows client for [pangolin](https://github.com/xitongsys/pangolin).
It supports UDP and PTCP protocol.

![win](imgs/win.png)

## Quick start

### Run with UI
Run the ```pangolin-ui.exe``` using Administrator.

### Run in command line
1. Configure the config.txt file. 
```json
{
    "role": "client",
    "server": "0.0.0.0:12345",
    "tun": "10.0.0.22/8",
    "dns": "8.8.8.8",
    "mtu": 1400,
    "protocol": "ptcp",
    "tokens": ["token01", "token02"]
}
```

2. Run the ```pangolin-win.exe``` using Administrator.
