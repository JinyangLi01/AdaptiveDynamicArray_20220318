import numpy as np
import matplotlib.pyplot as plt




# Opening file
result_file = open('wl2.txt', 'r')
count = 0
 
# Using for loop
for line in result_file:
    count += 1
    print("Line{}: {}".format(count, line.strip()))
    if count % 3 == 1:
        continue
    items = line.split(',')
    for i in range(1, len(items)):
        


# data to plot
n_groups = 4
means_frank = (90, 55, 40, 65)
means_guido = (85, 62, 54, 20)

# create plot
fig, ax = plt.subplots()
index = np.arange(n_groups)
bar_width = 0.35
opacity = 0.8

rects1 = plt.bar(index, means_frank, bar_width,
alpha=opacity,
color='b',
label='Frank')

rects2 = plt.bar(index + bar_width, means_guido, bar_width,
alpha=opacity,
color='g',
label='Guido')

plt.xlabel('Person')
plt.ylabel('Scores')
plt.title('Scores by person')
plt.xticks(index + bar_width, ('A', 'B', 'C', 'D'))
plt.legend()

plt.tight_layout()
plt.save
plt.show()



# Closing files
result_file.close()