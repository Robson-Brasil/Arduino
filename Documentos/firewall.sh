#!/bin/bash

# Ativa roteamento no kernel
echo "1" > /proc/sys/net/ipv4/ip_forward

# Proteção contra IP spoofing
echo "1" > /proc/sys/net/ipv4/conf/all/rp_filter

# Proteções diversas contra portscanners, ping of death, ataques DoS, etc.
iptables -A FORWARD -p icmp --icmp-type echo-request -m limit --limit 1/s -j ACCEPT
iptables -A FORWARD -p tcp -m limit --limit 1/s -j ACCEPT
iptables -A FORWARD -m state --state ESTABLISHED,RELATED -j ACCEPT
iptables -A FORWARD -p tcp --tcp-flags SYN,ACK,FIN,RST RST -m limit --limit 1/s -j ACCEPT
iptables -A FORWARD --protocol tcp --tcp-flags ALL SYN,ACK -j DROP

# Proteção contra worms
iptables -A FORWARD -p tcp --dport 135 -i IF_INTERNA -j REJECT

# Dropa pacotes TCP indesejáveis
# -------------------------------------------------------
iptables -A FORWARD -p tcp ! --syn -m state --state NEW -j LOG --log-level 6 --log-prefix "FIREWALL: NEW sem syn: " 
iptables -A FORWARD -p tcp ! --syn -m state --state NEW -j DROP 

# Aceita os pacotes que realmente devem entrar
# -------------------------------------------------------
iptables -A INPUT -i  IF_EXTERNA -j ACCEPT
iptables -A INPUT -m state --state ESTABLISHED,RELATED -j ACCEPT
iptables -A OUTPUT -m state --state ESTABLISHED,RELATED,NEW -j ACCEPT
iptables -A FORWARD -m state --state ESTABLISHED,RELATED,NEW -j ACCEPT

# Proteção contra syn-flood
# -------------------------------------------------------
iptables -A FORWARD -p tcp --syn -m limit --limit 2/s -j ACCEPT

# Proteção contra ping da morte
# -------------------------------------------------------
iptables -A FORWARD -p icmp --icmp-type echo-request -m limit --limit 1/s -j ACCEPT

# Abre para a interface de loopback.
# Esta regra é essencial para o KDE e outros programas gráficos funcionarem adequadamente.
iptables -A INPUT -p tcp --syn -s 127.0.0.1/255.0.0.0 -j ACCEPT

# Ignora qualquer pacote de entrada, vindo de qualquer endereço, a menos que especificado o contrário acima. Bloqueia tudo.
iptables -A INPUT -p tcp --syn -j DROP
