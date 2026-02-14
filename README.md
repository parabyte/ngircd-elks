This is ngircd for elks,
At the moment can only support 5 concurrent users. "Still got some work to do!"
Daemon mode at this time does not work due to a few technical issues that need to be worked out.
so its incredibly important to issue -n flag to ngircd when running this daemon

The bare minimal config for ngircd is just its ngircd.cnf file which i expect the end user to manually put into elks/elkscmd/rootfs_template/ manually

I expect the end user to manually make a sbin folder or a folder of there choosing in the rootfs_template and stick the binary in there

the Doc folder in the root of the ngircd project needs to live in the same folder as ngircd. 

motd.txt is completely optional and seasoned irc admins can figure this out, otherwise do not worry!


to make this project you need to ensure you have the openwatcom tools installed somewhere on your machine, 

to build you need to ensure to build elks libc Large mem model, 
and with elks all setup and built

in the ngircd folder just issue

make -f Makefile.owc

okay you  need to run this with ./ngircd.os2 -n 

The tiny irc client you need to do the following to connect to this daemon

tinyirc myusername ircaddress 6667 

then once connected issue

/join #elks

enjoy!
