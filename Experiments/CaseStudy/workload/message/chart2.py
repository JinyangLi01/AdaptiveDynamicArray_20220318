import numpy as np
import matplotlib.pyplot as plt




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
            Tinsert_asyn.append(int(items[i]))
    elif count == 3:
        for i in range(1, len(items)):
            Tinsert_syn.append(int(items[i]))
    elif count == 5:
        for i in range(1, len(items)):
            Tdelete_asyn.append(int(items[i]))
    elif count == 6:
        for i in range(1, len(items)):
            Tdelete_syn.append(int(items[i]))
    elif count == 8:
        for i in range(1, len(items)):
            Treorder.append(int(items[i]))
    elif count == 10:
        for i in range(1, len(items)):
            Tswap.append(int(items[i]))
    elif count == 12:
        for i in range(1, len(items)):
            Tquery_pre.append(int(items[i]))
    else:
        for i in range(1, len(items)):
            Tquery_nonpre.append(int(items[i]))
    count += 1

print(Tinsert_asyn)


# labels for bars
tick_label = ['query', 'insert', 'delete', 'reorder', 'swap']

datastructures = ['ADA', 'SA', 'CBT', 'LL', 'TV']
x_pos = [i for i, _ in enumerate(datastructures)]
# query
# plotting a bar chart
plt.bar(x_pos, Tquery_pre, color = ['blue'])
# naming the x-axis
plt.xlabel('x - axis')
# naming the y-axis
plt.ylabel('y - axis')
# plot title
plt.title('query')
plt.xticks(x_pos, datastructures)
plt.yscale('log')
plt.savefig('query.png',dpi=400)
# function to show the plot
plt.show()


# insert
# plotting a bar chart
plt.bar(x_pos, Tinsert_asyn, color = ['blue'])
# naming the x-axis
plt.xlabel('x - axis')
# naming the y-axis
plt.ylabel('y - axis')
# plot title
plt.title('insert')
plt.xticks(x_pos, datastructures)
plt.yscale('log')
plt.savefig('insert.png',dpi=400)
# function to show the plot
plt.show()

# delete
# plotting a bar chart
plt.bar(x_pos, Tdelete_asyn, color = ['blue'])
# naming the x-axis
plt.xlabel('x - axis')
# naming the y-axis
plt.ylabel('y - axis')
# plot title
plt.title('delete')
plt.xticks(x_pos, datastructures)
plt.yscale('log')
plt.savefig('delete.png',dpi=400)
# function to show the plot
plt.show()

# reorder
# plotting a bar chart
plt.bar(x_pos, Treorder, color = ['blue'])
# naming the x-axis
plt.xlabel('x - axis')
# naming the y-axis
plt.ylabel('y - axis')
# plot title
plt.title('reorder')
plt.xticks(x_pos, datastructures)
plt.yscale('log')
plt.savefig('reorder.png',dpi=400)
# function to show the plot
plt.show()

# swap
# plotting a bar chart
plt.bar(x_pos, Tswap, color = ['blue'])
# naming the x-axis
plt.xlabel('x - axis')
# naming the y-axis
plt.ylabel('y - axis')
# plot title
plt.title('swap')
plt.xticks(x_pos, datastructures)
plt.yscale('log')
plt.savefig('swap.png',dpi=400)
# function to show the plot
plt.show()

# Closing files
result_file.close()