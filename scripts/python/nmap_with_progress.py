#!/usr/bin/python3

import sys 
import os
import subprocess
import shlex
import time
import re

command = ""
title = ""


def print_title():
    if title:
        print(f"{title}")

def print_progress_bar(percent_float, length = 100, fill = '█', printEnd = "\r", decimals = 1, prefix = '', suffix = 'Complete'):
    filled_length = int(percent_float)
    bar = fill * filled_length + '-' * (length - filled_length)
    print(f'\r{prefix} |{bar}| {int(percent_float)}% {suffix}', end = printEnd) 
    if filled_length == 100:
        print()


def execute_command():
    splited_command = shlex.split(command)
    if splited_command[0] == "nmap":
        splited_command.insert(1, "--stats-every")
        splited_command.insert(2, "1s")
    #print(f"Split command: {splited_command}")
    try:
        process = subprocess.Popen(splited_command,
                                    stdout=subprocess.PIPE,
                                    stderr=subprocess.PIPE)
        current_status_message = ""
        while True:
            line=process.stdout.readline().decode('UTF-8')[:-1]
            if not line:
                break
            if "About" in line:
                status_message=line.split(':')[0]
                percent=float(re.search(": About (.*)% done;", line).group(1))
                if status_message != current_status_message:
                    if current_status_message:
                        print_progress_bar(float(100), prefix=current_status_message)
                    current_status_message = status_message
                    #Updates progress bar
                    print_progress_bar(0, prefix=current_status_message)
                print_progress_bar(percent, prefix=current_status_message) 
            elif "Stats" in line:
                continue
    except:
        return

if __name__ == "__main__":
    if len(sys.argv) < 2 or len(sys.argv) > 3:
        print(f"Usage: {sys.argv[0]} <nmap command> [<title>]")
        exit(1)
    command = sys.argv[1]
    if len(sys.argv) == 3:
        title = sys.argv[2]
    print_title()
    execute_command()
    


