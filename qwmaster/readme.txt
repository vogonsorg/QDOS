	Daikatana/GameSpy Encode Type 0 Emulator 0.1 by [HCI]Maraakate

Quickly, this is mainly for daikatana (but should work OK with older encode
type 0 gamespy enabled games).  Based off of Q2Master v1.1 by QwazyWabbit.
Supports the TCP handshake, the secret key validation is ignored.  Probable
exploits, this is one of my first times playing around with sockets.
If you find some serious issues email me emoaddict15@gmail.com.

New command line switches:
 -sendack - by default gamespy doesn't not send this type of packet out.
            if you want to extend the courtesy of acknowleding the
            heartbeat then enable this setting.

 -quickvalidate - by default the master server requires 1 extra heartbeat
                  and a successful ping request to be added to the query
                  list.  Set this to allow any new server to show up
                  immediately.


 -validationrequired <1, 2, or 3> - Require validation from the challenge key
                                    cross-checked with the games secure key.
                                    1 - client list requests only.
                                    2 - servers.
                                    3 - clients and servers (recommended).

 -timestamp <1 or 2> - Debug outputs are timestampped.  1 - for AM/PM.
                       2 for military.

 -heartbeatinterval <time in minutes> - Time in minutes for sending heartbeats.
                                        Must be at least 1 minute.

 -minimumheartbeats x - Minimum number of sucessful heartbeats that need to be
                        sent before a server will be added to the list.

 -tcpport - xxxxx causes server to bind to a particular TCP port for the
 gamespy list query from clients. Default is 28900.
 If you depart from this you need to communicate this to your users somehow.
 This feature is included since this code could be modified to provide
 master services for other games.

-serverlist <filename> - Adds servers from a list.  Hostnames are supported.
Format is <ip>,<query port>,<gamename> i.e. maraakate.org,27982,daikatana.

... Original Readme...

			Quake 2 Master v1.1 by QwazyWabbit

This program is a modification of R1ch's GloomMaster server.
The GloomMaster only accepted heartbeats from Gloom servers but this version
will accept heartbeats from any Quake2 game server and respond to client
queries with the list of servers it knows about.

This version runs as a Windows Console application, a Windows Service
or a Linux console application or a Linux daemon. The daemon fork code
is based on BSD so it should also run on BSD or other Unix based systems
and even Mac OS/X without much modification.

This server is a single executable file, no other files are required.
In debug mode the server remains attached to the console that invoked it
and it prints status messages to the screen.

In Windows, it creates registry entries for the IP address and port that
it will bind to, this allows for binding to specific addresses and ports
on multi-homed network cards in service mode since services usually don't
process or receive command line arguments.

Windows Installation is simple.

1. Copy the program executable to a directory you intend to run it from.
   This is usually /windows/system32 for services but you can put it
   in a general tools folder if you prefer.

2. With the cmd prompt in the folder containing the executable, type:
   "master -ip xxx.xxx.xxx.xxx" to set the IP address to bind.
   Don't use this command if 0.0.0.0 (all IP's on host) is OK.

3. Type "master -install" to install the service in the registry.

4. Type "net start q2masterserver" to start the service. The server listens
   on UDP port 27900 by default and this is where most clients expect
   Q2 master servers to be.

Linux installation varies, so I won't attempt to describe it here. Starting
the process is simple, command line: "master -ip xxx.xxx.xxx.xxx" will cause
the server to bind to the specified IP and detatch from the console that
started the server. Use "master -debug -ip x.x.x.x" to keep it in debug mode.
Use whatever method your Linux distro requires to install it as a daemon.

// General command line switches:

// -debug	Asserts debug mode. The program prints status messages to console
//			while running. Shows pings, heartbeats, number of servers listed.

// -ip xxx.xxx.xxx.xxx causes server to bind to a particular IP address when
//	used with multi-homed hosts. Default is 0.0.0.0 (any).

// -port xxxxx causes server to bind to a particular port. Default is 27900.
// Default port for Quake 2 master servers is 27900. If you depart from this
// you need to communicate this to your users somehow. This feature is included
// since this code could be modified to provide master services for other games.

// *** Windows *** usage:

// Place executable in %systemroot%\system32 or other known location.
// To debug it as a console program, command: "master -debug" and it outputs
// status messages to the console. Ctrl-C shuts it down.
//
// From a cmd prompt type: q2master -install to install the service with defaults.
// The service will be installed as "Q2MasterServer" in the Windows service list.
//

// -install	Installs the service on Windows.
// -remove	Stops and removes the service.
// When the service is installed it is installed with "Automatic" startup type
// to allow it to start up when Windows starts. Use SCM to change this.

// Other commands:
// net start q2masterserver to start the service.
// net stop q2masterserver to stop the service.
//
// Use the Services control panel applet to start/stop, change startup modes etc.
// To uninstall the server type "master -remove" to stop and remove the active service.

// *** Linux *** usage:

// -debug Sets process to debug mode and it remains attached to console.
// If debug is not specified on command line the process forks a daemon and
// detaches from terminal.
//
// Send the process a SIGTERM to stop the daemon. "kill -n SIGTERM <pid>"
// Use "netstat -anup" to see the processes/pids listening on UDP ports.
// Use "ps -ux" to list detached processes, this will show the command line that
// invoked the q2master process.
// 
// A Linux binary "q2master" is included with this source archive.
