/*
 * =====================================================================================
 *
 *       Filename:  jackclient.h
 *
 *    Description:  Jackclient Class Header
 *
 *        Version:  1.0
 *        Created:  11/21/2012 12:33:56 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Shani Hadiyanto
 *
 * =====================================================================================
 */
#ifndef __JACKCLIENT_H
#define __JACKCLIENT_H

#include <jack/jack.h>
#include <jack/midiport.h>


class Jackclient
{
    public:

	enum {INPUT, OUTPUT, MIDI};

	Jackclient (const char  *jname, const char *jserv);
	virtual ~Jackclient (void);

	const char *jname (void) const { return _jname; }
	int fsamp (void) const { return _fsamp; }
	int bsize (void) const { return _bsize; }

    private:

	void init (const char *jname, const char *jserv);
	void fini (void);

	int  jack_process (jack_nframes_t nframes);

	jack_client_t  *_client;
	jack_port_t    *_ports [3];
	const char     *_jname;
	int             _bsize;
	int             _fsamp;
	int             _shift;
	int             _note;

	static void jack_static_shutdown (void *arg);
	static int  jack_static_process (jack_nframes_t nframes, void *arg);
};

#endif
