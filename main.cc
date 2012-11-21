/*
 * =====================================================================================
 *
 *       Filename:  main.cc
 *
 *    Description:  Main program
 *
 *        Version:  1.0
 *        Created:  11/21/2012 02:03:13 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Shani Hadiyanto
 *
 * =====================================================================================
 */
#include <unistd.h>
#include <iostream>
#include <signal.h>
#include <jackclient.h>

static const char	*jname = "wsola";
static bool		stop = false;

    static void
signal_handler(int)
{
    std::cerr << "signal received, exiting ..." << std::endl;
    stop = true;
}
    int
main (int argc, char *argv[])
{
    Jackclient *J;

    J = new Jackclient (jname, 0);

     //install a signal handler to properly quits jack client 
    signal(SIGQUIT, signal_handler);
    signal(SIGTERM, signal_handler);
    signal(SIGHUP, signal_handler);
    signal(SIGINT, signal_handler);

     //keep running until the Ctrl+C 
    while (! stop)
    {
	sleep (1);
    }

    delete J;
    exit (0);
}


