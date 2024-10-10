import csv
from datetime import datetime, timedelta
import random

def generate_csv(filename, start_date, end_date, interval_minutes):
    with open(filename, mode='w', newline='') as file:
        writer = csv.writer(file, delimiter=';')

        current_date = start_date
        while current_date <= end_date:
            temperature = random.randint(-99, 99)
            writer.writerow([
                current_date.strftime('%Y'),
                current_date.strftime('%m'),
                current_date.strftime('%d'),
                current_date.strftime('%H'),
                current_date.strftime('%M'),
                temperature
            ])
            current_date += timedelta(minutes=interval_minutes)

start_date = datetime(2021, 1, 16, 1, 1)
end_date = datetime(2021, 12, 16, 1, 1)
interval_minutes = 2

generate_csv('temperature_data.csv', start_date, end_date, interval_minutes)
