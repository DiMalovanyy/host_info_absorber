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


usage() {
  cat <<EOF
Usage: $(basename "${BASH_SOURCE[0]}") [-h] [-v] [-d] <hostname> 
Script get all possible information about nameservers.

Available options:

-h, --help      Print this help and exit
-v, --verbose   Print script run source code
-d, --debug     Print script debug messages
--no-color      Disable colors in script
EOF
  exit
}

cleanup() {
  trap - SIGINT SIGTERM ERR EXIT
  # script cleanup here
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

msg "${ORANGE}Start processing ${hostname}...${NOFORMAT}"

#Get all addresses
ipv4_addresses=( $(dig +short ${hostname} A) )
ipv6_addresses=( $(dig +short ${hostname} AAAA) )


