
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
from matplotlib.ticker import FuncFormatter

plt.rc('text', usetex=True)
plt.rc('font', family='serif')

sns.set_palette("Paired")
# sns.set_palette("deep")
sns.set_context("poster", font_scale=2)
sns.set_style("whitegrid")
# sns.palplot(sns.color_palette("deep", 10))
# sns.palplot(sns.color_palette("Paired", 9))

line_style = ['-', '--', ':', '-.', '-', '--']
marker = ['o', 's', '^', 'p', 'h', 'D']
color = ['C5', 'C1', 'C7', 'C3', 'C9']
alpha = [1, 0.7, 0.4]
# color = ['red', 'blue', 'green', 'orange', 'black']

# plt_title = ["BlueNile", "COMPAS", "Credit Card"]

label = ["ADA", "SA", "LL", "TV", "VEC", "CBT"]
line_width = 8
marker_size = 15

f_size = (14, 10)



# def hundreds_formatter(x, pos):
#     return int(x/100)




x_list = ["100K", "1M", "10M"]



time100K = {}
time1M = {}
time10M = {}
operations = ["delete", "insert", "pointquery", "query", "reorder", "swap"]

def read_files(op):
    # 1M
    input_folder = r"1Mrepeat5"
    input_path = input_folder + "/" + op + "/" + "avg.csv"
    input_file = open(input_path, "r")
    Lines = input_file.readlines()
    count = 0
    for line in Lines:
        count += 1
        if line == '\n':
            break
        if count < 2:
            continue
        items = line.strip().split(',')
        time1M[float(items[0])] = []
        for i in range(0, 5):
            time1M[float(items[0])].append(float(items[i+1]))
    # 10M
    input_folder = r"10Mrepeat5"
    input_path = input_folder + "/" + op + "/" + "avg.csv"
    input_file = open(input_path, "r")
    Lines = input_file.readlines()
    count = 0
    for line in Lines:
        count += 1
        if line == '\n':
            break
        if count < 2:
            continue
        items = line.strip().split(',')
        time10M[float(items[0])] = []
        for i in range(0, 5):
            time10M[float(items[0])].append(float(items[i+1]))

    # 100K
    input_folder = r"100Krepeat5"
    input_path = input_folder + "/" + op + "/" + "avg.csv"
    input_file = open(input_path, "r")
    Lines = input_file.readlines()
    count = 0
    for line in Lines:
        count += 1
        if line == '\n':
            break
        if count < 2:
            continue
        items = line.strip().split(',')
        time100K[float(items[0])] = []
        for i in range(0, 6):
            time100K[float(items[0])].append(float(items[i+1]))


op = "swap"
read_files(op)
print(time1M, time100K, time10M)

percen = [0.001, 0.01, 0.1]

fig, ax = plt.subplots(1, 1, figsize=f_size)
for d in range(0, 5):
    for i in range(0, 3):
        per = percen[i]
        time = [time100K[per][d], time1M[per][d], time10M[per][d]]
        print(d, i, per, time)
        plt.plot(x_list, time, color=color[d], label=label[d]+str(per), alpha=alpha[i], linewidth=line_width,
                 markersize=marker_size, marker=marker[i])




plt.xlabel("data size")
plt.xticks(ticks=x_list)

plt.ylabel('µs')
plt.yscale('log')
# plt.yticks(ticks=[100, 1000])

plt.legend(loc="upper center", bbox_to_anchor=(0.5, 1.35), ncol=5, fontsize=22)

plt.grid(True)
fig.tight_layout()
plt.savefig(op+".png",
            bbox_inches='tight')
plt.show()
plt.close()


plt.clf()


