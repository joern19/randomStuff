# Choosing the library
## Discord
Currently, discord.py cannot recieve audio, it is WIP however.
discord.py is the only python library. Other language candiates are Go, Java, Javascript and Rust.
Looking at this matrix: https://discordapi.com/unofficial/comparison.html and considering the chosen SIP-VoIP library, JDA and Discord4J are both viable options.
Given that JDA is a bit more popular, I chose that library.

At the point of writing this (2.8.23), recieving audio is not documented by discord. It may or may not work.

## VoIP
The only library I found that look promising was PJSIP. It is written in C/C++ and has bindings for Java and Python.

# Communication with VoIP Gateway
The primary focus of this library is, that it is compatible with FRITZ!Box.
Opus is not supported, which is the encoding discord uses.
The supportet codec are:
- G.711a (A-Law)
- G.711u (µ-Law)
- G.711 HD
- G.722
- G.726-24
- G.726-32
- G.726-40
- iLBC 13.3 (iLBC 30)
- iLBC 15.2 (iLBC 20)

PJSIP supports following overlapping codec:
- G.711
- G.722
- G.722.1/C
- Intel IPP codecs (G.722.1, G.723.1, G.726, G.728, G.729, AMR, and AMR-WB)

G.722 matches exactly. This will most likely be the used codec.
