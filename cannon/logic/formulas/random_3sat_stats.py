import glob
import yaml
import numpy as np
import matplotlib.pyplot as plt
import collections

def run():
    stat_files = glob.glob("/home/cannon/Documents/cannon/cannon/logic/formulas/random_3sat/*.yaml")

    times = collections.defaultdict(list)
    calls = collections.defaultdict(list)
    sats = collections.defaultdict(list)

    for f in stat_files:
        exp_part = f.split("/")[-1].split("_")[0]
        with open(f, 'r') as of:
            stats = yaml.load(of)
            stats = stats['stats']
        
            try:
                times[exp_part].append(int(stats["time_micros"]))
                calls[exp_part].append(int(stats["calls"]))
                sats[exp_part].append(bool(stats["sat"]))
            except:
                print("Ill-formed yaml found")

    x_100 = []
    time_medians_100_y = []
    call_medians_100_y = []
    sat_avg_100_y = []

    x_150 = []
    time_medians_150_y = []
    call_medians_150_y = []
    sat_avg_150_y = []

    for k in times.keys():
        print("Stats for {} with {} exps".format(k, len(times[k])))
        print("\tAverage time: {} seconds".format(np.array([float(t)/1e6 for t in times[k]]).mean()))
        print("\tMedian time: {} seconds".format(np.median(np.array([float(t)/1e6 for t in times[k]]))))
        print("\tAverage calls: {}".format(np.array([float(c) for c in calls[k]]).mean()))
        print("\tMedian calls: {}".format(np.median(np.array([float(c) for c in calls[k]]))))
        print("\tAverage sat: {}".format(np.array([float(s) for s in sats[k]]).mean()))

        if k[1:4] == '100':
            x_100.append(float(k[5:8]) / 100.0)
            time_medians_100_y.append(np.median(np.array([float(t)/1e6 for t in times[k]])))
            call_medians_100_y.append(np.median(np.array([float(c) for c in calls[k]])))
            sat_avg_100_y.append(np.array([float(s) for s in sats[k]]).mean())
        else:
            x_150.append(float(k[5:8]) / 150.0)
            time_medians_150_y.append(np.median(np.array([float(t)/1e6 for t in times[k]])))
            call_medians_150_y.append(np.median(np.array([float(c) for c in calls[k]])))
            sat_avg_150_y.append(np.array([float(s) for s in sats[k]]).mean())

    x_100 = np.array(x_100)
    time_medians_100_y = np.array(time_medians_100_y)
    call_medians_100_y = np.array(call_medians_100_y)
    sat_avg_100_y = np.array(sat_avg_100_y)
    x_150 = np.array(x_150)
    time_medians_150_y = np.array(time_medians_150_y)
    call_medians_150_y = np.array(call_medians_150_y)
    sat_avg_150_y = np.array(sat_avg_150_y)
    order_100 = np.argsort(x_100)
    order_150 = np.argsort(x_150)

    # Median time
    plt.subplot(1, 3, 1)
    plt.plot(x_100[order_100], time_medians_100_y[order_100], label="N=100")
    plt.plot(x_150[order_150], time_medians_150_y[order_150], label="N=150")
    plt.hlines(1200, 3.0, 6.0, linestyles="dotted")
    plt.legend()
    plt.xlabel("L/N ratio")
    plt.ylabel("Median runtime (seconds)")

    # Median calls
    plt.subplot(1, 3, 2)
    plt.plot(x_100[order_100], call_medians_100_y[order_100], label="N=100")
    plt.plot(x_150[order_150], call_medians_150_y[order_150], label="N=150")
    plt.legend()
    plt.xlabel("L/N ratio")
    plt.ylabel("Median DPLL calls")

    # Average sat
    plt.subplot(1, 3, 3)
    plt.plot(x_100[order_100], sat_avg_100_y[order_100], label="N=100")
    plt.plot(x_150[order_150], sat_avg_150_y[order_150], label="N=150")
    plt.legend()
    plt.xlabel("L/N ratio")
    plt.ylabel("Probability of satisfiability")

    plt.show()

if __name__ == "__main__":
    run()
