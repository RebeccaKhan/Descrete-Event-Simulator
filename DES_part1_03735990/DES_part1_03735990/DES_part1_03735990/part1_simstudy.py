from simparam import SimParam
from simulation import Simulation
import random
import matplotlib.pyplot as plt
import warnings
import numpy as np
warnings.filterwarnings("ignore", category=UserWarning)

"""
This file should be used to keep all necessary code that is used for the simulation study in part 1 of the programming
assignment. It contains the tasks 1.7.1, 1.7.2 and 1.7.3.

The function do_simulation_study() should be used to run the simulation routine, that is described in the assignment.
"""


def task_1_7_1():
    """
    Execute task 1.7.1 and perform a simulation study according to the task assignment.
    :return: Minimum number of buffer spaces to meet requirements.
    """
    sim_param = SimParam()
    random.seed(sim_param.SEED)
    sim = Simulation(sim_param)
    return do_simulation_study(sim)


def task_1_7_2():
    """
    Execute task 1.7.2 and perform a simulation study according to the task assignment.
    :return: Minimum number of buffer spaces to meet requirements.
    """
    sim_param = SimParam()
    random.seed(sim_param.SEED)
    sim_param.SIM_TIME = 1000000
    sim_param.MAX_DROPPED = 100
    sim_param.NO_OF_RUNS = 100
    sim = Simulation(sim_param)
    return do_simulation_study(sim)


def task_1_7_3():
    """
    Execute task 1.7.3.
    """
    # TODO Task 1.7.3: Your code goes here (if necessary)
    simtime_array = [100000, 500000, 1000000, 1500000, 2000000]
    blockingprob_array = []
    std_array = []
    sim_param = SimParam()
    random.seed(sim_param.SEED)
    plt.figure(figsize=(20, 20))
    for k in range(len(simtime_array)):
        blockingprob_array[:] = []
        sim_param.SIM_TIME = simtime_array[k]
        sim_param.MAX_DROPPED = simtime_array[k] / 10000
        sim_param.NO_OF_RUNS = 100
        sim_param.S = 4
        sim = Simulation(sim_param)
        label_string = ["100 sec", "500 sec", "1000 sec", "1500 sec", "2000 sec"]
        for i in range(sim.sim_param.NO_OF_RUNS):
            sim.reset()
            blockingprob_array += [sim.do_simulation().blocking_probability]  # Record the blocking prob of each run
        plt.subplot(311)
        plt.title('CDF of the Blocking probabilities')
        plt.ylabel('Probability')  # Plot CDF
        plt.hist(blockingprob_array, density=True, cumulative=True, bins=1024, histtype='step', label=label_string[k])
        plt.legend(loc='upper left')
        plt.subplot(312)
        plt.title(" Histogram of the Blocking probabilities")
        plt.ylabel('No of Occurences')  # Plot histogram
        plt.hist(blockingprob_array, density=False, cumulative=False, bins=10, histtype='step',
                 label=label_string[k])
        plt.legend(loc='upper right')
        std_array += [np.std(blockingprob_array)]
        print("Standard Deviation for simulation time = " + str(label_string[k]) + " is " +
              str(np.std(blockingprob_array)))
    plt.subplot(313)
    plt.title('Standard Deviation vs SimTime')
    plt.ylabel('Standard Deviation')
    plt.plot(label_string, std_array)
    plt.show()


def do_simulation_study(sim):
    """
    Implement according to task description.
    """
    # TODO Task 1.7.1: Your code goes here
    flag = True
    while flag:

        success = 0
        for i in range(sim.sim_param.NO_OF_RUNS):  # Iterate through the number of runs
            sim.reset()
            if sim.do_simulation().packets_dropped < sim.sim_param.MAX_DROPPED:  # if packet drop more than threshold
                success = success + 1  # inc success
        print("Buffer Size: " + str(sim.sim_param.S) + "    Success Rate: " +
              str(float(success) / float(sim.sim_param.NO_OF_RUNS)))
        if (float(success) / float(sim.sim_param.NO_OF_RUNS)) >= 0.8:  # Enter Verification mode try 10times
            print("Test Successful, starting retest routine")
            for j in range(10):
                success = 0
                for i in range(sim.sim_param.NO_OF_RUNS):
                    sim.reset()
                    if sim.do_simulation().packets_dropped < sim.sim_param.MAX_DROPPED:
                        success = success + 1
                if (float(success) / float(sim.sim_param.NO_OF_RUNS)) >= 0.8:
                    print("Successful ReTest Number " + str(j + 1) + " had Success Rate of: " +
                          str(float(success) / float(sim.sim_param.NO_OF_RUNS)))
                    if j == 9:
                        print("The Queue length required such that after " +
                              str(sim.sim_param.SIM_TIME / 1000) + " seconds, less than "
                              + str(sim.sim_param.MAX_DROPPED) + " packets are dropped in 80% of the cases is " +
                              str(sim.sim_param.S))

                        flag = False
                else:  # if the success rate is less than 80 even once
                    print("ReTest number " + str(j + 1) + " Failed because the Success Rate was" +
                          str(float(success) / float(sim.sim_param.NO_OF_RUNS)))
                    break  # increment buffer size and start again

        if flag:
            sim.sim_param.S = sim.sim_param.S + 1

    return sim.sim_param.S


if __name__ == '__main__':
    task_1_7_1()
    task_1_7_2()
    task_1_7_3()
