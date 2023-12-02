from nksr import *

def check_row(row: list, ip:str, ports:str):
    if row[0] == ip and row[4] != ports:
        print(f"{ip} has {row[4]}, not {ports}")
        return -1
    elif row[0] == ip and row[4] == ports:
        print(f"{ip} has {row[4]}")
'''
8.8.8.8: 53-DNS,443-HTTPS
40.89.244.232:  80-HTTP,443-HTTPS
'''
rows = start('40.89.244.232/32', ports=[53,80,443])
print(len(rows))
check_row(rows[0], ip='40.89.244.232', port='80-DNS,443-HTTPS')
rows = start('8.8.8.8/32', ports=[53,80,443])
print(len(rows))
check_row(rows[0], ip='8.8.8.8', port='53-DNS,443-HTTPS')

