#!/bin/bash

IFS=$'\n' 
read -d '' -r -a lines < $1

output_file=output.csv
> ${output_file}

#PROTOCOL - PORT map
PROTOCOL_PORT=( "21:FTP"
				"22:SSH"
				"25:SMTP (sending email)"
				"53:DNS (domain name service)"
				"80:HTTP (web server)"
				"110:POP3 (email inbox)"
				"123:NTP (Network Time Protocol)"
				"143:IMAP (email inbox)"
				"443:HTTPS (secure web server)"
				"465:SMTPS (send secure email)"
				"631:CUPS (print server)"
				"993:IMAPS (secure email inbox)"
				"995:POP3 (secure email inbox)"
			)

function get_protocol_by_port {
	for entry in "${PROTOCOL_PORT[@]}"; do
		KEY="${entry%%:*}"
		if [ "${KEY}" == "${1}" ]; then
			echo "${entry##*:}"
			exit
		fi
	done
	echo "not found"
}
RESERVED_PORTS=("21" "22" "25" "53" "80" "110" "123" "143" "443" "465" "631" "993" "995")
RESERVED_PORTS_STR=$( IFS=','; echo "${RESERVED_PORTS[*]}" );

#print_header
#	@params: output name
function print_header {
	printf "%s,%s,%s," "Host name" "IP addr" "DNS server" >> ${1}
	#TODO: Is Working and DDoS guard
	for i in "${PROTOCOL_PORT[@]}"; do
		printf "%s," ${i} >> ${1}
	done
	printf "%s,%s\n" "Other Open Ports" "Is Working" >> ${1}

	#HEADER_SIZE=$(4 + ${#RESERVED_PORTS[@]})
}

#process_host
#	@params: host_name
function process_host {
	#Get Host Name
	local host_name=${1}
	printf "Processing: %s\n" ${host_name}
	printf "%s," ${host_name} >> ${output_file}					#Host_Name 1

	#Get Ip Address
	local ip_addresses=( $(dig +short ${1}) )
	printf "\tIp Addresses:"
	printf "\"" >> ${output_file}
	for ip_addr in "${ip_addresses[@]}"; do
		printf " %s" ${ip_addr}
		printf "%s\n" ${ip_addr} >> ${output_file}
	done
	printf "\"," >> ${output_file}
	printf "\n"

	#Get DNS server
	local dns_servers=( $(dig ns +short ${1}) )
	printf "\tDNS servers:"
	printf "\"" >> ${output_file}
	for dns_server in "${dns_servers[@]}"; do
		printf " %s" ${dns_server}
		printf "%s\n" ${dns_server} >> ${output_file}
	done
	printf "\"," >> ${output_file}
	printf "\n"

	local is_alive=0
	#Get ports
	for port in "${RESERVED_PORTS[@]}"; do 
		#Get proto name
		local protocol=$(get_protocol_by_port ${port})
		if [ "${protocol}" == "not found" ]; then 
			#TODO: RESERVED_PORTS != Keys in MAP
			#make function to get keys array from map
			printf "\tProtocol by port num %s not fount\n" "${port}"
			continue
		else
			printf "\tScan port %s:%s\n" "${port}" "${protocol}"
			local nmap_output=$(nmap -p${port} -oG - ${host_name})
			local nmap_ports_line=$(echo "${nmap_output}" | grep "Ports")
			if [ ! -z "${nmap_ports_line}" ]; then
				local is_alive=1
				printf "\t\tPort exist\n"

				#Get port status
				local port_status=$(echo ${nmap_ports_line} | awk -F "\t" '{print $2}' | awk -F " " '{print $2}' | awk -F "/"  '{print $2}')
				printf "\t\tPort status: %s\n" ${port_status}
				if [ -z "${port_status}" ]; then
					echo ${nmap_output}
				fi
				
				#Get rDNS if exist
				local rdns=$(echo ${nmap_ports_line} | awk -F "\t" '{print $1}' | awk -F '[()]' '{print $2}')

				printf "\"" >> ${output_file}
				printf "STATUS: %s\n" ${port_status} >> ${output_file}

				if [ -n "${rdns}" ]; then
					printf "\t\trDNS: %s\n" ${rdns}
					printf "rDNS: %s\n" ${rdns} >> ${output_file}
				fi
				printf "\"," >> ${output_file}
			else
				printf "\t\tPort does not exist\n"
				printf "Not Exist," >> ${output_file}
			fi
		fi
	done

	#TODO: Other ports
	printf "TODO," >> ${output_file}

	if [[ ${is_alive} -eq 0 ]]; then
		printf "\tHost is DOWN\n"
		printf "DOWN\n" >> ${output_file}
	else
		printf "\tHost is UP\n"
		printf "UP\n" >> ${output_file}
	fi
}



#Start (Main)

print_header ${output_file}
for i in "${lines[@]}"; do
	process_host ${i}
done



