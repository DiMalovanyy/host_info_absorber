#!/bin/bash

printf "Processing ${1} file\n"

while read p; do
  ./lookup_dns.sh -d ${p}
done < ${1}
