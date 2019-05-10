import matplotlib.pyplot as plt
import csv
from statistics import mean, pstdev, median


def read_values(filename):
    timestamps_list = []
    with open(filename) as csv_file:
        csv_reader = csv.reader(csv_file, delimiter=',')
        line_count = 0
        for row in csv_reader:
            # Remove the last el which is '' to avoid failure during typecasting
            for val in row:
                timestamps_list.append(int(val))
    return timestamps_list


def calc_samples_distances(samples):
    distances = []
    for i in range(1, len(samples)):
        diff = samples[i] - samples[i-1]
        distances.append(diff)
    return distances


timestamps_gettimeofday = read_values("with_gettimeofday.csv")
timestamps_without = read_values("without_gettimeofday.csv")


duration_gettimeofday = (
    timestamps_gettimeofday[-1] - timestamps_gettimeofday[0])/1000.0
duration_without = (timestamps_without[-1] - timestamps_without[0])/1000.0

print(f'Duration was {duration_gettimeofday}, {duration_without}')

distances_gettimeofday = calc_samples_distances(timestamps_gettimeofday)
distances_without = calc_samples_distances(timestamps_without)

mean_gettimeofday = mean(distances_gettimeofday)
mean_without = mean(distances_without)

stdev_gettimeofday = pstdev(distances_gettimeofday)
stdev_without = pstdev(distances_without)

median_gettimeofday = median(distances_gettimeofday)
median_without = median(distances_without)

min_gettimeofday = min(distances_gettimeofday)
min_without = min(distances_without)

max_gettimeofday = max(distances_gettimeofday)
max_without = max(distances_without)

print('Mean is: ', mean_gettimeofday, mean_without)
print('Standard deviation is', stdev_gettimeofday, stdev_without)
print('The median is ', median_gettimeofday, median_without)
print(f'Max difference is {max_gettimeofday} and {max_without}')
print(f'Min difference is {min_gettimeofday} and {min_without}')

plt.figure(1, figsize=(14, 10))
plt.plot(distances_gettimeofday, '.')
plt.savefig('distances.png', bbox_inches='tight')

names = ['using gettimeofday', 'without gettimeofday']

plt.figure(2, figsize=(7, 5))
plt.scatter(names, [mean_gettimeofday, mean_without])
plt.suptitle('Mean')
plt.savefig('mean.png', bbox_inches='tight')

plt.figure(3, figsize=(7, 5))
plt.scatter(names, [median_gettimeofday, median_without])
plt.suptitle('Median')
plt.savefig('median.png', bbox_inches='tight')


plt.figure(4, figsize=(7, 5))
plt.scatter(names, [stdev_gettimeofday, stdev_without])
plt.suptitle('Standard Deviation')
plt.savefig('stdev.png', bbox_inches='tight')

plt.figure(5, figsize=(7, 5))
plt.scatter(names, [max_gettimeofday, max_without])
plt.suptitle('Max')
plt.savefig('max.png', bbox_inches='tight')

plt.figure(6, figsize=(7, 5))
plt.scatter(names, [min_gettimeofday, min_without])
plt.suptitle('Min')
plt.savefig('min.png', bbox_inches='tight')