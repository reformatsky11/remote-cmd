# Main Page - remote-cmd #

## About ##

---

This application will take a single command equivalent of `ssh {host} {command}`
**or** it will take a file containing a list of commands to run on the remote host.

this should replace the need for rexec.
By using libssh2 only one connection is made to the host, then multiple commands are sent in serial.
By default the script suppresses stdout, this keeps the bandwidth requirements low, and should run nicely on slow connections.

---


## Supported authentication ##
  * RSA, DSA or RSA1(identity) key
  * password


---

## Valid Arguments ##
```
    -d turns on DEBUG and displays the commands sent to the client to STDOUT
    -h displays the Usage Statement and exits RC 255
    -p Causes the application to pause and prompt user to hit Enter When errors are encountered in the commands run on the client
    -v turns on Verbose mode, will display STDOUT and STDERR for each command run on the destination client.
    -s specifies skip errors that meet regexp(s) in {errfile}
    -e specifies file of environment variables to set on remote side
    -t specifies connection timeout in seconds DEFAULTS to 5 seconds
    -T {n} this will enable tracing at level 'n'
    -P This will allocate a controlling PTY on the session
    -c {command}  specifies a single command to run on the destination client
    -f {filename} specifies a file containing commands to run on the destination client
    {HOSTNAME} this is the destination client hostname or IP address
```


---

## Return Codes ##
  * exits with RC 255 on a usage error
  * exits with RC -1 on all other errors
  * exits with RC 0 upon successful execution


---

## Prerequisites ##
  * [openssl](http://www.openssl.org/)
  * [libssh2](http://www.libssh2.org/)