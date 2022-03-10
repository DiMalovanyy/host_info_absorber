#!/bin/bash

# Copyright (C) 2021 Dmytro Malovanyi <dmitrymalovanyy@gmail.com> - All Rights Reserved
# You may use, distribute and modify this code under the
# terms of the XYZ license, which unfortunately won't be
# written for another century.
#
# You should have received a copy of the XYZ license with
# this file. If not, please write to: , or visit :
#

#TODO:
# Make sense to scan some networks:
#	Network where host was found
#	Network where DNS servers located
#Rewrite all 'nmap' usage with python/nmap_with_progress.py

#Plan:
# Found all nameservers bu given hostname
# Try to get maximum information about nameserver
#	TTR, OS, all RR(if possible), full SOA, etc.


# Arguments:
#	@param: hostname

set -Eeuo pipefail
trap cleanup SIGINT SIGTERM ERR EXIT
readonly script_dir=$(cd "$(dirname "${BASH_SOURCE[0]}")" &>/dev/null && pwd -P)
readonly nmap_binary=$(which nmap)
is_debug=0
is_axfr=0
is_root=0
AWK_BINARY=""

usage() {
  cat <<EOF
Usage: $(basename "${BASH_SOURCE[0]}") [-h] [-v] [-d] <hostname> 
Script get all possible information about nameservers.

Available options:

-h, --help      Print this help and exit
-v, --verbose   Print script run source code
-d, --debug     Print script debug messages
-a, --axfr      Check Zone transfering by AXRF
--no-color      Disable colors in script
EOF
  exit
}

cleanup() {
  trap - SIGINT SIGTERM ERR EXIT
  # script cleanup here
  msg "${YELLOW}Processing ${hostname} finished.${NOFORMAT}"
  exit 0
}

setup_colors() {
  if [[ -t 2 ]] && [[ -z "${NO_COLOR-}" ]] && [[ "${TERM-}" != "dumb" ]]; then
    NOFORMAT='\033[0m' RED='\033[0;31m' GREEN='\033[0;32m' ORANGE='\033[0;33m' BLUE='\033[0;34m' PURPLE='\033[0;35m' CYAN='\033[0;36m' YELLOW='\033[1;33m'
  else
    NOFORMAT='' RED='' GREEN='' ORANGE='' BLUE='' PURPLE='' CYAN='' YELLOW=''
  fi
}

msg() {
  echo >&2 -e "${1-}"
}

log_success() {
	echo >&2 -e "[SUCCESS]: ${GREEN}${1-}${NOFORMAT}"
}
log_info() {
	echo >&2 -e "[INFO]: ${1-}"
}
log_fail() {
	echo >&2 -e "[FAIL]: ${RED}${1-}${NOFORMAT}"
}
log_warn() {
	echo >&2 -e "[WARN]: ${YELLOW}${1-}${NOFORMAT}"
}
log_debug() {
	if [ ${is_debug} -eq 1 ]; then
		echo >&2 -e "[DEBUG]: ${1-}"
	fi
}

die() {
  local msg=$1
  local code=${2-1} # default exit status 1
  msg "$msg"
  exit "$code"
}

