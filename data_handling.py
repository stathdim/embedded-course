import plotly.plotly as py
import plotly.graph_objs as go
import csv
from statistics import mean, pstdev, median

def read_values(filename):
    timestamps_list = []
    with open(filename) as csv_file:
        csv_reader = csv.reader(csv_file, delimiter=',')
        line_count = 0
        for row in csv_reader:
            #Remove the last el which is '' to avoid failure during typecasting
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

distances_gettimeofday = calc_samples_distances(timestamps_gettimeofday)
distances_without = calc_samples_distances(timestamps_without)

mean_gettimeofday = mean(distances_gettimeofday)
mean_without = mean(distances_without)

stdev_gettimeofday = pstdev(distances_gettimeofday)
stdev_without = pstdev(distances_without)

median_gettimeofday = median(distances_gettimeofday)
median_without = median(distances_without)

print(median_gettimeofday, median_without)