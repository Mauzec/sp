import tkinter as tk
from tkinter import ttk
from nksr import *

class App:
    def __init__(self, root):
        self.root = root
        self.root.title("Resizable GUI with Table")

        # Create and place widgets
        self.create_widgets()

        # Configure resizable behavior
        self.root.columnconfigure(0, weight=1)
        self.root.rowconfigure(0, weight=1)

        self.rows = []
    
    def add_sample_data(self):
        port_range = [self.port_range_entry1.get().strip(), self.port_range_entry2.get().strip()]
        try:
            started = int(port_range[0])
            end = int(port_range[1])
            port_range = [started + i for i in range(end - started + 1)]
        except Exception as e:
            print(e)
            port_range = None

        ports = None
        if port_range == None:
            ports = self.port_not_range_entry.get().strip().split(',')
        try:
            ports = list(map(int, ports))
        except Exception as e:
            print(e)
            ports = None
        print(port_range, ports)
        tmp_rows = start(self.ip_range_entry1.get(), port_range if port_range != None else ports)

        for row in tmp_rows:
            if row not in self.rows:
                self.table.insert('', 'end', values=(row[0], row[1], row[2], row[3],row[4]))
                self.rows.append(row)

    def create_widgets(self):
        # Create frame for the table
        table_frame = ttk.Frame(self.root)
        table_frame.grid(row=0, column=0, padx=10, pady=10, sticky="nsew")

        # Create table
        table_columns = ["ip", "mac", "country", "provider", "ports"]
        self.table = ttk.Treeview(table_frame, columns=table_columns, show="headings")

        # Set column headings
        for col in table_columns:
            self.table.heading(col, text=col)

        # Place table
        self.table.grid(row=0, column=0, sticky="nsew")

        # Create frame for data entry
        entry_frame = ttk.Frame(self.root)
        entry_frame.grid(row=1, column=0, padx=10, pady=10, sticky="nsew")

        # Create and place Entry widgets for port range
        ttk.Label(entry_frame, text="Port range:").grid(row=0, column=0, padx=5, pady=5)
        self.port_range_entry1 = ttk.Entry(entry_frame)
        self.port_range_entry1.grid(row=0, column=1, padx=5, pady=5)
        ttk.Label(entry_frame, text="|").grid(row=0, column=2, padx=5, pady=5)
        self.port_range_entry2 = ttk.Entry(entry_frame)
        self.port_range_entry2.grid(row=0, column=3, padx=5, pady=5)

        # Create and place Entry widget for port not range
        ttk.Label(entry_frame, text="Port not range:").grid(row=1, column=0, padx=5, pady=5)
        self.port_not_range_entry = ttk.Entry(entry_frame)
        self.port_not_range_entry.grid(row=1, column=1, columnspan=3, padx=5, pady=5)

        # Create and place Entry widgets for IP range
        ttk.Label(entry_frame, text="IP\mask:").grid(row=2, column=0, padx=5, pady=5)
        self.ip_range_entry1 = ttk.Entry(entry_frame)
        self.ip_range_entry1.grid(row=2, column=1, padx=5, pady=5)

        # Create frame for additional widgets
        additional_frame = ttk.Frame(self.root)
        additional_frame.grid(row=0, column=1, rowspan=2, padx=10, pady=10, sticky="nsew")

        # Create and place Entry widgets for additional filters
        ttk.Label(additional_frame, text="Filter").grid(row=0, column=1, padx=5, pady=5)

        ttk.Label(additional_frame, text="IP(or):").grid(row=1, column=0, padx=5, pady=5)
        self.ip_entry = ttk.Entry(additional_frame)
        self.ip_entry.grid(row=1, column=1, padx=5, pady=5)

        ttk.Label(additional_frame, text="Country(or):").grid(row=2, column=0, padx=5, pady=5)
        self.country_entry = ttk.Entry(additional_frame)
        self.country_entry.grid(row=2, column=1, padx=5, pady=5)

        ttk.Label(additional_frame, text="Mac(or):").grid(row=3, column=0, padx=5, pady=5)
        self.mac_entry = ttk.Entry(additional_frame)
        self.mac_entry.grid(row=3, column=1, padx=5, pady=5)

        ttk.Label(additional_frame, text="Ports(or):").grid(row=4, column=0, padx=5, pady=5)
        self.ports_entry = ttk.Entry(additional_frame)
        self.ports_entry.grid(row=4, column=1, padx=5, pady=5)

        # Create and place a Button with text "Start"
        start_button = ttk.Button(additional_frame, text="Start", command=self.add_sample_data)
        start_button.grid(row=5, column=0, columnspan=2, pady=10)

        self.filter_button = ttk.Button(additional_frame, text="Filter", command=self.filter)
        self.filter_button.grid(row=6, column=0, columnspan=2, pady=10)

        clear_button= ttk.Button(additional_frame, text="Clear", command=self.clear)
        clear_button.grid(row=7, column=0, columnspan=2, pady=10)

    def clear(self):
        for item in self.table.get_children():
            self.table.delete(item)

        self.rows = []

    def filter(self):
        rows = self.rows

        ips = self.ip_entry.get().strip()
        if ips == '': ips = set()
        else: ips = set(ips.split(','))
        if len(ips) != 0:
            rows = list(filter(lambda row: row[0] in ips, rows))
        print(rows)

        countries = self.country_entry.get().strip()
        if countries == '': countries = set()
        else: countries = set(countries.split(','))
        if len(countries) != 0:
            rows = list(filter(lambda row: row[2] in countries, rows))
        print(rows)
        
        macs = self.mac_entry.get().strip()
        if macs == '': macs = set()
        else: macs = set(macs.split(','))
        if len(macs) != 0:
            rows = list(filter(lambda row: row[1] in macs, rows))
        print(rows)

        ports = self.ports_entry.get().strip()
        if ports == '': ports = set()
        else: ports = set(ports.split(','))
        if len(ports) != 0:
            tmp_rows = []

            for i in range(len(rows)):
                tmp = rows[i][4]
                if tmp == '': continue
                tmp = tmp.split(',')
                
                no = False
                for p in tmp:
                    delim = p.find('-')
                    if p[:delim] not in ports:
                        print(p[:delim])
                        no = True
                        break
                
                if not no:
                    tmp_rows.append(rows[i])

            rows = tmp_rows

        for item in self.table.get_children():
            self.table.delete(item)
        for row in rows:
            self.table.insert('', 'end', values=(row[0], row[1], row[2], row[3],row[4]))

        self.filter_button.config(text="Return", command=self.return_data)
        
    def return_data(self):
        for item in self.table.get_children():
            self.table.delete(item)
        for row in self.rows:
            self.table.insert('', 'end', values=(row[0], row[1], row[2], row[3],row[4]))

        self.filter_button.config(text="Filter", command=self.filter)

        

if __name__ == "__main__":
    root = tk.Tk()
    app = App(root)
    root.mainloop()
