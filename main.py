# Imports
from pystray import Icon, MenuItem, Menu
from PIL import Image, ImageDraw
import psutil
import os
import time
import threading
import keyboard
import win32gui
import win32process
import win32ui

# Variables
menu_states = {
    "Autostart": False,
}

# Functions
def get_pid():
    window = win32gui.GetForegroundWindow()
    _, pid = win32process.GetWindowThreadProcessId(window)
    return pid

def kill_pid(pid):
    try:
        prc = psutil.Process(pid)
        prc.kill()
        print(f"KILLED PID {pid}")
    except Exception as e:
        print(f"FAILED TO KILL PID {pid}: {e}")

def listen_key():
    while True:
        try:
            if keyboard.is_pressed('alt') and keyboard.is_pressed('f5'):
                pid = get_pid()
                if pid:
                    kill_pid(pid)
                time.sleep(0.5)
        except Exception as e:
            print(f"Error in listen_key: {e}")
            time.sleep(1)

def tImg(w, h, c1, c2):
    img = Image.new('RGB', (w, h), c1)
    draw = ImageDraw.Draw(img)
    draw.rectangle(
        (w // 4, h // 4, w * 3 // 4, h * 3 // 4),
        fill=c2,
    )
    return img

def toggle_Autostart(icon, item):
    menu_states["Autostart"] = not menu_states["Autostart"]
    print(f"Autostart state: {menu_states['Autostart']}")
    #TODO: autostart logik

def tStart():
    def tExit(icon, item):
        print("STOPPING")
        icon.stop()

    img = tImg(64, 64, "black", "red")

    menu = Menu(
        MenuItem(
            "Autostart",
            toggle_Autostart,
            checked=lambda item: menu_states["Autostart"],
        ),
        Menu.SEPARATOR,
        MenuItem("Exit", tExit),
    )

    icon = Icon("Process Sigma v0.2b", img, "Process Sigma v0.2b", menu)
    icon.run()

# Main function
def main():
    print("RUNNING")
    hotkey_thread = threading.Thread(target=listen_key, daemon=True)
    hotkey_thread.start()
    tStart()

if __name__ == "__main__":
    main()
