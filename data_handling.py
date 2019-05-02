import plotly.plotly as py
import plotly.graph_objs as go
import csv


def read_values(filename):
    timestamps_list = []
    with open(filename) as csv_file:
        csv_reader = csv.reader(csv_file, delimiter=',')
        line_count = 0
        for row in csv_reader:
            timestamps_list.append(row[0])
    return timestamps_list



timestamps_gettimeofday = read_values("with_gettimeofday.csv")
timestamps_withoit = read_values("without_gettimeofday.csv")


