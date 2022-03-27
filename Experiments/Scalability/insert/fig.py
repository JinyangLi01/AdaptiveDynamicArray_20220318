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
color = ['C5', 'C1', 'C2', 'C3', 'C4', 'C0']
plt_title = ["BlueNile", "COMPAS", "Credit Card"]

label = ["ADA", "SA", "LL", "CBT", "TV", "VEC"]
line_width = 8
marker_size = 15
# f_size = (14, 10)

f_size = (14, 10)


def thousands_formatter(x, pos):
    return int(x / 1000)


x_list = [1, 2, 3, 4]
execution_time = [[], [], [], [], [], []]
execution_time1 = list()
execution_time2 = list()
execution_time3 = list()
execution_time4 = list()
execution_time5 = list()
execution_time6 = list()

execution_time[0] = [406, 2054, 5655, 349, 1818, 405]
execution_time[1] = [818, 13023, 84139, 815, 4048, 1415]
execution_time[2] = [625, 59380.5, 240842.5, 654, 5239.5, 3400.5]
execution_time[3] = [3450, 2017809, 7492140, 0, 31902.5, 134461]

fig, ax = plt.subplots(1, 1, figsize=f_size)
for i in range(0, 3):
    plt.plot(x_list, [execution_time[j][i] for j in range(4)], color=color[i], label=label[i], linewidth=line_width,
             markersize=marker_size)
plt.plot(x_list[:3], [execution_time[j][3] for j in range(3)], color=color[3], label=label[3], linewidth=line_width,
         markersize=marker_size)
for i in range(4, 6):
    plt.plot(x_list, [execution_time[j][i] for j in range(4)], color=color[i], label=label[i], linewidth=line_width,
             markersize=marker_size)

plt.xticks(ticks=[1, 2, 3, 4], labels=["1K", "10K", "100K", "1M"])
# ax.xaxis.set_major_formatter(FuncFormatter(hundreds_formatter))
plt.ylabel('µs')
plt.yscale('log')
ax.yaxis.set_major_formatter(FuncFormatter(thousands_formatter))
plt.legend(loc="upper center", bbox_to_anchor=(0.5, 1.15), ncol=6, fontsize=22)

plt.grid(True)
fig.tight_layout()
plt.savefig("insert.png",
            bbox_inches='tight')
plt.show()
plt.close()

plt.clf()
