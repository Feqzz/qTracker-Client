# qTracker-Client README
qTracker is an application built to function with the [BitTorrent](https://en.wikipedia.org/wiki/BitTorrent) peer-to-peer file sharing protocol.
The client is used for downloading and uploading torrents. It has common functionalities such as hit and runs, bonus points, ratios and invites.

# Dependencies
* `Qt 5.*`
* `libqt5sql5-mysql`
* `Boost 1.23+`
* `libssl-dev`

# Documentation
The online documentation is available at this [website](https://tarves.no/docs/QTracker-Client/).You might find the screenshots of the application and the development process in the [wiki](https://github.com/Feqzz/qTracker-Client/wiki) interesting.

# Tracker
The torrent files downloaded from the client carries a personal announce url. Each time your BitTorrent client announces the torrent it will send and receive data from
our tracker software. It is written in `C++` and can be found [here](https://github.com/Feqzz/qTracker-Tracker).

# Server
When sending out invites, a server is being used to send the e-mails. The source code can be found [here](https://github.com/Feqzz/qTracker-Server).

## Credits
Much appreciations to [Kornelius Hauge](https://www.behance.net/korneliushauge) for creating the logo and icons.<br>
Thanks to [jimporter](https://github.com/jimporter/bencode.hpp) for the bencode header-only library already included.
