import glob
import yaml
import numpy as np
import matplotlib.pyplot as plt
import collections

def run():
    random_stat_files = list(glob.glob("/home/cannon/Documents/cannon/cannon/logic/formulas/random_3sat/random_reports/*.yaml"))
    random_stat_files.sort()
    two_clause_stat_files = list(glob.glob("/home/cannon/Documents/cannon/cannon/logic/formulas/random_3sat/2_clause/*.yaml"))
    two_clause_stat_files.sort()
    custom_stat_files = list(glob.glob("/home/cannon/Documents/cannon/cannon/logic/formulas/random_3sat/*.yaml"))
    custom_stat_files.sort()

    ## Random experiment processing
    random_times = collections.defaultdict(list)
    random_calls = collections.defaultdict(list)
    random_sats = collections.defaultdict(list)

    for f in random_stat_files:
        exp_part = f.split("/")[-1].split("_")[0]
        with open(f, 'r') as of:
            stats = yaml.load(of)
            stats = stats['stats']
        
            try:
                random_times[exp_part].append(int(stats["time_micros"]))
                random_calls[exp_part].append(int(stats["calls"]))
                random_sats[exp_part].append(bool(stats["sat"]))
            except:
                print("Ill-formed yaml found")

    random_x_100 = []
    random_time_medians_100_y = []
    random_call_medians_100_y = []
    random_sat_avg_100_y = []

    random_x_150 = []
    random_time_medians_150_y = []
    random_call_medians_150_y = []
    random_sat_avg_150_y = []

    for k in random_times.keys():
        if k[1:4] == '100':
            random_x_100.append(float(k[5:8]) / 100.0)
            random_time_medians_100_y.append(np.median(np.array([float(t)/1e6 for t in random_times[k]])))
            random_call_medians_100_y.append(np.median(np.array([float(c) for c in random_calls[k]])))
            random_sat_avg_100_y.append(np.array([float(s) for s in random_sats[k]]).mean())
        else:
            random_x_150.append(float(k[5:8]) / 150.0)
            random_time_medians_150_y.append(np.median(np.array([float(t)/1e6 for t in random_times[k]])))
            random_call_medians_150_y.append(np.median(np.array([float(c) for c in random_calls[k]])))
            random_sat_avg_150_y.append(np.array([float(s) for s in random_sats[k]]).mean())


    ## 2-clause experiment processing
    two_clause_times = collections.defaultdict(list)
    two_clause_calls = collections.defaultdict(list)
    two_clause_sats = collections.defaultdict(list)

    for f in two_clause_stat_files:
        exp_part = f.split("/")[-1].split("_")[0]
        with open(f, 'r') as of:
            stats = yaml.load(of)
            stats = stats['stats']
        
            try:
                two_clause_times[exp_part].append(int(stats["time_micros"]))
                two_clause_calls[exp_part].append(int(stats["calls"]))
                two_clause_sats[exp_part].append(bool(stats["sat"]))
            except:
                print("Ill-formed yaml found")

    two_clause_x_100 = []
    two_clause_time_medians_100_y = []
    two_clause_call_medians_100_y = []
    two_clause_sat_avg_100_y = []

    two_clause_x_150 = []
    two_clause_time_medians_150_y = []
    two_clause_call_medians_150_y = []
    two_clause_sat_avg_150_y = []

    for k in two_clause_times.keys():
        if k[1:4] == '100':
            two_clause_x_100.append(float(k[5:8]) / 100.0)
            two_clause_time_medians_100_y.append(np.median(np.array([float(t)/1e6 for t in two_clause_times[k]])))
            two_clause_call_medians_100_y.append(np.median(np.array([float(c) for c in two_clause_calls[k]])))
            two_clause_sat_avg_100_y.append(np.array([float(s) for s in two_clause_sats[k]]).mean())
        else:
            two_clause_x_150.append(float(k[5:8]) / 150.0)
            two_clause_time_medians_150_y.append(np.median(np.array([float(t)/1e6 for t in two_clause_times[k]])))
            two_clause_call_medians_150_y.append(np.median(np.array([float(c) for c in two_clause_calls[k]])))
            two_clause_sat_avg_150_y.append(np.array([float(s) for s in two_clause_sats[k]]).mean())

    ## Custom heuristic experiment processing
    custom_times = collections.defaultdict(list)
    custom_calls = collections.defaultdict(list)
    custom_sats = collections.defaultdict(list)

    for f in custom_stat_files:
        exp_part = f.split("/")[-1].split("_")[0]
        with open(f, 'r') as of:
            stats = yaml.load(of)
            stats = stats['stats']
        
            try:
                custom_times[exp_part].append(int(stats["time_micros"]))
                custom_calls[exp_part].append(int(stats["calls"]))
                custom_sats[exp_part].append(bool(stats["sat"]))
            except:
                print("Ill-formed yaml found")

    custom_x_100 = []
    custom_time_medians_100_y = []
    custom_call_medians_100_y = []
    custom_sat_avg_100_y = []

    custom_x_150 = []
    custom_time_medians_150_y = []
    custom_call_medians_150_y = []
    custom_sat_avg_150_y = []

    for k in custom_times.keys():
        print("Stats for {} with {} exps".format(k, len(custom_times[k])))
        print("\tAverage time: {} seconds".format(np.array([float(t)/1e6 for t in custom_times[k]]).mean()))
        print("\tMedian time: {} seconds".format(np.median(np.array([float(t)/1e6 for t in custom_times[k]]))))
        print("\tAverage calls: {}".format(np.array([float(c) for c in custom_calls[k]]).mean()))
        print("\tMedian calls: {}".format(np.median(np.array([float(c) for c in custom_calls[k]]))))
        print("\tAverage sat: {}".format(np.array([float(s) for s in custom_sats[k]]).mean()))

        if k[1:4] == '100':
            custom_x_100.append(float(k[5:8]) / 100.0)
            custom_time_medians_100_y.append(np.median(np.array([float(t)/1e6 for t in custom_times[k]])))
            custom_call_medians_100_y.append(np.median(np.array([float(c) for c in custom_calls[k]])))
            custom_sat_avg_100_y.append(np.array([float(s) for s in custom_sats[k]]).mean())
        else:
            custom_x_150.append(float(k[5:8]) / 150.0)
            custom_time_medians_150_y.append(np.median(np.array([float(t)/1e6 for t in custom_times[k]])))
            custom_call_medians_150_y.append(np.median(np.array([float(c) for c in custom_calls[k]])))
            custom_sat_avg_150_y.append(np.array([float(s) for s in custom_sats[k]]).mean())

    random_x_100 = np.array(random_x_100)
    random_time_medians_100_y = np.array(random_time_medians_100_y)
    random_call_medians_100_y = np.array(random_call_medians_100_y)
    random_sat_avg_100_y = np.array(random_sat_avg_100_y)
    random_x_150 = np.array(random_x_150)
    random_time_medians_150_y = np.array(random_time_medians_150_y)
    random_call_medians_150_y = np.array(random_call_medians_150_y)
    random_sat_avg_150_y = np.array(random_sat_avg_150_y)
    random_order_100 = np.argsort(random_x_100)
    random_order_150 = np.argsort(random_x_150)

    two_clause_x_100 = np.array(two_clause_x_100)
    two_clause_time_medians_100_y = np.array(two_clause_time_medians_100_y)
    two_clause_call_medians_100_y = np.array(two_clause_call_medians_100_y)
    two_clause_sat_avg_100_y = np.array(two_clause_sat_avg_100_y)
    two_clause_x_150 = np.array(two_clause_x_150)
    two_clause_time_medians_150_y = np.array(two_clause_time_medians_150_y)
    two_clause_call_medians_150_y = np.array(two_clause_call_medians_150_y)
    two_clause_sat_avg_150_y = np.array(two_clause_sat_avg_150_y)
    two_clause_order_100 = np.argsort(two_clause_x_100)
    two_clause_order_150 = np.argsort(two_clause_x_150)

    custom_x_100 = np.array(custom_x_100)
    custom_time_medians_100_y = np.array(custom_time_medians_100_y)
    custom_call_medians_100_y = np.array(custom_call_medians_100_y)
    custom_sat_avg_100_y = np.array(custom_sat_avg_100_y)
    custom_x_150 = np.array(custom_x_150)
    custom_time_medians_150_y = np.array(custom_time_medians_150_y)
    custom_call_medians_150_y = np.array(custom_call_medians_150_y)
    custom_sat_avg_150_y = np.array(custom_sat_avg_150_y)
    custom_order_100 = np.argsort(custom_x_100)
    custom_order_150 = np.argsort(custom_x_150)

    # Median time
    plt.subplot(1, 3, 1)
    plt.plot(random_x_100[random_order_100], random_time_medians_100_y[random_order_100], label="N=100 (random)")
    plt.plot(random_x_150[random_order_150], random_time_medians_150_y[random_order_150], label="N=150 (random)")
    plt.plot(two_clause_x_100[two_clause_order_100], two_clause_time_medians_100_y[two_clause_order_100], label="N=100 (2-clause)")
    plt.plot(two_clause_x_150[two_clause_order_150], two_clause_time_medians_150_y[two_clause_order_150], label="N=150 (2-clause)")
    plt.plot(custom_x_100[custom_order_100], custom_time_medians_100_y[custom_order_100], label="N=100 (custom)")
    plt.plot(custom_x_150[custom_order_150], custom_time_medians_150_y[custom_order_150], label="N=150 (custom)")
    plt.hlines(1200, 3.0, 6.0, linestyles="dotted")
    plt.legend()
    plt.xlabel("L/N ratio")
    plt.ylabel("Median runtime (seconds)")

    # Median calls
    plt.subplot(1, 3, 2)
    plt.plot(random_x_100[random_order_100], random_call_medians_100_y[random_order_100], label="N=100 (random)")
    plt.plot(random_x_150[random_order_150], random_call_medians_150_y[random_order_150], label="N=150 (random)")
    plt.plot(two_clause_x_100[two_clause_order_100], two_clause_call_medians_100_y[two_clause_order_100], label="N=100 (2-clause)")
    plt.plot(two_clause_x_150[two_clause_order_150], two_clause_call_medians_150_y[two_clause_order_150], label="N=150 (2-clause)")
    plt.plot(custom_x_100[custom_order_100], custom_call_medians_100_y[custom_order_100], label="N=100 (custom)")
    plt.plot(custom_x_150[custom_order_150], custom_call_medians_150_y[custom_order_150], label="N=150 (custom)")
    plt.legend()
    plt.xlabel("L/N ratio")
    plt.ylabel("Median DPLL calls")

    # Average sat
    plt.subplot(1, 3, 3)
    plt.plot(random_x_100[random_order_100], random_sat_avg_100_y[random_order_100], label="N=100 (random)")
    plt.plot(random_x_150[random_order_150], random_sat_avg_150_y[random_order_150], label="N=150 (random)")
    plt.plot(two_clause_x_100[two_clause_order_100], two_clause_sat_avg_100_y[two_clause_order_100], label="N=100 (2-clause)")
    plt.plot(two_clause_x_150[two_clause_order_150], two_clause_sat_avg_150_y[two_clause_order_150], label="N=150 (2-clause)")
    plt.plot(custom_x_100[custom_order_100], custom_sat_avg_100_y[custom_order_100], label="N=100 (custom)")
    plt.plot(custom_x_150[custom_order_150], custom_sat_avg_150_y[custom_order_150], label="N=150 (custom)")
    plt.legend()
    plt.xlabel("L/N ratio")
    plt.ylabel("Probability of satisfiability")

    plt.show()

if __name__ == "__main__":
    run()
