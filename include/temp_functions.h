#ifndef TEMP_FUNCTIONS_H
#define TEMP_FUNCTIONS_H

#include <stdio.h>

#define MAX_LINE_LENGTH 1024
#define MAX_DATA_LENGTH 1000

typedef struct {
    int year;
    int month;
    int day;
    int hour;
    int minute;
    float temp;
} TemperatureData;


void read_csv_to_list(const char *file_path, TemperatureData data_list[], int *data_count);

void calculate_temperature_stats(TemperatureData data_list[], int data_count);

void calculate_temperature_stats_for_month(TemperatureData data_list[], int data_count, int month);

void print_data_list(TemperatureData data_list[], int data_count);

#endif
