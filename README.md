xl: The simple X11 lock
==

xscreensaver, xlock, xlockmore, all of them, completley useless. They have
overflows, they have buggy drawning routines, they are very prone to crashing.
The more complex they get, the more useless they are. If a screen locking
program decides to crash on me, its not doing a very good job of keeping my
terminal locked. I've had many people just go to my screen and hit tons of keys
just to make it crash, or cycling the screen saver to get to a certain one
known to crash. Absolutly ridiculous!

This is my solution to the problem. Very simple, very small, very few features.
Doesn't even blank your screen, all this program does is lock your keyboard and
mouse, and only unlocks it if you type your login password and hit return.

You can also override the password by specifying it as the first argument of
the command line. A third way is to set the XLPASSWD environment variable.

2021 Addition: use PAM to check password (for those who have accounts set up
with LDAP, NDIS, etc., and where the crypted passwords are not in the passwd
or shadow file). 

Perhaps this violates the philosphy of "super-simple", but should work on more
systems. 
