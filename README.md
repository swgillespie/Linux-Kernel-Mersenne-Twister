## Mersenne Twister - Linux Kernel Module

This project was done as a small project to introduce myself into Linux kernel programming. The goal was to do something nontrivial, but still easy enough to not distract from the goal of learning how to hack inside the kernel. I chose to implement the Mersenne Twister algorithm - an algorithm for generating random numbers. The Linux kernel already has randomness in the form of /dev/random, but I wanted to make my own.

To build this project, first `git clone` this repository and `make`. Install the kernel module with `insmod linux_twister.ko`, and remove it with `rmmod linux_twister`. Don't just take my word about installing this module, though! Read through my code and make sure it's safe before you install any untrusted kernel modules!

I tried to document my code as best I could to help assist people who are also trying to dive in to Linux kernel programming. I might do some more kernel stuff later, too. 