# Imports
from pystray import Icon, MenuItem, Menu
from PIL import Image, ImageDraw
from win10toast import ToastNotifier
import psutil
import os
import time
import threading
import keyboard
import win32gui
import win32process
import winreg as reg
import shutil
import sys

# Variables
menu_states = {
    "Autostart": False,
}
toaster = ToastNotifier()

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
        notify_process_killed(pid, 1, "")
    except Exception as e:
        print(f"FAILED TO KILL PID {pid}: {e}")
        notify_process_killed(pid, 2, f"{e}")

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




    menu_states["Autostart"] = not menu_states["Autostart"]
    if menu_states["Autostart"]:
        add_to_autostart()
    else:
        remove_from_autostart()
    print(f"Autostart state: {menu_states['Autostart']}")

def get_startup_folder():
    return os.path.join(os.environ['APPDATA'], r"Microsoft\Windows\Start Menu\Programs\Startup")

def get_self_path():
    if getattr(sys, 'frozen', False):
        return sys.executable
    else:
        return os.path.abspath(__file__)

def copy_to_startup():
    try:
        script_path = get_self_path()
        startup_folder = get_startup_folder()
        destination = os.path.join(startup_folder, os.path.basename(script_path))

        shutil.copy(script_path, destination)
        print(f"Copied to Startup folder: {destination}")
    except Exception as e:
        print(f"Failed to copy to Startup folder: {e}")

def remove_from_startup():
    try:
        startup_folder = get_startup_folder()
        startup_script = os.path.join(startup_folder, os.path.basename(get_self_path()))

        if os.path.exists(startup_script):
            os.remove(startup_script)
            print("Removed from Startup folder.")
        else:
            print("File not found in Startup folder.")
    except Exception as e:
        print(f"Failed to remove from Startup folder: {e}")

def is_in_startup():
    startup_folder = get_startup_folder()
    startup_script = os.path.join(startup_folder, os.path.basename(get_self_path()))
    return os.path.exists(startup_script)

def toggle_Autostart(icon, item):
    menu_states["Autostart"] = not menu_states["Autostart"]
    if menu_states["Autostart"]:
        copy_to_startup()
        notify_autostart(True)
    else:
        remove_from_startup()
        notify_autostart(False)
    print(f"Autostart state: {menu_states['Autostart']}")
    return None

def tStart():
    def tExit(icon, item):
        print("STOPPING")
        icon.stop()

    img = tImg(64, 64, "black", "red")

    menu_states["Autostart"] = is_in_startup()

    menu = Menu(
        MenuItem(
            "Autostart",
            toggle_Autostart,
            checked=lambda item: menu_states["Autostart"],
        ),
        Menu.SEPARATOR,
        MenuItem("Exit", tExit),
    )

    icon = Icon("Process Sigma v0.3", img, "Process Sigma v0.3", menu)
    icon.run()

def notify_process_killed(pid, state, note):
    if state == 1:
        message = f"KILLED {pid} SUCCESSFULLY\nNOTE: {note}"
        toaster.show_toast("Process Killed", message, duration=5)
    elif state == 0:
        message = f"KILLING {pid} FAILED\nNOTE: {note}"
        toaster.show_toast("Process Killed", message, duration=5)

def notify_autostart(state):
    if state == True:
        message = f"Autostart has been enabled."
        toaster.show_toast("Autostart Status", message, duration=5)
    elif state == False:
        message = f"Autostart has been disabled."
        toaster.show_toast("Autostart Status", message, duration=5)

# Main function
def main():
    print("RUNNING")
    hotkey_thread = threading.Thread(target=listen_key, daemon=True)
    hotkey_thread.start()
    tStart()

if __name__ == "__main__":
    main()
