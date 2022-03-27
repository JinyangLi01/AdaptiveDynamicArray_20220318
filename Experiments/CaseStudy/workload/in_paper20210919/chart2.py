import numpy as np
import matplotlib.pyplot as plt


plt.rcParams.update({'font.size': 24})

# Opening file
result_file = open('m2_result_awards1.csv', 'r')
count = 0

Tinsert_asyn = []
Tinsert_syn = []
Tdelete_asyn = []
Tdelete_syn = []
Treorder = []
Tswap = []
Tquery_pre = []
Tquery_nonpre = []


# Using for loop
for line in result_file:
    print("Line{}: {}\n".format(count, line.strip()))
    if count <= 1 or count == 4 or count == 7 or count == 9 or count == 11:
        count += 1
        continue
    items = line.split(',')
    if count == 2:
        for i in range(1, len(items)):
            Tinsert_asyn.append(int(items[i])/1000)
    elif count == 3:
        for i in range(1, len(items)):
            Tinsert_syn.append(int(items[i])/1000)
    elif count == 5:
        for i in range(1, len(items)):
            Tdelete_asyn.append(int(items[i])/1000)
    elif count == 6:
        for i in range(1, len(items)):
            Tdelete_syn.append(int(items[i])/1000)
    elif count == 8:
        for i in range(1, len(items)):
            Treorder.append(int(items[i])/1000)
    elif count == 10:
        for i in range(1, len(items)):
            Tswap.append(int(items[i])/1000)
    elif count == 12:
        for i in range(1, len(items)):
            Tquery_pre.append(int(items[i])/1000)
    else:
        for i in range(1, len(items)):
            Tquery_nonpre.append(int(items[i])/1000)
    count += 1

print(Tinsert_asyn)


# labels for bars
tick_label = ['query', 'insert', 'delete', 'reorder', 'swap']

datastructures = ['ADA', 'SA', 'CBT', 'LL', 'TV']
x_pos = [i for i, _ in enumerate(datastructures)]
# query
# plotting a bar chart
plt.bar(x_pos, Tquery_pre, color = ['blue'])
# plot title
plt.xticks(x_pos, datastructures)
ax = plt.gca()
ax.annotate('ms', xy=(-0.07, 1.06), xytext=(-15,2), ha='left', va='top', xycoords='axes fraction', textcoords='offset points', fontsize=24)


plt.yscale('log')
plt.savefig('query.png',dpi=400)
# function to show the plot
plt.show()


# insert
# plotting a bar chart
plt.bar(x_pos, Tinsert_asyn, color = ['blue'])
# plot title

plt.xticks(x_pos, datastructures)
ax = plt.gca()
ax.annotate('ms', xy=(-0.07, 1.06), xytext=(-15,2), ha='left', va='top', xycoords='axes fraction', textcoords='offset points', fontsize=24)

plt.yscale('log')
plt.savefig('insert.png',dpi=400)
# function to show the plot
plt.show()

# delete
# plotting a bar chart
plt.bar(x_pos, Tdelete_asyn, color = ['blue'])
# plot title

plt.xticks(x_pos, datastructures)
ax = plt.gca()
ax.annotate('ms', xy=(-0.07, 1.06), xytext=(-15,2), ha='left', va='top', xycoords='axes fraction', textcoords='offset points', fontsize=24)

plt.yscale('log')
plt.savefig('delete.png',dpi=400)
# function to show the plot
plt.show()

# reorder
# plotting a bar chart
plt.bar(x_pos, Treorder, color = ['blue'])
plt.xticks(x_pos, datastructures)
ax = plt.gca()
ax.annotate('ms', xy=(-0.07, 1.06), xytext=(-15,2), ha='left', va='top', xycoords='axes fraction', textcoords='offset points', fontsize=24)

plt.yscale('log')
plt.savefig('reorder.png',dpi=400)
# function to show the plot
plt.show()

# swap
# plotting a bar chart
plt.bar(x_pos, Tswap, color = ['blue'])
# naming the x-axis
# naming the y-axis
# plot title
plt.xticks(x_pos, datastructures)
ax = plt.gca()
ax.annotate('ms', xy=(-0.07, 1.06), xytext=(-15,2), ha='left', va='top', xycoords='axes fraction', textcoords='offset points', fontsize=24)

plt.yscale('log')
plt.savefig('swap.png',dpi=400)
# function to show the plot
plt.show()

# Closing files
result_file.close()