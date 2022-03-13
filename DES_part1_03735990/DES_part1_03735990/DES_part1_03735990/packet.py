class Packet(object):

    """
    Packet represents a data packet processed by the DES.

    It contains variables for measurements, which include timestamps (arrival, service start, service completion)
    and the status of the packet (served, completed).
    """

    def __init__(self, sim, iat=None):
        """
        Initialize a packet with its arrival time and (optionally) the inter-arrival time w.r.t. the last packet.
        :param sim: simulation, the packet belongs to
        :param iat: inter-arrival time with respect to the last arrival
        :return: packet object
        """
        self.sim = sim

        self.t_arrival = sim.sim_state.now
        self.t_start = -1
        self.t_complete = -1
        self.iat = iat

        self.waiting = True
        self.served = False
        self.completed = False

    def start_service(self):
        """
        Change the status of the packet once the serving process starts.
        """
        # TODO Task 2.1.1: Your code goes here
        pass
        
    def complete_service(self):
        """
        Change the status of the packet once the serving process is completed.
        """
        # TODO Task 2.1.1: Your code goes here
        pass

    def get_waiting_time(self):
        """
        Return the waiting time of the packet. An error occurs when the packet has not been served yet.
        :return: waiting time
        """
        # TODO Task 2.1.1: Your code goes here
        pass
    
    def get_service_time(self):
        """
        Calculate and return the service time
        :return: service time
        """
        # TODO Task 2.1.1: Your code goes here
        pass

    def get_system_time(self):
        """
        Calculate and return the system time (waiting time + service time)
        :return: system time (waiting time + serving time)
        """
        # TODO Task 2.1.1: Your code goes here
        pass

    def get_interarrival_time(self):
        """
        Return the inter-arrival time between the current and the last customer/packet
        :return: inter-arrival time
        """
        # TODO Task 2.1.1: Your code goes here
        pass