parse_params() {
  # default values of variables set from params
  flag=0
  param=''

  while :; do
    case "${1-}" in
    -h | --help) usage ;;
    -v | --verbose) set -x ;;
	-d | --debug) is_debug=1 ;;
	-a | --axfr) is_axfr=1 ;;
    --no-color) NO_COLOR=1 ;;
    -?*) die "Unknown option: $1" ;;
    *) hostname=$1
		break ;;
    esac
    shift
  done

  args=("$@")

  # check required params and arguments
  [[ ${#args[@]} -eq 0 ]] && die "Missing script arguments"

  if [ "$EUID" -eq 0 ]; then 
	  is_root=1
  else
	  log_warn "For better scanning recomended to run with root privilage"
  fi

  if [ -z "${nmap_binary}" ]; then
	  log_fail "Nmap required"
	  die "Missing nmap binary"
  else
	  log_debug "Using nmap: ${nmap_binary}"
  fi

  #Check awk
  which gawk &> /dev/null
  if [ $? != 0 ]; then
	  AWK_BINARY='gawk'
  else
	  which awk &> /dev/null
	  if [ $? != 0 ]; then
		  log_fail "Awk required"
		  die "Missing awk binary"
	  else
		# Check required awk version and dialect
		if awk --version 2>&1 | grep -q "GNU Awk" ; then
			AWK_BINARY='awk'
		else 
			log_fail "Required GNU dialect of awk (gawk)"
			die "Missing gawk library"
		fi
	  fi
  fi

  return 0
}

setup_colors
parse_params "$@"

# ********************************************************************
# Script logic starts here
# ********************************************************************

# @note: function ONLY check if host is availabale
# @params:
#		ip_address
#		is_ipv6 (1 if ipv6, 0 if ipv4)
# @return:
#		UP, DOWN, ERROR
# @todo:
#		1. Add TCP ACK ping (on specific ports)
#		2. Add TCP SYN ping (on specific ports)
#		3. Add UDP ping     (on specific ports)
#       4. Add specific icmp ping types
#		n. Add v6 support
check_host_discovering() {
	readonly local is_ipv6=$( [ $# -eq 1 ] && echo 0 || echo $2 )
	#First check by default "Ping scan"
	#log_debug "Check if address ${1} is availabale"
	#@params:
	#	-n  - Disable reverse DNS resolution
	#   -oG - Grepable output
	#   -sn - Not to perform port scaning after host discovering
	#   -   - Inform that after '-' goes host/address
	local nmap_host_discovering_params="-n -sn"
	[ ${is_ipv6} -eq 1 ] && local nmap_host_discovering_params="${nmap_host_discovering_params} -6"
	#Add '-' to params to indicate that after goes address/host
	local nmap_host_discovering_params="${nmap_host_discovering_params} -oG -"

	#1. Default host discoveri
	#@packets
	#	@root
	#		ICMP echo request
	#		TCP SYN to 443
	#		TCP ACK to 80
	#		ICMP timestamp
	#		ARP (if local network)
	#	@user
	#		TCP SYN via connect syscall to 80 and 443
	readonly local default_host_discovery_status=$(nmap ${nmap_host_discovering_params} ${1} | \
		awk '/Status:/ {print $5}')
	[ $? -ne 0 ] && die "Some error occured while executing nmap. Nmap exit with code: $?"
	[ -z ${default_host_discovery_status} ] && { log_warn "Awk could not get Status from nmap output"; echo "ERROR"; exit; }
	[ "${default_host_discovery_status}" == "Up" ] && { echo "UP"; exit; }

	echo "DOWN"
}

# @params:
#	nameserver_name
# @todo:
#	ipv6 hostname discovering support
research_nameserver() {
	local nameserver_name=${1}
	local ipv4_nameserver_addresses=( $(dig +short -q ${nameserver_name} -t A) )
	local ipv6_nameserver_addresses=( $(dig +short -q ${nameserver_name} -t AAAA) )

	log_debug "Start processing ${BLUE}${nameserver_name}${NOFORMAT} nameserver"
	if [ ${#ipv4_nameserver_addresses[@]} -ne 0 ]; then
		log_debug "\t${nameserver_name}(v4):"
		for nameserver_addr_v4 in "${ipv4_nameserver_addresses[@]}"; do	
			log_debug "\t\t${nameserver_addr_v4} - $(check_host_discovering ${nameserver_addr_v4})"
		done
	fi
	if [ ${#ipv6_nameserver_addresses[@]} -ne 0 ]; then
		log_debug "\t${nameserver_name}(v6):"
		for nameserver_addr_v6 in "${ipv6_nameserver_addresses[@]}"; do
			log_debug "\t\t${nameserver_addr_v6} - (Check ipv6 host not available)"
		done
	fi

	#Check if axfr and it is able to transfer dns
	if [ ${is_axfr} -eq 1 ]; then
		local axfr_response="$(dig +short -q ${hostname} @${nameserver_name} axfr)"
		if echo ${axfr_response} | grep -q "failed"; then
			log_debug "\t${ORANGE}Could not get zone setting by AXFR${NOFORMAT}"
		else
			log_success "${GREEN}Successfully recevied zone of nameserver: ${nameserver_name} for host: ${hostname}.${NOFORMAT}"
			log_info "Please contact script author <dmitrymalovanyy@gmail.com>"
		fi
	fi
}


log_info "${ORANGE}Start processing ${hostname}...${NOFORMAT}"

#Get all addresses
#NOTE: This will always get ip_address from DNS cache
ipv4_addresses=( $(dig +short -q ${hostname} -t A) )
if [ ${#ipv4_addresses[@]} -ne 0 ]; then
	log_debug "IPv4 addresses: ${ipv4_addresses[*]}"
fi
ipv6_addresses=( $(dig +short -q ${hostname} -t AAAA) )
if [ ${#ipv6_addresses[@]} -ne 0 ]; then
	log_debug "IPv6 addresses: ${ipv6_addresses[*]}"
fi


#Search all NS (nameservers) in zone
nameservers=( $(dig +short -q ${hostname} -t NS) )
if [ ${#nameservers[@]} -ne 0 ]; then
	log_debug "Domain nameservers: ${nameservers[*]}"
else
	log_fail "hostname ${hostname} does not have nameservers"
	exit 0
fi

for nameserver in "${nameservers[@]}"; do
	research_nameserver ${nameserver}
done


