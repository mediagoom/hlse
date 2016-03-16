# hlse
HTTP LIVE STREAM encryption / decryption command line utility.

Build
-----

In order to build the solution you need to download the aes project in a folder at the same level with hlse and build one of the project 
in order to a aes.lib library to link to.

- aes
- hlse


Usage
-----

hls [input file] [output file] [E/D] [key file] [hls sequence number]

where:
- input file is the hls chunk file to be encrypted or decrypted.
- output file is the resulting file.
- E for encryption. D for decrytion.
- key file is a 16 byte file.
- hls sequence number is the sequence number of the hls chunk.

