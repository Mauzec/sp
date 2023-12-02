import scapy.all as scapy
import requests  # Add this line for making HTTP requests

def scan(ip):
    arp_request = scapy.ARP(pdst=ip)
    broadcast = scapy.Ether(dst="ff:ff:ff:ff:ff:ff")
    packet = broadcast / arp_request
    ask_list = scapy.srp(packet, timeout=1, verbose=False)[0]

    packet_list = []
    for i in ask_list:
        packet_dict = {"ip": i[1].psrc, "mac": i[1].hwsrc}
        packet_list.append(packet_dict)
    return packet_list

def get_country_provider(ip):
    try:
        response = requests.get(f"https://ipinfo.io/{ip}/json?token=149973a586e857")
        data = response.json()
        country = data.get("country", "Unknown")
        provider = data.get("org", "Unknown")
        return country, provider
    except Exception as e:
        return "Unknown", "Unknown"
        

def scan_ports(ip, common_ports):
    open_ports = []
    for port in common_ports:
        # TCP SYN scan
        response = scapy.sr1(scapy.IP(dst=ip) / scapy.TCP(dport=port, flags="S"), timeout=1, verbose=False)
        if response and response.haslayer(scapy.TCP) and response.getlayer(scapy.TCP).flags == 0x12:
            # SYN-ACK response, port is open
            service = get_service(port)
            open_ports.append({"port": port, "service": service})
    return open_ports

def get_service(port):
    services = {
        21: "FTP",
        22: "SSH",
        23: "Telnet",
        25: "SMTP",
        53: "DNS",
        80: "HTTP",
        110: "POP3",
        443: "HTTPS",
        445: "SMB",
        3389: "RDP"
    }
    return services.get(port, "Unknown")

def print_res(res, open_ports):
    print("IP\t\t\tMAC Address\t\tCountry\t\tProvider\t\tOpen Ports")
    rows = []
    for n in res:
        ip = n["ip"]
        mac = n["mac"]
        country, provider = get_country_provider(ip)
        ports_info = ""
        ports = []
        for ip_ports in open_ports:
            if ip_ports[0] == ip:
                ports_info = ",".join(f"{port['port']}-{port['service']}" for port in ip_ports[1])
        print(f"{ip}\t\t{mac}\t\t{country}\t\t{provider}\t\t{ports_info}")
        rows.append([ip, mac, country, provider, ports_info])
    return rows
    

def start(ip_mask="8.8.8.8/31", ports=None):
    if ports == None:
        return -1
    print(ip_mask)
    scan_result = scan(ip_mask)
    open_ports_result = []
    for n in scan_result:
        ip = n["ip"]
        open_ports_result.append( [ip, scan_ports(ip, ports)] )
    rows = print_res(scan_result, open_ports_result)
    return rows


