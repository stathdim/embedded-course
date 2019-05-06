import csv
from statistics import mean, pstdev, median


def read_values(filename):
    timestamps_list = []
    with open(filename) as csv_file:
        csv_reader = csv.reader(csv_file, delimiter=',')
        line_count = 0
        for row in csv_reader:
            # Remove the last el which is '' to avoid failure during typecasting
            row.pop()
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

print(timestamps_without[-1] - timestamps_without[0])

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
