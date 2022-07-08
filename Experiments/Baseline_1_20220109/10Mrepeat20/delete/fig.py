
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

line_style = ['o-', 's--', '^:', '-.p', 'h-', 'D--']
color = ['C5', 'C1', 'C7', 'C3', 'C9']
plt_title = ["BlueNile", "COMPAS", "Credit Card"]

label = ["ADA", "SA", "LL", "TV", "VEC", "CBT"]
line_width = 8
marker_size = 15
# f_size = (14, 10)

f_size = (14, 10)



def hundreds_formatter(x, pos):
    return int(x/100)




x_list = list()
execution_time = [[], [], [], [], [], []]
execution_time1 = list()
execution_time2 = list()
execution_time3 = list()
execution_time4 = list()
execution_time5 = list()
execution_time6 = list()


input_path = r'avg.csv'
input_file = open(input_path, "r")


# Using readlines()
Lines = input_file.readlines()

count = 0
# Strips the newline character
for line in Lines:
    count += 1
    if line == '\n':
        break
    if count < 2:
        continue

    items = line.strip().split(',')
    x_list.append(float(items[0]))
    for i in range(0, 5):
        execution_time[i].append(float(items[i+1]))



fig, ax = plt.subplots(1, 1, figsize=f_size)
for i in range(0, 5):
    plt.plot(x_list, execution_time[i], color=color[i], label=label[i], linewidth=line_width,
         markersize=marker_size)


plt.xscale('log')
plt.xlabel('\%')
plt.xticks(ticks=[0.0001, 0.001, 0.01, 0.1, 1], labels=[0.001, 0.01, 1, 10, 100])
# ax.xaxis.set_major_formatter(FuncFormatter(hundreds_formatter))
plt.ylabel('µs')
plt.yscale('log')
# plt.yticks(ticks=[1000, 10000, 100000, 1000000])
plt.legend(loc="upper center", bbox_to_anchor=(0.5, 1.15), ncol=6, fontsize=22)

plt.grid(True)
fig.tight_layout()
plt.savefig("delete.png",
            bbox_inches='tight')
plt.show()
plt.close()



plt.clf()


