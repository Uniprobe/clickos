// -*- c-basic-offset: 4 -*-
#ifndef CLICK_DELAYSHAPER_HH
#define CLICK_DELAYSHAPER_HH
#include <click/element.hh>
#include <click/timer.hh>
#include <click/notifier.hh>
CLICK_DECLS

/*
=c

DelayShaper(DELAY)

=s packet scheduling

shapes traffic to meet delay requirements

=d

Pulls packets from the single input port. Delays them for at least DELAY
seconds, with microsecond precision. A packet with timestamp T will be emitted
no earlier than time (T + DELAY). On output, the packet's timestamp is set to
the current time.

Differs from DelayUnqueue in that both its input and output are pull. Packets
being delayed are enqueued until the necessary time has passed. At most one
packet is enqueued at a time. DelayUnqueue returns null for every pull request
until the enqueued packet is ready.

SetTimestamp element can be used to stamp the packet.

=a BandwidthShaper, DelayUnqueue, SetTimestamp */

class DelayShaper : public Element, public ActiveNotifier { public:
  
    DelayShaper();
    ~DelayShaper();

    const char *class_name() const	{ return "DelayShaper"; }
    const char *processing() const	{ return PULL; }
    void *cast(const char *);
    DelayShaper *clone() const		{ return new DelayShaper; }

    int configure(Vector<String> &, ErrorHandler *);
    int initialize(ErrorHandler *);
    void cleanup(CleanupStage);
    void add_handlers();
    static String read_param(Element *e, void *);

    Packet *pull(int);
    void run_timer();

    // from ActiveNotifier
    SearchOp notifier_search_op();
    
  private:

    Packet *_p;
    struct timeval _delay;
    Timer _timer;
    NotifierSignal _upstream_signal;

};

CLICK_ENDDECLS
#endif
