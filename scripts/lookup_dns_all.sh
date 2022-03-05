#!/bin/bash

trap cleanup SIGINT SIGTERM ERR EXIT
printf "Processing ${1} file\n"

cleanup() {
  trap - SIGINT SIGTERM ERR EXIT

  exit 0
}

while read p; do
  ./lookup_dns.sh -d ${p}
done < ${1}
