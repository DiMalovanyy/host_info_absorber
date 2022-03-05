#!/bin/bash

# Copyright (C) 2021 Dmytro Malovanyi <dmitrymalovanyy@gmail.com> - All Rights Reserved
# You may use, distribute and modify this code under the
# terms of the XYZ license, which unfortunately won't be
# written for another century.
#
# You should have received a copy of the XYZ license with
# this file. If not, please write to: , or visit :
#


#Plan:
# Found all nameservers bu given hostname
# Try to get maximum information about nameserver
#	TTR, OS, all RR(if possible), full SOA, etc.


# Arguments:
#	@param: hostname

set -Eeuo pipefail
trap cleanup SIGINT SIGTERM ERR EXIT
script_dir=$(cd "$(dirname "${BASH_SOURCE[0]}")" &>/dev/null && pwd -P)
is_debug=0
is_axfr=0


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
  msg ${YELLOW}"Processing ${hostname} finished.${NOFORMAT}"
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

  return 0
}

parse_params "$@"
setup_colors

# ********************************************************************
# Script logic starts here
# ********************************************************************

# @note: function check if host is availabale
# @params:
#    ip_address
check_host_discovering() {
	


}

# @params:
#	nameserver_name
research_nameserver() {
	local nameserver_name=${1}
	local ipv4_nameserver_addresses=( $(dig +short -q ${nameserver_name} -t A) )
	local ipv6_nameserver_addresses=( $(dig +short -q ${nameserver_name} -t AAAA) )

	#Try to get AXFR from server
	log_debug "Start processing ${nameserver_name} nameserver"
	if [ ${#ipv4_nameserver_addresses[@]} -ne 0 ]; then
		log_debug "\t${nameserver_name}(v4)\t - \t ${ipv4_nameserver_addresses[*]}"
	fi
	if [ ${#ipv6_nameserver_addresses[@]} -ne 0 ]; then
		log_debug "\t${nameserver_name}(v6)\t - \t ${ipv6_nameserver_addresses[*]}"
	fi
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
fi

for nameserver in "${nameservers[@]}"; do
	research_nameserver ${nameserver}
done


