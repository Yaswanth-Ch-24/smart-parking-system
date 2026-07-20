"""
Smart Parking Lot Management & Dynamic Guidance System
PC Terminal Simulator — No hardware needed

Simulates the exact UART output of the STM32 F446RE system:
  - Vehicle entry/exit detection via ultrasonic
  - Servo gate open/close
  - Slot assignment with LED states
  - MAX7219 dot-matrix display updates
  - Full FULL / parking free scenarios

Run: python simulate.py
"""

import time
import sys

GREEN  = "\033[92m"
RED    = "\033[91m"
YELLOW = "\033[93m"
CYAN   = "\033[96m"
WHITE  = "\033[97m"
DIM    = "\033[2m"
BOLD   = "\033[1m"
RESET  = "\033[0m"

TOTAL_SLOTS = 3

class ParkingSystem:
    def __init__(self):
        self.slots = [True] * TOTAL_SLOTS  # True = free
        self.available = TOTAL_SLOTS

    def display(self):
        line = "\n  Slot Status: "
        for i, free in enumerate(self.slots):
            if free:
                line += f"{GREEN}[S{i+1}:FREE]{RESET} "
            else:
                line += f"{RED}[S{i+1}:OCC ]{RESET} "
        if self.available == 0:
            line += f"  {RED}{BOLD}DISPLAY: FULL{RESET}"
        else:
            line += f"  {CYAN}DISPLAY: {self.available} FREE{RESET}"
        print(line)

    def assign_slot(self):
        for i, free in enumerate(self.slots):
            if free:
                self.slots[i] = False
                self.available -= 1
                return i + 1
        return None

    def release_slot(self):
        for i, free in enumerate(self.slots):
            if not free:
                self.slots[i] = True
                self.available += 1
                return i + 1
        return None

def gate(action, lane):
    ch = "Entry" if lane == "entry" else "Exit"
    deg = "90deg" if action == "open" else "0deg"
    verb = "OPENING" if action == "open" else "CLOSING"
    print(f"{YELLOW}[GATE]  {ch} gate {verb} (servo {deg}){RESET}")
    time.sleep(0.5)

def banner():
    print(f"\n{CYAN}{'='*45}{RESET}")
    print(f"{BOLD}{WHITE}  Smart Parking System{RESET}")
    print(f"{BOLD}{WHITE}  STM32 F446RE | Yaswanth Chlliboina{RESET}")
    print(f"{CYAN}{'='*45}{RESET}\n")

def run():
    banner()
    p = ParkingSystem()

    print(f"[INIT] Total slots: {TOTAL_SLOTS} | Available: {p.available}")
    p.display()
    time.sleep(0.8)

    events = [
        ("entry", "Vehicle 1 (dist=8cm)"),
        ("entry", "Vehicle 2 (dist=6cm)"),
        ("entry", "Vehicle 3 (dist=9cm)"),
        ("entry", "Vehicle 4 (dist=7cm) — parking full"),
        ("exit",  "Vehicle 1 leaving (dist=7cm)"),
        ("entry", "Vehicle 5 (dist=8cm) — 1 slot now free"),
        ("exit",  "Vehicle 2 leaving"),
        ("exit",  "Vehicle 3 leaving"),
    ]

    for ev_type, label in events:
        time.sleep(0.9)
        print(f"\n{WHITE}[{ev_type.upper()}] {label}{RESET}")

        if ev_type == "entry":
            if p.available > 0:
                gate("open", "entry")
                slot = p.assign_slot()
                print(f"{RED}[SLOT]  Slot {slot} assigned >> Red LED ON{RESET}")
                print(f"[COUNT] Available: {p.available}")
                p.display()
                gate("close", "entry")
            else:
                print(f"{RED}{BOLD}[FULL]  Parking FULL — Entry gate locked{RESET}")
                print(f"{RED}        DISPLAY: FULL{RESET}")

        elif ev_type == "exit":
            gate("open", "exit")
            slot = p.release_slot()
            print(f"{GREEN}[SLOT]  Slot {slot} released >> Green LED ON{RESET}")
            print(f"[COUNT] Available: {p.available}")
            p.display()
            gate("close", "exit")

    print(f"\n{CYAN}{'='*45}{RESET}")
    print(f"{BOLD}  Simulation complete.{RESET}")
    print(f"{DIM}  On real hardware, this runs 24/7 on STM32 F446RE.{RESET}")
    print(f"{CYAN}{'='*45}{RESET}\n")

if __name__ == "__main__":
    try:
        run()
    except KeyboardInterrupt:
        print(f"\n{DIM}Simulation stopped.{RESET}\n")
