/*
 * =====================================================================================
 *
 *       Filename:  jackclient.cc
 *
 *    Description:  Jackclient Class
 *
 *        Version:  1.0
 *        Created:  10/24/2012 07:47:47 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Shani Hadiyanto
 *
 * =====================================================================================
 */

#include "jackclient.h"
#include <iostream>
#include <cstdio>

Jackclient::Jackclient (const char *jname, const char*jserv) :
    _client (0)
{
    init (jname, jserv);
}


Jackclient::~Jackclient (void)
{
    fini ();
}


void Jackclient::init (const char *jname, const char *jserv)
{
    jack_status_t       status;
    jack_options_t      options = JackNullOption;

    //TODO add server name
	    //int my_option = JackNullOption | JackServerName;
	    //options = (jack_options_t)my_option;
    _client = jack_client_open (jname, options, &status, jserv);

    if (_client == 0)
    {
	std::cerr << "Can't connect to Jack, is the server running ?" << std::endl;
	exit (1);
    }

    jack_set_process_callback (_client, jack_static_process, (void *) this);
    jack_on_shutdown (_client, jack_static_shutdown, (void *) this);

    _bsize = 0;
    _fsamp = 0;

//TODO prepare filter table
//
    _note = 60;
    _jname = jack_get_client_name (_client);
    _bsize = jack_get_buffer_size (_client);
    _fsamp = jack_get_sample_rate (_client);

    /* create input/output ports */
    _ports[INPUT] = jack_port_register (_client, "input",
	    JACK_DEFAULT_AUDIO_TYPE,
	    JackPortIsInput, 0);
    _ports[OUTPUT] = jack_port_register (_client, "output",
	    JACK_DEFAULT_AUDIO_TYPE,
	    JackPortIsOutput, 0);
    _ports[MIDI] = jack_port_register (_client, "midi",
	    JACK_DEFAULT_MIDI_TYPE,
	    JackPortIsInput, 0);

    if ((_ports[INPUT] == NULL) || (_ports[OUTPUT] == NULL) || (_ports[MIDI] == NULL))
    {
	std::cerr << "no more JACK ports available" << std::endl;
	exit (1);
    }

    if (jack_activate (_client))
    {
	std::cerr << "Can't activate Jack" << std::endl;
	exit (1);
    }
}


void Jackclient::fini (void)
{
    if (_client)
    {
	jack_deactivate (_client);
	jack_client_close (_client);
    }
}


void Jackclient::jack_static_shutdown (void *arg)
{
    std::cout << "Jack server shutting down..." << std::endl;
    //TODO clean up memory
    exit(1);
}


int Jackclient::jack_static_process (jack_nframes_t nframes, void *arg)
{
    return ((Jackclient *) arg)->jack_process (nframes);
}


int Jackclient::jack_process (jack_nframes_t nframes)
{
    //Read MIDI port
    void *midi_buf;
    jack_midi_event_t in_event;
    jack_nframes_t event_index = 0;
    jack_nframes_t event_count;

    midi_buf = jack_port_get_buffer(_ports[MIDI], nframes);
    event_count = jack_midi_get_event_count(midi_buf);

    jack_midi_event_get(&in_event, midi_buf, event_index);
    std::cout << std::hex << static_cast<unsigned int> (*(in_event.buffer)) << std::endl;
    if (event_count)
    {

    //Only the FIRST NOTEON on each frame is read
    /*
     *while ((((*(in_event.buffer)) & 0xf0) != 0x90) & (event_index < event_count))
     *{
     *jack_midi_event_get(&in_event, midi_buf, event_index++);
     *}
     */

    /*
     *if (((*(in_event.buffer)) & 0xf0) == 0x90)
     *{
     *    _note = *(in_event.buffer + 1);
     *}
     */
    }

    _shift = _note - 60;
    if (_shift > 12)
    {
	_shift = 12;
    }
    if (_shift < -12)
    {
	_shift = -12;
    }

    int i = 0;
    jack_default_audio_sample_t *in, *out;

    in = (jack_default_audio_sample_t*)jack_port_get_buffer (_ports[INPUT], nframes);
    out = (jack_default_audio_sample_t*)jack_port_get_buffer (_ports[OUTPUT], nframes);

    if (_shift == 0)
    {
	for( i=0; i < nframes; i++ )
	{
	    out[i] = in[i];
	}
    }
    //else
    //{
    //sola(in, out, _shift, nframes);
    //}

    return 0;
}
