# Jinglepings-server

Ping! Ping! Jingleping! Make the pings go fast!

This a simple C++ implementation of a Jinglepings server.

Flow of packets is:

```
 (ICMPv6 packet)
        ↓
      Socket
        ↓
 JingleController
        ↓
  JingleBuffer(s)
        ↓
Gstreamer Pipeline
        ↓
  (RTMP server)
```

Configuration is done in `CMakeLists.txt`.
