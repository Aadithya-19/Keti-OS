# Keti-OS

One thing I want to highlight is, I will continuously add more stuff and this is my README, so I'll probably move this down as I add ways to personalize this for others, and how to do what I did. btw, if you are reading this and are feeling lost, dont worry. I was in your shoes (if you are size 13 UK) in June 14th 2026 (the day I started), and I asked Claude (cuz our brains are shrinking lol) for some resources to learn and it made an insanely good looking html page that you can run and use all the resources I used. I also asked for a checklist and this has the checklist html as well. Shoutout to NanoByte, and many others. You guys are the GOATs!


Log 1 (06/14): 

I am going to log this as it goes, currently, Keti is just a simple OS, and it just prints the name and my name.

Eventually I am going to spruce up this README and hopefully I can make the OS better. Maybe this is like one 
of those directors cut type note lol.

On a serious note though, what I want KetiOS to look like is pretty far in the future (hopefully not that far) but I want to 
add some games, probably Chess (hopefully I can figure out a way to bring in chess.com as an app?), i have thought of tetris, and maybe more
Probably will make a calculator type setup, but it wont be anything too complicated, something simple and fun. I don't want a windows level GUI but
anything that looks appealing to the eye and is not a black screen that is static, should  be a win.

One thing to note is, rather than running your qasm and nemu lines continuously, just make a MAKEFILE (thank you ECE 264 :>).
Also install the assembly extensions.

Log 2 (06/16):

Up till now I managed to add the link.ld and the loader.asm files, these are basically the entry points to using C and the loader has the CAFEBABE statement going to eax (Thank you Little OS Book!), and basically helps the GRUB load the kernel and continues. Next is the link.ld, so basically this is a map that has the object files you need, like everything you write can be split and the script helps distribute it. The files have all the things mentioned, there is one for read-only variable, unassigned data, assigned data, and all code sections. Little OS Book said that going with 4KB is the strat and the kernel needs to be loaded at a memory address of >= 1MB. Below that is taken by the BIOS and GRUB itself (Seriously, Little Book OS is the GOAT). 

After much debugging and seeing why the code is stuck at read from DVD and CD, (with the help of Claude) we have managed to fix the bootloader and the Keti-OS is in a 32-bit protected mode! The fix was in build/isodir/boot/grub/grub.cfg, the set timeout had to be changed from 0 to 3 so there is some delay before boot. Now that we have the successful bootloader (GRUB), and C can be run!

So far (and Im gonna commit for now), what exists are mentioned above. I am improving the make file to have the run commands.
make just runs everything (i mean the command is there so you can do make run, make clean (make all does run im pretty sure)). 