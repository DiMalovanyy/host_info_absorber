#!/usr/bin/python3

import sys 
import os
import subprocess
import shlex
import time
import re
from tqdm import tqdm, trange

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
        splited_command.insert(2, "0.4s")
    try:
        process = subprocess.Popen(splited_command,
                                    stdout=subprocess.PIPE,
                                    stderr=subprocess.PIPE)
        need_clear = False
        with tqdm(total=100, ncols=140) as t:
            current_status_message = ""
            while True:
                line=process.stdout.readline().decode('UTF-8')[:-1]
                if not line:
                    break
                if ": About" in line:
                    status_message=line.split(':')[0]
                    search_result = re.search(": About (.*)% done", line)
                    if not search_result:
                        continue

                    cur_percent=float(search_result.group(1))

                    #New processing
                    if current_status_message != status_message:
                        if current_status_message:
                            t.update(int(100) - t.n)
                            print()
                        current_status_message = status_message
                        need_clear = True
                        t.reset()
                        t.set_description(desc=current_status_message, refresh=True)
                    t.update(int(cur_percent) - t.n)
                elif "Stats" in line:
                    continue
                elif need_clear:
                    need_clear = False
                    t.update(int(100) - t.n)
    except:
        print ("Exception occured:", sys.exc_info())
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
    


