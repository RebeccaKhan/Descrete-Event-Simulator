from counter import TimeIndependentCounter
from simulation import Simulation
from matplotlib import pyplot

"""
This file should be used to keep all necessary code that is used for the simulation section in part 5
of the programming assignment. It contains tasks 5.2.1, 5.2.2, 5.2.3 and 5.2.4.
"""


def task_5_2_1():
    """
    Run task 5.2.1. Make multiple runs until the blocking probability distribution reaches
    a confidence level alpha. Simulation is performed for 100s and 1000s and for alpha = 90% and 95%.
    """
    results = [None, None, None, None]
    # TODO Task 5.2.1: Your code goes here

    # print and return results
    print(
        'SIM TIME:  100s; ALPHA: 10%; NUMBER OF RUNS: ' + str(results[0]) + '; TOTAL SIMULATION TIME (SECONDS): ' + str(
            results[0] * 100))
    print(
        'SIM TIME:  100s; ALPHA:  5%; NUMBER OF RUNS: ' + str(results[1]) + '; TOTAL SIMULATION TIME (SECONDS): ' + str(
            results[1] * 100))
    print('SIM TIME: 1000s; ALPHA: 10%; NUMBER OF RUNS:  ' + str(
        results[2]) + '; TOTAL SIMULATION TIME (SECONDS): ' + str(results[2] * 1000))
    print('SIM TIME: 1000s; ALPHA:  5%; NUMBER OF RUNS:  ' + str(
        results[3]) + '; TOTAL SIMULATION TIME (SECONDS): ' + str(results[3] * 1000))
    return results


def task_5_2_2():
    """
    Run simulation in batches. Start the simulation with running until a customer count of n=100 or (n=1000) and
    continue to increase the number of customers by dn=n.
    Count the blocking proabability for the batch and calculate the confidence interval width of all values, that have
    been counted until now.
    Do this until the desired confidence level is reached and print out the simulation time as well as the number of
    batches.
    """
    results = [None, None, None, None]
    # TODO Task 5.2.2: Your code goes here

    # print and return results
    print('BATCH SIZE:  100; ALPHA: 10%; TOTAL SIMULATION TIME (SECONDS): ' + str(results[0] / 1000))
    print('BATCH SIZE:  100; ALPHA:  5%; TOTAL SIMULATION TIME (SECONDS): ' + str(results[1] / 1000))
    print('BATCH SIZE: 1000; ALPHA: 10%; TOTAL SIMULATION TIME (SECONDS): ' + str(results[2] / 1000))
    print('BATCH SIZE: 1000; ALPHA:  5%; TOTAL SIMULATION TIME (SECONDS): ' + str(results[3] / 1000))
    return results


def task_5_2_4():
    """
    Plot confidence interval as described in the task description for task 5.2.4.
    We use the function plot_confidence() for the actual plotting and run our simulation several times to get the
    samples. Due to the different configurations, we receive eight plots in two figures.
    """
    # TODO Task 5.2.4: Your code goes here
    pass


def plot_confidence(sim, x, y_min, y_max, calc_mean, act_mean, ylabel):
    """
    Plot confidence levels in batches. Inputs are given as follows:
    :param sim: simulation, the measurement object belongs to.
    :param x: defines the batch ids (should be an array).
    :param y_min: defines the corresponding lower bound of the confidence interval.
    :param y_max: defines the corresponding upper bound of the confidence interval.
    :param calc_mean: is the mean calculated from the samples.
    :param act_mean: is the analytic mean (calculated from the simulation parameters).
    :param ylabel: is the y-label of the plot
    :return:
    """
    # TODO Task 5.2.3: Your code goes here
    """
    Note: You can change the input parameters, if you prefer to.
    """


if __name__ == '__main__':
    task_5_2_1()
    task_5_2_2()
    task_5_2_4()
