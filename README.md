# suexec

	suexec username command [argument...]

`suexec` closes stdin, clears enviroment, switches user and executes command.

## Warning
[It is not safe](https://www.openwall.com/lists/oss-security/2017/06/03/9) to use `suexec` within interactive sessions instead of `su` or `sudo`.